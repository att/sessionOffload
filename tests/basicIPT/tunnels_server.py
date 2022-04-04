#============LICENSE_START=============================================================================================================
# Copyright (C) 2020 AT&T Intellectual Property. All rights reserved.
#===================================================================
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#============LICENSE_END===============================================================================================================

"""The Python implementation of the WB Application Offload tunnel table server."""

from concurrent import futures
from re import match
import time
import logging
import socket
import struct
import sys

import grpc
import google.protobuf.timestamp_pb2

#from grpc_status import rpc_status

from google.protobuf import any_pb2
#from google.rpc import code_pb2, status_pb2, error_details_pb2

from dataclasses import dataclass

@dataclass
class Counters:
    in_packets: int = 1
    in_packets_drops: int = 0
    in_bytes: int = 0
    in_bytes_drops: int = 0
    out_packets: int = 0
    out_packets_drops: int = 0
    out_bytes: int = 0
    out_bytes_drops: int = 0

import tunneloffload_pb2
import tunneloffload_pb2_grpc


TUNNEL_FORMATTED = """**********
Tunnel ID: {tunnel_id}
Tunnel Type: {tunnel_type}
**********"""

TUNNEL_CHAIN_SEPERATOR = "     ▼"

class TunnelValidationExcp(Exception):
    pass

class AddTunnelErrors:
   '''
   '''
   def __init__(self):
       self._addTunnelErrors= list()
   def addTunnelErrorMembers(self, tunnelResponseError):
       self._addTunnelErrors.append(tunnelResponseError)
   def __iter__(self):
       ''' Returns the Iterator object '''
       return AddTunnelErrorsIterator(self)

class AddTunnelErrorsIterator:
   ''' Iterator class '''
   def __init__(self, addTunnelErrors_list):
       # AddTunnelErrors object reference
       self._addTunnelErrors_list = addTunnelErrors_list
       # member variable to keep track of current index
       self._index = 0
   def __next__(self):
       ''''Returns the next value from team object's lists '''
       if self._index < (len(self._addTunnelErrors_list._addTunnelErrors) ) :
           result = (self._addTunnelErrors_list._addTunnelErrors[self._index])
           self._index +=1
           return result
       # End of Iteration
       raise StopIteration


class Tunnel(object):
    def __init__(self, tunnel):
        self.tunnel_proto = tunnel
        self.match_criteria = self.tunnel_proto.match_criteria
        self.tunnel_type = None
        self.counters = Counters()

    @staticmethod
    def validate_ipv6_pair(ipv6_pair):
        if len(ipv6_pair.sourceIp) != 32:
            raise TunnelValidationExcp("IPV6 Source IP Isn't valid")

        if len(ipv6_pair.destinationIp) != 32:
            raise TunnelValidationExcp("IPV6 Destination IP Isn't valid")

    @staticmethod
    def validate_mac_pair(mac_pair):
        if len(mac_pair.sourceMac) != 12:
            raise TunnelValidationExcp("Source MAC isn't valid in geneveEncap")
        
        if len(mac_pair.destinationMac) != 12:
            raise TunnelValidationExcp("Destination MAC isn't valid in geneveEncap")

    def check_geneve_encap_validity(self, geneve_encap):
        # Checking MAC's validity
        self.validate_mac_pair(geneve_encap.innerMacPair)

        if geneve_encap.vni > (2**24 -1):
            raise TunnelValidationExcp("VNI is more than 24 bit, invalid")

        outer_ip =  geneve_encap.WhichOneof('ip')
        if not outer_ip:
            raise TunnelValidationExcp("No outer IP encapsulation found on geneve encap, invalid")

        if outer_ip == 'outerIpv6Pair':
            self.validate_ipv6_pair(geneve_encap.outerIpv6Pair)

        
    def check_geneve_decap_validity(self, geneve_decap):
        pass

    def check_geneve_validity(self, geneve_params):
        geneve_encap_decap = geneve_params.WhichOneof('encap_decap')

        if geneve_encap_decap == 'geneveEncap':
            self.tunnel_type = 'GENEVE Encap'
            self.check_geneve_encap_validity(getattr(geneve_params, geneve_encap_decap))
        else:
            self.tunnel_type = 'GENEVE Decap'
            self.check_geneve_decap_validity(getattr(geneve_params, geneve_encap_decap))

    def check_ipsec_tunnel_validity(self, ipsec_params):
        ipsec_mode = ipsec_params.WhichOneof("ipsec")

        if ipsec_mode == "ipsecDec":
            self.tunnel_type = 'IPSec Decryption'
            ipsec_params = ipsec_params.ipsecDec
        else:
            self.tunnel_type = 'IPSEC Encryption'
            ipsec_params = ipsec_params.ipsecEnc 

        if ipsec_params.encryptionType not in [tunneloffload_pb2._AES256GCM8, tunneloffload_pb2._AES256GCM12, tunneloffload_pb2._AES256GCM16]:
            raise TunnelValidationExcp("Not encryption type set on tunnel")

        if len(ipsec_params.encryptionKey) != 64: # Checking the key length
            raise TunnelValidationExcp("Key used for IPSec isnt in right size of 256-bit")

        # Enc specific checks
        if ipsec_mode == "ipsecEnc":
            if ipsec_params.SPI == 0:
                raise TunnelValidationExcp("SPI isn't set on IPSec Enc Request")

    def check_tunnel_validity(self):

        TUNNEL_TO_VALIDITY_DICT = {
        'geneve': self.check_geneve_validity,
        'ipsecTunnel': self.check_ipsec_tunnel_validity,   
        }

        tunnel_type = self.tunnel_proto.WhichOneof("tunnel") 
        if tunnel_type is None:
            raise TunnelValidationExcp("No tunnel type passed to tunnel")

        if tunnel_type not in TUNNEL_TO_VALIDITY_DICT:
            raise TunnelValidationExcp("Internel problem in server, no validation to tunnel")

        TUNNEL_TO_VALIDITY_DICT[tunnel_type](getattr(self.tunnel_proto, tunnel_type))



def print_tunnel_summary(tunnels):
    sa_tunnels = []
    tunnels_chaining = []
    scanned_tunnels = []

    for tunnel_id, tunnel in tunnels.items():
        partial_chaining = False
        if tunnel_id in scanned_tunnels:
            continue

        scanned_tunnels.append(tunnel_id)

        if not tunnel.match_criteria.tunnelId:
            sa_tunnels.append(tunnel_id)
            continue

        matched_tunnel_id = tunnel.match_criteria.tunnelId[0]
        if matched_tunnel_id not in tunnels:
            raise TunnelValidationExcp(f"Tunnel id {tunnel_id} is matched traffic from " \
                                       f"tunnel id {matched_tunnel_id} which not exists")

        if tunnel.match_criteria.ipv4Match.sourceIp:
            partial_chaining = True
        
                    
        if matched_tunnel_id in sa_tunnels:
            sa_tunnels.remove(matched_tunnel_id)

        for chain in tunnels_chaining:
            if matched_tunnel_id in chain and not partial_chaining:
                chain.append(tunnel_id)
                break
        else:
            if partial_chaining:
                tunnels_chaining.append((matched_tunnel_id, tunnel_id))
            else:
                tunnels_chaining.append([matched_tunnel_id, tunnel_id])
            scanned_tunnels.append(matched_tunnel_id)

    print("The following tunnels are 'stand-alone' tunnels, "
          "and not chained to any other tunnel")

    for tunnel_id in sa_tunnels:
        print(TUNNEL_FORMATTED.format(tunnel_id=tunnel_id, tunnel_type=tunnels[tunnel_id].tunnel_type))


    for chain in tunnels_chaining:
        # import pudb; pudb.set_trace()
        if not isinstance(chain, tuple):
            print("----------------------------------------------")
            print("Following tunnels are chained to each other:")
            print("----------------------------------------------")
        else:
            print("----------------------------------------------")
            print("Following tunnels might be chained to each other, they match via tunnel")
            print("and shuold match via IP as well")
            print("----------------------------------------------")


        for i, tunnel in enumerate(chain):
            print(TUNNEL_FORMATTED.format(tunnel_id=tunnel, tunnel_type=tunnels[tunnel].tunnel_type))
            if i != len(chain) - 1:
                print(TUNNEL_CHAIN_SEPERATOR)
            else:
                print("\n")
    # print(tunnels_chaining)
                


    
class ipTunnelServiceServicer(tunneloffload_pb2_grpc.ipTunnelServiceServicer):
    """Provides methods that implement functionality of tunnel table server."""
    """ rpc createIpTunnels(ipTunnel) returns (createIpTunnelResponse) {} """


    def __init__(self):
        """do some init stuff"""
        self.tunnels = {} # List of tunnels according to ID's

    def createIpTunnel(self, request_iterator, context):
        tunnelErrors_value=AddTunnelErrors()
        for request in request_iterator:
            if request.operation == tunneloffload_pb2._CREATE:
                print(f"############ Create Tunnel ID - {request.tunnelId} ##################")
                print(request)

                print("Checking validity of the request")
                tunnel = Tunnel(request)
                tunnel.check_tunnel_validity()

                # Adding tunnel to be part of tunnels
                self.tunnels[request.tunnelId] = tunnel
            elif request.operation == tunneloffload_pb2._UPDATE:
                print(f"############ Update Tunnel ID - {request.tunnelId} ##################")
                tunnel = self.tunnels[request.tunnelId].tunnel_proto.match_criteria.CopyFrom(request.match_criteria)
                
            else:
                print(f"############# Removing tunnel {request.tunnelId} ##############")
                self.tunnels.pop(request.tunnelId, None)
            
        print("Printing tunnels summary")
        print_tunnel_summary(self.tunnels)

        return tunneloffload_pb2.createIpTunnelResponse(requestStatus=tunneloffload_pb2._TUNNEL_ACCEPTED)

    # Get request
    def getIpTunnel(self, request, context):
        
        res = tunneloffload_pb2.ipTunnelResponse()
        tunnel = self.tunnels[request.tunnelId]

        # Filling tunnel id 
        res.tunnelId = request.tunnelId

        # Counters
        res.tunnelCounters.inPackets = tunnel.counters.in_packets
        res.tunnelCounters.outPackets = tunnel.counters.out_packets
        res.tunnelCounters.inBytes = tunnel.counters.in_bytes
        res.tunnelCounters.outBytes = tunnel.counters.out_bytes
        res.tunnelCounters.inPacketsDrops = tunnel.counters.in_packets_drops
        res.tunnelCounters.outPacketsDrops = tunnel.counters.out_packets_drops
        res.tunnelCounters.inBytesDrops = tunnel.counters.in_bytes_drops
        res.tunnelCounters.outBytesDrops = tunnel.counters.out_bytes_drops

        # Filling the tunnel proto
        res.ipTunnel.CopyFrom(tunnel.tunnel_proto)

        return res
         

    # Capabilities
    def Capabilities(self, request, context):
        res = tunneloffload_pb2.CapabilityResponse()
        res.matchCapabilities.geneveMatching = True
        res.matchCapabilities.ingressInterfaceMatching = True
        res.matchCapabilities.spiMatching = True

        res.ipsecCapabilities.tunnelTypeSupported.extend([tunneloffload_pb2.TRANSPORT, tunneloffload_pb2.TUNNEL, tunneloffload_pb2.TRANSPORT_NAT_TRAVERSAL, tunneloffload_pb2.TUNNEL_NAT_TRAVERSAL])
        res.ipsecCapabilities.tunnelTypeSupported.extend([tunneloffload_pb2._SHA256])
        res.ipsecCapabilities.encryptionSupported.extend([tunneloffload_pb2._AES256GCM8, tunneloffload_pb2._AES256GCM12, tunneloffload_pb2._AES256GCM16])

        res.geneveCapabilities.geneveOptions = 5
        
        return res

def tunnelServe():
    server = grpc.server(futures.ThreadPoolExecutor(max_workers=5))
    tunneloffload_pb2_grpc.add_ipTunnelServiceServicer_to_server(
        ipTunnelServiceServicer(), server)
    with open('ssl/server.key', 'rb') as f:
         private_key = f.read()
    with open('ssl/server.crt', 'rb') as f:
         certificate_chain = f.read()
    server_credentials = grpc.ssl_server_credentials( ( (private_key, certificate_chain), ) )
    server.add_secure_port('localhost:3443', server_credentials)
    server.start()
    server.wait_for_termination()

if __name__ == '__main__':
    logging.basicConfig()
    tunnelServe()

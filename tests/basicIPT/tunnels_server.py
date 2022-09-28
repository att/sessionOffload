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
import random
import grpc
import google.protobuf.timestamp_pb2
from collections.abc import Iterable

from google.protobuf import any_pb2

from dataclasses import dataclass

@dataclass
class Counters:
    in_packets: int 
    in_packets_drops: int 
    in_bytes: int 
    in_bytes_drops: int
    out_packets: int
    out_packets_drops: int
    out_bytes: int
    out_bytes_drops: int 

import tunneloffload_pb2
import tunneloffload_pb2_grpc

SUPPORTED_IPSEC_ENC = [tunneloffload_pb2._AES256GCM64, tunneloffload_pb2._AES256GCM96, tunneloffload_pb2._AES256GCM128]

MIN_NUMBER_PACKETS = 1
MAX_NUMBER_PACKETS = 5000


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
        self.tunnel_id = self.tunnel_proto.tunnel_id
        self.match_criteria = self.tunnel_proto.match_criteria
        self.tunnel_type = None
        self.counters = self.generate_random_counters_for_sesssion()

    @staticmethod
    def generate_random_counters_for_sesssion():
        """
        @dataclass
        class Counters:
            in_packets: int 
            in_packets_drops: int 
            in_bytes: int 
            in_bytes_drops: int
            out_packets: int
            out_packets_drops: int
            out_bytes: int
            out_bytes_drops: int 
        """
        in_packets = random.randint(MIN_NUMBER_PACKETS, MAX_NUMBER_PACKETS)
        in_packets_bytes = in_packets * 8
        out_packets = random.randint(MIN_NUMBER_PACKETS, MAX_NUMBER_PACKETS)
        out_packets_bytes = out_packets * 8
        in_packets_drops = random.randint(MIN_NUMBER_PACKETS, MAX_NUMBER_PACKETS)
        in_packets_bytes_drops = in_packets_drops * 8
        out_packets_drops = random.randint(MIN_NUMBER_PACKETS, MAX_NUMBER_PACKETS)
        out_packets_bytes_drops = out_packets_drops * 8
        return Counters(in_packets=in_packets,
                        in_packets_drops=in_packets_drops,
                        in_bytes=in_packets_bytes,
                        in_bytes_drops=in_packets_bytes_drops,
                        out_packets=out_packets,
                        out_packets_drops=out_packets_drops,
                        out_bytes=out_packets_bytes,
                        out_bytes_drops=out_packets_bytes_drops)

    @staticmethod
    def validate_ipv6_pair(ipv6_pair):
        if len(ipv6_pair.source_ip) != 32:
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

        for option in geneve_encap.geneveOption:
            self.validate_geneve_option(option)

    @staticmethod
    def validate_geneve_option(option):
        print(f"Validating option {option}")

        if option.optionClass >= 2**16:
            raise TunnelValidationExcp("Optionclass is only 16 bits!")

        if option.type >= 2**8:
            raise TunnelValidationExcp("Type is only 8 bits!")

        if option.length >= 2**5:
            raise TunnelValidationExcp("Length of option is only 5 bits!")

        num_bytes_option_data = len(option.data)

        if (num_bytes_option_data % 4) != 0:
            raise TunnelValidationExcp("Data of an option must be multiply of 4")

        if num_bytes_option_data / 4 != option.length:
            raise TunnelValidationExcp("Option data and length field aren't matched!") 

            
            



        
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

        if ipsec_mode == "ipsec_dec":
            self.tunnel_type = 'IPSec Decryption'
            ipsec_params = ipsec_params.ipsec_dec
        else:
            self.tunnel_type = 'IPSEC Encryption'
            ipsec_params = ipsec_params.ipsec_enc

        if ipsec_params.encryption_type not in SUPPORTED_IPSEC_ENC:
            raise TunnelValidationExcp("Not encryption type set on tunnel")

        if ipsec_mode == "ipsec_enc":
            ipsec_sa_params_array = [ipsec_params.ipsec_sa]
        else:
            ipsec_sa_params_array = ipsec_params.ipsec_sas

        spi_found = False

        for ipsec_sa_params in ipsec_sa_params_array:
            if ipsec_sa_params.spi != 0:
                spi_found = True
                if ipsec_sa_params.operation !=  tunneloffload_pb2._DELETE and len(ipsec_sa_params.encryption_key) != 64: # Checking the key length
                    raise TunnelValidationExcp("Key used for IPSec isnt in right size of 256-bit")

            if not spi_found:
                raise TunnelValidationExcp("No SPI found on tunnel")

    def check_tunnel_validity(self):

        TUNNEL_TO_VALIDITY_DICT = {
        'geneve': self.check_geneve_validity,
        'ipsec_tunnel': self.check_ipsec_tunnel_validity,
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

        if not tunnel.match_criteria.tunnel_id:
            sa_tunnels.append(tunnel_id)
            continue

        matched_tunnel_id = tunnel.match_criteria.tunnel_id
        if matched_tunnel_id not in tunnels:
            raise TunnelValidationExcp(f"Tunnel id {tunnel_id} is matched traffic from " \
                                       f"tunnel id {matched_tunnel_id} which not exists")

        if tunnel.match_criteria.ipv4_match.source_ip:
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
                


    
class ip_tunnelServiceServicer(tunneloffload_pb2_grpc.IpTunnelServiceServicer):
    """Provides methods that implement functionality of tunnel table server."""
    """ rpc CreateIpTunnels(ip_tunnel) returns (createIpTunnelResponse) {} """


    def __init__(self):
        """do some init stuff"""
        self.tunnels = {} # List of tunnels according to ID's

    def CreateIpTunnel(self, request_iterator, context):
        tunnelErrors_value=AddTunnelErrors()
        for request in request_iterator:
            if request.operation == tunneloffload_pb2._CREATE:
                print(f"############ Create Tunnel ID - {request.tunnel_id} ##################")
                print(request)

                print("Checking validity of the request")
                tunnel = Tunnel(request)
                tunnel.check_tunnel_validity()

                # Adding tunnel to be part of tunnels
                self.tunnels[request.tunnel_id] = tunnel
            elif request.operation == tunneloffload_pb2._UPDATE:
                print(f"############ Update Tunnel ID - {request.tunnel_id} ##################")
                print(request)
                # This is just an example of how an update can work, and be enhanced
                self.tunnels[request.tunnel_id].tunnel_proto.ipsec_tunnel.CopyFrom(request.ipsec_tunnel)
            else:
                print(f"############# Removing tunnel {request.tunnel_id} ##############")
                print(request)
                self.tunnels.pop(request.tunnel_id, None)
            
        print("Printing tunnels summary")
        print_tunnel_summary(self.tunnels)

        return tunneloffload_pb2.CreateIpTunnelResponse()

    # Get request
    def GetIpTunnel(self, request, context):
        
        res = tunneloffload_pb2.IpTunnelResponse()
        tunnel = self.tunnels[request.tunnel_id]
        print(f"Getting IP Tunnel {request.tunnel_id}")

        # Filling tunnel id 
        res.tunnel_id = request.tunnel_id

        # Counters
        res.tunnel_counters.in_packets = tunnel.counters.in_packets
        res.tunnel_counters.out_packets = tunnel.counters.out_packets
        res.tunnel_counters.in_bytes = tunnel.counters.in_bytes
        res.tunnel_counters.out_bytes = tunnel.counters.out_bytes
        res.tunnel_counters.in_packets_drops = tunnel.counters.in_packets_drops
        res.tunnel_counters.out_packets_drops = tunnel.counters.out_packets_drops
        res.tunnel_counters.in_bytes_drops = tunnel.counters.in_bytes_drops
        res.tunnel_counters.out_bytes_drops = tunnel.counters.out_bytes_drops

        # Filling the tunnel proto
        res.ip_tunnel.CopyFrom(tunnel.tunnel_proto)
        print(res)

        return res
         
    def GetIpTunnelStats(self, request, context):
        
        res = tunneloffload_pb2.IpTunnelStatsResponse()
        tunnel = self.tunnels[request.tunnel_id]
        print(f"Getting IP Tunnel stats {request.tunnel_id}")

        # Filling tunnel id 
        res.tunnel_id = request.tunnel_id

        # Counters
        res.tunnel_counters.in_packets = tunnel.counters.in_packets
        res.tunnel_counters.out_packets = tunnel.counters.out_packets
        res.tunnel_counters.in_bytes = tunnel.counters.in_bytes
        res.tunnel_counters.out_bytes = tunnel.counters.out_bytes
        res.tunnel_counters.in_packets_drops = tunnel.counters.in_packets_drops
        res.tunnel_counters.out_packets_drops = tunnel.counters.out_packets_drops
        res.tunnel_counters.in_bytes_drops = tunnel.counters.in_bytes_drops
        res.tunnel_counters.out_bytes_drops = tunnel.counters.out_bytes_drops
        print(res)

        return res

    # Capabilities
    def Capabilities(self, request, context):
        res = tunneloffload_pb2.CapabilityResponse()
        res.MatchCapabilities.geneve_matching = True
        res.MatchCapabilities.ingress_interface_matching = True
        res.MatchCapabilities.spi_matching = True

        res.ipsec_capabilities.tunnel_type_supported.extend([tunneloffload_pb2.TRANSPORT, tunneloffload_pb2.TUNNEL, tunneloffload_pb2.TRANSPORT_NAT_TRAVERSAL, tunneloffload_pb2.TUNNEL_NAT_TRAVERSAL])
        res.ipsec_capabilities.encryption_supported.extend(SUPPORTED_IPSEC_ENC)

        res.geneve_capabilities.number_geneve_options_supported = 5
        
        return res

    def GetAllIpTunnelsStats(self, request, context):

        print("Got getAllIpTunnelsStats request!")
        print(f"Number of stats per response: {request.tunnels_per_request}")
        iteration_count = request.tunnels_per_request
        tunnels_to_iterate = list(self.tunnels.values())
        num_tunnels = len(tunnels_to_iterate)

        tunnels_to_send = []
        for i, tunnel in enumerate(tunnels_to_iterate):
            
            specific_tunnel = tunneloffload_pb2.IpTunnelStatsResponse()
            specific_tunnel.tunnel_id = tunnel.tunnel_id
            specific_tunnel.tunnel_counters.in_packets = tunnel.counters.in_packets
            specific_tunnel.tunnel_counters.out_packets = tunnel.counters.out_packets
            specific_tunnel.tunnel_counters.in_bytes = tunnel.counters.in_bytes
            specific_tunnel.tunnel_counters.out_bytes = tunnel.counters.out_bytes
            specific_tunnel.tunnel_counters.in_packets_drops = tunnel.counters.in_packets_drops
            specific_tunnel.tunnel_counters.out_packets_drops = tunnel.counters.out_packets_drops
            specific_tunnel.tunnel_counters.in_bytes_drops = tunnel.counters.in_bytes_drops
            specific_tunnel.tunnel_counters.out_bytes_drops = tunnel.counters.out_bytes_drops
            tunnels_to_send.append(specific_tunnel)

            if len(tunnels_to_send) == iteration_count or i == num_tunnels - 1:
                msg = tunneloffload_pb2.IpTunnelStatsResponses()
                msg.responses.extend(tunnels_to_send)
                tunnels_to_send = []
                print("Sending response in stream!")
                print(msg)
                yield msg
                

def tunnelServe():
    server = grpc.server(futures.ThreadPoolExecutor(max_workers=5))
    tunneloffload_pb2_grpc.add_IpTunnelServiceServicer_to_server(
        ip_tunnelServiceServicer(), server)
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

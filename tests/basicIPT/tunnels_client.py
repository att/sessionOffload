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


from __future__ import print_function

import random
import logging
from re import match
import socket
import struct
import sys

import grpc

import tunneloffload_pb2
import tunneloffload_pb2_grpc

def ipv4_to_int(ipv4):
    return int.from_bytes(socket.inet_pton(socket.AF_INET, ipv4), byteorder=sys.byteorder)


def tunnel_add_IPSEC_GENEVE(stub):
    ####
    ## IPSec Dec Tunnel
    ### 
    ipsec_dec_tunnel = tunneloffload_pb2.ipTunnelRequest()
    ipsec_dec_tunnel.tunnelId = 10000
    ipsec_dec_tunnel.nextAction = tunneloffload_pb2.RECIRCULATE
    # Assigning match critiera to IPSec
    match_criteria = ipsec_dec_tunnel.match_criteria
    match_criteria.ipsecMatch.spi = 780
    match_criteria.ipv4Match.sourceIp = ipv4_to_int("11.0.0.1")
    match_criteria.ipv4Match.sourceIpPrefix = 32
    match_criteria.ipv4Match.destinationIp = ipv4_to_int("11.0.0.2")
    match_criteria.ipv4Match.destinationIpPrefix = 32
    # Defining IPSec Decryption
    ipsec_params = ipsec_dec_tunnel.ipsecTunnel.ipsecDec
    ipsec_params.tunnelType = tunneloffload_pb2.TUNNEL_NAT_TRAVERSAL
    ipsec_params.encryptionType = 2
    ipsec_params.encryptionKey = b'f9cb6358b98ebdd029142048bdee473ac72bc1fad0325d61f68f85bcb06bb602' # 256 bit 


    ###
    ## GENEVE Encap Tunnel
    ###
    geneve_encap_tunnel=tunneloffload_pb2.ipTunnelRequest()
    geneve_encap_tunnel.tunnelId=10001
    geneve_encap_tunnel.nextAction = tunneloffload_pb2.FORWARD
    # Assigning the match criteria
    match_criteria = geneve_encap_tunnel.match_criteria
    match_criteria.tunnelId = ipsec_dec_tunnel.tunnelId

    # Assigning a GENEVE ENCAPSULATION
    # This geneve encapsulation is without any options
    geneve_encap = geneve_encap_tunnel.geneve.geneveEncap
    geneve_encap.outerIpv4Pair.sourceIp = ipv4_to_int("5.5.5.5")
    geneve_encap.outerIpv4Pair.destinationIp = ipv4_to_int("6.6.6.6")
    geneve_encap.innerMacPair.sourceMac = b'102030405060'
    geneve_encap.innerMacPair.destinationMac = b'605040302010'
    geneve_encap.vni = 500

    ###
    ## GENEVE Decap Tunnel
    ###
    geneve_decap_tunnel=tunneloffload_pb2.ipTunnelRequest()
    geneve_decap_tunnel.tunnelId=10002
    geneve_decap_tunnel.nextAction = tunneloffload_pb2.RECIRCULATE
    # Assigning the match criteria
    match_criteria = geneve_decap_tunnel.match_criteria
    match_criteria.ipv4Match.sourceIp = ipv4_to_int("11.0.0.1")
    match_criteria.ipv4Match.sourceIpPrefix = 32
    match_criteria.ipv4Match.destinationIp = ipv4_to_int("11.0.0.2")
    match_criteria.ipv4Match.destinationIpPrefix = 32
    match_criteria.geneveMatch.vni = 500

    # Assigning a GENEVE Decapsulation
    # SetInParent is used for detecting that is is the operation to perform
    geneve_decap_tunnel.geneve.geneveDecap.SetInParent()


    ####
    ## IPSec Enc Tunnel
    ### 
    ipsec_enc_tunnel = tunneloffload_pb2.ipTunnelRequest()
    ipsec_enc_tunnel.tunnelId = 10003
    ipsec_enc_tunnel.nextAction = tunneloffload_pb2.FORWARD
    # Assigning match critiera to IPSec
    match_criteria = ipsec_enc_tunnel.match_criteria
    match_criteria.tunnelId = geneve_decap_tunnel.tunnelId

    # Defining IPSec Encryption
    ipsec_params = ipsec_enc_tunnel.ipsecTunnel.ipsecEnc
    ipsec_params.SPI = 4587
    ipsec_params.tunnelType = tunneloffload_pb2.TUNNEL_NAT_TRAVERSAL
    ipsec_params.encryptionType = 2
    ipsec_params.encryptionKey = b'f9cb6358b98ebdd029142048bdee473ac72bc1fad0325d61f68f85bcb06bb602' # 256 bit 
    ipsec_params.ipv4_tunnel.sourceIp = ipv4_to_int("12.0.0.1")
    ipsec_params.ipv4_tunnel.destinationIp = ipv4_to_int("12.0.0.2")

    # Making iterator of one in order to send via gNMI
    add_tunnels_iterators = iter([geneve_decap_tunnel, ipsec_enc_tunnel])

    print("Sending the request!")
    stub.createIpTunnel(add_tunnels_iterators)

    # Making iterator of one in order to send via gNMI
    add_tunnels_iterators = iter([ipsec_dec_tunnel, geneve_encap_tunnel, geneve_decap_tunnel, ipsec_enc_tunnel])

    print("Sending the request!")
    stub.createIpTunnel(add_tunnels_iterators)

def run():
    # NOTE(gRPC Python Team): tunneloffload_pb2.close() is possible on a channel and should be
    # used in circumstances in which the with statement does not fit the needs
    # of the code.
    with open('ssl/server.crt', 'rb') as f:
        creds = grpc.ssl_channel_credentials(f.read())
        channel = grpc.secure_channel('localhost:3443', creds)
        stub = tunneloffload_pb2_grpc.ipTunnelServiceStub(channel)
        # needs to be turned into a commmand line argument so that robot can run it without the inteartive debugger
        #import pudb; pudb.set_trace()
        tunnel_add_IPSEC_GENEVE(stub)

if __name__ == '__main__':
    # set_trace causes robot tests to not complete
    #import pudb; pudb.set_trace()
    logging.basicConfig()
    run()


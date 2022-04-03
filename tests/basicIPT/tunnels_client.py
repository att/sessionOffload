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

from typing import NamedTuple

import grpc

import tunneloffload_pb2
import tunneloffload_pb2_grpc

from tunnels_helper import create_geneve_decap, create_geneve_encap, create_ipsec_dec_tunnel, create_ipsec_enc_tunnel, Match


def tunnel_add_IPSEC_GENEVE(stub):
    ####
    ## IPSec Dec Tunnel
    ### 
    match = Match(source_ip="11.0.0.1",
                  dest_ip="11.0.0.2",
                  spi=780)

    ipsec_dec_tunnel = create_ipsec_dec_tunnel(tunnelid=10000, 
                                               enc_type=tunneloffload_pb2._AES256GCM8,
                                               tunnel_type=tunneloffload_pb2.TUNNEL_NAT_TRAVERSAL,
                                               match=match)


    ###
    ## GENEVE Encap Tunnel
    ###
    match = Match(tunnel_id=ipsec_dec_tunnel.tunnelId)

    geneve_encap_tunnel = create_geneve_encap(match=match,
                                              tunnelid=10001,
                                              next_action=tunneloffload_pb2.FORWARD,
                                              geneve_source_ip="5.5.5.5",
                                              geneve_dest_ip="6.6.6.6",
                                              geneve_source_mac="102030405060",
                                              geneve_dest_mac="605040302010",
                                              geneve_vni=500)



    ###
    ## GENEVE Decap Tunnel
    ###
    match = Match(source_ip="11.0.0.1",
                  dest_ip="11.0.0.2",
                  geneve_vni=500)
                  

    geneve_decap_tunnel = create_geneve_decap(tunnelid=10002,
                                              match=match)


    ####
    ## IPSec Enc Tunnel
    ### 
    match = Match(tunnel_id=geneve_decap_tunnel.tunnelId)

    ipsec_enc_tunnel = create_ipsec_enc_tunnel(tunnelid=10003,
                                               match=match,
                                               next_action=tunneloffload_pb2.FORWARD,
                                               tunnel_source_ip="11.0.0.2",
                                               tunnel_destination_ip="11.0.0.1",
                                               spi=4587,
                                               tunnel_type=tunneloffload_pb2.TUNNEL_NAT_TRAVERSAL,
                                               enc_type=tunneloffload_pb2._AES256GCM8)

    # Making iterator of one in order to send via gNMI
    add_tunnels_iterators = iter([ipsec_dec_tunnel, geneve_encap_tunnel, geneve_decap_tunnel, ipsec_enc_tunnel])

    print("Sending IPSec Tunnel Request")
    stub.createIpTunnel(add_tunnels_iterators)

    ####
    # In this scenario we're doing rekey of the ipsec scenario
    ####
    
    # Doing rekey is creating another tunnel with different tunnel ID and than removing
    # the other tunnel
    # We've already created the ipsec tunnel 10003, we'll perform rekey by creating another tunnel ID with
    # different SPI and different match
    ipsec_enc_tunnel_rekey = create_ipsec_enc_tunnel(tunnelid=10011,
                                                     match=match,
                                                     next_action=tunneloffload_pb2.FORWARD,
                                                     tunnel_source_ip="11.0.0.2",
                                                     tunnel_destination_ip="11.0.0.1",
                                                     spi=4587,
                                                     tunnel_type=tunneloffload_pb2.TUNNEL_NAT_TRAVERSAL,
                                                     enc_type=tunneloffload_pb2._AES256GCM8)

    # Chaning match of previous tunnel
    ipsec_enc_disabled = tunneloffload_pb2.ipTunnelRequest()
    ipsec_enc_disabled.tunnelId = 10003
    ipsec_enc_disabled.operation = tunneloffload_pb2._DELETE

    print("Performing REKEY!")
    tunnels_iterators = iter([ipsec_enc_tunnel_rekey, ipsec_enc_disabled])
    stub.createIpTunnel(tunnels_iterators)    


def four_tunnel_chain(stub):

    BASE_TUNNEL_ID = 20000

    match = Match(source_ip="12.0.0.1",
                  dest_ip="12.0.0.2",
                  spi=980)

    ipsec_dec_tunnel = create_ipsec_dec_tunnel(tunnelid=BASE_TUNNEL_ID, 
                                               enc_type=tunneloffload_pb2._AES256GCM8,
                                               tunnel_type=tunneloffload_pb2.TUNNEL_NAT_TRAVERSAL,
                                               match=match)

    ###
    ## GENEVE Decap Tunnel
    ###
    match = Match(tunnel_id=ipsec_dec_tunnel.tunnelId)
                  

    geneve_decap_tunnel = create_geneve_decap(tunnelid=BASE_TUNNEL_ID+1,
                                              match=match)


    ###
    ## GENEVE Encap Tunnel
    ###
    match = Match(tunnel_id=geneve_decap_tunnel.tunnelId)

    geneve_encap_tunnel = create_geneve_encap(match=match,
                                              tunnelid=BASE_TUNNEL_ID+2,
                                              geneve_source_ip="7.7.7.7",
                                              geneve_dest_ip="8.8.8.8",
                                              geneve_source_mac="102030405060",
                                              geneve_dest_mac="605040302010",
                                              geneve_vni=700)



    ####
    ## IPSec Enc Tunnel
    ### 
    match = Match(spi=920,
                  tunnel_id=geneve_encap_tunnel.tunnelId)

    ipsec_enc_tunnel = create_ipsec_enc_tunnel(tunnelid=BASE_TUNNEL_ID+3,
                                               match=match,
                                               next_action=tunneloffload_pb2.FORWARD,
                                               tunnel_source_ip="13.0.0.2",
                                               tunnel_destination_ip="13.0.0.1",
                                               spi=45888,
                                               tunnel_type=tunneloffload_pb2.TUNNEL_NAT_TRAVERSAL,
                                               enc_type=tunneloffload_pb2._AES256GCM8)

    # Making iterator of one in order to send via gNMI
    add_tunnels_iterators = iter([ipsec_dec_tunnel, geneve_decap_tunnel,  geneve_encap_tunnel, ipsec_enc_tunnel])

    print("Sending IPSec Tunnel Request")
    stub.createIpTunnel(add_tunnels_iterators)


def tunnel_recursion_without_tunnel_id(stub):

    BASE_TUNNEL_ID = 30000

    match = Match(source_ip="2.2.2.2",
                  dest_ip="3.3.3.3")

    ipsec_enc_tunnel = create_ipsec_enc_tunnel(tunnelid=BASE_TUNNEL_ID,
                                               match=match,
                                               tunnel_source_ip="14.0.0.2",
                                               tunnel_destination_ip="14.0.0.1",
                                               spi=50000,
                                               tunnel_type=tunneloffload_pb2.TUNNEL_NAT_TRAVERSAL,
                                               enc_type=tunneloffload_pb2._AES256GCM8)

    match = Match(source_ip="14.0.0.2",
                  dest_ip="14.0.0.1")



    geneve_encap_tunnel = create_geneve_encap(match=match,
                                              tunnelid=BASE_TUNNEL_ID+1,
                                              geneve_source_ip="9.9.9.9",
                                              geneve_dest_ip="10.10.10.10",
                                              geneve_source_mac="102030405060",
                                              geneve_dest_mac="605040302010",
                                              geneve_vni=701)

    # Making iterator of one in order to send via gNMI
    add_tunnels_iterators = iter([ipsec_enc_tunnel, geneve_encap_tunnel])

    print("Sending IPSec Tunnel Request")
    stub.createIpTunnel(add_tunnels_iterators)


def tunnel_recursion_with_tunnel_id_and_ip(stub):
    """
    This is an example of match on tunnel ID & IP's as well
    The first tunnel is IPSec Decryption with Tunnel Mode, the inner IP can be any IP (Tunnel ID 40000)    
    The other two IPSec tunnels are GENEVE tunnels that matches on Tunnel ID 40000 and IP,
    According to the inner IP of IPSec tunnel mode (that will be outer IP after decryption) the next tunnel is 
    the chain will be matched 
    
    """

    match = Match(source_ip="2.2.2.2",
                  dest_ip="3.3.3.3",
                  spi=80000)

    ipsec_enc_tunnel = create_ipsec_dec_tunnel(tunnelid=40000,
                                               match=match,
                                               tunnel_type=tunneloffload_pb2.TUNNEL_NAT_TRAVERSAL,
                                               enc_type=tunneloffload_pb2._AES256GCM8)

    match = Match(tunnel_id=40000,
                  source_ip="15.0.0.2",
                  dest_ip="15.0.0.1")

    geneve_encap_tunnel_1 = create_geneve_encap(match=match,
                                              tunnelid=40001,
                                              geneve_source_ip="9.9.9.9",
                                              geneve_dest_ip="10.10.10.10",
                                              geneve_source_mac="102030405060",
                                              geneve_dest_mac="605040302010",
                                              geneve_vni=701)

    match = Match(tunnel_id=40000,
                  source_ip="16.0.0.2",
                  dest_ip="16.0.0.1")

    geneve_encap_tunnel_2 = create_geneve_encap(match=match,
                                              tunnelid=40002,
                                              geneve_source_ip="11.11.11.11",
                                              geneve_dest_ip="12.12.12.12",
                                              geneve_source_mac="102030405060",
                                              geneve_dest_mac="605040302010",
                                              geneve_vni=702)


    # Making iterator of one in order to send via gNMI
    add_tunnels_iterators = iter([ipsec_enc_tunnel, geneve_encap_tunnel_1, geneve_encap_tunnel_2])

    print("Sending IPSec Tunnel Request")
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
        a = tunneloffload_pb2.tunnelId()
        a.tunnelId = 10000
        res = stub.getIpTunnel(a)
        print(res)
        four_tunnel_chain(stub)
        tunnel_recursion_without_tunnel_id(stub)
        tunnel_recursion_with_tunnel_id_and_ip(stub)

if __name__ == '__main__':
    # set_trace causes robot tests to not complete
    #import pudb; pudb.set_trace()
    logging.basicConfig()
    run()


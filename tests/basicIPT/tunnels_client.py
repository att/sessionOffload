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
import socket
import struct
import sys

import grpc

import tunneloffload_pb2
import tunneloffload_pb2_grpc

def ipv4_to_int(ipv4):
    return int.from_bytes(socket.inet_pton(socket.AF_INET, ipv4), byteorder=sys.byteorder)



def geneveEncapsulation(stub):
    tunnel=tunneloffload_pb2.ipTunnelRule()
    
    # Assigning the match criteria
    match_criteria = tunnel.match_criteria
    match_criteria.ipv4Match.sourceIp = int.from_bytes(socket.inet_pton(socket.AF_INET, "10.0.0.1"), byteorder=sys.byteorder)
    match_criteria.ipv4Match.sourceIpPrefix = 32
    match_criteria.ipv4Match.destinationIp = int.from_bytes(socket.inet_pton(socket.AF_INET, "10.0.0.1"), byteorder=sys.byteorder)
    match_criteria.ipv4Match.destinationIpPrefix = 32

    # Assigning a GENEVE ENCAPSULATION
    # This geneve encapsulation is without any options
    geneve_encap = tunnel.geneve.geneveEncap
    geneve_encap.outerIpv4Pair.sourceIp = ipv4_to_int("5.5.5.5")
    geneve_encap.outerIpv4Pair.destinationIp = ipv4_to_int("6.6.6.6")
    geneve_encap.innerMacPair.sourceMac = b'102030405060'
    geneve_encap.innerMacPair.destinationMac = b'60504302010'
    geneve_encap.vni = 500

    # Making iterator of one in order to send via gNMI
    add_tunnels_iterators = iter([tunnel])

    print("Sending the request!")

    stub.createIpTunnel(add_tunnels_iterators)


    

    
    
    # session.sessionId= 12345678910
    # session.inLif= 1
    # session.outLif= 2
    # session.sourceIp=int.from_bytes(socket.inet_pton(socket.AF_INET, "10.0.0.1"), byteorder=sys.byteorder)
    # session.sourcePort=12345
    # session.destinationIp=int.from_bytes(socket.inet_pton(socket.AF_INET, "10.1.0.3"), byteorder=sys.byteorder)
    # session.destinationPort=80
    # session.protocolId=openoffload_pb2._TCP
    # session.ipVersion=openoffload_pb2._IPV4
    # session.action.actionType=openoffload_pb2._FORWARD
    # session.action.actionNextHop = int.from_bytes(socket.inet_pton(socket.AF_INET,"12.2.3.4"),byteorder=sys.byteorder)
    # sessions_value=Sessions()
    # sessions_value.addSessionMembers(session)
    # session_iterator=iter(sessions_value)
    # addSessionResponse =  stub.addSession( session_iterator)
    # print("addSessionResponse:",addSessionResponse.requestStatus)

    # sessionErrors_value=addSessionResponse.responseError
    # sessionErrors_iterator=iter(sessionErrors_value)

    # for sessionError in sessionErrors_iterator:
    #      print("addSessionErrorResponse:",sessionError.requestStatus)

    # return addSessionResponse.requestStatus




def run():
    # NOTE(gRPC Python Team): tunneloffload_pb2.close() is possible on a channel and should be
    # used in circumstances in which the with statement does not fit the needs
    # of the code.
    with open('ssl/server.crt', 'rb') as f:
        creds = grpc.ssl_channel_credentials(f.read())
        channel = grpc.secure_channel('localhost:3443', creds)
        stub = tunneloffload_pb2_grpc.ipTunnelServiceStub(channel)
        import pudb; pudb.set_trace()
        geneveEncapsulation(stub)

if __name__ == '__main__':
    import pudb
    logging.basicConfig()
    run()


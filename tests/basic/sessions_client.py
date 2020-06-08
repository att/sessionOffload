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

import grpc

import sessions_pb2
import sessions_pb2_grpc

def session_addSession(stub):
    session=sessions_pb2.sessionRequest()
    session.inLif="ge0/0/1"
    session.outLif="ge0/0/2"
    session.sourceIp=socket.inet_pton(socket.AF_INET, "10.0.0.1")
    session.sourcePort=12345
    session.destinationIp=socket.inet_pton(socket.AF_INET, "10.1.0.3")
    session.destinationPort=80
    session.protocolId=sessions_pb2._TCP
    session.ipVersion=sessions_pb2._IPV4
    session.action.actionType=sessions_pb2._FORWARD
    session.action.actionNextHop = "12.2.3.4"
    sessionResponse =  stub.addSession( session)
    print("Adding Session")
    print(sessionResponse.sessionId)

def session_addSession_ipv6(stub):
    session=sessions_pb2.sessionRequest()
    session.inLif="ge0/0/1"
    session.outLif="ge0/0/2"
    session.sourceIp=socket.inet_pton(socket.AF_INET6, "2001:0db8:85a3:0000:0000:8a2e:0370:7332")
    session.sourcePort=4430
    session.destinationIp=socket.inet_pton(socket.AF_INET6, "2001:0db8:85a3:0000:0000:8a03:0370:234F")
    session.destinationPort=80
    session.protocolId=sessions_pb2._TCP
    session.ipVersion=sessions_pb2._IPV6
    session.action.actionType=sessions_pb2._FORWARD
    session.action.actionNextHop = "12.2.3.4"
    sessionResponse =  stub.addSession( session)
    print("Adding Session")
    print(sessionResponse.sessionId)

def session_getSession(stub):
    sessionResponse =  stub.getSession( sessions_pb2.sessionId(sessionId=1001))
    print("Getting Session")
    print("SessionId:", sessionResponse.sessionId)
    print("Session State:", sessions_pb2._SESSION_STATE.values_by_number[sessionResponse.sessionState].name)
    print("Session RequestStatus:",sessionResponse.requestStatus)
    print("Session SessionCloseCode:",sessionResponse.sessionCloseCode)
    print("Session InPackets",sessionResponse.inPackets)
    print("Session OutPackets",sessionResponse.outPackets)
    print("Session startTime",sessionResponse.startTime)
    print("Session endTime",sessionResponse.endTime)

def session_deleteSession(stub):
    sessionResponse =  stub.deleteSession( sessions_pb2.sessionId(sessionId=1001))
    print("Getting Session")
    print("SessionId:", sessionResponse.sessionId)
    print("Session RequestStatus:",sessionResponse.requestStatus)
    print("Session State:", sessions_pb2._SESSION_STATE.values_by_number[sessionResponse.sessionState].name)
    print("Session RequestStatus:",sessionResponse.requestStatus)
    print("Session SessionCloseCode:",sessionResponse.sessionCloseCode)
    print("Session InPackets",sessionResponse.inPackets)
    print("Session OutPackets",sessionResponse.outPackets)
    print("Session startTime",sessionResponse.startTime)
    print("Session endTime",sessionResponse.endTime)

def session_addMirrorSession(stub):
    session=sessions_pb2.sessionRequest()
    session.inLif="ge0/1/1"
    session.outLif="ge0/1/2"
    session.sourceIp=socket.inet_pton(socket.AF_INET, "10.0.0.1")
    session.sourcePort=12345
    session.destinationIp=socket.inet_pton(socket.AF_INET, "10.1.0.3")
    session.destinationPort=80
    session.protocolId=sessions_pb2._UDP
    session.action.actionType=sessions_pb2._MIRROR
    session.action.actionNextHop = "12.2.3.4"
    sessionResponse =  stub.addSession( session)
    print("Adding Session")
    print(sessionResponse.sessionId)


def session_getClosedSessions(stub):
    for sessionResponse in stub.getClosedSessions(sessions_pb2.statisticsRequestArgs(pageSize=0)):
        print("### RECEIVED SESSION #####")
        print("SessionId:", sessionResponse.sessionId)
        print("Session State:", sessions_pb2._SESSION_STATE.values_by_number[sessionResponse.sessionState].name)
        print("Session RequestStatus:",sessionResponse.requestStatus)
        print("Session SessionCloseCode:",sessionResponse.sessionCloseCode)
        print("Session InPackets",sessionResponse.inPackets)
        print("Session OutPackets",sessionResponse.outPackets)
        print("Session startTime",sessionResponse.startTime)
        print("Session endTime",sessionResponse.endTime)
        print("##########################")


def session_getAllSessions(stub):
    for sessionResponse in stub.getClosedSessions(sessions_pb2.statisticsRequestArgs(pageSize=0)):
        print("### RECEIVED SESSION #####")
        print("SessionId:", sessionResponse.sessionId)
        print("Session State:", sessions_pb2._SESSION_STATE.values_by_number[sessionResponse.sessionState].name)
        print("Session RequestStatus:",sessionResponse.requestStatus)
        print("Session SessionCloseCode:",sessionResponse.sessionCloseCode)
        print("Session InPackets",sessionResponse.inPackets)
        print("Session OutPackets",sessionResponse.outPackets)
        print("Session startTime",sessionResponse.startTime)
        print("Session endTime",sessionResponse.endTime)
        print("##########################")

def run():
    # NOTE(gRPC Python Team): sessions_pb2.close() is possible on a channel and should be
    # used in circumstances in which the with statement does not fit the needs
    # of the code.
    with open('ssl/server.crt', 'rb') as f:
        creds = grpc.ssl_channel_credentials(f.read())
        channel = grpc.secure_channel('localhost:3443', creds)
        stub = sessions_pb2_grpc.SessionTableStub(channel)
        print("-------------- Add IPv4 Session --------------")
        session_addSession(stub)
        print("-------------- Add IPv6 Session --------------")
        session_addSession_ipv6(stub)
        print("-------------- Get Session --------------")
        session_getSession(stub)
        print("-------------- Delete Session --------------")
        session_deleteSession(stub)
        print("-------------- Add Mirror and Forward Session --------------")
        session_addMirrorSession(stub)

    with open('ssl/server.crt', 'rb') as f:
        creds = grpc.ssl_channel_credentials(f.read())
        statsChannel = grpc.secure_channel('localhost:3444', creds)
        statsStub = sessions_pb2_grpc.SessionStatisticsTableStub(statsChannel)
        print("-------------- Check for Closed Sessions --------------")
        session_getClosedSessions(statsStub)
        session_getAllSessions(statsStub)


if __name__ == '__main__':
    logging.basicConfig()
    run()


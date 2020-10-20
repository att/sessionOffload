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

import openoffload_pb2
import openoffload_pb2_grpc


class Sessions:
   '''
   '''
   def __init__(self):
       self._sessions= list()
   def addSessionMembers(self, session):
       self._sessions.append(session)
   def __iter__(self):
       ''' Returns the Iterator object '''
       return SessionsIterator(self)

class SessionsIterator:
   ''' Iterator class '''
   def __init__(self, session_list):
       # Sessions object reference
       self._session_list = session_list
       # member variable to keep track of current index
       self._index = 0
   def __next__(self):
       ''''Returns the next value from team object's lists '''
       if self._index < (len(self._session_list._sessions) ) :
           result = (self._session_list._sessions[self._index])
           self._index +=1
           return result
       # End of Iteration
       raise StopIteration


def session_addSession(stub):
    session=openoffload_pb2.sessionRequest()
    session.sessionId= 12345678910
    session.inLif= 1
    session.outLif= 2
    session.sourceIp=int.from_bytes(socket.inet_pton(socket.AF_INET, "10.0.0.1"), byteorder=sys.byteorder)
    session.sourcePort=12345
    session.destinationIp=int.from_bytes(socket.inet_pton(socket.AF_INET, "10.1.0.3"), byteorder=sys.byteorder)
    session.destinationPort=80
    session.protocolId=openoffload_pb2._TCP
    session.ipVersion=openoffload_pb2._IPV4
    session.action.actionType=openoffload_pb2._FORWARD
    session.action.actionNextHop = int.from_bytes(socket.inet_pton(socket.AF_INET,"12.2.3.4"),byteorder=sys.byteorder)
    sessions_value=Sessions()
    sessions_value.addSessionMembers(session)
    session_iterator=iter(sessions_value)
    addSessionResponse =  stub.addSession( session_iterator)
    print("addSessionResponse:",addSessionResponse.requestStatus)
    return addSessionResponse.requestStatus

def session_addSession_ipv6(stub):
    session=openoffload_pb2.sessionRequest()
    session.sessionId= 12345678910
    session.inLif= 1
    session.outLif= 2
    session.sourceIpV6=socket.inet_pton(socket.AF_INET6, "2001:0db8:85a3:0000:0000:8a2e:0370:7332")
    session.sourcePort=4430
    session.destinationIpV6=socket.inet_pton(socket.AF_INET6, "2001:0db8:85a3:0000:0000:8a03:0370:234F")
    session.destinationPort=80
    session.protocolId=openoffload_pb2._TCP
    session.ipVersion=openoffload_pb2._IPV6
    session.action.actionType=openoffload_pb2._FORWARD
    session.action.actionNextHop =int.from_bytes(socket.inet_pton(socket.AF_INET, "12.2.3.4"), byteorder=sys.byteorder)
    sessions_value=Sessions()
    sessions_value.addSessionMembers(session)
    session_iterator=iter(sessions_value)
    addSessionResponse =  stub.addSession( session_iterator)
    print("addSessionResponse:",addSessionResponse.requestStatus)
    return addSessionResponse.requestStatus

def session_getSession(stub):
    sessionResponse =  stub.getSession( openoffload_pb2.sessionId(sessionId=1001))
    print("Getting Session")
    print("SessionId:", sessionResponse.sessionId)
    print("Session State:", openoffload_pb2._SESSION_STATE.values_by_number[sessionResponse.sessionState].name)
    print("Session RequestStatus:",sessionResponse.requestStatus)
    print("Session SessionCloseCode:",sessionResponse.sessionCloseCode)
    print("Session InPackets",sessionResponse.inPackets)
    print("Session OutPackets",sessionResponse.outPackets)
    print("Session startTime",sessionResponse.startTime)
    print("Session endTime",sessionResponse.endTime)

def session_deleteSession(stub):
    sessionResponse =  stub.deleteSession( openoffload_pb2.sessionId(sessionId=1001))
    print("Getting Session")
    print("SessionId:", sessionResponse.sessionId)
    print("Session RequestStatus:",sessionResponse.requestStatus)
    print("Session State:", openoffload_pb2._SESSION_STATE.values_by_number[sessionResponse.sessionState].name)
    print("Session RequestStatus:",sessionResponse.requestStatus)
    print("Session SessionCloseCode:",sessionResponse.sessionCloseCode)
    print("Session InPackets",sessionResponse.inPackets)
    print("Session OutPackets",sessionResponse.outPackets)
    print("Session startTime",sessionResponse.startTime)
    print("Session endTime",sessionResponse.endTime)

def session_addMirrorSession(stub):
    session=openoffload_pb2.sessionRequest()
    session.sessionId= 12345678910
    session.inLif= 1
    session.outLif= 2
    session.sourceIp=int.from_bytes(socket.inet_pton(socket.AF_INET, "10.0.0.1"), byteorder=sys.byteorder)
    session.sourcePort=12345
    session.destinationIp=int.from_bytes(socket.inet_pton(socket.AF_INET, "10.1.0.4"), byteorder=sys.byteorder)
    session.destinationPort=80
    session.protocolId=openoffload_pb2._UDP
    session.action.actionType=openoffload_pb2._MIRROR
    session.action.actionNextHop=int.from_bytes(socket.inet_pton(socket.AF_INET, "12.2.3.4"), byteorder=sys.byteorder)
    sessions_value=Sessions()
    sessions_value.addSessionMembers(session)
    session_iterator=iter(sessions_value)
    sessionResponse =  stub.addSession( session_iterator)
    print("SessionResponse:",sessionResponse.requestStatus)
    return sessionResponse.requestStatus


def session_getClosedSessions(stub):
    for sessionResponse in stub.getClosedSessions(openoffload_pb2.statisticsRequestArgs(pageSize=0)):
        print("### RECEIVED SESSION #####")
        print("SessionId:", sessionResponse.sessionId)
        print("Session State:", openoffload_pb2._SESSION_STATE.values_by_number[sessionResponse.sessionState].name)
        print("Session RequestStatus:",sessionResponse.requestStatus)
        print("Session SessionCloseCode:",sessionResponse.sessionCloseCode)
        print("Session InPackets",sessionResponse.inPackets)
        print("Session OutPackets",sessionResponse.outPackets)
        print("Session startTime",sessionResponse.startTime)
        print("Session endTime",sessionResponse.endTime)
        print("##########################")


def session_getAllSessions(stub):
    for sessionResponse in stub.getClosedSessions(openoffload_pb2.statisticsRequestArgs(pageSize=0)):
        print("### RECEIVED SESSION #####")
        print("SessionId:", sessionResponse.sessionId)
        print("Session State:", openoffload_pb2._SESSION_STATE.values_by_number[sessionResponse.sessionState].name)
        print("Session RequestStatus:",sessionResponse.requestStatus)
        print("Session SessionCloseCode:",sessionResponse.sessionCloseCode)
        print("Session InPackets",sessionResponse.inPackets)
        print("Session OutPackets",sessionResponse.outPackets)
        print("Session startTime",sessionResponse.startTime)
        print("Session endTime",sessionResponse.endTime)
        print("##########################")

def activation_registerDevice(stub):
    register = openoffload_pb2.deviceDescription()
    register.name="Acme-1"
    register.description = "Acme SmartNIC"
    register.type= openoffload_pb2._SMARTNIC
    register.sessionCapacity = 2000000
    register.sessionRate = 100000
    register.tcpSessionTimeout = 15
    register.udpSessionTimeout = 30
    registerResponse =  stub.registerOffloadDevice(register)
    print("Adding Device Description: ", register.name)
    print("Status: ", openoffload_pb2._REGISTRATION_STATUS_TYPE.values_by_number[registerResponse.status].name)
    register = openoffload_pb2.deviceDescription()
    register.name="XDP"
    register.description = "Software Implementation"
    register.type= openoffload_pb2._SOFTWARE
    register.sessionCapacity = 200000
    register.sessionRate = 10000
    register.tcpSessionTimeout = 15
    register.udpSessionTimeout = 30
    registerResponse =  stub.registerOffloadDevice(register)
    print("Adding Device Description: ", register.name)
    print("Status: ", openoffload_pb2._REGISTRATION_STATUS_TYPE.values_by_number[registerResponse.status].name)

def activation_getAllDevices(stub):
    Devices = stub.getRegisteredOffloadDevices(openoffload_pb2.Empty())
    listDevices = Devices.devices
    for device in listDevices:
        print("### Registered Device ####")
        print("Name: ", device.name)
        print("Description: ", device.description)
        print("Type: ", openoffload_pb2._INTERFACE_TYPE.values_by_number[device.type].name)
        print("Session Capacity: ", device.sessionCapacity)
        print("Session Rate: ",device.sessionRate)
        print("TCP Session Timeout: ", device.tcpSessionTimeout)
        print("UDP Session Timeout: ",device.udpSessionTimeout)

def activation_activateDevice(stub):
    register = openoffload_pb2.deviceDescription()
    print("### Activating Device ####")
    register.name="Acme-1"
    register.description = "Acme SmartNIC"
    register.type= openoffload_pb2._SMARTNIC
    register.sessionCapacity = 2000000
    register.sessionRate = 100000
    register.tcpSessionTimeout = 15
    register.udpSessionTimeout = 30
    activateStatus = stub.activateOffload(register)
    print("Activated Device: ", register.name)
    print("Status: ", openoffload_pb2._ACTIVATION_STATUS_TYPE.values_by_number[activateStatus.status].name)

def run_add_session_ipv4():
    with open('ssl/server.crt', 'rb') as f:
        creds = grpc.ssl_channel_credentials(f.read())
        channel = grpc.secure_channel('localhost:3443', creds)
        stub = openoffload_pb2_grpc.SessionTableStub(channel)
        print("-------------- Add IPv4 Session --------------")
        result = session_addSession(stub)
        print("Request Status=",result)
def run_add_session_ipv6():
    with open('ssl/server.crt', 'rb') as f:
        creds = grpc.ssl_channel_credentials(f.read())
        channel = grpc.secure_channel('localhost:3443', creds)
        stub = openoffload_pb2_grpc.SessionTableStub(channel)
        print("-------------- Add IPv6 Session --------------")
        result = session_addSession_ipv6(stub)
        print("RequestStatus=",result)
def run_get_session():
    with open('ssl/server.crt', 'rb') as f:
        creds = grpc.ssl_channel_credentials(f.read())
        channel = grpc.secure_channel('localhost:3443', creds)
        stub = openoffload_pb2_grpc.SessionTableStub(channel)
        print("-------------- Get Session --------------")
        session_getSession(stub)
def run_delete_session():
    with open('ssl/server.crt', 'rb') as f:
        creds = grpc.ssl_channel_credentials(f.read())
        channel = grpc.secure_channel('localhost:3443', creds)
        stub = openoffload_pb2_grpc.SessionTableStub(channel)
        print("-------------- Delete Session --------------")
        session_deleteSession(stub)
def run_add_mirror_session():
    with open('ssl/server.crt', 'rb') as f:
        creds = grpc.ssl_channel_credentials(f.read())
        channel = grpc.secure_channel('localhost:3443', creds)
        stub = openoffload_pb2_grpc.SessionTableStub(channel)
        print("-------------- Add Mirror Session --------------")
        result=session_addMirrorSession(stub)
        print("SESSIONID=",result)
def run_get_closed_sessions():
    with open('ssl/server.crt', 'rb') as f:
        creds = grpc.ssl_channel_credentials(f.read())
        channel = grpc.secure_channel('localhost:3443', creds)
        stub = openoffload_pb2_grpc.SessionTableStub(channel)
        print("-------------- Check for Closed Sessions --------------")
        session_getClosedSessions(stub)
def run_get_all_sessions():
    with open('ssl/server.crt', 'rb') as f:
        creds = grpc.ssl_channel_credentials(f.read())
        channel = grpc.secure_channel('localhost:3443', creds)
        stub = openoffload_pb2_grpc.SessionTableStub(channel)
        print("-------------- Get All Sessions --------------")
        session_getAllSessions(stub)

def run_activation_sequence():
    with open('ssl/server.crt', 'rb') as f:
        creds = grpc.ssl_channel_credentials(f.read())
        activationChannel =  grpc.secure_channel('localhost:3445',creds)
        activationStub = openoffload_pb2_grpc.ActivationStub(activationChannel)
        print("\n\n------------Creating new devices---------------------\n")
        activation_registerDevice(activationStub)
        print("\n------------- Listing available Devices --------------------\n")
        activation_getAllDevices(activationStub)
        print("\n------------- Activating Device --------------------\n")
        activation_activateDevice(activationStub)
        print("\n------------- Activation Tests Complete --------------------\n")

def run():
    # NOTE(gRPC Python Team): openoffload_pb2.close() is possible on a channel and should be
    # used in circumstances in which the with statement does not fit the needs
    # of the code.
    with open('ssl/server.crt', 'rb') as f:
        creds = grpc.ssl_channel_credentials(f.read())
        channel = grpc.secure_channel('localhost:3443', creds)
        stub = openoffload_pb2_grpc.SessionTableStub(channel)
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
        print("-------------- Check for Closed Sessions --------------")
        session_getClosedSessions(stub)
        session_getAllSessions(stub)

    with open('ssl/server.crt', 'rb') as f:
        creds = grpc.ssl_channel_credentials(f.read())
        activationChannel =  grpc.secure_channel('localhost:3445',creds)
        activationStub = openoffload_pb2_grpc.ActivationStub(activationChannel)
        print("\n\n------------Creating new devices---------------------\n")
        activation_registerDevice(activationStub)
        print("\n------------- Listing available Devices --------------------\n")
        activation_getAllDevices(activationStub)
        print("\n------------- Activating Device --------------------\n")
        activation_activateDevice(activationStub)
        print("\n------------- Activation Tests Complete --------------------\n")


if __name__ == '__main__':
    logging.basicConfig()
    run()


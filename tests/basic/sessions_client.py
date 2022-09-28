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


class AddSessionErrors:
   '''
   '''
   def __init__(self):
       self._addSessionErrors= list()
   def addSessionErrorMembers(self, addSessionError):
       self._addSessionErrors.append(addSessionError)
   def __iter__(self):
       ''' Returns the Iterator object '''
       return AddSessionErrorsIterator(self)

class AddSessionErrorsIterator:
   ''' Iterator class '''
   def __init__(self, addSessionErrors_list):
       # AddSessionErrors object reference
       self._addSessionErrors_list = addSessionErrors_list
       # member variable to keep track of current index
       self._index = 0
   def __next__(self):
       ''''Returns the next value from team object's lists '''
       if self._index < (len(self._addSessionErrors_list._addSessionErrors) ) :
           result = (self._addSessionErrrors_list._addSessionErrors[self._index])
           self._index +=1
           return result
       # End of Iteration
       raise StopIteration

def session_addSession(stub):
    session=openoffload_pb2.SessionRequest()
    session.session_id= 12345678910
    session.in_lif= 1
    session.out_lif= 2
    session.source_ip=int.from_bytes(socket.inet_pton(socket.AF_INET, "10.0.0.1"), byteorder=sys.byteorder)
    session.source_port=12345
    session.destination_ip=int.from_bytes(socket.inet_pton(socket.AF_INET, "10.1.0.3"), byteorder=sys.byteorder)
    session.destination_port=80
    session.protocol_id=openoffload_pb2._TCP
    session.ip_version=openoffload_pb2._IPV4
    session.action.action_type=openoffload_pb2._FORWARD
    session.action.action_next_hop = int.from_bytes(socket.inet_pton(socket.AF_INET,"12.2.3.4"),byteorder=sys.byteorder)
    sessions_value=Sessions()
    sessions_value.addSessionMembers(session)
    session_iterator=iter(sessions_value)
    addSessionResponse =  stub.AddSession( session_iterator)
    print("addSessionResponse:",addSessionResponse.request_status)

    sessionErrors_value=addSessionResponse.response_error
    sessionErrors_iterator=iter(sessionErrors_value)

    for sessionError in sessionErrors_iterator:
         print("addSessionErrorResponse:",sessionError.request_status)

    return addSessionResponse.request_status

def session_addSession_error(stub):
    # use specific session_id to test error response
    session=openoffload_pb2.SessionRequest()
    session.session_id= 99999999999
    session.in_lif= 1
    session.out_lif= 2
    session.source_ip=int.from_bytes(socket.inet_pton(socket.AF_INET, "10.9.0.1"), byteorder=sys.byteorder)
    session.source_port=12345
    session.destination_ip=int.from_bytes(socket.inet_pton(socket.AF_INET, "10.99.0.3"), byteorder=sys.byteorder)
    session.destination_port=80
    session.protocol_id=openoffload_pb2._TCP
    session.ip_version=openoffload_pb2._IPV4
    session.action.action_type=openoffload_pb2._FORWARD
    session.action.action_next_hop = int.from_bytes(socket.inet_pton(socket.AF_INET,"12.2.3.4"),byteorder=sys.byteorder)
    sessions_value=Sessions()
    sessions_value.addSessionMembers(session)
    session_iterator=iter(sessions_value)
    addSessionResponse =  stub.AddSession( session_iterator)
    print("addSessionResponse:",addSessionResponse.request_status)

    sessionErrors_value=addSessionResponse.response_error
    sessionErrors_iterator=iter(sessionErrors_value)

    for sessionError in sessionErrors_iterator:
         print("addSessionErrorResponse.session_id:",sessionError.session_id)
         print("addSessionErrorResponse.error_status:",sessionError.error_status)

def session_addSession_ipv6(stub):
    session=openoffload_pb2.SessionRequest()
    session.session_id= 12345678910
    session.in_lif= 1
    session.out_lif= 2
    session.source_ipv6=socket.inet_pton(socket.AF_INET6, "2001:0db8:85a3:0000:0000:8a2e:0370:7332")
    session.source_port=4430
    session.destination_ipv6=socket.inet_pton(socket.AF_INET6, "2001:0db8:85a3:0000:0000:8a03:0370:234F")
    session.destination_port=80
    session.protocol_id=openoffload_pb2._TCP
    session.ip_version=openoffload_pb2._IPV6
    session.action.action_type=openoffload_pb2._FORWARD
    session.action.action_next_hop =int.from_bytes(socket.inet_pton(socket.AF_INET, "12.2.3.4"), byteorder=sys.byteorder)
    sessions_value=Sessions()
    sessions_value.addSessionMembers(session)
    session_iterator=iter(sessions_value)
    addSessionResponse =  stub.AddSession( session_iterator)
    print("addSessionResponse:",addSessionResponse.request_status)
    return addSessionResponse.request_status

def session_getSession(stub):
    sessionResponse =  stub.GetSession( openoffload_pb2.SessionId(session_id=1001))
    print("Getting Session")
    print("SessionId:", sessionResponse.session_id)
    print("Session State:", openoffload_pb2._SESSIONSTATE.values_by_number[sessionResponse.session_state].name)
    print("Session RequestStatus:",sessionResponse.request_status)
    print("Session SessionCloseCode:",sessionResponse.session_close_code)
    print("Session InPackets",sessionResponse.in_packets)
    print("Session OutPackets",sessionResponse.out_packets)
    print("Session startTime",sessionResponse.start_time)
    print("Session endTime",sessionResponse.end_time)

def session_deleteSession(stub):
    sessionResponse =  stub.DeleteSession( openoffload_pb2.SessionId(session_id=1001))
    print("Getting Session")
    print("SessionId:", sessionResponse.session_id)
    print("Session RequestStatus:",sessionResponse.request_status)
    print("Session State:", openoffload_pb2._SESSIONSTATE.values_by_number[sessionResponse.session_state].name)
    print("Session RequestStatus:",sessionResponse.request_status)
    print("Session SessionCloseCode:",sessionResponse.session_close_code)
    print("Session InPackets",sessionResponse.in_packets)
    print("Session OutPackets",sessionResponse.out_packets)
    print("Session startTime",sessionResponse.start_time)
    print("Session endTime",sessionResponse.end_time)

def session_addMirrorSession(stub):
    session=openoffload_pb2.SessionRequest()
    session.session_id= 12345678910
    session.in_lif= 1
    session.out_lif= 2
    session.source_ip=int.from_bytes(socket.inet_pton(socket.AF_INET, "10.0.0.1"), byteorder=sys.byteorder)
    session.source_port=12345
    session.destination_ip=int.from_bytes(socket.inet_pton(socket.AF_INET, "10.1.0.4"), byteorder=sys.byteorder)
    session.destination_port=80
    session.protocol_id=openoffload_pb2._UDP
    session.action.action_type=openoffload_pb2._MIRROR
    session.action.action_next_hop=int.from_bytes(socket.inet_pton(socket.AF_INET, "12.2.3.4"), byteorder=sys.byteorder)
    sessions_value=Sessions()
    sessions_value.addSessionMembers(session)
    session_iterator=iter(sessions_value)
    sessionResponse =  stub.AddSession( session_iterator)
    print("SessionResponse:",sessionResponse.request_status)
    return sessionResponse.request_status


def session_getClosedSessions(stub):
    for sessionResponse in stub.GetClosedSessions(openoffload_pb2.SessionRequestArgs(page_size=0)):
        print("### RECEIVED SESSION #####")
        print("SessionId:", sessionResponse.session_id)
        print("Session State:", openoffload_pb2._SESSIONSTATE.values_by_number[sessionResponse.session_state].name)
        print("Session RequestStatus:",sessionResponse.request_status)
        print("Session SessionCloseCode:",sessionResponse.session_close_code)
        print("Session InPackets",sessionResponse.in_packets)
        print("Session OutPackets",sessionResponse.out_packets)
        print("Session startTime",sessionResponse.start_time)
        print("Session endTime",sessionResponse.end_time)
        print("##########################")


def session_getAllSessions(stub):
    sessionResponses = stub.GetAllSessions(openoffload_pb2.SessionRequestArgs(page_size=0))
    for sessionResponse in sessionResponses.session_info: 
        print("### RECEIVED SESSION #####")
        print("SessionId:", sessionResponse.session_id)
        print("Session State:", openoffload_pb2._SESSIONSTATE.values_by_number[sessionResponse.session_state].name)
        print("Session RequestStatus:",sessionResponse.request_status)
        print("Session SessionCloseCode:",sessionResponse.session_close_code)
        print("Session InPackets",sessionResponse.in_packets)
        print("Session OutPackets",sessionResponse.out_packets)
        print("Session startTime",sessionResponse.start_time)
        print("Session endTime",sessionResponse.end_time)
        print("##########################")

def run_add_session_ipv4():
    with open('ssl/server.crt', 'rb') as f:
        creds = grpc.ssl_channel_credentials(f.read())
        channel = grpc.secure_channel('localhost:3443', creds)
        stub = openoffload_pb2_grpc.SessionTableStub(channel)
        print("-------------- Add IPv4 Session --------------")
        result = session_addSession(stub)
        print("Request Status=",result)
def run_add_session_ipv4_error():
    with open('ssl/server.crt', 'rb') as f:
        creds = grpc.ssl_channel_credentials(f.read())
        channel = grpc.secure_channel('localhost:3443', creds)
        stub = openoffload_pb2_grpc.SessionTableStub(channel)
        print("-------------- Add IPv4 Session --------------")
        result = session_addSession_error(stub)
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

if __name__ == '__main__':
    logging.basicConfig()
    run()


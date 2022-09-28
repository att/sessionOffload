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

"""The Python implementation of the WB Application Offload session table server."""

from concurrent import futures
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

import openoffload_pb2
import openoffload_pb2_grpc



class AddSessionErrors:
   '''
   '''
   def __init__(self):
       self._addSessionErrors= list()
   def addSessionErrorMembers(self, sessionResponseError):
       self._addSessionErrors.append(sessionResponseError)
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
           result = (self._addSessionErrors_list._addSessionErrors[self._index])
           self._index +=1
           return result
       # End of Iteration
       raise StopIteration


class SessionTableServicer(openoffload_pb2_grpc.SessionTableServicer):
    """Provides methods that implement functionality of session table server."""
    """ rpc addSession(sessionRequest) returns (addSessionResponse) {} """
    """ rpc getSession(sessionId) returns (sessionResponse) {}      """
    """ rpc deleteSession(sessionId) returns (sessionResponse) {}   """
    """ rpc getAllSessions(sessionStatisticsArgs) returns (sessionResponse) {}   """
    """ rpc getClosedSessions(sessionStatisticsArgs) returns (sessionResponse) {}   """


    def __init__(self):
        """do some init stuff"""

    def AddSession(self, request_iterator, context):
        sessionErrors_value=AddSessionErrors()
        for request in request_iterator:
            print("############ ADD SESSION ##################")
            #print("sessionID:",request.sessionId)
            #print("protocolID 6=TCP,17=UDP:",request.protocolId)
            print("IP Version:", openoffload_pb2._IPVERSION.values_by_number[request.ip_version].name)
            print("protocolID :",openoffload_pb2._PROTOCOLID.values_by_number[request.protocol_id].name)
            

            if request.ip_version == openoffload_pb2._IPV4:
              print ("source_ip:", socket.inet_ntop(socket.AF_INET, request.source_ip.to_bytes(4,byteorder=sys.byteorder)))
            else:
              print ("source_ipv6:", socket.inet_ntop(socket.AF_INET6, request.source_ipv6))
            print("sourcePort:", int(request.source_port))
            if request.ip_version == openoffload_pb2._IPV4:
              print ("destination_ip:", socket.inet_ntop(socket.AF_INET, request.destination_ip.to_bytes(4,byteorder=sys.byteorder)))
            else:
              print ("destination_ipv6:", socket.inet_ntop(socket.AF_INET6, request.destination_ipv6))
            print("destination_port:", int(request.destination_port))
            #print("ActionType 0=DROP,1=FORWARD,2=MIRROR,3=SNOOP:" , request.action.actionType)
            print("ActionType:" , openoffload_pb2._ACTIONTYPE.values_by_number[request.action.action_type].name)
            print("ActionNextHop:" , request.action.action_next_hop)
            if  request.session_id == 99999999999:
                print("Error test case")
                #sessionError=openoffload_pb2.addSessionResponse.responseError()
                sessionError=openoffload_pb2.SessionResponseError()
                sessionError.session_id=request.session_id
                sessionError.error_status=openoffload_pb2._SESSION_TABLE_FULL
                sessionErrors_value.addSessionErrorMembers( sessionError)

        return openoffload_pb2.AddSessionResponse(request_status=openoffload_pb2._ACCEPTED, response_error=sessionErrors_value)

    def GetSession(self, request, context):
            print("############ GET SESSION ##################")
            print("session_id:",request.session_id)
            timestamp = google.protobuf.timestamp_pb2.Timestamp()
            timestamp.GetCurrentTime()
            return openoffload_pb2.SessionResponse(session_id=1001,session_state=openoffload_pb2._ESTABLISHED,
              request_status=openoffload_pb2._ACCEPTED, in_packets=1000, out_packets=200000,
              start_time=timestamp)
    def DeleteSession(self, request, context):
            print("############ DELETE SESSION ##################")
            print("session_id:",request.session_id)
            timestamp = google.protobuf.timestamp_pb2.Timestamp()
            timestamp.GetCurrentTime()
            return openoffload_pb2.SessionResponse(session_id=1001, session_state=openoffload_pb2._CLOSING_1,
              request_status=openoffload_pb2._ACCEPTED, in_packets=2000, out_packets=400000,
              start_time=timestamp, end_time=timestamp)

    def GetClosedSessions(self, request, context):
            print("############ GET CLOSED SESSIONS ##################")
            timestamp = google.protobuf.timestamp_pb2.Timestamp()
            timestamp.GetCurrentTime()
            session1 = openoffload_pb2.SessionResponse(session_id=1001, session_state=openoffload_pb2._CLOSED,
              request_status=openoffload_pb2._ACCEPTED, in_packets=1000, out_packets=200000, start_time=timestamp)
            session2 = openoffload_pb2.SessionResponse(session_id=1002, session_state=openoffload_pb2._CLOSED,
              request_status=openoffload_pb2._ACCEPTED, in_packets=2000, out_packets=400000, start_time=timestamp)
            yield session1
            yield session2

    def GetAllSessions(self, request, context):
            print("############ GET ALL SESSIONS ##################")
            sessionResponses = openoffload_pb2.SessionResponses()
            timestamp = google.protobuf.timestamp_pb2.Timestamp()
            timestamp.GetCurrentTime()

            session1 = openoffload_pb2.SessionResponse(session_id=1001, session_state=openoffload_pb2._CLOSED,
              request_status=openoffload_pb2._ACCEPTED, in_packets=1000, out_packets=200000, start_time=timestamp)
            sessionResponses.session_info.append(session1)

            session2 = openoffload_pb2.SessionResponse(session_id=1002, session_state=openoffload_pb2._CLOSED,
              request_status=openoffload_pb2._ACCEPTED, in_packets=2000, out_packets=400000, start_time=timestamp)
            sessionResponses.session_info.append(session2)

            return sessionResponses


def sessionServe():
    server = grpc.server(futures.ThreadPoolExecutor(max_workers=5))
    openoffload_pb2_grpc.add_SessionTableServicer_to_server(
        SessionTableServicer(), server)
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
    sessionServe()

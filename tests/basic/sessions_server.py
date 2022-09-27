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
            print("IP Version:", openoffload_pb2._IPVERSION.values_by_number[request.ipversion].name)
            print("protocolID :",openoffload_pb2._PROTOCOLID.values_by_number[request.protocolid].name)
            

            if request.ipversion == openoffload_pb2._IPV4:
              print ("sourceip:", socket.inet_ntop(socket.AF_INET, request.sourceip.to_bytes(4,byteorder=sys.byteorder)))
            else:
              print ("sourceipv6:", socket.inet_ntop(socket.AF_INET6, request.sourceipv6))
            print("sourcePort:", int(request.sourceport))
            if request.ipversion == openoffload_pb2._IPV4:
              print ("destinationip:", socket.inet_ntop(socket.AF_INET, request.destinationip.to_bytes(4,byteorder=sys.byteorder)))
            else:
              print ("destinationipv6:", socket.inet_ntop(socket.AF_INET6, request.destinationipv6))
            print("destinationport:", int(request.destinationport))
            #print("ActionType 0=DROP,1=FORWARD,2=MIRROR,3=SNOOP:" , request.action.actionType)
            print("ActionType:" , openoffload_pb2._ACTIONTYPE.values_by_number[request.action.actiontype].name)
            print("ActionNextHop:" , request.action.actionnexthop)
            if  request.sessionid == 99999999999:
                print("Error test case")
                #sessionError=openoffload_pb2.addSessionResponse.responseError()
                sessionError=openoffload_pb2.SessionResponseError()
                sessionError.sessionid=request.sessionid
                sessionError.errorstatus=openoffload_pb2._SESSION_TABLE_FULL
                sessionErrors_value.addSessionErrorMembers( sessionError)

        return openoffload_pb2.AddSessionResponse(requeststatus=openoffload_pb2._ACCEPTED, responseerror=sessionErrors_value)

    def GetSession(self, request, context):
            print("############ GET SESSION ##################")
            print("sessionid:",request.sessionid)
            timestamp = google.protobuf.timestamp_pb2.Timestamp()
            timestamp.GetCurrentTime()
            return openoffload_pb2.SessionResponse(sessionid=1001,sessionstate=openoffload_pb2._ESTABLISHED,
              requeststatus=openoffload_pb2._ACCEPTED, inpackets=1000, outpackets=200000,
              starttime=timestamp)
    def DeleteSession(self, request, context):
            print("############ DELETE SESSION ##################")
            print("sessionid:",request.sessionid)
            timestamp = google.protobuf.timestamp_pb2.Timestamp()
            timestamp.GetCurrentTime()
            return openoffload_pb2.SessionResponse(sessionid=1001, sessionstate=openoffload_pb2._CLOSING_1,
              requeststatus=openoffload_pb2._ACCEPTED, inpackets=2000, outpackets=400000,
              starttime=timestamp, endtime=timestamp)

    def GetClosedSessions(self, request, context):
            print("############ GET CLOSED SESSIONS ##################")
            timestamp = google.protobuf.timestamp_pb2.Timestamp()
            timestamp.GetCurrentTime()
            session1 = openoffload_pb2.SessionResponse(sessionid=1001, sessionstate=openoffload_pb2._CLOSED,
              requeststatus=openoffload_pb2._ACCEPTED, inpackets=1000, outpackets=200000, starttime=timestamp)
            session2 = openoffload_pb2.SessionResponse(sessionid=1002, sessionstate=openoffload_pb2._CLOSED,
              requeststatus=openoffload_pb2._ACCEPTED, inpackets=2000, outpackets=400000, starttime=timestamp)
            yield session1
            yield session2

    def GetAllSessions(self, request, context):
            print("############ GET ALL SESSIONS ##################")
            sessionResponses = openoffload_pb2.SessionResponses()
            timestamp = google.protobuf.timestamp_pb2.Timestamp()
            timestamp.GetCurrentTime()

            session1 = openoffload_pb2.SessionResponse(sessionid=1001, sessionstate=openoffload_pb2._CLOSED,
              requeststatus=openoffload_pb2._ACCEPTED, inpackets=1000, outpackets=200000, starttime=timestamp)
            sessionResponses.sessioninfo.append(session1)

            session2 = openoffload_pb2.SessionResponse(sessionid=1002, sessionstate=openoffload_pb2._CLOSED,
              requeststatus=openoffload_pb2._ACCEPTED, inpackets=2000, outpackets=400000, starttime=timestamp)
            sessionResponses.sessioninfo.append(session2)

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

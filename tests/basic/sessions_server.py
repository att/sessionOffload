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

    def addSession(self, request_iterator, context):
        sessionErrors_value=AddSessionErrors()
        for request in request_iterator:
            print("############ ADD SESSION ##################")
            print("sessionId:",request.sessionId)
            print("protocolID :",openoffload_pb2._PROTOCOL_ID.values_by_number[request.protocolId].name)
            print("IP Version:", openoffload_pb2._IP_VERSION.values_by_number[request.ipVersion].name)
            

            if request.ipVersion == openoffload_pb2._IPV4:
              print ("sourceIp:", socket.inet_ntop(socket.AF_INET, request.sourceIp.to_bytes(4,byteorder=sys.byteorder)))
            else:
              print ("sourceIpV6:", socket.inet_ntop(socket.AF_INET6, request.sourceIpV6))
            print("sourcePort:", int(request.sourcePort))
            if request.ipVersion == openoffload_pb2._IPV4:
              print ("destinationIp:", socket.inet_ntop(socket.AF_INET, request.destinationIp.to_bytes(4,byteorder=sys.byteorder)))
            else:
              print ("destinationIpV6:", socket.inet_ntop(socket.AF_INET6, request.destinationIpV6))
            print("destinationPort:", int(request.destinationPort))
            print("ActionType:" , openoffload_pb2._ACTION_TYPE.values_by_number[request.action.actionType].name)
            print("MatchType:", openoffload_pb2._MATCH_TYPE.values_by_number[request.matchType].name)
            print("tunnelEndpointId:", request.tunnelEndpointId)
            if request.action.actionType == 4:
                print("encapTunnelEndpointId:" , request.action.encapTunnelEndpointId)
                print("encapMatchDestinationIp:" , socket.inet_ntop(socket.AF_INET,request.action.encapMatchDestinationIp.to_bytes(4,byteorder=sys.byteorder)))
                print("encapTrafficMarking:" , request.action.encapTrafficMarking)
                print("ingressTrafficRate:" , request.action.ingressTrafficRate)
                print("egressTrafficRate:" , request.action.egressTrafficRate)
            print("ActionNextHop:" , request.action.actionNextHop)
            if  request.sessionId == 99999999999:
                print("Error test case")
                #sessionError=openoffload_pb2.addSessionResponse.responseError()
                sessionError=openoffload_pb2.sessionResponseError()
                sessionError.sessionId=request.sessionId
                sessionError.errorStatus=openoffload_pb2._SESSION_TABLE_FULL
                sessionErrors_value.addSessionErrorMembers( sessionError)

        return openoffload_pb2.addSessionResponse(requestStatus=openoffload_pb2._ACCEPTED, responseError=sessionErrors_value)

    def getSession(self, request, context):
            print("############ GET SESSION ##################")
            print("sessionId:",request.sessionId)
            timestamp = google.protobuf.timestamp_pb2.Timestamp()
            timestamp.GetCurrentTime()
            return openoffload_pb2.sessionResponse(sessionId=1001,sessionState=openoffload_pb2._ESTABLISHED,
              requestStatus=openoffload_pb2._ACCEPTED, inPackets=1000, outPackets=200000,
              startTime=timestamp)
    def deleteSession(self, request, context):
            print("############ DELETE SESSION ##################")
            print("sessionId:",request.sessionId)
            timestamp = google.protobuf.timestamp_pb2.Timestamp()
            timestamp.GetCurrentTime()
            return openoffload_pb2.sessionResponse(sessionId=request.sessionId, sessionState=openoffload_pb2._CLOSING_1,
              requestStatus=openoffload_pb2._ACCEPTED, inPackets=2000, outPackets=400000,
              startTime=timestamp, endTime=timestamp)

    def getClosedSessions(self, request, context):
            print("############ GET CLOSED SESSIONS ##################")
            timestamp = google.protobuf.timestamp_pb2.Timestamp()
            timestamp.GetCurrentTime()
            session1 = openoffload_pb2.sessionResponse(sessionId=1001, sessionState=openoffload_pb2._CLOSED,
              requestStatus=openoffload_pb2._ACCEPTED, inPackets=1000, outPackets=200000, startTime=timestamp)
            session2 = openoffload_pb2.sessionResponse(sessionId=1002, sessionState=openoffload_pb2._CLOSED,
              requestStatus=openoffload_pb2._ACCEPTED, inPackets=2000, outPackets=400000, startTime=timestamp)
            yield session1
            yield session2

    def getAllSessions(self, request, context):
            print("############ GET ALL SESSIONS ##################")
            sessionResponses = openoffload_pb2.sessionResponses()
            timestamp = google.protobuf.timestamp_pb2.Timestamp()
            timestamp.GetCurrentTime()

            session1 = openoffload_pb2.sessionResponse(sessionId=1001, sessionState=openoffload_pb2._CLOSED,
              requestStatus=openoffload_pb2._ACCEPTED, inPackets=1000, outPackets=200000, startTime=timestamp)
            sessionResponses.sessionInfo.append(session1)

            session2 = openoffload_pb2.sessionResponse(sessionId=1002, sessionState=openoffload_pb2._CLOSED,
              requestStatus=openoffload_pb2._ACCEPTED, inPackets=2000, outPackets=400000, startTime=timestamp)
            sessionResponses.sessionInfo.append(session2)

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
    #server.add_insecure_port('localhost:3443') 
    server.start()
    server.wait_for_termination()

if __name__ == '__main__':
    logging.basicConfig()
    sessionServe()

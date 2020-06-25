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

import grpc
import google.protobuf.timestamp_pb2

import openoffload_pb2
import openoffload_pb2_grpc


class SessionStatisticsTableServicer(openoffload_pb2_grpc.SessionStatisticsTableServicer):
    """Provides methods that implement functionality of session statistics table server."""
    """ rpc getAllSessions(sessionStatisticsArgs) returns (sessionResponse) {}   """
    """ rpc getClosedSessions(sessionStatisticsArgs) returns (sessionResponse) {}   """
    def getClosedSessions(self, request, context):
            print("############ GET CLOSED SESSIONS ##################");
            timestamp = google.protobuf.timestamp_pb2.Timestamp()
            timestamp.GetCurrentTime()
            session1 = openoffload_pb2.sessionResponse(sessionId=1001, sessionState=openoffload_pb2._CLOSED,
              requestStatus=openoffload_pb2._ACCEPTED, inPackets=1000, outPackets=200000, startTime=timestamp);
            session2 = openoffload_pb2.sessionResponse(sessionId=1002, sessionState=openoffload_pb2._CLOSED,
              requestStatus=openoffload_pb2._ACCEPTED, inPackets=2000, outPackets=400000, startTime=timestamp);
            yield session1
            yield session2

    def getAllSessions(self, request, context):
            print("############ GET ALL SESSIONS ##################");
            timestamp = google.protobuf.timestamp_pb2.Timestamp()
            timestamp.GetCurrentTime()
            session1 = openoffload_pb2.sessionResponse(sessionId=1001, sessionState=openoffload_pb2._CLOSED,
              requestStatus=openoffload_pb2._ACCEPTED, inPackets=1000, outPackets=200000, startTime=timestamp);
            session2 = openoffload_pb2.sessionResponse(sessionId=1002, sessionState=openoffload_pb2._CLOSED,
              requestStatus=openoffload_pb2._ACCEPTED, inPackets=2000, outPackets=400000, startTime=timestamp);
            yield session1
            yield session2


def statsServe():
    statsServer = grpc.server(futures.ThreadPoolExecutor(max_workers=5))
    openoffload_pb2_grpc.add_SessionStatisticsTableServicer_to_server(
        SessionStatisticsTableServicer(), statsServer)
    with open('ssl/server.key', 'rb') as fs:
         private_key = fs.read()
    with open('ssl/server.crt', 'rb') as fs:
         certificate_chain = fs.read()
    server_credentials = grpc.ssl_server_credentials( ( (private_key, certificate_chain), ) )
    statsServer.add_secure_port('localhost:3444', server_credentials)
    statsServer.start()
    statsServer.wait_for_termination()

if __name__ == '__main__':
    logging.basicConfig()
    statsServe()

#!/usr/bin/env python3
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
# Rich Bowman rb0161@att.com

"""The Python implementation of the WB Application Offload session table server with a simulation of the NOS."""

from concurrent import futures
import time
import random
import logging
import threading
import socket

import grpc
import google.protobuf.timestamp_pb2

import sessions_pb2
import sessions_pb2_grpc

offloadSessionTable = {}

class SessionTableServicer(sessions_pb2_grpc.SessionTableServicer):
    """Provides methods that implement functionality of session table server."""
    """ rpc addSession(sessionRequest) returns (sessionResponse) {} """
    """ rpc getSession(sessionId) returns (sessionResponse) {}      """
    """ rpc deleteSession(sessionId) returns (sessionResponse) {}   """
    global offloadSessionTable
    lastSessionId = 1
    #maxSessionId = 500
    # set it low and watch session Ids roll over and get reused
    # set it low to see how a FW client handles receiving failures adding new sessions since the table is full
    maxSessionId = 20

    def __init__(self):
        """do some init stuff"""
        self.simulateSessionsTrafficThread =  threading.Thread(target=self.simulateSessionsTraffic)
        self.simulateSessionsTrafficThread.start()

    def nextSessionId(self):

        cnt=0
        while True:
            cnt=cnt+1
            if cnt == self.maxSessionId:
                return -1

            if self.lastSessionId == self.maxSessionId:
               self.lastSessionId=1

            if self.lastSessionId in offloadSessionTable.keys():
              self.lastSessionId = self.lastSessionId + 1
            else:
              return self.lastSessionId


    def addSession(self, request, context):
            print("############ ADD SESSION ##################");
            print("IP Version:", sessions_pb2._IP_VERSION.values_by_number[request.ipVersion].name)
            if request.ipVersion == sessions_pb2._IPV4:
              print("sourceIp:",socket.inet_ntop(socket.AF_INET, request.sourceIp));
            else:
              print("sourceIp:",socket.inet_ntop(socket.AF_INET6, request.sourceIp));
            print("sourcePort:", request.sourcePort);
            if request.ipVersion == sessions_pb2._IPV4:
              print("destinationIp:",socket.inet_ntop(socket.AF_INET, request.destinationIp));
            else:
              print("destinationIp:",socket.inet_ntop(socket.AF_INET6, request.destinationIp));
            print("destinationPort:", request.destinationPort);
            print("protocolId:", request.protocolId);
            print("ActionType 0=DROP,1=FORWARD,2=MIRROR,3=SNOOP:" , request.action.actionType)
            print("ActionNextHop:" , request.action.actionNextHop)

            # check that all the required fields were included in the request
            if request.inLif is None or request.outLif is None or request.ipVersion is None or request.action is None or request.protocolId is None or request.sourceIp is None or request.sourcePort is None or request.destinationIp is None or request.destinationPort is None:
                msg = (f"ERROR: Invalid Request")
                print(msg)
                # Raise Exception on GRPC client
                context.set_details(msg)
                context.set_code(grpc.StatusCode.UNKNOWN)
                return sessions_pb2.sessionResponse(requestStatus=sessions_pb2._REJECTED)


            # check that the same 7 tuple doesn't already exist
            existingSessionId = self.findSessionByTuple(request.inLif, request.outLif, request.sourceIp, request.sourcePort, request.destinationIp, request.destinationPort, request.protocolId)
            # RICH DOES a closed session that has not been picked up by the FW count?
            if existingSessionId:
                msg = (f"ERROR: There already exists a session with the same 7 tuple.")
                print(msg)
                # Raise Exception on GRPC client
                #context.set_details(msg)
                #context.set_code(grpc.StatusCode.UNKNOWN)
                return sessions_pb2.sessionResponse(requestStatus=sessions_pb2._REJECTED_SESSION_ALREADY_EXISTS)

            newSessionId = self.nextSessionId()
            # check if offloadSessionTable is already full
            if newSessionId < 0:
                msg = (f"INFO: The session table is full and cannot support any new offload sessions at this time.")
                print(msg)
                return sessions_pb2.sessionResponse(requestStatus=sessions_pb2._REJECTED_SESSION_TABLE_FULL)



            print(f"Found slot for new offload session @ id = {newSessionId}")
            timestamp = google.protobuf.timestamp_pb2.Timestamp()
            timestamp.GetCurrentTime()
            offloadSessionTable[newSessionId] = {
                                                "inLif": request.inLif,
                                                "outLif": request.outLif,
                                                "ipVersion": request.ipVersion,
                                                "action": request.action,
                                                "protocolId": request.protocolId,
                                                "sourceIp": request.sourceIp,
                                                "sourcePort": request.sourcePort,
                                                "destinationIp": request.destinationIp,
                                                "destinationPort": request.destinationPort,
                                                "state": sessions_pb2._ESTABLISHED,
                                                "inPackets": 0,
                                                "inBytes": 0,
                                                "outPackets": 0,
                                                "outBytes": 0,
                                                "startTime": timestamp,
                                                "endTime": None,
                                                "timeOut": 3600,
                                                "sessionCloseCode": sessions_pb2._NOT_CLOSED
                                              }
            return sessions_pb2.sessionResponse(sessionId=newSessionId, requestStatus=sessions_pb2._ACCEPTED);

    def findSessionByTuple(self, inLif, outLif, sourceIp, sourcePort, destinationIp, destinationPort, protocolId):
            for sessionId, session in offloadSessionTable.items():
                if sourceIp == session["sourceIp"] and \
                   sourcePort == session["sourcePort"] and \
                   destinationIp == session["destinationIp"] and \
                   destinationPort == session["destinationPort"] and \
                   protocolId == session["protocolId"] and \
                   inLif == session["inLif"] and \
                   outLif == session["outLif"]:
                       return sessionId

            return None


    def getSession(self, request, context):
            print("############ GET SESSION ##################");
            print("sessionId:",request.sessionId);

            try:
              session = offloadSessionTable[request.sessionId]
            except KeyError:
              msg = (f"ERROR: The session was not found.")
              print(msg)
              context.set_details(msg)
              context.set_code(grpc.StatusCode.UNKNOWN)
              return sessions_pb2.sessionResponse(requestStatus=sessions_pb2._REJECTED_SESSION_NONEXISTENT)
            else:
              yield sessions_pb2.sessionResponse(sessionId=sessionId, sessionState=session["state"], requestStatus=sessions_pb2._ACCEPTED, inPackets=session["inPackets"], inBytes=session["inBytes"], outPackets=session["outPackets"], outBytes=session["outBytes"], startTime=session["startTime"], endTime=session["endTime"], sessionCloseCode=session["sessionCloseCode"]);



    def deleteSession(self, request, context):
            print("############ DELETE SESSION ##################");
            print("sessionId:",request.sessionId);

            try:
              session = offloadSessionTable[request.sessionId]
            except KeyError:
              print(f"ERROR ID {request.sessionId} not found")
              msg = 'session id cannot be found'
              return sessions_pb2.sessionResponse(requestStatus=sessions_pb2._REJECTED_SESSION_NONEXISTENT)

            try:
              del offloadSessionTable[request.sessionId]
            except KeyError:
              print(f"ERROR ID {request.sessionId} can not be deleted")
              context.set_details(msg)
              context.set_code(grpc.StatusCode.UNKNOWN)
              return sessions_pb2.sessionResponse()


            timestamp = google.protobuf.timestamp_pb2.Timestamp()
            timestamp.GetCurrentTime()
            return sessions_pb2.sessionResponse(sessionId=request.sessionId, sessionState=sessions_pb2._CLOSING_1,
              requestStatus=sessions_pb2._ACCEPTED,
              startTime=timestamp, endTime=timestamp);





    def simulateSessionsTraffic(self):

        while(True):
          print(f"\n----- START simulateSessionsTraffic() -----")
          closedCnt=0
          for sessionId, session in offloadSessionTable.items():
            if session["state"] == sessions_pb2._CLOSED:
                closedCnt = closedCnt + 1
                continue

            sessionAction = random.choice(['updateCounters', 'updateCounters', 'updateCounters', 'updateCounters', 'updateCounters', 'updateCounters','updateCounters','updateCounters', 'updateCounters', 'updateCounters','updateCounters','updateCounters', 'updateCounters', 'updateCounters','updateCounters','updateCounters', 'noOp', 'noOp', 'noOp', 'noOp', 'noOp', 'noOp', 'noOp', 'timeout', 'clientFin', 'serverFin'])


            if sessionAction == 'updateCounters':
                print(f"\tsimulateSessionsTraffic() - id:{sessionId} updateCounters random add bytes/packets")
                session["inPackets"] = session["inPackets"] + random.randint(0,333)
                session["inBytes"] = session["inBytes"] + random.randint(0,33333)
                session["outPackets"] = session["outPackets"] + random.randint(0,333)
                session["outBytes"] = session["outBytes"] + random.randint(0,33333)

            elif sessionAction == 'noOp':
                print(f"\tsimulateSessionsTraffic() - id:{sessionId} noOp")

            elif sessionAction == 'timeout':
                print(f"\tsimulateSessionsTraffic() - id:{sessionId} timeout")
                session["state"] = sessions_pb2._CLOSED
                session["sessionCloseCode"] = sessions_pb2._TIMEOUT
                now = time.time()
                seconds = int(now)
                nanos = int((now - seconds) * 10**9)
                timestamp = google.protobuf.timestamp_pb2.Timestamp(seconds=seconds, nanos=nanos)
                session["endTime"] = timestamp

            elif sessionAction == 'clientFin':
                print(f"\tsimulateSessionsTraffic() - id:{sessionId} clientFin")
                session["state"] = sessions_pb2._CLOSED
                session["sessionCloseCode"] = sessions_pb2._FINACK
                now = time.time()
                seconds = int(now)
                nanos = int((now - seconds) * 10**9)
                timestamp = google.protobuf.timestamp_pb2.Timestamp(seconds=seconds, nanos=nanos)
                session["endTime"] = timestamp


            elif sessionAction == 'serverFin':
                print(f"\tsimulateSessionsTraffic() - id:{sessionId} serverFin")
                session["state"] = sessions_pb2._CLOSED
                session["sessionCloseCode"] = sessions_pb2._FINACK
                now = time.time()
                seconds = int(now)
                nanos = int((now - seconds) * 10**9)
                timestamp = google.protobuf.timestamp_pb2.Timestamp(seconds=seconds, nanos=nanos)
                session["endTime"] = timestamp

          activeCnt = len(offloadSessionTable) - closedCnt
          print(f"----- END simulateSessionsTraffic() total sessions ACTIVE={activeCnt} & CLOSED={closedCnt} -----")
          time.sleep(2)

class SessionStatisticsTableServicer(sessions_pb2_grpc.SessionStatisticsTableServicer):
    """Provides methods that implement functionality of session statistics table server."""
    """ rpc getAllSessions(sessionStatisticsArgs) returns (sessionResponse) {}   """
    """ rpc getClosedSessions(sessionStatisticsArgs) returns (sessionResponse) {}   """
    global offloadSessionTable

    def getClosedSessions(self, request, context):
            print("############ GET CLOSED SESSIONS ##################");

            for sessionId in list(offloadSessionTable):
              #print(f"session {sessionId}")
              session = offloadSessionTable[sessionId]
              if session["state"] == sessions_pb2._CLOSED:
                del offloadSessionTable[sessionId]
                yield sessions_pb2.sessionResponse(sessionId=sessionId, sessionState=session["state"], requestStatus=sessions_pb2._ACCEPTED, inPackets=session["inPackets"], inBytes=session["inBytes"], outPackets=session["outPackets"], outBytes=session["outBytes"], startTime=session["startTime"], endTime=session["endTime"], sessionCloseCode=session["sessionCloseCode"]);

    def getAllSessions(self, request, context):
            print("############ GET ALL SESSIONS ##################");

            for sessionId, session in offloadSessionTable.items():
              print(f" returning session {sessionId}")
              yield sessions_pb2.sessionResponse(sessionId=sessionId, sessionState=session["state"], requestStatus=sessions_pb2._ACCEPTED, inPackets=session["inPackets"], inBytes=session["inBytes"], outPackets=session["outPackets"], outBytes=session["outBytes"], startTime=session["startTime"], endTime=session["endTime"], sessionCloseCode=session["sessionCloseCode"]);

def serve():
    server = grpc.server(futures.ThreadPoolExecutor(max_workers=5))
    sessions_pb2_grpc.add_SessionTableServicer_to_server(SessionTableServicer(), server)
    sessions_pb2_grpc.add_SessionStatisticsTableServicer_to_server(SessionStatisticsTableServicer(), server)
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
    serve()

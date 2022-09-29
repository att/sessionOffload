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
#
# nos_sessions_server_simulator.py - this script was written to simulate the offload device that 
# implements the GRPC service and L4 Statefull Offload.
#
# We keep a session table in a Python dictionary named offloadSessionTable.

"""The Python implementation of the WB Application Offload session table server with a simulation of the NOS."""

from concurrent import futures
import time
import random
import logging
import threading
import socket
import sys

import grpc
import google.protobuf.timestamp_pb2

import openoffload_pb2
import openoffload_pb2_grpc

# the L4 sessions that have been offloaded
offloadSessionTable = {}

class SessionTableServicer(openoffload_pb2_grpc.SessionTableServicer):
    """Provides methods that implement functionality of session table server."""
    """ rpc AddSession(sessionRequest) returns (addSessionResponse) {} """
    """ rpc GetSession(sessionId) returns (sessionResponse) {}      """
    """ rpc DeleteSession(sessionId) returns (sessionResponse) {}   """
    """ rpc GetAllSessions(sessionStatisticsArgs) returns (sessionResponse) {}   """
    """ rpc GetClosedSessions(sessionStatisticsArgs) returns (sessionResponse) {}   """
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


    def AddSession(self, request_iterator, context):
        addSessionResponse = openoffload_pb2.AddSessionResponse()

        for request in request_iterator:
            addNewSession = True
            print("############ ADD SESSION ##################");
            print("Client Session ID:", request.session_id)
            print("IP Version:", openoffload_pb2._IPVERSION.values_by_number[request.ip_version].name)
            if request.ip_version == openoffload_pb2._IPV4:
              print("source_ip:",socket.inet_ntop(socket.AF_INET, request.source_ip.to_bytes(4,byteorder=sys.byteorder)));
            else:
              print("source_ipv6:",socket.inet_ntop(socket.AF_INET6, request.source_ipv6));
            print("source_port:", request.source_port);
            if request.ip_version == openoffload_pb2._IPV4:
              print("destinationIp:",socket.inet_ntop(socket.AF_INET, request.destination_ip.to_bytes(4,byteorder=sys.byteorder)));
            else:
              print("destination_ipv6:",socket.inet_ntop(socket.AF_INET6, request.destination_ipv6));
            print("destination_port:", request.destination_port);
            print("protocol_id:", request.protocol_id);
            print("ActionType 0=DROP,1=FORWARD,2=MIRROR,3=SNOOP:" , request.action.action_type)
            print("ActionNextHop:" , request.action.action_next_hop)



            # check that all the required fields were included in the request
            if request.session_id is None or request.in_lif is None or request.out_lif is None or request.ip_version is None or request.action is None or request.protocol_id is None or request.source_ip is None or request.source_port is None or request.destination_ip is None or request.destination_port is None:
                msg = (f"ERROR: Invalid Request")
                print(msg)
                # Raise Exception on GRPC client
                context.set_details(msg)
                context.set_code(grpc.StatusCode.UNKNOWN)


                sessionResponseError = openoffload_pb2.sessionResponseError(sessionId=request.sessionId, errorStatus=openoffload_pb2._SESSION_REJECTED)
                addSessionResponse.responseError.append(sessionResponseError)
                addNewSession = False
                #return openoffload_pb2.addSessionResponse(requestStatus=openoffload_pb2._REJECTED)


            # check that the same 7 tuple doesn't already exist
            existingSessionId = self.findSessionByTuple(request.in_lif, request.out_lif, request.source_ip, request.source_port, request.destination_ip, request.destination_port, request.protocol_id)
            # RICH DOES a closed session that has not been picked up by the FW count?
            if existingSessionId:
                msg = (f"ERROR: There already exists a session with the same 7 tuple.")
                print(msg)
                # Raise Exception on GRPC client
                #context.set_details(msg)
                #context.set_code(grpc.StatusCode.UNKNOWN)
                sessionResponseError = openoffload_pb2.sessionResponseError(sessionId=request.sessionId, errorStatus=openoffload_pb2._SESSION_ALREADY_EXISTS)
                addSessionResponse.responseError.append(sessionResponseError)
                addNewSession = False
                #return openoffload_pb2.addSessionResponse(requestStatus=openoffload_pb2._REJECTED_SESSION_ALREADY_EXISTS)

            newSessionId = self.nextSessionId()
            # check if offloadSessionTable is already full
            if newSessionId < 0:
                msg = (f"INFO: The session table is full and cannot support any new offload sessions at this time.")
                print(msg)
                sessionResponseError = openoffload_pb2.SessionResponseError(session_id=request.session_id, error_status=openoffload_pb2._SESSION_TABLE_FULL)
                addSessionResponse.response_error.append(sessionResponseError)
                addNewSession = False
                #return openoffload_pb2.addSessionResponse(requestStatus=openoffload_pb2._REJECTED_SESSION_TABLE_FULL)


            if addNewSession: 
                print(f"Excepting new session for Offload. Found slot for new offload session @ id = {newSessionId}")
                timestamp = google.protobuf.timestamp_pb2.Timestamp()
                timestamp.GetCurrentTime()
                offloadSessionTable[newSessionId] = {
                                                "clientSessionId": request.session_id,
                                                "inLif": request.in_lif,
                                                "outLif": request.out_lif,
                                                "ipVersion": request.ip_version,
                                                "action": request.action,
                                                "protocolId": request.protocol_id,
                                                "sourceIp": request.source_ip,
                                                "sourceIpV6": request.source_ipv6,
                                                "sourcePort": request.source_port,
                                                "destinationIp": request.destination_ip,
                                                "destinationIpV6": request.destination_ipv6,
                                                "destinationPort": request.destination_port,
                                                "state": openoffload_pb2._ESTABLISHED,
                                                "inPackets": 0,
                                                "inBytes": 0,
                                                "outPackets": 0,
                                                "outBytes": 0,
                                                "startTime": timestamp,
                                                "endTime": None,
                                                "timeOut": 3600,
                                                "sessionCloseCode": openoffload_pb2._NOT_CLOSED
                                              }


        return addSessionResponse 
            #return openoffload_pb2.addSessionResponse(requestStatus=openoffload_pb2._ACCEPTED);


    # find and return a single session by the passed in sessionId
    def GetSession(self, request, context):
            global offloadSessionTable
            print("############ GET SESSION ##################");
            print("request sessionId:",request.session_id);

            try:
              # now find this client session id in our local Session Tablea
              for sessionId, session in offloadSessionTable.items():
                print(f"checking our session id:{sessionId} client session id:", session["clientSessionId"], " matches the request sessionId:", request.session_id )
                if session["clientSessionId"] == request.session_id:
                  return openoffload_pb2.SessionResponse(session_id=request.session_id, session_state=session["state"], request_status=openoffload_pb2._ACCEPTED, in_packets=session["inPackets"], in_bytes=session["inBytes"], out_packets=session["outPackets"], out_bytes=session["outBytes"], start_time=session["startTime"], end_time=session["endTime"], session_close_code=session["sessionCloseCode"]);


              msg = (f"ERROR: The session was not found.")
              print(msg)
              context.set_details(msg)
              context.set_code(grpc.StatusCode.UNKNOWN)
              return openoffload_pb2.SessionResponse(requestStatus=openoffload_pb2._REJECTED_SESSION_NONEXISTENT)
            except: 
              msg = (f"ERROR: The session was not found heer.")
              print(msg)
              context.set_details(msg)
              context.set_code(grpc.StatusCode.UNKNOWN)
              return openoffload_pb2.SessionResponse(request_status=openoffload_pb2._REJECTED_SESSION_NONEXISTENT)



    def DeleteSession(self, request, context):
            print("############ DELETE SESSION ##################");
            print("sessionId:",request.sessionId);

            ourSessionId=None
            try:
              for sessionId, session in offloadSessionTable.items():
                if session["clientSessionId"] == request.sessionId:
                  ourSessionId=sessionId
                  break

              if not ourSessionId:
                print(f"ERROR ID {request.sessionId} not found")
                msg = 'session id cannot be found'
                return openoffload_pb2.sessionResponse(requestStatus=openoffload_pb2._REJECTED_SESSION_NONEXISTENT)

            except KeyError:
              print(f"ERROR ID {request.sessionId} not found")
              msg = 'session id cannot be found'
              return openoffload_pb2.sessionResponse(requestStatus=openoffload_pb2._REJECTED_SESSION_NONEXISTENT)

            try:
              del offloadSessionTable[ourSessionId]
            except KeyError:
              print(f"ERROR ID {request.sessionId} can not be deleted")
              context.set_details(msg)
              context.set_code(grpc.StatusCode.UNKNOWN)
              return openoffload_pb2.sessionResponse()


            timestamp = google.protobuf.timestamp_pb2.Timestamp()
            timestamp.GetCurrentTime()
            return openoffload_pb2.sessionResponse(sessionId=request.sessionId, sessionState=openoffload_pb2._CLOSING_1,
              requestStatus=openoffload_pb2._ACCEPTED,
              startTime=timestamp, endTime=timestamp);

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



    # This method would not exist in a production environment.  Its purpose is to simulate
    # traffic flowing through the offloaded sesstions, client RST, client FIN, server RST, server FIN,
    # and session timeouts.  This runs in its own thread.
    def simulateSessionsTraffic(self):

        while(True):
          print(f"\n----- START simulateSessionsTraffic() -----")
          closedCnt=0
          for sessionId, session in offloadSessionTable.items():
            if session["state"] == openoffload_pb2._CLOSED:
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
                session["state"] = openoffload_pb2._CLOSED
                session["sessionCloseCode"] = openoffload_pb2._TIMEOUT
                now = time.time()
                seconds = int(now)
                nanos = int((now - seconds) * 10**9)
                timestamp = google.protobuf.timestamp_pb2.Timestamp(seconds=seconds, nanos=nanos)
                session["endTime"] = timestamp

            elif sessionAction == 'clientFin':
                print(f"\tsimulateSessionsTraffic() - id:{sessionId} clientFin")
                session["state"] = openoffload_pb2._CLOSED
                session["sessionCloseCode"] = openoffload_pb2._FINACK
                now = time.time()
                seconds = int(now)
                nanos = int((now - seconds) * 10**9)
                timestamp = google.protobuf.timestamp_pb2.Timestamp(seconds=seconds, nanos=nanos)
                session["endTime"] = timestamp


            elif sessionAction == 'serverFin':
                print(f"\tsimulateSessionsTraffic() - id:{sessionId} serverFin")
                session["state"] = openoffload_pb2._CLOSED
                session["sessionCloseCode"] = openoffload_pb2._FINACK
                now = time.time()
                seconds = int(now)
                nanos = int((now - seconds) * 10**9)
                timestamp = google.protobuf.timestamp_pb2.Timestamp(seconds=seconds, nanos=nanos)
                session["endTime"] = timestamp

          activeCnt = len(offloadSessionTable) - closedCnt
          print(f"----- END simulateSessionsTraffic() total sessions ACTIVE={activeCnt} & CLOSED={closedCnt} -----")
          time.sleep(2)

    def GetClosedSessions(self, request, context):
            print("############ GET CLOSED SESSIONS ##################");

            for sessionId in list(offloadSessionTable):
              session = offloadSessionTable[sessionId]
              if session["state"] == openoffload_pb2._CLOSED:
                del offloadSessionTable[sessionId]
                yield openoffload_pb2.SessionResponse(session_id=session["clientSessionId"], session_state=session["state"], request_status=openoffload_pb2._ACCEPTED, in_packets=session["inPackets"], in_bytes=session["inBytes"], out_packets=session["outPackets"], out_bytes=session["outBytes"], start_time=session["startTime"], end_time=session["endTime"], session_close_code=session["sessionCloseCode"]);

    def GetAllSessions(self, request, context):
            print("############ GET ALL SESSIONS ##################");
            #print("page_size:",request.page_size);
            #print("page:",request.page);
    
            sessionCount = len(offloadSessionTable)

            print(f"Offload Session Table has {sessionCount} entries.")
            if request.page_size and request.page:
              if request.page == 1:
                startIndex = 0
              else:
                startIndex = request.page_size * (request.page - 1)

              endIndex = request.page_size * request.page - 1
              if endIndex > sessionCount - 1:
                endIndex = sessionCount - 1

            else:
              startIndex = 0
              endIndex = sessionCount - 1

            #print(f"start index = {startIndex} end index = {endIndex}")

            sessionResponses = openoffload_pb2.SessionResponses()
            for x in range(startIndex, endIndex + 1):
              sessionId = list(offloadSessionTable)[x]
              session = offloadSessionTable[sessionId]
              #print(f"index = {x} returning session {sessionId}")
              sessionResponse = openoffload_pb2.SessionResponse(session_id=session["clientSessionId"], session_state=session["state"], request_status=openoffload_pb2._ACCEPTED, in_packets=session["inPackets"], in_bytes=session["inBytes"], out_packets=session["outPackets"], out_bytes=session["outBytes"], start_time=session["startTime"], end_time=session["endTime"], session_close_code=session["sessionCloseCode"])

              sessionResponses.session_info.append(sessionResponse)

            sessionResponses.next_key=23333
            return sessionResponses
                

def serve():
    server = grpc.server(futures.ThreadPoolExecutor(max_workers=5))
    openoffload_pb2_grpc.add_SessionTableServicer_to_server(SessionTableServicer(), server)
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

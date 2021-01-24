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


from __future__ import print_function

import random
import logging
import time
from random import getrandbits
from ipaddress import IPv4Network, IPv4Address

import grpc

import openoffload_pb2
import openoffload_pb2_grpc


def session_getOffloadedSessions(stub, paramPageSize, paramPage):
    sessionCnt=0
    sessionResponseArray = stub.getAllSessions(openoffload_pb2.statisticsRequestArgs(pageSize=paramPageSize, page=paramPage))

    for sessionResponse in sessionResponseArray.responseArray:
      sessionCnt=sessionCnt+1
      print(f"SessionId: {sessionResponse.sessionId}")
      print(f"\tSession State: {openoffload_pb2._SESSION_STATE.values_by_number[sessionResponse.sessionState].name}")
      print(f"\tSession InPackets: {sessionResponse.inPackets} InBytes: {sessionResponse.inBytes} OutPackets: {sessionResponse.outPackets} OutBytes: {sessionResponse.outBytes}")
      print(f"\tSession End Reason: {openoffload_pb2._SESSION_CLOSE_CODE.values_by_number[sessionResponse.sessionCloseCode].name}\n")

    print(f"***Found {sessionCnt} sessions")


def run():
    global sessionTable, sessionId
    # NOTE(gRPC Python Team): .close() is possible on a channel and should be
    # used in circumstances in which the with statement does not fit the needs
    # of the code.
    with open('ssl/server.crt', 'rb') as f:
        creds = grpc.ssl_channel_credentials(f.read())
        channel = grpc.secure_channel('localhost:3443', creds)
        stub = openoffload_pb2_grpc.SessionTableStub(channel)
        print("\n\n-------------- Watch the Sessions --------------")
        while True:
          print("\n\n-------------- Get All the Sessions --------------")
          session_getOffloadedSessions(stub, 0, 0)

          # lets try out Paging
          print("\n\n-------------- Get the second five Sessions --------------")
          session_getOffloadedSessions(stub, 5, 2)
          time.sleep(3)

          print("\n")

if __name__ == '__main__':
    logging.basicConfig()
    run()


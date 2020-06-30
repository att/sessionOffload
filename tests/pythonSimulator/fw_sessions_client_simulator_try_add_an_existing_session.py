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
import socket

import grpc

import openoffload_pb2
import openoffload_pb2_grpc


def session_addSession(stub):
    session=openoffload_pb2.sessionRequest()
    session.inLif= 1
    session.outLif= 2
    session.ipVersion=openoffload_pb2._IPV4
    session.sourceIp=socket.inet_pton(socket.AF_INET, '10.2.2.4')
    session.sourcePort=12345
    session.destinationIp=socket.inet_pton(socket.AF_INET, '172.16.2.3')
    session.destinationPort=443
    session.protocolId=openoffload_pb2._TCP
    session.action.actionType=openoffload_pb2._FORWARD
    session.action.actionNextHop = "12.2.3.4"

    print("Adding Session to Offload Engine...")
    try:
      sessionResponse =  stub.addSession(session)
    except grpc.RpcError as e:
      print(f"ERROR Exception Caught: {e}")
      print(f"exception details: {e.details()}")
      status_code = e.code()
      print(f"exception status code: #{status_code.name}")
      print(f"exception status code value: #{status_code.value}")

    else:
      if sessionResponse.requestStatus == openoffload_pb2._REJECTED_SESSION_ALREADY_EXISTS:
          print(f"ERROR session already exists")
      elif sessionResponse.requestStatus == openoffload_pb2._REJECTED_SESSION_TABLE_FULL:
          print(f"WARNING session table is full")
      else:
          print(f"new offload session added id: {sessionResponse.sessionId}")




def run():
    # NOTE(gRPC Python Team): openoffload_pb2.close() is possible on a channel and should be
    # used in circumstances in which the with statement does not fit the needs
    # of the code.
    with open('ssl/server.crt', 'rb') as f:
        creds = grpc.ssl_channel_credentials(f.read())
        channel = grpc.secure_channel('localhost:3443', creds)
        stub = openoffload_pb2_grpc.SessionTableStub(channel)

        print("adding session")
        session_addSession(stub)

        print("adding the same session")
        session_addSession(stub)

if __name__ == '__main__':
    logging.basicConfig()
    run()


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

import sessions_pb2
import sessions_pb2_grpc





def run():
    # NOTE(gRPC Python Team): sessions_pb2.close() is possible on a channel and should be
    # used in circumstances in which the with statement does not fit the needs
    # of the code.
    with open('ssl/server.crt', 'rb') as f:
        creds = grpc.ssl_channel_credentials(f.read())
        channel = grpc.secure_channel('localhost:3443', creds)
        stub = sessions_pb2_grpc.SessionTableStub(channel)


        print("\n\ntry to delete and send a int64 session id that does not exist...")
        try:
          sessionDeleteResponse = stub.deleteSession( sessions_pb2.sessionId(sessionId=12333333))
        except grpc.RpcError as e:
          print(f"ERROR Exception Caught: {e}")

          print(f"exception details: {e.details()}")
          status_code = e.code()
          print(f"exception status code: #{status_code.name}")
          print(f"exception status code value: #{status_code.value}")
        else:
            if sessionDeleteResponse.requestStatus == sessions_pb2._REJECTED_SESSION_NONEXISTENT:
                print("Delete failed as expected since that session id did not exist")

if __name__ == '__main__':
    logging.basicConfig()
    run()


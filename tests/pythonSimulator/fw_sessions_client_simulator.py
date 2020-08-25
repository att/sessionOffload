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
# fw_sessions_client_simulator.py - This script simulates a FW that would receive the first packets of a
# new session and then decide to offload by calling GRPC services on the nos_sessions_server_simulator.py
#

from __future__ import print_function

import random
import logging
import time
import datetime
from random import getrandbits
from ipaddress import IPv4Network, IPv4Address, IPv6Address
import socket

import grpc

import openoffload_pb2
import openoffload_pb2_grpc
from google.protobuf.timestamp_pb2 import Timestamp

sessionTable = {}
startSessionId = 1000
lastSessionId = startSessionId
maxSessionId = 999999


def nextSessionId():
    global sessionTable, lastSessionId, maxSessionId, startSessionId
    if lastSessionId == maxSessionId:
      lastSessionId=startSessionId

    while True:
      if lastSessionId in sessionTable.keys():
        lastSessionId = lastSessionId + 1
      else:
        return lastSessionId



def session_addSession(stub):
    global sessionTable

    sessionId = nextSessionId()

    session=openoffload_pb2.sessionRequest()
    session.sessionId = sessionId
    session.inLif= 1
    session.outLif= 2
    session.ipVersion=openoffload_pb2._IPV4
    session.sourceIp=socket.inet_pton(socket.AF_INET, randomIp())
    session.sourcePort=int(random.randint(8192,65535))
    session.destinationIp=socket.inet_pton(socket.AF_INET, randomIp())
    session.destinationPort=randomServerPort()
    session.protocolId=openoffload_pb2._TCP
    session.action.actionType=openoffload_pb2._FORWARD
    session.action.actionNextHop = "12.2.3.4"

    print(f"\nAdding Session to local table new id {sessionId}")
    sessionTable[sessionId] = {
                                "inLif": session.inLif,
                                "outLif": session.outLif,
                                "ipVersion": session.ipVersion,
                                "sourceIp": session.sourceIp,
                                "sourcePort": session.sourcePort,
                                "destinationIp": session.destinationIp,
                                "destinationPort": session.destinationPort,
                                "protocolId": session.protocolId,
                                "startTime": time.time(),
                                "inPackets": 4,
                                "inBytes": random.randint(10,9999),
                                "outPackets": 3,
                                "outBytes": random.randint(10,9999),
                                "timeout": 3600,
                                "offloaded": False,
                                "offloadSessionId": None
                              }

    print("Requesting Offload of IPv4 Session...")
    try:
      sessionResponse =  stub.addSession(session)
    except grpc.RpcError as e:
      print(f"ERROR Exception Caught: {e}")
      print(f"exception details: {e.details()}")
      status_code = e.code()
      print(f"exception status code: #{status_code.name}")
      print(f"exception status code value: #{status_code.value}")
      return False
    else:
      if sessionResponse.requestStatus == openoffload_pb2._ACCEPTED:
        #:print(f"new session added Offload SessionId: {sessionResponse.sessionId}")
        print(f"new session added to Offload device")
        sessionTable[sessionId]["offloaded"] = True
        #sessionTable[sessionId]["offloadSessionId"] = sessionResponse.sessionId
      elif sessionResponse.requestStatus == openoffload_pb2._REJECTED_SESSION_TABLE_FULL:
        print(f"Offload Engine has no room for this session, Offload failed since offload session table is full")
      else:
        print("ERROR Offload Engine returned unknown response.")

    return sessionId


def session_addSessionIpv6(stub):
    global sessionTable

    sessionId = nextSessionId()
    session=openoffload_pb2.sessionRequest()
    session.sessionId = sessionId
    session.inLif= 1
    session.outLif= 2
    session.ipVersion=openoffload_pb2._IPV6
    session.sourceIp=socket.inet_pton(socket.AF_INET6, randomIpv6())
    session.sourcePort=int(random.randint(8192,65535))
    session.destinationIp=socket.inet_pton(socket.AF_INET6, randomIpv6())
    session.destinationPort=randomServerPort()
    session.protocolId=openoffload_pb2._UDP
    session.action.actionType=openoffload_pb2._FORWARD
    session.action.actionNextHop = "12.2.3.4"

    print(f"\nAdding Session to local table new id {sessionId}")
    timestamp = Timestamp()
    sessionTable[sessionId] = {
                                "inLif": session.inLif,
                                "outLif": session.outLif,
                                "ipVersion": session.ipVersion,
                                "sourceIp": session.sourceIp,
                                "sourcePort": session.sourcePort,
                                "destinationIp": session.destinationIp,
                                "destinationPort": session.destinationPort,
                                "protocolId": session.protocolId,
                                "startTime": time.time(),
                                "inPackets": 4,
                                "inBytes": random.randint(10,9999),
                                "outPackets": 3,
                                "outBytes": random.randint(10,9999),
                                "timeout": 3600,
                                "offloaded": False,
                                "offloadSessionId": None
                              }

    #print(f" dump table: {sessionTable}")
    print("Requesting Offload of IPv6 Session...")
    try:
      sessionResponse =  stub.addSession(session)
    except grpc.RpcError as e:
      print(f"ERROR Exception Caught: {e}")
      print(f"exception details: {e.details()}")
      status_code = e.code()
      print(f"exception status code: #{status_code.name}")
      print(f"exception status code value: #{status_code.value}")
      return False
    else:
      if sessionResponse.requestStatus == openoffload_pb2._ACCEPTED:
        print(f"new session added Offload device")
        sessionTable[sessionId]["offloaded"] = True
      elif sessionResponse.requestStatus == openoffload_pb2._REJECTED_SESSION_TABLE_FULL:
        print(f"Offload Engine has no room for this session, Offload failed since offload session table is full")
      else:
        print("ERROR Offload Engine returned unknown response.")

    return sessionId





def session_getSession(stub, sessionId):
    print(f"Getting Session id: {sessionId}")
    sessionResponse =  stub.getSession( openoffload_pb2.sessionId(sessionId=sessionId))
    print("SessionId:", sessionResponse.sessionId)
    print("Session State 0=ESTABLISHED :",sessionResponse.sessionState)
    print("Session RequestStatus:",sessionResponse.requestStatus)
    print("Session SessionEndCode:",sessionResponse.sessionCloseCode)
    print("Session InPackets",sessionResponse.inPackets)
    print("Session InBytes",sessionResponse.inBytes)
    print("Session OutPackets",sessionResponse.outPackets)
    print("Session OutBytes",sessionResponse.outBytes)
    print("Session startTime",sessionResponse.startTime)
    print("Session endTime",sessionResponse.endTime)


def session_deleteSession(stub, sessionId):
    sessionResponse =  stub.deleteSession( openoffload_pb2.sessionId(sessionId=sessionId))
    print("Getting Session")
    print("SessionId:", sessionResponse.sessionId)
    print("Session State 0=ESTABLISHED 1=CLOSING_1 :",sessionResponse.sessionState)
    print("Session RequestStatus:",sessionResponse.requestStatus)
    print("Session SessionEndCode:",sessionResponse.sessionCloseCode)
    print("Session InPackets",sessionResponse.inPackets)
    print("Session InBytes",sessionResponse.inBytes)
    print("Session startTime",sessionResponse.startTime)
    print("Session endTime",sessionResponse.endTime)

def session_addMirrorSession(stub):
    # not yet implemented within the local session table
    session=openoffload_pb2.sessionRequest()
    session.inLif= 1
    session.outLif= 2
    session.sourceIp="10.0.1.1"
    session.sourcePort=str(int.random(8096,65355))
    session.destinationIp="10.1.1.1"
    session.destinationPort=randomServerPort()
    session.protocolId=openoffload_pb2._UDP
    session.ipVersion=openoffload_pb2._IPV4
    session.action.actionType=openoffload_pb2._MIRROR
    session.action.actionNextHop = "12.2.3.4"
    sessionResponse =  stub.addSession( session)
    print("Adding Session")
    print(sessionResponse.sessionId)


def sessionClosedSessions(stub):
    global sessionTable

    #print(f"getClosed() dump table: {sessionTable}")
    for sessionResponse in stub.getClosedSessions(openoffload_pb2.sessionId(sessionId=0)):
      print(f"\n\nOFFLOAD SESSION END - Closed Session Offload SessionId: {sessionResponse.sessionId}")
      print(f"current sessionTable size: {len(sessionTable)}")

      if not sessionResponse.sessionId in sessionTable.keys():
        print(f"\tERROR session id: {sessionResponse.sessionId} not found in local session table")
      else: 
        session = sessionTable[sessionResponse.sessionId]
        print(f"\tFW sessionID: {sessionResponse.sessionId}  Session State: {openoffload_pb2._SESSION_STATE.values_by_number[sessionResponse.sessionState].name}")
        print(f"\tSession Start Time our table:",datetime.datetime.fromtimestamp(session["startTime"]), "Start Time Offload table:", sessionResponse.startTime.seconds, "End Time Offfload table:", sessionResponse.endTime.seconds)
        if session["ipVersion"] == openoffload_pb2._IPV4:
          print("\tsrcIP: %s srcPort: %d destIP: %s destPort: %d" % (socket.inet_ntop(socket.AF_INET, session["sourceIp"]), session["sourcePort"], socket.inet_ntop(socket.AF_INET, session["destinationIp"]), session["destinationPort"]));
        else:
          print("\tsrcIP: %s srcPort: %d destIP: %s destPort: %d" % (socket.inet_ntop(socket.AF_INET6, session["sourceIp"]), session["sourcePort"], socket.inet_ntop(socket.AF_INET6, session["destinationIp"]), session["destinationPort"]));
        print("\tSession InPackets: %d InBytes: %d OutPackets: %d OutBytes: %d" % (sessionResponse.inPackets + session["inPackets"], sessionResponse.inBytes + session["inBytes"], sessionResponse.outPackets + session["outPackets"], sessionResponse.outBytes + session["outBytes"]))
        print(f"\tSession End Reason: {openoffload_pb2._SESSION_CLOSE_CODE.values_by_number[sessionResponse.sessionCloseCode].name}")
        print(f"\tUPDATING FW Session Table to reflect how it closed and its counters.\n\tFW LOG would be created at session close.")

        # delete from local Session Table
        del sessionTable[sessionResponse.sessionId]




def randomServerPort():
    numberList = [22,80,443,162,110,25,23]
    return random.choice(numberList)

def randomIp():

    # network containing all addresses from 10.0.0.0 to 10.0.0.255
    subnet = IPv4Network("10.0.0.0/8")

    # subnet.max_prefixlen contains 32 for IPv4 subnets and 128 for IPv6 subnets
    # subnet.prefixlen is 24 in this case, so we'll generate only 8 random bits
    bits = getrandbits(subnet.max_prefixlen - subnet.prefixlen)

    # here, we combine the subnet and the random bits
    # to get an IP address from the previously specified subnet
    addr = IPv4Address(subnet.network_address + bits)
    addr_str = str(addr)
    return addr_str

def randomIpv6():

    # subnet.max_prefixlen contains 32 for IPv4 subnets and 128 for IPv6 subnets
    # subnet.prefixlen is 24 in this case, so we'll generate only 8 random bits
    bits = getrandbits(128)
    addr = IPv6Address(bits)
    addr_str = str(addr)
    return addr_str


def session_getOffloadedSessions(stub, paramPageSize, paramPage):
    sessionCnt=0
    for sessionResponse in stub.getAllSessions(openoffload_pb2.statisticsRequestArgs(pageSize=paramPageSize, page=paramPage)):
      sessionCnt=sessionCnt+1
      print(f"SessionId: {sessionResponse.sessionId}")
      print(f"\tSession State: {openoffload_pb2._SESSION_STATE.values_by_number[sessionResponse.sessionState].name}")
      print(f"\tSession InPackets: {sessionResponse.inPackets} InBytes: {sessionResponse.inBytes} OutPackets: {sessionResponse.outPackets} OutBytes: {sessionResponse.outBytes}")
      print(f"\tSession End Reason: {openoffload_pb2._SESSION_CLOSE_CODE.values_by_number[sessionResponse.sessionCloseCode].name}")
    print(f"\n\tFound {sessionCnt} offloaded sessions")

def run():
    # NOTE(gRPC Python Team): openoffload_pb2.close() is possible on a channel and should be
    # used in circumstances in which the with statement does not fit the needs
    # of the code.
    with open('ssl/server.crt', 'rb') as f:
        creds = grpc.ssl_channel_credentials(f.read())
        channel = grpc.secure_channel('localhost:3443', creds)
        stub = openoffload_pb2_grpc.SessionTableStub(channel)
        statsStub = openoffload_pb2_grpc.SessionStatisticsTableStub(channel)
        print("-------------- Initialize the FW session table ---------------")
        print(" Some firewalls may want to get all the current offloaded sessions")
        print(" and load them into its current session table.  An example could ")
        print(" be if a firewall has offload sessions and crashes/reboots.\n\n") 
        session_getOffloadedSessions(statsStub, 0, 0)
        # we currently just print these on the screen, but an actual production 
        # implementation would want to consider loading them into the firwall's 
        # session table.


        print("-------------- Adding 10 IPv4 & 10 IPv6 Sessions --------------")
        newSessionId=None
        for x in range(10):
          newSessionId=session_addSession(stub)
          print(f"Added new session id: {newSessionId}")

        print("Fetch the session from Offload device for testing purposes.")
        session_getSession(stub, newSessionId)

        for x in range(10):
          newSessionId=session_addSessionIpv6(stub)

        # We did not thread this code, but would expect parrallism be used to add new offload
        # sessions and to receive the offloaded sessions that have closed.
        while True:
          print("\n-------------- Get All the Closed Sessions and update our session table  --------------")
          sessionClosedSessions(statsStub)

          for x in range(2):
            print("\nAdding new IPv4 Session")
            newSessionId=session_addSession(stub)
          for x in range(2):
            print("\nAdding new IPv6 Session")
            newSessionId=session_addSessionIpv6(stub)

          time.sleep(2)

if __name__ == '__main__':
    logging.basicConfig()
    run()


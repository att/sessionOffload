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
import sys, getopt

import grpc

import openoffload_pb2
import openoffload_pb2_grpc
from google.protobuf.timestamp_pb2 import Timestamp

sessionTable = {}
startSessionId = 1000
lastSessionId = startSessionId
maxSessionId = 999999
offloadedIPv4count = 0
offloadedIPv6count = 0
attemptsToOffload = 0


def printSessionResponse(sessionResponse):
        global sessionTable

        session = sessionTable[sessionResponse.sessionId]
        print(f"\tFW sessionID: {sessionResponse.sessionId}  Session State: {openoffload_pb2._SESSION_STATE.values_by_number[sessionResponse.sessionState].name}")
        print(f"\tSession Start Time our table:",datetime.datetime.fromtimestamp(session["startTime"]), "Start Time Offload table:", sessionResponse.startTime.seconds, "End Time Offfload table:", sessionResponse.endTime.seconds)
        if session["ipVersion"] == openoffload_pb2._IPV4:
          #print("\tsrcIP: %s srcPort: %d destIP: %s destPort: %d" % (socket.inet_ntop(socket.AF_INET, session["sourceIp"]), session["sourcePort"], socket.inet_ntop(socket.AF_INET, session["destinationIp"]), session["destinationPort"]));
          print("\tsrcIP: %s srcPort: %d destIP: %s destPort: %d" % (socket.inet_ntop(socket.AF_INET, session["sourceIp"].to_bytes(4,byteorder=sys.byteorder)), session["sourcePort"], socket.inet_ntop(socket.AF_INET, session["destinationIp"].to_bytes(4,byteorder=sys.byteorder)), session["destinationPort"]));
        else:
          print("\tsrcIP: %s srcPort: %d destIP: %s destPort: %d" % (socket.inet_ntop(socket.AF_INET6, session["sourceIpV6"]), session["sourcePort"], socket.inet_ntop(socket.AF_INET6, session["destinationIpV6"]), session["destinationPort"]));
        print("\tSession InPackets: %d InBytes: %d OutPackets: %d OutBytes: %d" % (sessionResponse.inPackets + session["inPackets"], sessionResponse.inBytes + session["inBytes"], sessionResponse.outPackets + session["outPackets"], sessionResponse.outBytes + session["outBytes"]))
        print(f"\tSession End Reason: {openoffload_pb2._SESSION_CLOSE_CODE.values_by_number[sessionResponse.sessionCloseCode].name}")


def nextSessionId():
    global sessionTable, lastSessionId, maxSessionId, startSessionId
    if lastSessionId == maxSessionId:
      lastSessionId=startSessionId

    while True:
      if lastSessionId in sessionTable.keys():
        lastSessionId = lastSessionId + 1
      else:
        return lastSessionId



def session_addSessions(stub, cnt = 1, type = 'IPv4'):
    global sessionTable
    newSessions=[]

    print(f"session_addSessions() add {cnt} {type} sessions ")
    for x in range(cnt):

      sessionId = nextSessionId()

      session=openoffload_pb2.sessionRequest()
      session.sessionId = sessionId
      session.inLif= 1
      session.outLif= 2
      session.sourcePort=int(random.randint(8192,65535))
      session.destinationPort=randomServerPort()
      session.protocolId=openoffload_pb2._TCP
      session.action.actionType=openoffload_pb2._FORWARD
      session.action.actionNextHop=int.from_bytes(socket.inet_pton(socket.AF_INET, "12.2.3.4"), byteorder=sys.byteorder)
      if type == 'IPv4':
        session.ipVersion=openoffload_pb2._IPV4
        session.sourceIp=int.from_bytes(socket.inet_pton(socket.AF_INET, randomIp()), byteorder=sys.byteorder)
        session.destinationIp=int.from_bytes(socket.inet_pton(socket.AF_INET, randomIp()), byteorder=sys.byteorder)
      else:
        session.ipVersion=openoffload_pb2._IPV6
        session.sourceIpV6=socket.inet_pton(socket.AF_INET6, randomIpv6())
        session.destinationIpV6=socket.inet_pton(socket.AF_INET6, randomIpv6())

      newSessions.append(session)

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
                                "offloaded": True
                              }
      if type == 'IPv4':
        sessionTable[sessionId]["sourceIp"] = session.sourceIp
        sessionTable[sessionId]["destinationIp"] = session.destinationIp
      else:
        sessionTable[sessionId]["sourceIpV6"] = session.sourceIpV6
        sessionTable[sessionId]["destinationIpV6"] = session.destinationIpV6

    try:
      print(f"calling GRPC rpc addSession to attempt to offload {len(newSessions)} sessions")
      addSessionResponse = stub.addSession(iter(newSessions))

    except grpc.RpcError as e:
      print(f"ERROR Exception Caught: {e}")
      print(f"exception details: {e.details()}")
      status_code = e.code()
      print(f"exception status code: {status_code.name}")
      print(f"exception status code value: {status_code.value}")
      # this exception handler should make getSession calls to determine if any of the passed sessions were successfully offloaded.

    if addSessionResponse.responseError:
      for error in addSessionResponse.responseError:
        print(f"INFO Failed Offloading session id:{error.sessionId}   errornumber:{error.errorStatus}  errorname:{openoffload_pb2._ADD_SESSION_STATUS.values_by_number[error.errorStatus].name}")
        sessionTable[error.sessionId]["offloaded"] = False


def session_getSession(stub, sessionId):
    print(f"Getting Session id: {sessionId}")
    sessionResponse = stub.getSession( openoffload_pb2.sessionId(sessionId=sessionId))
    printSessionResponse(sessionResponse)

def session_deleteSession(stub, sessionId):
    print(f"Deleting Session Id:{sessionId}")
    sessionResponse =  stub.deleteSession( openoffload_pb2.sessionId(sessionId=sessionId))

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

      if not sessionResponse.sessionId in sessionTable.keys():
        print(f"\tERROR session id: {sessionResponse.sessionId} not found in local session table")
      else: 
        session = sessionTable[sessionResponse.sessionId]
        print(f"\tFW sessionID: {sessionResponse.sessionId}  Session State: {openoffload_pb2._SESSION_STATE.values_by_number[sessionResponse.sessionState].name}")
        print(f"\tSession Start Time our table:",datetime.datetime.fromtimestamp(session["startTime"]), "Start Time Offload table:", sessionResponse.startTime.seconds, "End Time Offfload table:", sessionResponse.endTime.seconds)
        if session["ipVersion"] == openoffload_pb2._IPV4:
          print("\tsrcIP: %s srcPort: %d destIP: %s destPort: %d" % (socket.inet_ntop(socket.AF_INET, session["sourceIp"].to_bytes(4,byteorder=sys.byteorder)), session["sourcePort"], socket.inet_ntop(socket.AF_INET, session["destinationIp"].to_bytes(4,byteorder=sys.byteorder)), session["destinationPort"]));
        else:
          print("\tsrcIP: %s srcPort: %d destIP: %s destPort: %d" % (socket.inet_ntop(socket.AF_INET6, session["sourceIpV6"]), session["sourcePort"], socket.inet_ntop(socket.AF_INET6, session["destinationIpV6"]), session["destinationPort"]));
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
    sessionResponseArray = stub.getAllSessions(openoffload_pb2.statisticsRequestArgs(pageSize=paramPageSize, page=paramPage))

    for sessionResponse in sessionResponseArray.responseArray:
      sessionCnt=sessionCnt+1
      #print(f"\n\n\n******************\ndump session = {sessionResponse}")
      print(f"SessionId: {sessionResponse.sessionId}")
      print(f"\tSession State: {openoffload_pb2._SESSION_STATE.values_by_number[sessionResponse.sessionState].name}")
      print(f"\tSession InPackets: {sessionResponse.inPackets} InBytes: {sessionResponse.inBytes} OutPackets: {sessionResponse.outPackets} OutBytes: {sessionResponse.outBytes}")
      print(f"\tSession End Reason: {openoffload_pb2._SESSION_CLOSE_CODE.values_by_number[sessionResponse.sessionCloseCode].name}\n")
    print(f"\n\n\n\tFound {sessionCnt} offloaded sessions")


def run(loopmax):
    # NOTE(gRPC Python Team): openoffload_pb2.close() is possible on a channel and should be
    # used in circumstances in which the with statement does not fit the needs
    # of the code.
    with open('ssl/server.crt', 'rb') as f:
        creds = grpc.ssl_channel_credentials(f.read())
        channel = grpc.secure_channel('localhost:3443', creds)
        stub = openoffload_pb2_grpc.SessionTableStub(channel)
        print("-------------- Initialize the FW session table ---------------")
        print(" Some firewalls may want to get all the current offloaded sessions")
        print(" and load them into its current session table.  An example could ")
        print(" be if a firewall has offload sessions and crashes/reboots.\n\n") 
        session_getOffloadedSessions(stub, 0, 0)
        # we currently just print these on the screen, but an actual production 
        # implementation would want to consider loading them into the firwall's 
        # session table.
        time.sleep(3)

        print("\n\n\n")
        print("-------------- Adding 10 IPv4 & 10 IPv6 Sessions --------------")
        session_addSessions(stub, 10, 'IPv4')
        session_addSessions(stub, 10, 'IPv6')

        time.sleep(3)
        print("\n\n\n")

        # We did not thread this code, but would expect parrallism be used to add new offload
        # sessions and to receive the offloaded sessions that have closed.
        #while True:
        loop=0
        while loop <= loopmax:
          if (loopmax > 0):
              loop=loop+1
          print("\n-------------- Get All the Closed Sessions and update our session table  --------------")
          sessionClosedSessions(stub)

          print("\n\n------------ Adding new IPv4 Sessions ------------")
          session_addSessions(stub, 3, 'IPv4')
          print("\n\n\n")

          # test grpc rpc call to get a single session
          for sessionId  in sessionTable:
            if sessionTable[sessionId]["offloaded"] == True:
              print(f"------------ Fetch the session from Offload device for testing purposes using sessionId: #{sessionId}. ------------")
              session_getSession(stub, sessionId)
              break
          print("\n\n\n")

          print("\n\n------------ Adding new IPv6 Sessions ------------")
          session_addSessions(stub, 3, 'IPv6')
          print("\n\n\n")

          # test grpc rpc call to get a single session
          for sessionId  in sessionTable:
            if sessionTable[sessionId]["offloaded"] == True:
              print(f"------------ Fetch the session from Offload device for testing purposes using sessionId: #{sessionId}. ------------")
              session_getSession(stub, sessionId)
              break
          print("\n\n\n")

          time.sleep(2)

          print("\n\n------------ List All OffloadedSessions ------------") 
          session_getOffloadedSessions(stub, 0, 0)
          print("\n\n\n")

          time.sleep(5)




if __name__ == '__main__':
   loopmax= 0
   try:
       opts, args = getopt.getopt(sys.argv[1:],"h",["loopmax="])
   except getopt.GetoptError:
      print('fw_sessions_client_simulator.py --loopmax <max_add_sessions>')
      sys.exit(2)
   for opt, arg in opts:
      if opt == '-h':
         print('fw_sessions_client_simulator.py --loopmax <max_add_sessions>')
         print('loopmax blank or 0 is infinite loop')
         sys.exit()
      elif opt in ("--loopmax"):
         loopmax= int(arg)
   print ('loopmax is ', loopmax)
   logging.basicConfig()
   run(loopmax)


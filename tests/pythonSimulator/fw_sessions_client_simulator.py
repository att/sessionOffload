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

        session = sessionTable[sessionResponse.session_id]
        print(f"\tFW sessionID: {sessionResponse.session_id}  Session State: {openoffload_pb2._SESSIONSTATE.values_by_number[sessionResponse.session_state].name}")
        print(f"\tSession Start Time our table:",datetime.datetime.fromtimestamp(session["start_time"]), "Start Time Offload table:", sessionResponse.start_time.seconds, "End Time Offfload table:", sessionResponse.end_time.seconds)
        if session["ip_version"] == openoffload_pb2._IPV4:
          #print("\tsrcIP: %s srcPort: %d destIP: %s destPort: %d" % (socket.inet_ntop(socket.AF_INET, session["sourceIp"]), session["sourcePort"], socket.inet_ntop(socket.AF_INET, session["destinationIp"]), session["destinationPort"]));
          print("\tsrcIP: %s srcPort: %d destIP: %s destPort: %d" % (socket.inet_ntop(socket.AF_INET, session["source_ip"].to_bytes(4,byteorder=sys.byteorder)), session["source_port"], socket.inet_ntop(socket.AF_INET, session["destination_ip"].to_bytes(4,byteorder=sys.byteorder)), session["destination_port"]));
        else:
          print("\tsrcIP: %s srcPort: %d destIP: %s destPort: %d" % (socket.inet_ntop(socket.AF_INET6, session["source_ipv6"]), session["source_port"], socket.inet_ntop(socket.AF_INET6, session["destination_ipv6"]), session["destination_port"]));
        print("\tSession InPackets: %d InBytes: %d OutPackets: %d OutBytes: %d" % (sessionResponse.in_packets + session["in_packets"], sessionResponse.in_bytes + session["in_bytes"], sessionResponse.out_packets + session["out_packets"], sessionResponse.out_bytes + session["out_bytes"]))
        print(f"\tSession End Reason: {openoffload_pb2._SESSIONCLOSECODE.values_by_number[sessionResponse.session_close_code].name}")


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

      session=openoffload_pb2.SessionRequest()
      session.session_id = sessionId
      session.in_lif= 1
      session.out_lif= 2
      session.source_port=int(random.randint(8192,65535))
      session.destination_port=randomServerPort()
      session.protocol_id=openoffload_pb2._TCP
      session.action.action_type=openoffload_pb2._FORWARD
      session.action.action_next_hop=int.from_bytes(socket.inet_pton(socket.AF_INET, "12.2.3.4"), byteorder=sys.byteorder)
      if type == 'IPv4':
        session.ip_version=openoffload_pb2._IPV4
        session.source_ip=int.from_bytes(socket.inet_pton(socket.AF_INET, randomIp()), byteorder=sys.byteorder)
        session.destination_ip=int.from_bytes(socket.inet_pton(socket.AF_INET, randomIp()), byteorder=sys.byteorder)
      else:
        session.ip_version=openoffload_pb2._IPV6
        session.source_ipv6=socket.inet_pton(socket.AF_INET6, randomIpv6())
        session.destination_ipv6=socket.inet_pton(socket.AF_INET6, randomIpv6())

      newSessions.append(session)

      print(f"\nAdding Session to local table new id {sessionId}")
      sessionTable[sessionId] = {
                                "in_lif": session.in_lif,
                                "out_lif": session.out_lif,
                                "ip_version": session.ip_version,
                                "source_ip": session.source_ip,
                                "source_port": session.source_port,
                                "destination_ip": session.destination_ip,
                                "destination_port": session.destination_port,
                                "protocol_id": session.protocol_id,
                                "start_time": time.time(),
                                "in_packets": 4,
                                "in_bytes": random.randint(10,9999),
                                "out_packets": 3,
                                "out_bytes": random.randint(10,9999),
                                "timeout": 3600,
                                "offloaded": True
                              }
      if type == 'IPv4':
        sessionTable[sessionId]["source_ip"] = session.source_ip
        sessionTable[sessionId]["destination_ip"] = session.destination_ip
      else:
        sessionTable[sessionId]["source_ipv6"] = session.source_ipv6
        sessionTable[sessionId]["destination_ipv6"] = session.destination_ipv6

    try:
      print(f"calling GRPC rpc addSession to attempt to offload {len(newSessions)} sessions")
      addSessionResponse = stub.AddSession(iter(newSessions))

    except grpc.RpcError as e:
      print(f"ERROR Exception Caught: {e}")
      print(f"exception details: {e.details()}")
      status_code = e.code()
      print(f"exception status code: {status_code.name}")
      print(f"exception status code value: {status_code.value}")
      # this exception handler should make getSession calls to determine if any of the passed sessions were successfully offloaded.

    if addSessionResponse.response_error:
      for error in addSessionResponse.response_error:
        print(f"INFO Failed Offloading session id:{error.session_id}   errornumber:{error.error_status}  errorname:{openoffload_pb2._ADDSESSIONSTATUS.values_by_number[error.error_status].name}")
        sessionTable[error.session_id]["offloaded"] = False


def session_getSession(stub, sessionId):
    print(f"Getting Session id: {sessionId}")
    sessionResponse = stub.GetSession( openoffload_pb2.SessionId(session_id=sessionId))
    printSessionResponse(sessionResponse)

def session_deleteSession(stub, sessionId):
    print(f"Deleting Session Id:{sessionId}")
    sessionResponse =  stub.DeleteSession( openoffload_pb2.SessionId(session_id=sessionId))

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
    for sessionResponse in stub.GetClosedSessions(openoffload_pb2.SessionId(session_id=0)):
      print(f"\n\nOFFLOAD SESSION END - Closed Session Offload SessionId: {sessionResponse.session_id}")

      if not sessionResponse.session_id in sessionTable.keys():
        print(f"\tERROR session id: {sessionResponse.sessionId} not found in local session table")
      else: 
        session = sessionTable[sessionResponse.session_id]
        print(f"\tFW sessionID: {sessionResponse.session_id}  Session State: {openoffload_pb2._SESSIONSTATE.values_by_number[sessionResponse.session_state].name}")
        print(f"\tSession Start Time our table:",datetime.datetime.fromtimestamp(session["start_time"]), "Start Time Offload table:", sessionResponse.start_time.seconds, "End Time Offfload table:", sessionResponse.end_time.seconds)
        if session["ip_version"] == openoffload_pb2._IPV4:
          print("\tsrcIP: %s srcPort: %d destIP: %s destPort: %d" % (socket.inet_ntop(socket.AF_INET, session["source_ip"].to_bytes(4,byteorder=sys.byteorder)), session["source_port"], socket.inet_ntop(socket.AF_INET, session["destination_ip"].to_bytes(4,byteorder=sys.byteorder)), session["destination_port"]));
        else:
          print("\tsrcIP: %s srcPort: %d destIP: %s destPort: %d" % (socket.inet_ntop(socket.AF_INET6, session["source_ipv6"]), session["source_port"], socket.inet_ntop(socket.AF_INET6, session["destination_ipv6"]), session["destination_port"]));
        print("\tSession InPackets: %d InBytes: %d OutPackets: %d OutBytes: %d" % (sessionResponse.in_packets + session["in_packets"], sessionResponse.in_bytes + session["in_bytes"], sessionResponse.out_packets + session["out_packets"], sessionResponse.out_bytes + session["out_bytes"]))
        print(f"\tSession End Reason: {openoffload_pb2._SESSIONCLOSECODE.values_by_number[sessionResponse.session_close_code].name}")
        print(f"\tUPDATING FW Session Table to reflect how it closed and its counters.\n\tFW LOG would be created at session close.")

        # delete from local Session Table
        del sessionTable[sessionResponse.session_id]




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
    sessionResponses = stub.GetAllSessions(openoffload_pb2.SessionRequestArgs(page_size=paramPageSize, page=paramPage))

    for sessionResponse in sessionResponses.session_info:
      sessionCnt=sessionCnt+1
      #print(f"\n\n\n******************\ndump session = {sessionResponse}")
      print(f"SessionId: {sessionResponse.session_id}")
      print(f"\tSession State: {openoffload_pb2._SESSIONSTATE.values_by_number[sessionResponse.session_state].name}")
      print(f"\tSession InPackets: {sessionResponse.in_packets} InBytes: {sessionResponse.in_bytes} OutPackets: {sessionResponse.out_packets} OutBytes: {sessionResponse.out_bytes}")
      print(f"\tSession End Reason: {openoffload_pb2._SESSIONCLOSECODE.values_by_number[sessionResponse.session_close_code].name}\n")
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


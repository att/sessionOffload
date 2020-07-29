
# OpenOffload Python Simulator

gRPC API to offload TCP and UDP stateful packet processing from an application to a hardware device.  The simulator consists of two main scripts.  The first script we execute is the nos_sessions_server_simulator.py.  This script should be thought of as the hardware device that implements the stateful session offload and implements the gRPC server side. The second script we execute is the fw_sessions_client_simulator.py. This second script should be thought of as the firewall device which desires to offload stateful existing sessions.


# Installation and build 

This section describes how to build and execute the simulators.

## Requisites

* Install Python 3.6+ (we used 3.6.9)
* Install gRPC for Python (see References)

## Build

The following command builds the Python Simulator code.  This basic shell script will create a new directory named buildPythonSimulator and run the gRPC code to generate the server and client interfaces from the openoffload protobuffer.  It will then copy the necessary scripts and SSL/TLS keys to the new directory leaving the user with a test environment.

```bash
$ ./builditPythonSimulator.sh
```

## Testing the sample code

Create three separate terminal windows to run the server, client, and a script to watch the offload sessions. 

### Server 1 Window (Offload sessions server)

Start the offload server in one window by going to the build directory and running the script below.

```bash
$ cd buildPythonSimulator
$ ./runServer.sh
```

Sample Output
```bash
----- START simulateSessionsTraffic() -----
----- END simulateSessionsTraffic() total sessions ACTIVE=0 & CLOSED=0 -----
############ GET ALL SESSIONS ##################
pageSize: 0
page: 0
Session table has 0 entries.
start index = 0 end index = -1
############ ADD SESSION ##################
IP Version: _IPV4
sourceIp: 10.63.72.212
sourcePort: 56666
destinationIp: 10.205.201.193
destinationPort: 162
protocolId: 6
ActionType 0=DROP,1=FORWARD,2=MIRROR,3=SNOOP: 1
ActionNextHop: 12.2.3.4
Found slot for new offload session @ id = 1
############ ADD SESSION ##################
IP Version: _IPV4
sourceIp: 10.180.28.28
sourcePort: 36620
destinationIp: 10.231.144.220
destinationPort: 110
protocolId: 6
ActionType 0=DROP,1=FORWARD,2=MIRROR,3=SNOOP: 1
ActionNextHop: 12.2.3.4
Found slot for new offload session @ id = 2
############ ADD SESSION ##################
IP Version: _IPV4
sourceIp: 10.152.155.134
sourcePort: 51991
destinationIp: 10.223.160.68
destinationPort: 23
protocolId: 6
ActionType 0=DROP,1=FORWARD,2=MIRROR,3=SNOOP: 1
ActionNextHop: 12.2.3.4
Found slot for new offload session @ id = 3
```



### Server 2 Window (Firewall offload client)

Start the firewall client in another window by going to the build directory and running the script below. This script will simulate new sessions being processed by the firewall and then trying to send them to the offload sessions server.  It randomily creates sessions and then calls the offload server to receive back offloaded sessions that have closed / completed on the offload sessions server.

```bash
$ cd buildBasic
$ ./runClient.sh
```

Sample Output
```bash
-------------- Initialize the FW session table ---------------
 Some firewalls may want to get all the current offloaded sessions
 and load them into its current session table.  An example could
 be if a firewall has offload sessions and crashes/reboots.



        Found 0 offloaded sessions
-------------- Adding 10 IPv4 & 10 IPv6 Sessions --------------

Adding Session to local table new id 1000
Requesting Offload of IPv4 Session...
new session added Offload SessionId: 1

Adding Session to local table new id 1001
Requesting Offload of IPv4 Session...
new session added Offload SessionId: 2

Adding Session to local table new id 1002
Requesting Offload of IPv4 Session...
new session added Offload SessionId: 3

Adding Session to local table new id 1003
Requesting Offload of IPv4 Session...
new session added Offload SessionId: 4

Adding Session to local table new id 1004
Requesting Offload of IPv4 Session...
new session added Offload SessionId: 5

Adding Session to local table new id 1005
Requesting Offload of IPv4 Session...
new session added Offload SessionId: 6

Adding Session to local table new id 1006
Requesting Offload of IPv4 Session...
new session added Offload SessionId: 7

Adding Session to local table new id 1007
Requesting Offload of IPv4 Session...
new session added Offload SessionId: 8

Adding Session to local table new id 1008
Requesting Offload of IPv4 Session...
new session added Offload SessionId: 9

Adding Session to local table new id 1009
Requesting Offload of IPv4 Session...
new session added Offload SessionId: 10

Adding Session to local table new id 1010
Requesting Offload of IPv6 Session...
new session added Offload SessionId: 11

Adding Session to local table new id 1011
Requesting Offload of IPv6 Session...
new session added Offload SessionId: 12

Adding Session to local table new id 1012
Requesting Offload of IPv6 Session...
new session added Offload SessionId: 13

Adding Session to local table new id 1013
Requesting Offload of IPv6 Session...
new session added Offload SessionId: 14

Adding Session to local table new id 1014
Requesting Offload of IPv6 Session...
new session added Offload SessionId: 15

Adding Session to local table new id 1015
Requesting Offload of IPv6 Session...
new session added Offload SessionId: 16

Adding Session to local table new id 1016
Requesting Offload of IPv6 Session...
new session added Offload SessionId: 17

Adding Session to local table new id 1017
Requesting Offload of IPv6 Session...
new session added Offload SessionId: 18

Adding Session to local table new id 1018
Requesting Offload of IPv6 Session...
new session added Offload SessionId: 19

Adding Session to local table new id 1019
Requesting Offload of IPv6 Session...
Offload Engine has no room for this session, Offload failed since offload session table is full

-------------- Get All the Closed Sessions and update our session table  --------------

Adding new IPv4 Session

Adding Session to local table new id 1020
Requesting Offload of IPv4 Session...
Offload Engine has no room for this session, Offload failed since offload session table is full

Adding new IPv4 Session

Adding Session to local table new id 1021
Requesting Offload of IPv4 Session...
Offload Engine has no room for this session, Offload failed since offload session table is full

Adding new IPv6 Session

Adding Session to local table new id 1022
Requesting Offload of IPv6 Session...
Offload Engine has no room for this session, Offload failed since offload session table is full

Adding new IPv6 Session

Adding Session to local table new id 1023
Requesting Offload of IPv6 Session...
Offload Engine has no room for this session, Offload failed since offload session table is full
```

### Server 3 Window (Debugging tool to print out the offloaded sessions)

Start the watch offloaded sessions client in another window by going to the build directory and running the script below.

```bash
$ cd buildBasic
$ ./fw_watch_nos_sessions.py
```

Sample Output
```bash
-------------- Get All the Sessions --------------
SessionId: 1
        Session State: _ESTABLISHED
        Session InPackets: 298 InBytes: 42232 OutPackets: 491 OutBytes: 46158
        Session End Reason: _NOT_CLOSED
SessionId: 2
        Session State: _ESTABLISHED
        Session InPackets: 542 InBytes: 30594 OutPackets: 617 OutBytes: 41208
        Session End Reason: _NOT_CLOSED
SessionId: 3
        Session State: _ESTABLISHED
        Session InPackets: 260 InBytes: 32398 OutPackets: 374 OutBytes: 22732
        Session End Reason: _NOT_CLOSED
SessionId: 4
        Session State: _CLOSED
        Session InPackets: 37 InBytes: 30778 OutPackets: 250 OutBytes: 12010
        Session End Reason: _FINACK
SessionId: 5
        Session State: _ESTABLISHED
        Session InPackets: 81 InBytes: 18975 OutPackets: 97 OutBytes: 8081
        Session End Reason: _NOT_CLOSED
SessionId: 6
        Session State: _CLOSED
        Session InPackets: 0 InBytes: 0 OutPackets: 0 OutBytes: 0
        Session End Reason: _FINACK
SessionId: 7
        Session State: _ESTABLISHED
        Session InPackets: 116 InBytes: 11651 OutPackets: 211 OutBytes: 33072
        Session End Reason: _NOT_CLOSED
***Found 7 sessions


-------------- Get the second five Sessions --------------
SessionId: 6
        Session State: _CLOSED
        Session InPackets: 0 InBytes: 0 OutPackets: 0 OutBytes: 0
        Session End Reason: _FINACK
SessionId: 7
        Session State: _ESTABLISHED
        Session InPackets: 116 InBytes: 11651 OutPackets: 211 OutBytes: 33072
        Session End Reason: _NOT_CLOSED
SessionId: 8
        Session State: _CLOSED
        Session InPackets: 352 InBytes: 33973 OutPackets: 513 OutBytes: 23844
        Session End Reason: _FINACK
SessionId: 9
        Session State: _ESTABLISHED
        Session InPackets: 675 InBytes: 57692 OutPackets: 317 OutBytes: 25710
        Session End Reason: _NOT_CLOSED
SessionId: 10
        Session State: _ESTABLISHED
        Session InPackets: 721 InBytes: 17053 OutPackets: 672 OutBytes: 23144
        Session End Reason: _NOT_CLOSED
***Found 5 sessions

```
MAINTAINERS
-----------

Developed for AT&T by Brian Freeman and Richard Bowman, June 2020

Current maintainers:
 * Brian Freeman (at&t)
 * Richard Bowman (at&t)

# References

1. [gRPC Python Quick Start Guide](https://grpc.io/docs/quickstart/python/)



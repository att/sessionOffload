
# sessionOffload

gRPC API to offload TCP and UDP packet processing from an application to a hardware switch

# Requisites

* Install Python
* Install gRPC for Python (see References)

# Build

The following command builds the Basic Python code
Note: The PythonSimulator has more functional testing of error conditions and follows a similar build/test pattern.

```bash
$ ./builditBasic.sh
```

# Testing the sample code

Create three separate terminal windows a client and two server windows

## Server 1 Window (sessions)

Start the sessions server in one window by going to the build directory and running the script below.

```bash
$ cd buildBasic
$ ./runServer1.sh
```
## Server 2 Window (statistics)

Start the statistics server in another window by going to the build directory and running the script below.

```bash
$ cd buildBasic
$ ./runServer2.sh
```
## Client window

Start the client in the third window by going to the build directory and running the script below.

```bash
$ cd buildBasic
$ ./runClient.sh
```
There should be results in the client window similar to the below:

```bash
-------------- Add Session --------------
Adding Session
1001
-------------- Get Session --------------
Getting Session
SessionId: 1001
Session State 0=ESTABLISHED : 0
Session RequestStatus: 0
Session ReasonCode: 0
Session InPackets 1000
Session OutPackets 200000
Session startTime seconds: 1587584225
nanos: 580877000

Session endTime
-------------- Delete Session --------------
Getting Session
SessionId: 1001
Session State 0=ESTABLISHED 1=CLOSING_1 : 1
Session RequestStatus: 0
Session ReasonCode: 0
Session InPackets 2000
Session OutPackets 400000
Session startTime seconds: 1587584225
nanos: 581815000

Session endTime seconds: 1587584225
nanos: 581815000

-------------- Add Mirror and Forward Session --------------
Adding Session
1001
-------------- Check for Closed Sessions --------------
### RECEIVED SESSION #####
SessionId: 1001
Session State 0=ESTABLISHED 1=CLOSING_1 : 3
Session RequestStatus: 0
Session ReasonCode: 0
Session InPackets 1000
Session OutPackets 200000
Session startTime seconds: 1587584225
nanos: 583855000

Session endTime
##########################
### RECEIVED SESSION #####
SessionId: 1002
Session State 0=ESTABLISHED 1=CLOSING_1 : 3
Session RequestStatus: 0
Session ReasonCode: 0
Session InPackets 2000
Session OutPackets 400000
Session startTime seconds: 1587584225
nanos: 583855000

Session endTime
##########################
```

MAINTAINERS
-----------

Developed for AT&T by Brian Freeman and Richard Bowman, June 2020

Current maintainers:
 * Brian Freeman (at&t)
 * Richard Bowman (at&t)

# References

1. [gRPC Python Quick Start Guide](https://grpc.io/docs/quickstart/python/)



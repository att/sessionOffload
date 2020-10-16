
# OpenOffload

gRPC API to offload TCP and UDP packet processing from an application to a hardware switch

# Creating Language Bindings

There are three language bindings implemented in the language Makefile.

- Python
- Golang
- C++

To generate the language bindings for a specific language the appropriate language and related gRPC tools need to be installed. It is the responsibility of individual implementer to create there client and server code. 

## Generating code

### Python

```bash
$ cd openoffload
$ make all-py
```

### Golang

```bash
$ cd openoffload
$ make all-go
```
### C++

```bash
$ cd openoffload
$ make all-cpp
```

# Installation and build for tests

Depending on which language binding used different development tools need to be installed. To just run the tests only Python is required.

## Requisites

* Install Python
* Install gRPC for Python (see References)

## Build

The following command builds the Basic Python code
Note: The PythonSimulator has more functional testing of error conditions and follows a similar build/test pattern.

```bash
$ ./builditBasic.sh
```

## Testing the sample code

Create four separate terminal windows a client and three server windows

### Server 1  Window (Offload sessions)

Start the offload server in one window by going to the build directory and running the script below.

```bash
$ cd buildBasic
$ ./runOffloadServer.sh
```
### Server 2 Window (activation)

Start the activation server in another window by going to the build directory and running the script below.

```bash
$ cd buildBasic
$ ./runActivationServer.sh
```

### Client window

Start the client in the fourth window by going to the build directory and running the script below.

```bash
$ cd buildBasic
$ ./runClient.sh
```
There should be results in the client window similar to the below:

```bash
-------------- Add IPv4 Session --------------
Adding Session
1001
-------------- Add IPv6 Session --------------
Adding Session
1001
-------------- Get Session --------------
Getting Session
SessionId: 1001
Session State: _ESTABLISHED
Session RequestStatus: 0
Session SessionCloseCode: 0
Session InPackets 1000
Session OutPackets 200000
Session startTime seconds: 1593188883
nanos: 978987000

Session endTime
-------------- Delete Session --------------
Getting Session
SessionId: 1001
Session RequestStatus: 0
Session State: _CLOSING_1
Session RequestStatus: 0
Session SessionCloseCode: 0
Session InPackets 2000
Session OutPackets 400000
Session startTime seconds: 1593188883
nanos: 979860000

Session endTime seconds: 1593188883
nanos: 979860000

-------------- Add Mirror and Forward Session --------------
Adding Session
1001
-------------- Check for Closed Sessions --------------
### RECEIVED SESSION #####
SessionId: 1001
Session State: _CLOSED
Session RequestStatus: 0
Session SessionCloseCode: 0
Session InPackets 1000
Session OutPackets 200000
Session startTime seconds: 1593188883
nanos: 989916000

Session endTime
##########################
### RECEIVED SESSION #####
SessionId: 1002
Session State: _CLOSED
Session RequestStatus: 0
Session SessionCloseCode: 0
Session InPackets 2000
Session OutPackets 400000
Session startTime seconds: 1593188883
nanos: 989916000

Session endTime
##########################
### RECEIVED SESSION #####
SessionId: 1001
Session State: _CLOSED
Session RequestStatus: 0
Session SessionCloseCode: 0
Session InPackets 1000
Session OutPackets 200000
Session startTime seconds: 1593188883
nanos: 992831000

Session endTime
##########################
### RECEIVED SESSION #####
SessionId: 1002
Session State: _CLOSED
Session RequestStatus: 0
Session SessionCloseCode: 0
Session InPackets 2000
Session OutPackets 400000
Session startTime seconds: 1593188883
nanos: 992831000

Session endTime
##########################


------------Creating new devices---------------------

Adding Device Description:  Bluefield-1
Status:  _DEVICE_REGISTERED
Adding Device Description:  XDP
Status:  _DEVICE_REGISTERED

------------- Listing available Devices --------------------

### Registered Device ####
Name:  Bluefield-1
Description:  Acme SmartNIC
Type:  _SMARTNIC
Session Capacity:  2000000
Session Rate:  100000
### Registered Device ####
Name:  XDP
Description:  Software Implementation
Type:  _SOFTWARE
Session Capacity:  200000
Session Rate:  10000

------------- Activating Device --------------------

### Activating Device ####
Activated Device:  Bluefield-1
Status:  _DEVICE_ACTIVATED

------------- Activation Tests Complete --------------------

```

MAINTAINERS
-----------

Developed for AT&T by Brian Freeman and Richard Bowman, June 2020

Current maintainers:
 * Brian Freeman (at&t)
 * Richard Bowman (at&t)

# References

1. [gRPC Python Quick Start Guide](https://grpc.io/docs/quickstart/python/)



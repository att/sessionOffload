
# OpenOffload

gRPC API to offload TCP and UDP packet processing from an application to a hardware switch

# Creating Language Bindings

There are two language bindings implemented in the language Makefile.

- Python
- C++

To generate the language bindings for a specific language the appropriate language and related gRPC tools need to be installed. It is the responsibility of individual implementer to create there client and server code. 

## Generating code

### Python

```bash
$ cd openoffload
$ make all-py
```

### C++ recommended method using docker container for dependencies

#### Step 1

Create the basic build image with all the required libraries

```bash
$ cd openoffload/cpp/framework/build
$ docker build -t grpcbuild:v1 .
```

#### Step 2

Create the build container from the base image
There is a little hack necessary to get the proto file in the right place for docker

```bash
$ cd ..
$ cp ../../../protos/openoffload.proto .
```
```bash
$ docker build -t opofbld:v1 .
$ docker image ls
```

#### Step 3
Access the container to get the files

```bash
$ mkdir results
$ docker create -it --name results opofbld:v1 /bin/bash
$ docker cp results:/home/grpc/local/tests/bin/ results/
$ cd results
$ mkdir log
```
Run the binaries in separate windows :
```bash
./bin/opof_server_test 
and 
./bin/opof_client_test -f -v
```


### C++ alternate method assumes all dependencies are installed on the build path

```bash
$ cd openoffload
$ make all-cpp
```

# Installation and build for tests

Depending on which language binding used different development tools need to be installed. To just run the tests only Python is required.

## Requisites

* Install Python
* Install gRPC for Python (see References)
* Install python pudb (debugger)

See the github workflow main.yaml for details on the require library installations (.github/workflows/main.yaml)

## Build

The following command builds the Basic Python code
Note: The PythonSimulator has more functional testing of error conditions and follows a similar build/test pattern.

```bash
$ ./builditBasic.sh
```

## Testing the sample code with robot

A set of Robotframework tests are provided to quickly run the client and server tests.


```bash
$ cd robot
$ mvn clean install

==============================================================================
Acceptance                                                                    
==============================================================================
Acceptance.pythonBasicTests :: Executes the python basic gRPC Tests           
==============================================================================
Setup gRPC Tests                                                      | PASS |
------------------------------------------------------------------------------
Full Client Tests                                                     | PASS |
------------------------------------------------------------------------------
Add IPv4 Session Test                                                 | PASS |
------------------------------------------------------------------------------
Add IPv4 Session Error Test                                           | PASS |
------------------------------------------------------------------------------
Add IPV6 Session Test                                                 | PASS |
------------------------------------------------------------------------------
Get Session Test                                                      | PASS |
------------------------------------------------------------------------------
Delete Session Test                                                   | PASS |
------------------------------------------------------------------------------
Add Mirror Session Test                                               | PASS |
------------------------------------------------------------------------------
Get Closed Sessions Test                                              | PASS |
------------------------------------------------------------------------------
Get All Closed Sessions Test                                          | PASS |
------------------------------------------------------------------------------
Suite Teardown                                                        | PASS |
------------------------------------------------------------------------------
Acceptance.pythonBasicTests :: Executes the python basic gRPC Tests   | PASS |
11 critical tests, 11 passed, 0 failed
11 tests total, 11 passed, 0 failed
==============================================================================
Acceptance.pythonSimulatorTests :: Executes the python basic gRPC Tests       
==============================================================================
Setup gRPC Tests                                                      | PASS |
------------------------------------------------------------------------------
Run Client Tests                                                      | PASS |
------------------------------------------------------------------------------
Suite Teardown                                                        | PASS |
------------------------------------------------------------------------------
Acceptance.pythonSimulatorTests :: Executes the python basic gRPC ... | PASS |
3 critical tests, 3 passed, 0 failed
3 tests total, 3 passed, 0 failed
==============================================================================
Acceptance                                                            | PASS |
14 critical tests, 14 passed, 0 failed
14 tests total, 14 passed, 0 failed
==============================================================================
[INFO] ------------------------------------------------------------------------
[INFO] BUILD SUCCESS
[INFO] ------------------------------------------------------------------------
[INFO] Total time:  36.208 s
[INFO] Finished at: 2021-05-11T12:36:34Z
[INFO] ------------------------------------------------------------------------

```


## Testing the sample code with python directly

Create four separate terminal windows a client and three server windows

### Server Window (Offload sessions)

Start the offload server in one window by going to the build directory and running the script below.

```bash
$ cd buildBasic
$ ./runOffloadServer.sh
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
 * John McDowall (palo alto networks)

# References

1. [gRPC Python Quick Start Guide](https://grpc.io/docs/quickstart/python/)



# Overview
The framework directory provides the C Language interface into the C++ gRPC OpneOffload Client and Server. Users of the OpenOffload client and server do not need to write any C++ code the interfaces are abstract through a C API for both the client and server.

There are simple test programs for the client and server. The server code includes a simpel in-memory hashtable that allows the construction of more complex test cases.

# System Requirements
The current version of the programs are built for Centos 8. gRPC C++ must be installed and developer tools to build the system. A Vagrantfile is included in the distribution as an example of how to create a development environment. In addition a Dockerfile is included to build from a container.

## Install Development tools

```bash
$ yum -y install wget
$ yum -y group install "Development Tools"
```

## Install [libconfig](http://hyperrealm.github.io/libconfig/)
Libconfig is used to configure test files the the test client only.
[libconfig install instructions](https://github.com/hyperrealm/libconfig/blob/master/INSTALL)

```bash
$ wget http://hyperrealm.github.io/libconfig/dist/libconfig-1.7.2.tar.gz
$ tar xvzf libconfig-1.7.2
$ cd libconfig-1.7.2
$ ./configure
$ make 
$ make install
```
To configure you system to find the libconfig.so file
```bash
$ vi /etc/ld.so.conf
```

and add the path to your install directory of libconfig. The default is /usr/local/lib so
the file would look like this:

```bash
include ld.so.conf.d/*.conf
/usr/local/lib
```

Close the file and update the shared library config

```bash
$ ldconfig 
```

Check that the libconfig.so file is in config
```bash
sudo ldconfig -p | grep libconfig
```

This will print out links to libconfig.so

## Install gRPC C++

```bash
$ mkdir -p $HOME/local
$ export GRPC_INSTALL=$HOME/local
$ wget --no-check-certificate -q -O cmake-linux.sh https://github.com/Kitware/CMake/releases/download/v3.17.0/cmake-3.17.0-Linux-x86_64.sh
$ ./cmake-linux.sh  -- --skip-license --prefix=$HOME/local
$ rm cmake-linux.sh
$ export PATH=$GRPC_INSTALL/bin:$PATH
$ git config --global  http.sslVerify false
$ git clone --recurse-submodules -b v1.30.0 https://github.com/grpc/grpc
$ cd grpc 
$ mkdir -p $GRPC_INSTALL/grpc/cmake/build
$ pushd cmake/build
$ cmake -DgRPC_INSTALL=ON \
      -DgRPC_BUILD_TESTS=OFF \
      -DCMAKE_INSTALL_PREFIX=$GRPC_INSTALL \
      ../..
$ make -j 4
$ make install
$ popd


````

# Test Programs


## Build
```bash
$ cd "Install Directory"/sessionoffload/openoffload/cpp/framework
$ make all
```

## Run

Checkout the test program capabilities

```bash
$ bin/opof_test -h 
```

Run the server

```bash
$ bin/opof__server_test
```

Run the client

'n' in the number of sessions and 'b' is the buffer size to stream

```bash
$ bin/opof__client_test -n 128 - b 64
```
## Testing 

To run specific tests users can create configuration files and run them as specific tests. The configuration files are in the "config" directory.

```bash
$ bin/opof_client_test -t config/test1.cfg
```

# Programming the C Interface

## Client Program

There is an example file "opof_client_test.c" that shows how to use the C client interface. All the C++ enums and structures have been converted to C and are included from the file "opof.h". This is the major work for any changes to the openoffload.proto file. There may be a way to automate this but currently it is a manual process.

```C
/*
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef OPOF_H
#define OPOF_H

#define SUCCESS 0
#define FAILURE -1


typedef enum  { 
  _HOPOPT = 0,
  _TCP = 6,
  _UDP = 17,
} PROTOCOL_ID_T;

typedef enum  {
   _IPV4 = 0,
   _IPV6 = 1,
} IP_VERSION_T;

typedef enum {
   _DROP = 0,
   _FORWARD = 1,
   _MIRROR = 2,
   _SNOOP = 3,
 } ACTION_VALUE_T;

typedef enum {
  _ESTABLISHED = 0,
  _CLOSING_1   = 1,
  _CLOSING_2   = 2,
  _CLOSED      = 3,
} SESSION_STATE_T;

typedef enum {
  _NOT_CLOSED = 0,
  _FINACK     = 1,
  _RST        = 2,
  _TIMEOUT    = 3,
} SESSION_CLOSE_T;

typedef enum {
   _ACCEPTED = 0,
   _REJECTED = 1,
   _REJECTED_SESSION_NONEXISTENT = 2,
   _REJECTED_SESSION_TABLE_FULL = 3,
   _REJECTED_SESSION_ALREADY_EXISTS = 4,
} REQUEST_STATUS_T;

//typedef struct sessionRequestTuple {
//  int64_t  sessionId;
//} sessionRequest_t;

typedef struct addSessionResponseTuple {
  unsigned long  sessionId;
  REQUEST_STATUS_T requestStatus;
  //google.protobuf.Timestamp startTime = 3;
} addSessionResponse_t;

typedef struct sessionResponseTuple {
    unsigned long sessionId;
    long  inPackets;
    long  outPackets;
    long  inBytes;
    long  outBytes;
    SESSION_STATE_T sessionState;
    SESSION_CLOSE_T sessionCloseCode;
    REQUEST_STATUS_T requestStatus;
  //google.protobuf.Timestamp startTime = 9;
  //google.protobuf.Timestamp endTime = 10;
} sessionResponse_t;


typedef struct sessionRequestTuple {
    unsigned long sessId;
    unsigned int inlif;
    unsigned int outlif;
    unsigned int srcIP;
    unsigned int dstIP;
    unsigned int srcPort;
    unsigned int dstPort;
    PROTOCOL_ID_T proto;
    IP_VERSION_T ipver;
    ACTION_VALUE_T actType;
    unsigned int nextHop;
} sessionRequest_t;


#endif  /* OPOF_H */
``` 

### Initializing the client

Initializing the client interface is as follows:

```C
sessionTable_t *handle;
handle = opof_create_sessionTable(address, port, cert);
```
The address is a string representing the address of the server and the port is an int giving the port the server is listening on.

### Adding a session

```C
 status = opof_add_session(handle, &parameters, &addResp);
```

### Getting a session

```C
 status = opof_get_session(handle, sessionId, &resp);
```

### Deleting a session

```C
 status = opof_del_session(handle, sessionId, &resp);
```
### Extending the server

The server is started by calling 

```C
void opof_server(const char* address, unsigned short port, const char* cert, const char* key)
```

To implement the interfaces to the server the following functions defined in "opof_serverlib.h' need to be implemented

```C
 int opof_add_session_server(sessionRequest_t *parameters, addSessionResponse_t *response);
 int opof_get_session_server(unsigned long sessionId, sessionResponse_t *response);
 int opof_del_session_server(unsigned long sessionId, sessionResponse_t *response);
 sessionResponse_t **opof_get_closed_sessions_server(statisticsRequestArgs_t *request, int *sessionCount);
```
The file "opof_server_test.c" is a sample implementation of each of the functions. The sample implementaton implements a simple in memory hashtable to enable more complex test scenarios to be implemented.

# Building from dockerfiles

## Step 1 
Create the basic build image with all the required libraries

```bash
$ cd openoffload/cpp/framework/build
$ docker build -t grpcbuild:0.3 .
```
## Step 2
Create the build container from the base image
There is a little hack necessary to get the proto file in the right place for docker

```bash
$ cd ..
$ cp ../../../protos/openoffload.proto .
```
```bash
$ docker build -t opofbld:0.3 .
```
## Step 3
Access the container to get the files

```bash
$ mkdir dockerresults
$ docker create -it --name results opofbld:0.3 /bin/bash
$ docker cp results:/home/grpc/local/tests/bin/ results/
```

# References

1. [gRPC Quick Start Install](https://grpc.io/docs/languages/cpp/quickstart/)
2. [uthash Documentation](https://troydhanson.github.io/uthash/userguide.html)
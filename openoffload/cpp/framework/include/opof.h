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


#include <sys/socket.h>
#include <netinet/in.h>

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
   _NO_CLOSED_SESSIONS = 5,
} REQUEST_STATUS_T;


typedef struct {
  void *handle;
  unsigned int pageSize;
} streamArgs_t;

//typedef struct sessionRequestTuple {
//  int64_t  sessionId;
//} sessionRequest_t;

typedef struct addSessionResponseTuple {
  REQUEST_STATUS_T requestStatus;
  unsigned long errorStatus;
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

typedef struct statisticsRequestArgsTuple{
  //  pageSize = 0 will turn off paging
  //  does paging make sense for a stream ?
  //  the client should read/process each event on the stream anyway.
  unsigned int pageSize;
  unsigned int page;
  //  what other arguments make sense for retrieving or filtering streams 
}statisticsRequestArgs_t;

typedef struct sessionRequestTuple {
    unsigned long sessId;
    unsigned int inlif;
    unsigned int outlif;
    struct in_addr srcIP;
    struct in6_addr srcIPV6;
    struct in_addr dstIP;
    struct in6_addr dstIPV6;
    unsigned int srcPort;
    unsigned int dstPort;
    PROTOCOL_ID_T proto;
    IP_VERSION_T ipver;
    ACTION_VALUE_T actType;
    unsigned int nextHop;
} sessionRequest_t;


#endif  /* OPOF_H */
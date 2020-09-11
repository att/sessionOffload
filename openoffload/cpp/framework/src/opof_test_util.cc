// Copyright (C) 2020 Palo Alto Networks Intellectual Property. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
#include <arpa/inet.h>
#include <sys/types.h>          
#include <sys/socket.h>
#include <netinet/in.h>


#include "opof_test_util.h"


int range(int lower, int upper){
  return ((rand() %  (upper - lower + 1)) + lower); 
}

sessionResponse_t **createSessionResponse(int size, int *sessionCount){
  record_t *r, *tmp;
  int i=0;

  int count = 0;
  sessionResponse_t **responses = NULL;
  sessionResponse_t *response;
  responses = (sessionResponse_t **)malloc(size * (sizeof(sessionResponse_t *)));
  *sessionCount = 0;

  HASH_ITER(hh, sessions, r, tmp) {
     if (r->sessionState == _CLOSED){
        count++;
        response = (sessionResponse_t *)malloc(sizeof(sessionResponse_t));
        response->sessionId = r->key.sessionId;
        response->inPackets = range(100,1000);
        response->outPackets = range(110,1500);
        response->inBytes = range(1000,5000);
        response->outBytes = range(1000,5000);
        response->sessionState = r->sessionState;
        response->sessionCloseCode = _TIMEOUT;
        response->requestStatus = _ACCEPTED;
        HASH_DEL(sessions, r);  /* delete it (users advances to next) */
        free(r);             /* free it */
        responses[i] = response;
        i++;
        if (i == size){
          *sessionCount = count;
          return responses;
        }
      }
   }
  if (i == 0) {
    return NULL;
  }
  return NULL;
}


sessionRequest_t **createSessionRequest(int size, unsigned long start_sessionId){
  int status;
  sessionRequest_t *request;
  sessionRequest_t **requests = NULL;
  PROTOCOL_ID_T proto;
  IP_VERSION_T ipver;
  ACTION_VALUE_T action;
  proto = _TCP;
  ipver = _IPV4;
  action = _FORWARD;
  struct in_addr srcip;
  struct in_addr dstip;
  struct in_addr nexthopip;
  const char *src_addr = "10.0.1.1";
  const char *dst_addr= "10.1.0.1";
  const char *nextHop = "192.168.0.1";
  char address[INET_ADDRSTRLEN];
  status = inet_pton(AF_INET,src_addr, &srcip); 
  status = inet_pton(AF_INET,dst_addr, &dstip); 
  status = inet_pton(AF_INET,nextHop, &nexthopip); 
//#ifdef DEBUG

printf("DEBUG: Source IP: %s\n", inet_ntop(AF_INET,(void *)&srcip,address,INET_ADDRSTRLEN));
printf("DEBUG: Source IP as int: %d\n",srcip.s_addr);
printf("DEBUG: Dest IP: %s\n", inet_ntop(AF_INET,(void *)&dstip,address,INET_ADDRSTRLEN));
printf("DEBUG: NextHop IP: %s\n", inet_ntop(AF_INET,(void *)&nexthopip,address,INET_ADDRSTRLEN));

//#endif

  requests = (sessionRequest_t **)malloc(size * (sizeof(requests)));

  
  for (unsigned long i = 0; i < size; i++){
    request = (sessionRequest_t *)malloc(sizeof(*request));
    request->sessId = (i+start_sessionId);
    request->inlif = 1;
    request->outlif = 2;
    request->srcIP = (unsigned int)srcip.s_addr;
    request->dstIP = (unsigned int)dstip.s_addr;
    request->srcPort = 80;
    request->dstPort = 45678;
    request->proto = proto;
    request->ipver = ipver;
    request->nextHop = (unsigned int)nexthopip.s_addr;
    request->actType = action;
    requests[i] = request;
  }
return requests;
}



sessionResponse_t *getClosedSessionsFromHash(){

/*
* Iterate over sessions and return CLOSED Sessions
* If no CLOSED Sessions are found return NULL
* This is slow and inefficient but is simple for testing
*/
record_t *r, *tmp;
sessionResponse_t *response = NULL;
HASH_ITER(hh, sessions, r, tmp) {
  if (r->sessionState == _CLOSED){
    response = (sessionResponse_t *)malloc(sizeof(sessionResponse_t));
    response->sessionId = r->key.sessionId;
    response->inPackets = range(100,1000);
    response->outPackets = range(110,1500);
    response->inBytes = range(1000,5000);
    response->outBytes = range(1000,5000);
    response->sessionState = r->sessionState;
    response->sessionCloseCode = _TIMEOUT;
    response->requestStatus = _ACCEPTED;
    HASH_DEL(sessions, r);  /* delete it (users advances to next) */
    free(r);             /* free it */
  }
}
return response;

}
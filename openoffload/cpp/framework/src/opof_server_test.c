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


/**
* \ingroup testlibrary
*
* \brief gRPC Test Library for C/C++
*
*/

#include <stdlib.h>

#include "opof.h"
#include "opof_error.h"
#include "opof_serverlib.h"
#include "opof_hash.h"
#include "opof_test_util.h"



sessionResponse_t **getClosedSessions(int size, int *sessionCount){
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

sessionResponse_t **getAllSessions(int size, int *sessionCount){
  record_t *r, *tmp;
  int i=0;

  int count = 0;
  sessionResponse_t **responses = NULL;
  sessionResponse_t *response;
  size = HASH_COUNT(sessions);
  printf("DEBUG: getAllSessions number of sessions: %d\n", size);
  responses = (sessionResponse_t **)malloc(size * (sizeof(sessionResponse_t *)));
  *sessionCount = 0;

  HASH_ITER(hh, sessions, r, tmp) {
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
      responses[i] = response;
      i++;
      *sessionCount = count;
   }
   return responses;
}

int opof_add_session_server(sessionRequest_t *parameters, addSessionResponse_t *response ){
 
#ifdef DEBUG
  display_session_request(parameters, "Server addSession");
#endif
    record_t *r;
    //record_t *records = NULL;
    r =  (record_t *)malloc(sizeof (record_t));
    memset(r,0,sizeof(*r));
    //parameters->sessId = 1234;
    r->key.sessionId = parameters->sessId;
    HASH_ADD(hh, sessions, key, sizeof(record_key_t),r);
    r->inLif = parameters->inlif;
    r->outLif = parameters->outlif;
    r->ipVersion  = parameters->ipver;
    r->sourceIp = parameters->srcIP.s_addr;
    r->sourcePort = parameters->srcPort;
    r->dstIp = parameters->dstIP.s_addr;
    r->dstPort = parameters->dstPort;
    r->inPackets = range(100,1000);
    r->outPackets = range(110,1500);
    r->inBytes = range(1000,5000);
    r->outBytes = range(1000,5000);
    r->protocolId = parameters->proto;
    r->sessionState = _CLOSED;
    r->sessionClose = _NOT_CLOSED;
    r->actionValue = _FORWARD;
    //
    response->requestStatus = _ACCEPTED;
    //
  return SUCCESS;
}
int opof_get_session_server(unsigned long sessionId, sessionResponse_t *response){
  record_t *r,l;
  l.key.sessionId = sessionId;
  HASH_FIND(hh,sessions, &l.key, sizeof(record_key_t),r);
  if (r != NULL) {
    response->sessionId = sessionId;
    response->inPackets = r->inPackets;
    response->outPackets = r->outPackets;
    response->inBytes = r->inBytes;
    response->outBytes = r->outBytes;
    response->sessionState = r->sessionState;
    response->sessionCloseCode = r->sessionClose;
    response->requestStatus = _ACCEPTED;
    return SUCCESS;
    }
    response = NULL;
    return FAILURE;  
}

int opof_del_session_server(unsigned long sessionId, sessionResponse_t *response){
  record_t *r,l;
 
  l.key.sessionId = sessionId;
  HASH_FIND(hh,sessions, &l.key, sizeof(record_key_t),r);
  if (r !=NULL){
    HASH_DEL(sessions,r);
    r->sessionState = _CLOSED;
    HASH_ADD(hh, sessions, key, sizeof(record_key_t),r);
    response->sessionId = sessionId;
    response->inPackets = r->inPackets;
    response->outPackets = r->outPackets;
    response->inBytes = r->inBytes;
    response->outBytes = r->outBytes;
    response->sessionState = r->sessionState;
    response->sessionCloseCode = r->sessionClose;
    response->requestStatus = _ACCEPTED;
    return SUCCESS;
  }
  response = NULL;
  return FAILURE;
}


sessionResponse_t **opof_get_closed_sessions_server(statisticsRequestArgs_t *request, int *sessionCount){
  
  int count = 0;
  int nresponses = request->pageSize;
  sessionResponse_t **responses;
  *sessionCount = 0;

  responses = getClosedSessions(nresponses, &count);
  *sessionCount = count;
 
  return responses;
}

sessionResponse_t **opof_get_all_sessions_server(statisticsRequestArgs_t *request, int *sessionCount){
  
  int count = 0;
  int nresponses = request->pageSize;
  sessionResponse_t **responses;
  *sessionCount = 0;

  responses = getAllSessions(nresponses, &count);
  *sessionCount = count;
 
  return responses;
}



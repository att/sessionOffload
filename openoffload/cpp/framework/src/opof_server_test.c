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
#include <stdlib.h>

#include "opof.h"
#include "opof_error.h"
#include "opof_serverlib.h"
#include "opof_hash.h"
#include "opof_test_util.h"

int opof_add_session_server(sessionRequest_t *parameters, addSessionResponse_t *response ){
 
#ifdef DEBUG
  display_session_request(parameters);
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
    r->sourceIp = parameters->srcIP;
    r->sourcePort = parameters->srcPort;
    r->dstIp = parameters->dstIP;
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

int opof_del_session_server(unsigned long sessionId, sessionResponse_t *response){
  record_t *r,l;
 
  l.key.sessionId = sessionId;
  HASH_FIND(hh,sessions, &l.key, sizeof(record_key_t),r);
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


sessionResponse_t **opof_get_closed_sessions_server(statisticsRequestArgs_t *request, int *sessionCount){
  
  int count = 0;
  int nresponses = request->pageSize;
  sessionResponse_t **responses;
  *sessionCount = 0;

  responses = createSessionResponse(nresponses, &count);
  *sessionCount = count;
 
  return responses;
}




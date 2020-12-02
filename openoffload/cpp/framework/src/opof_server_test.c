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
#include <limits.h>

#include "opof.h"
#include "opof_error.h"
#include "opof_serverlib.h"
#include "opof_hash.h"
#include "opof_test_util.h"

/** 
* \ingroup servercinterface
* \brief Utility fumction to get all sessions from test hashtable
*
* This method hides the C++ constructor and returns the C__ instance as a void pointer in the 
* sessionTable_t typedef struct. All methods of the sessionTable client class are passed this handle.
*
* \param address The address the server is listening on either the IP address or "localhost"
* \param port    The port the port the server is listening on
* \param cert    The pulbic key fo the TLS connection
* \return        sessionTable_t, returns a handle to the C++ instance as a void handle.
*
*/
static record_t *start;
int getAllSessionsPage(int size, uint64_t *sessionStart, sessionResponse_t **responses){
  int i=0;

  record_t *r;
  if (*sessionStart == UINT_MAX){
    start = sessions;
  }
#ifdef DEBUG
  int max_size;
  max_size = HASH_COUNT(sessions);
  printf("DEBUG: getAllSessions number of sessions: %d\n", max_size);
#endif
  if (!start)
  {
    return i;
  }
  //HASH_ITER(hh, sessions, r, tmp) {
  for (r=start; r != NULL; r=r->hh.next){
    responses[i]->sessionId = r->key.sessionId;
    responses[i]->inPackets = range(100,1000);
    responses[i]->outPackets = range(110,1500);
    responses[i]->inBytes = range(1000,5000);
    responses[i]->outBytes = range(1000,5000);
    responses[i]->sessionState = r->sessionState;
    responses[i]->sessionCloseCode = _TIMEOUT;
    responses[i]->requestStatus = _ACCEPTED;
    i++;
    if (i == size){
      start = r->hh.next;
      *sessionStart = r->key.sessionId;
      return i;
    }
  }
  start = NULL;
  //*sessionStart = ;
  return i;
}
#if 0
/** 
* \ingroup servercinterface
* \brief Utility fumction to get all sessions from test hashtable
*
* This method hides the C++ constructor and returns the C__ instance as a void pointer in the 
* sessionTable_t typedef struct. All methods of the sessionTable client class are passed this handle.
*
* \param address The address the server is listening on either the IP address or "localhost"
* \param port    The port the port the server is listening on
* \param cert    The pulbic key fo the TLS connection
* \return        sessionTable_t, returns a handle to the C++ instance as a void handle.
*
*/
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
#endif
/** 
* \ingroup servercinterface
* \brief Utility fumction to add sessions to the test hashtable
*
* The function is called from within the C++ server method. It is the responsiblity of the server developer
* to implement this function. The code here is just an example.
*
* \param *parameters  A sessionRequest_t structure with the sesson information to add to the hardware device.
* \param *response    The response from the server as a addSessionResponse_t
* \return  SUCCESS or FAILURE
*
*/
int opof_add_session_server(sessionRequest_t *parameters, addSessionResponse_t *response ){
 
  int num_sessions;
#ifdef DEBUG
  display_session_request(parameters, "Server addSession");
#endif
    record_t *r, *f,l;
    //record_t *records = NULL;
    r =  (record_t *)malloc(sizeof (record_t));
    memset(r,0,sizeof(*r));
    //parameters->sessId = 1234;
    num_sessions = HASH_COUNT(sessions);
#ifdef DEBUG
    printf("MAX Sessions: %d\n", num_sessions);
#endif
    /*
    * Check capacity of session table
    */
    if (num_sessions > HASHTABLE_SIZE){
      //response->requestStatus = _REJECTED_SESSION_TABLE_FULL;
      return _RESOURCE_EXHAUSTED;
    }
    /*
    * Check if sessionId already exists
    */
    l.key.sessionId = parameters->sessId;
    HASH_FIND(hh,sessions, &l.key, sizeof(record_key_t),f);
    if (f != NULL){
      return _ALREADY_EXISTS;
    }

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
    //response->requestStatus = _ACCEPTED;
    //
  return _OK;
}

/** 
* \ingroup servercinterface
* \brief Utility fumction to get a session from test hashtable
*
* The function is called from within the C++ server method. It is the responsiblity of the server developer
* to implement this function. The code here is just an example.
*
* \param  session     The session to get.
* \param *response    The response from the server as a addSessionResponse_t
* \return  SUCCESS or FAILURE
*
*/
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
  } else {
    response->sessionId = sessionId;
    response->inPackets = 0;
    response->outPackets = 0;
    response->inBytes = 0;
    response->outBytes = 0;;
    response->sessionState = _UNKNOWN_STATE;
    response->sessionCloseCode = _UNKNOWN_CLOSE_CODE;
    response->requestStatus = _REJECTED_SESSION_NONEXISTENT;
  }
  return SUCCESS;  
}

/** 
* \ingroup servercinterface
* \brief Utility fumction to delete a session from the test hashtable
*
* The function is called from within the C++ server method. It is the responsiblity of the server developer
* to implement this function. The code here is just an example.
*
* \param  session     The session to delete.
* \param *response    The response from the server as a addSessionResponse_t
* \return  SUCCESS or FAILURE
*
*/
int opof_del_session_server(unsigned long sessionId, sessionResponse_t *response){
  record_t *r,l;
 
  l.key.sessionId = sessionId;
  HASH_FIND(hh,sessions, &l.key, sizeof(record_key_t),r);
  if (r !=NULL){
    response->sessionId = sessionId;
    response->inPackets = r->inPackets;
    response->outPackets = r->outPackets;
    response->inBytes = r->inBytes;
    response->outBytes = r->outBytes;
    response->sessionState = _CLOSED;
    response->sessionCloseCode = r->sessionClose;
    response->requestStatus = _ACCEPTED;
    HASH_DEL(sessions,r);
    free(r);
  }else {
    response->sessionId = sessionId;
    response->inPackets = 0;
    response->outPackets = 0;
    response->inBytes = 0;
    response->outBytes = 0;;
    response->sessionState = _UNKNOWN_STATE;
    response->sessionCloseCode = _UNKNOWN_CLOSE_CODE;
    response->requestStatus = _REJECTED_SESSION_NONEXISTENT;
  }
  return SUCCESS;
}

/** 
* \ingroup servercinterface
* \brief Utility fumction to get closed sessions from test hashtable
*
* The function is called from within the C++ server method. It is the responsiblity of the server developer
* to implement this function. The code here is just an example.
*
* \param  *request        The session request struct
* \param  *sesionCount    The number of sessions returned
* \return sessionResponse_t
*
*/
int opof_get_closed_sessions_server(statisticsRequestArgs_t *request, sessionResponse_t responses[]){
  
  record_t *r, *tmp;
  int i = 0;
  int size = request->pageSize;
 
  HASH_ITER(hh, sessions, r, tmp) {
     if (r->sessionState == _CLOSED){
        responses[i].sessionId = r->key.sessionId;
        responses[i].inPackets = range(100,1000);
        responses[i].outPackets = range(110,1500);
        responses[i].inBytes = range(1000,5000);
        responses[i].outBytes = range(1000,5000);
        responses[i].sessionState = r->sessionState;
        responses[i].sessionCloseCode = _TIMEOUT;
        responses[i].requestStatus = _ACCEPTED;
        HASH_DEL(sessions, r);  /* delete it (users advances to next) */
        free(r);             /* free it */
        i++;
        if (i == size){
          return i;
        }
      }
   }
  return i;
}

/** 
* \ingroup servercinterface
* \brief Utility fumction to get all sessions from test hashtable
*
* The function is called from within the C++ server method. It is the responsiblity of the server developer
* to implement this function. The code here is just an example.
*
* \param  *request        The session request struct
* \param  *sesionCount    The number of sessions returned
* \return sessionResponse_t
*
*/
int opof_get_all_sessions_server(int pageSize, uint64_t *sessionStart,int pageCount, sessionResponse_t **responses){
  
  int itemCount = 0;

  itemCount = getAllSessionsPage(pageSize, sessionStart, responses);
  //sessionCount = getAllSessionsPage(pageSize, pageCount,responses,r);
  return itemCount;
}



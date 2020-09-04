/*
 *  Copyright (C) 2020 Palo Alto Networks Intellectual Property. All rights reserved.
 *
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

#include <arpa/inet.h>
#include <sys/types.h>          
#include <sys/socket.h>
#include <time.h>
#include <pthread.h>

#include "opof.h"
#include "opof_test.h"
#include "opof_error.h"
#include "opof_clientlib.h"
#include "opof_util.h"



//int get_key(char *location, char *public_key);

void opof_client_test(const char *address, int max_sessions, unsigned int pageSize,unsigned short port, const char *cert){

  int status;
  pthread_t tid;
  streamArgs_t args;
  sessionTable_t *handle;
  char *src_addr = "10.0.1.1";
  char *dst_addr= "10.1.0.1";
  unsigned int srcip;
  unsigned int dstip;
  unsigned int nexthopip;
  status = inet_pton(AF_INET, src_addr, &srcip); 
  status = inet_pton(AF_INET, dst_addr, &dstip); 
  char *nextHop = "192.168.0.1";
  status = inet_pton(AF_INET, nextHop, &nexthopip); 
  PROTOCOL_ID_T proto;
  IP_VERSION_T ipver;
  ACTION_VALUE_T action;
  proto = _TCP;
  ipver = _IPV4;
  action = _FORWARD;
  
  sessionRequest_t **request;
  addSessionResponse_t addResp;
  sessionResponse_t resp;
  //statisticsRequestArgs_t statsArgs;
  args.pageSize = pageSize;

  //parameters.sessId = 1234;
  //parameters.inlif = 1;
  //parameters.outlif = 2;
  //parameters.srcIP = srcip;
  //parameters.dstIP = dstip;
  //parameters.srcPort = 80;
 // parameters.dstPort = 45678;
 // parameters.proto = proto;
  //parameters.ipver = ipver;
  //parameters.nextHop = nexthopip;
  //parameters.actType = action;
  handle = opof_create_sessionTable(address, port, cert);
  args.handle = handle;
  //printf("Handle created\n");
  //printf("\n\nAdding session\n");
  printf("\nNumber of Sessions: %d page size: %d\n",max_sessions, pageSize);
  clock_t begin = clock();
  //
  for (unsigned long  i=0; i < max_sessions; i+=pageSize){
    
    request = createSessionRequest(pageSize, i);
    status = opof_add_session(pageSize,handle, request, &addResp);
    if (status != FAILURE){
      //printf("Added session: %ld\n",addResp.sessionId);
      //printf("Success on add session test\n");
    }
  }
#ifdef FAST
  for (int i=0; i < max_sessions; i++){
    //printf("\n\nGetting session\n");
    status = opof_get_session(handle, i, &resp);
    if (status != FAILURE){
     // printf("Getting session: %ld\n",resp.sessionId);
      //printf("Success on get session test\n");
      //display_session_response(&resp);

    }

    status = opof_del_session(handle, i, &resp);
    if (status != FAILURE){
       //display_session_response(&resp);
     // printf("Getting session: %ld\n",resp.sessionId);
      //printf("Success on get session test\n");
    }
  }
#endif
  //pthread_create(&tid, NULL, opof_get_closed_sessions, &args);
 
  opof_get_closed_sessions(&args);
  //opof_get_closed_sessions(handle,pageSize);
  //while(1);
  clock_t end = clock();
  double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("\n\nSessions per second: %lf\n\n", (2.0 * (double)max_sessions)/(time_spent));
}
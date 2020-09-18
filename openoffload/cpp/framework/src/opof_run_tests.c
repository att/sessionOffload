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
#include "opof_test_util.h"


//int get_key(char *location, char *public_key);
sessionRequest_t **read_config(char *filename, int *nsessions);

void opof_run_tests(const char *address, int max_sessions, unsigned int pageSize,unsigned short port, const char *cert, char *test_config){

  int status;
  streamArgs_t args;
  sessionTable_t *handle;

  sessionRequest_t **request;
  addSessionResponse_t addResp;
#ifdef FAST
  sessionResponse_t resp;
#endif
  args.pageSize = pageSize;

 
  handle = opof_create_sessionTable(address, port, cert);
  args.handle = handle;
 
  
  clock_t begin = clock();
  //
  
    
    
    request = read_config(test_config, &max_sessions);
    printf("\nNumber of Sessions: %d page size: %d\n",max_sessions, max_sessions);
    //TODO handle more than 64 tests
    if (max_sessions > 64){
      fprintf(stderr,"ERROR: Truncated numberof session tests to 64, from %d\n", max_sessions);
      max_sessions = 64;
    }
    status = opof_add_session(max_sessions,handle, request, &addResp);
    if (status == FAILURE){
      printf("ERROR: Adding sessions: \n");
      exit(-1);
      //printf("Success on add session test\n");
  }
#ifdef FAST
  for (int i=0; i < max_sessions; i++){
    //printf("\n\nGetting session\n");
    status = opof_get_session(handle, i, &resp);
    if (status == FAILURE){
      printf("ERROR: getting sessions: %ld\n",i);
      exit(-1);
    }

    status = opof_del_session(handle, i, &resp);
    if (status == FAILURE){
      printf("ERROR: deleting sessions: %ld\n",i);
      exit(-1);
    }
  }
#endif
  
  opof_get_closed_sessions(&args);
 
  clock_t end = clock();
  double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("\n\nSessions per second: %lf\n\n", (2.0 * (double)max_sessions)/(time_spent));
}
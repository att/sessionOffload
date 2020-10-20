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

  /**
  * \ingroup testlibrary
  *
  * \brief gRPC Test Library for C/C++
  *
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
  int opof_test1(const char *address, int max_sessions, unsigned int pageSize,unsigned short port, const char *cert, char *test_config);
  int opof_test2(const char *address, int max_sessions, unsigned int pageSize,unsigned short port, const char *cert, char *test_config);
  int opof_test3(const char *address, int max_sessions, unsigned int pageSize,unsigned short port, const char *cert, char *test_config);


  void opof_run_tests(const char *address, int max_sessions, unsigned int pageSize,unsigned short port, const char *cert, char *test_config, int testid){

    int status;

    switch (testid){

    case 1:
      status = opof_test1(address,  max_sessions, pageSize,port, cert,test_config);
      break;
    case 2:
      status = opof_test2(address,  max_sessions, pageSize,port, cert,test_config);
      break;
    case 3:
      status = opof_test3(address,  max_sessions, pageSize,port, cert,test_config);
      break;
    default:
      printf("ERROR: Unkonwn Test ID: %d\n", testid);
      status = FAILURE;
      return;
    }
      if (status == SUCCESS){
        printf("INFO: Test: %d ran successfully\n", testid);
      } else {
        printf("ERROR: Test: %d Failed\n", testid);
      }
  }

  int opof_test1(const char *address, int max_sessions, unsigned int pageSize,unsigned short port, const char *cert, char *test_config){
  
    int status;
    streamArgs_t args;
    sessionTable_t *handle;
    unsigned long closed_sessions=1;
    sessionRequest_t **request;
    sessionResponse_t response;
    addSessionResponse_t addResp;
    args.pageSize = pageSize;

    printf("\n\n Test 1: Simple add sessions from config and return all as closed sessions\n\n");
   
    handle = opof_create_sessionTable(address, port, cert);
    args.handle = handle;
   
    clock_t begin = clock();
      
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
    }
    while(closed_sessions > 0){
      closed_sessions = opof_get_closed_sessions(&args,&response);
      if (closed_sessions > 0){
#ifdef DEBUG
      display_session_response(&response, "Test 1");
#endif
      printf("INFO: Closed Sessions: %lu\n",closed_sessions);
    }
   }
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("\n\nSessions per second: %lf\n\n", (2.0 * (double)max_sessions)/(time_spent));
    return SUCCESS;
  }

 int opof_test2(const char *address, int max_sessions, unsigned int pageSize,unsigned short port, const char *cert, char *test_config){

    int status;
    sessionTable_t *handle;

    sessionRequest_t **request,*temp;
    addSessionResponse_t addResp;
    sessionResponse_t resp;
    int sessionId;

    printf("\n\n Test 2: Simple add sessions from config and then get sessions and then delete them\n\n");
   
    handle = opof_create_sessionTable(address, port, cert);
   
    clock_t begin = clock();
      
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
    for (int i=0; i < max_sessions; i++){
      temp = request[i];
      sessionId = temp->sessId;
      status = opof_get_session(handle, sessionId, &resp);
      if (status == FAILURE){
        printf("ERROR: getting sessions: %d\n",i);
        exit(-1);
      }

      status = opof_del_session(handle, sessionId, &resp);
      if (status == FAILURE){
        printf("ERROR: deleting sessions: %d\n",i);
        exit(-1);
      }
    }
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("\n\nSessions per second: %lf\n\n", (2.0 * (double)max_sessions)/(time_spent));
    return SUCCESS;
 }

  int opof_test3(const char *address, int max_sessions, unsigned int pageSize,unsigned short port, const char *cert, char *test_config){

    int status;
    streamArgs_t args;
    sessionTable_t *handle;
    unsigned long closed_sessions=1;
    sessionRequest_t **request;
    sessionResponse_t response;
    addSessionResponse_t addResp;
    args.pageSize = pageSize;

    printf("\n\n Test 3: Simple add sessions from config and then get all  sessions and then get closed sessions\n\n");
   
    handle = opof_create_sessionTable(address, port, cert);
    args.handle = handle;
   
    clock_t begin = clock();
      
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
    opof_get_all_sessions(handle);
    while(closed_sessions > 0){
      closed_sessions = opof_get_closed_sessions(&args,&response);
      if (closed_sessions > 0){
#ifdef DEBUG
      display_session_response(&response, "Test 3");
#endif
      printf("INFO: Closed Sessions: %lu\n",closed_sessions);
    }
   }
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("\n\nSessions per second: %lf\n\n", (2.0 * (double)max_sessions)/(time_spent));
    return SUCCESS;
 }
  
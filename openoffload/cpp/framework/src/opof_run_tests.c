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
  #include <limits.h>
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
  int opof_test1(const char *address, int max_sessions, unsigned int pageSize,unsigned short port, const char *cert);
  //int opof_test1(const char *address, int max_sessions, unsigned int pageSize,unsigned short port, const char *cert, char *test_config);
  int opof_test2(const char *address, int max_sessions, unsigned int pageSize,unsigned short port, const char *cert);
  int opof_test3(const char *address, int max_sessions, unsigned int pageSize,unsigned short port, const char *cert);
  int opof_test4(const char *address, int max_sessions, unsigned int pageSize,unsigned short port, const char *cert);
  //int opof_test5(const char *address, int max_sessions, unsigned int pageSize,unsigned short port, const char *cert, char *test_config);
  //int opof_test6(const char *address, int max_sessions, unsigned int pageSize,unsigned short port, const char *cert, char *test_config);


  void opof_list_tests(){
    printf("\n\nAvaialble Tests\n");
    printf("\tTest 1: Use addSession to create a large number of sessions, then call getClosedSessions\n");
    printf("\tTest 2: Use addSession to create a large number of sessions, then call getAllSessions\n");
    printf("\tTest 3: Use addSession to create a session using config file and then get the sessions\n");
    printf("\tTest 4: Use addSession to create a session using config file and then delete the sessions\n");
    printf("\n");
  }

  int opof_run_tests(const char *address, int max_sessions, unsigned int pageSize,unsigned short port, const char *cert, char *test_config, int testid){

    int status;

    switch (testid){

    case 1:
      status = opof_test1(address,  max_sessions, pageSize,port, cert);
      break;
    case 2:
      status = opof_test2(address,  max_sessions, pageSize,port, cert);
      break;
    case 3:
      status = opof_test3(address,  max_sessions, pageSize,port, cert);
      break;
    case 4:
      status = opof_test4(address,  max_sessions, pageSize,port, cert);
      break;
#ifdef USED
    case 5:
      status = opof_test5(address,  max_sessions, pageSize,port, cert,test_config);
      break;
    case 6:
      status = opof_test6(address,  max_sessions, pageSize,port, cert,test_config);
      break;
#endif
    default:
      printf("ERROR: Unknown Test ID: %d\n", testid);
      status = FAILURE;
      return status;
    }
      if (status == SUCCESS){
        printf("\nINFO: Test: %d ran successfully\n\n", testid);
      } else {
        printf("ERROR: Test: %d Failed\n", testid);
      }
    return SUCCESS;
  }

  int opof_delete_all_sessions(void * handle, int pageSize){

    sessionResponse_t responses[BUFFER_MAX], resp;
    int nsessions = 1;
    int sessionId;
    int status = SUCCESS;
    uint64_t sessionStart = UINT_MAX;

  
    while (nsessions > 0 ){
      nsessions = opof_get_all_sessions(handle, &sessionStart, pageSize, responses);
      //printf("Number of sessions for get all: %d\n",nsessions);
      if (nsessions > 0){
        for (int i=0; i < nsessions; i++){
          sessionId = responses[i].sessionId;
          status = opof_del_session(handle, sessionId, &resp);
        }
      }
    }

    return status;
  }
int opof_test1(const char *address, int max_sessions, unsigned int pageSize,unsigned short port, const char *cert){

  int status;
  streamArgs_t args;
  sessionTable_t *handle;
  unsigned long closed_sessions=1;
  sessionRequest_t **request;
  addSessionResponse_t addResp;
 
  sessionResponse_t responses[BUFFER_MAX];
  int sessionCount =1;
  int bufferSize;
  int sessionId=0;

  handle = opof_create_sessionTable(address, port, cert);
  args.handle = handle;
  args.pageSize = pageSize;

  /*
  *  Clean up any exisitng data in cache
  */
  if (opof_delete_all_sessions(handle,pageSize) == FAILURE){
    return FAILURE;
  }
  printf("\n\nRunning Test 1: ");
  printf("\tNumber of Sessions: %d page size: %d\n",max_sessions, pageSize);
  //
  while(max_sessions > 0){

    sessionCount = max_sessions - pageSize;
    if (sessionCount < 0){
      bufferSize = max_sessions;
    } else {
      bufferSize = pageSize;
    }
    request = createSessionRequest(bufferSize, sessionId);
    status = opof_add_session(bufferSize,handle, request, &addResp);
    if (status == FAILURE){
      printf("ERROR: Adding sessions: \n");
      //for (i=0; i < pageSize; i++){
      //  if ((addResp.errorStatus & (error << i)) > 0) {
      //    printf("Session index: %u failed code: %lu", i, (addResp.errorStatus & (error << i)));
      //  }
      return FAILURE;
    }
     max_sessions = sessionCount;
     sessionId += bufferSize;
  }

  print_response_header();
  while(closed_sessions > 0){
    closed_sessions = opof_get_closed_sessions(&args,responses);
    printf("Closed sessions: %lu\n", closed_sessions);
    if (closed_sessions > 0){
      for (int i=0; i < closed_sessions; i++){
        print_response( &responses[i]);
      }
    }
  }
  return SUCCESS;
}

int opof_test2(const char *address, int max_sessions, unsigned int pageSize,unsigned short port, const char *cert){

  int nsessions = 1;
  int status;
 
  sessionTable_t *handle;
  
  sessionRequest_t **request;
  addSessionResponse_t addResp;
  
  sessionResponse_t responses[BUFFER_MAX];
  int page_number =1;
  int sessionCount =1;
  int bufferSize;
  handle = opof_create_sessionTable(address, port, cert);
  int sessionId=0;

  
  uint64_t sessionStart = UINT_MAX;
  /*
  *  Clean up any exisitng data in cache
  */
  if (opof_delete_all_sessions(handle,pageSize) == FAILURE){
    return FAILURE;
  }
  printf("\n\nRunning Test 2: ");
  printf("\tNumber of Sessions: %d page size: %d\n",max_sessions, pageSize);
  //
  while(max_sessions > 0){

    sessionCount = max_sessions - pageSize;
    if (sessionCount < 0){
      bufferSize = max_sessions;
    } else {
      bufferSize = pageSize;
    }
    request = createSessionRequest(bufferSize, sessionId);
    status = opof_add_session(bufferSize,handle, request, &addResp);
    if (status == FAILURE){
      printf("ERROR: Adding sessions: \n");
      //for (i=0; i < pageSize; i++){
      //  if ((addResp.errorStatus & (error << i)) > 0) {
      //    printf("Session index: %u failed code: %lu", i, (addResp.errorStatus & (error << i)));
      //  }
      return FAILURE;
    }
     max_sessions = sessionCount;
     sessionId += bufferSize;
  }

#ifdef DEBUG
  printf("\n\n Get all sessions test \n\n");
  printf("\nNumber of Sessions: %d page size: %d\n",max_sessions, pageSize);
  printf("\n\nDisplaying all Sessions\n");
#endif
  while (nsessions > 0 ){
    nsessions = opof_get_all_sessions(handle, &sessionStart, pageSize, responses);
    if (nsessions > 0){
      printf("\n\nDisplaying Page: %d\n",page_number);
      print_response_header();
      for (int i=0; i < nsessions; i++){
        print_response(&responses[i]);
        sessionStart = responses[i].sessionId;
      }
      //printf("sessionStart: %lu\n",sessionStart);
      page_number++;
      if (page_number >5){
        return SUCCESS;
      }
    }
  }

  return SUCCESS;
}

int opof_test3(const char *address, int max_sessions, unsigned int pageSize,unsigned short port, const char *cert){

  int status;
  
  sessionTable_t *handle;
  
  
  sessionRequest_t **request;
  sessionResponse_t resp;
  addSessionResponse_t addResp;
  
  int sessionCount =1;
  int bufferSize;
  handle = opof_create_sessionTable(address, port, cert);
  int sessionId=0;
  int total_sessions;
  

  /*
  *  Clean up any exisitng data in cache
  */
  if (opof_delete_all_sessions(handle,pageSize) == FAILURE){
    return FAILURE;
  }
  printf("\n\nRunning Test 3: ");
  printf("\tNumber of Sessions: %d page size: %d\n",max_sessions, pageSize);
 
  //
  total_sessions = max_sessions;
  while(max_sessions > 0){

    sessionCount = max_sessions - pageSize;
    if (sessionCount < 0){
      bufferSize = max_sessions;
    } else {
      bufferSize = pageSize;
    }
    request = createSessionRequest(bufferSize, sessionId);
    status = opof_add_session(bufferSize,handle, request, &addResp);
    if (status == FAILURE){
      printf("ERROR: Adding sessions: \n");
      //for (i=0; i < pageSize; i++){
      //  if ((addResp.errorStatus & (error << i)) > 0) {
      //    printf("Session index: %u failed code: %lu", i, (addResp.errorStatus & (error << i)));
      //  }
      return FAILURE;
    }
     max_sessions = sessionCount;
     sessionId += bufferSize;
  }

  print_response_header();
  for (int i = 0; i < total_sessions; i++){
    status = opof_get_session(handle, i, &resp);
    print_response(&resp);
    if (status == FAILURE){
      printf("ERROR: getting session: %d\n", sessionId);
      exit(-1);
    }
  }
  return SUCCESS;
}


int opof_test4(const char *address, int max_sessions, unsigned int pageSize,unsigned short port, const char *cert){

  int nsessions = 1;
  int status;
  
  sessionTable_t *handle;
 
  sessionRequest_t **request;
  sessionResponse_t resp;
  addSessionResponse_t addResp;
 
  sessionResponse_t responses[BUFFER_MAX];
  int page_number =0;
  int sessionCount =1;
  int bufferSize;
  handle = opof_create_sessionTable(address, port, cert);
  int sessionId=0;
  int total_sessions;
  
  uint64_t sessionStart = UINT_MAX;
  /*
  *  Clean up any exisitng data in cache
  */
  if (opof_delete_all_sessions(handle,pageSize) == FAILURE){
    return FAILURE;
  }
  printf("\n\nRunning Test 4: ");
  printf("\tNumber of Sessions: %d page size: %d\n",max_sessions, pageSize);
  
  //
  total_sessions = max_sessions;
  while(max_sessions > 0){

    sessionCount = max_sessions - pageSize;
    if (sessionCount < 0){
      bufferSize = max_sessions;
    } else {
      bufferSize = pageSize;
    }
    request = createSessionRequest(bufferSize, sessionId);
    status = opof_add_session(bufferSize,handle, request, &addResp);
    if (status == FAILURE){
      printf("ERROR: Adding sessions: \n");
      //for (i=0; i < pageSize; i++){
      //  if ((addResp.errorStatus & (error << i)) > 0) {
      //    printf("Session index: %u failed code: %lu", i, (addResp.errorStatus & (error << i)));
      //  }
      return FAILURE;
    }
     max_sessions = sessionCount;
     sessionId += bufferSize;
  }
  printf("\n\nDeleting following sessions from cache\n");
  print_response_header();
  for (int i = 0; i < total_sessions; i++){
    status = opof_del_session(handle, i, &resp);
    print_response(&resp);
    if (status == FAILURE){
      printf("ERROR: getting session: %d\n", sessionId);
      exit(-1);
    }
  }



  //printf("\n\n Get all sessions test \n\n");
  //printf("\nNumber of Sessions: %d page size: %d\n",max_sessions, pageSize);
  printf("\n\nDisplaying all Sessions\n");
  print_response_header();
  while (nsessions > 0 ){
    nsessions = opof_get_all_sessions(handle, &sessionStart, pageSize, responses);
    //printf("Number of sessions for get all: %d\n",nsessions);
    if (nsessions > 0){
      //printf("Page: %d\n",page_number);
      
      for (int i=0; i < nsessions; i++){
        print_response(&responses[i]);
      }
      //printf("sessionStart: %lu\n",sessionStart);
      page_number++;
      if (page_number >5){
        return SUCCESS;
      }
    }
  }
  
  return SUCCESS;
}

#ifdef USED
int opof_test0(const char *address, int max_sessions, unsigned int pageSize,unsigned short port, const char *cert, char *test_config){
  
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

 int opof_test21(const char *address, int max_sessions, unsigned int pageSize,unsigned short port, const char *cert, char *test_config){

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

  int opof_test31(const char *address, int max_sessions, unsigned int pageSize,unsigned short port, const char *cert, char *test_config){

    int status;
    streamArgs_t args;
    sessionTable_t *handle;
    unsigned long number_sessions=pageSize;
    sessionRequest_t **request;
    sessionResponse_t response;
    addSessionResponse_t addResp;
    args.pageSize = pageSize;
    sessionResponse_t **responses;
    uint64_t startSession = UINT_MAX;
    responses = (sessionResponse_t **)malloc(pageSize * sizeof(sessionResponse_t*));
    for (int i=0; i < pageSize; i++){
      responses[i] = (sessionResponse_t *)malloc(sizeof(sessionResponse_t));
    }
    printf("\n\n Test 3: Simple add sessions from config and then get all sessions\n\n");
   
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
    //opof_get_all_sessions(handle,pageSize, responses);
    while(number_sessions == pageSize){
      number_sessions = opof_get_all_sessions(handle, startSession, pageSize,responses);
      if (number_sessions > 0){
#ifdef DEBUG
      for (int i=0; i <number_sessions; i++){
        display_session_response(responses[i], "Test 3");
      }
#endif
      printf("INFO: All Sessions: %lu\n",number_sessions);
    }
    }
    for (int i=0; i < pageSize; i++){
      free(responses[i]); 
    }
    free(responses);
  
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("\n\nSessions per second: %lf\n\n", (2.0 * (double)max_sessions)/(time_spent));
    return SUCCESS;
 }

  int opof_test41(const char *address, int max_sessions, unsigned int pageSize,unsigned short port, const char *cert, char *test_config){

    int status;
    streamArgs_t args;
    sessionTable_t *handle;
    unsigned long closed_sessions=1;
    sessionRequest_t **request;
    sessionResponse_t response;
    addSessionResponse_t addResp;
    args.pageSize = pageSize;
    sessionResponse_t **responses;
    uint64_t startSession = UINT_MAX;
    printf("\n\n Test 4: Simple add sessions from config and then get all sessions and then get closed sessions\n\n");
   
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
    opof_get_all_sessions(handle,startSession,pageSize, responses);
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

  int opof_test5(const char *address, int max_sessions, unsigned int pageSize,unsigned short port, const char *cert, char *test_config){
  int status;
  
  sessionTable_t *handle;

  sessionRequest_t **request;
  addSessionResponse_t addResp;
 



 
  handle = opof_create_sessionTable(address, port, cert);
  
 
  
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
#if 0
  opof_get_all_sessions(handle);
  opof_get_closed_sessions(handle,&args);

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
#endif
 // while(1){
 //   opof_get_closed_sessions(handle,&args);  
 // }
 
  clock_t end = clock();
  double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("\n\nSessions per second: %lf\n\n", (2.0 * (double)max_sessions)/(time_spent));
  return SUCCESS;
}

  int opof_test6(const char *address, int max_sessions, unsigned int pageSize,unsigned short port, const char *cert, char *test_config){
  int status;
  sessionTable_t *handle;

  sessionResponse_t resp;
  sessionRequest_t **request;
  int sessionId;

 
  handle = opof_create_sessionTable(address, port, cert);
 

  //
  
    
    
    request = read_config(test_config, &max_sessions);
    printf("\nNumber of Sessions: %d page size: %d\n",max_sessions, max_sessions);
    //TODO handle more than 64 tests
    if (max_sessions > 64){
      fprintf(stderr,"ERROR: Truncated numberof session tests to 64, from %d\n", max_sessions);
      max_sessions = 64;
    }

    sessionId = request[0]->sessId;
    status = opof_get_session(handle, sessionId, &resp);
    if (status == FAILURE){
      printf("ERROR: getting session: %d\n", sessionId);
      exit(-1);
    }


  return SUCCESS;
}
#endif
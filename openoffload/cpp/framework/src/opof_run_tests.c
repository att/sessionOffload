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
  #include <stdbool.h>
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

unsigned int g_set_deadline(int deadline);
unsigned int g_get_deadline(void);
int g_retryInterval=15;
int g_retryAttempts=100;
int setRetryInterval(int retryInterval);
int setRetryAttempts(int retryAttempts);
int getRetryInterval(void);
int getRetryAttempts(void);

int setRetryInterval(int retryInterval){
  g_retryInterval = retryInterval;
  return g_retryInterval;
}
int setRetryAttempts(int retryAttempts){
  g_retryAttempts = retryAttempts;
  return g_retryAttempts;
}
int getRetryInterval(void){
  return g_retryInterval;
}
int getRetryAttempts(void){
  return g_retryAttempts;
}
  char * getAddResponseError(ADD_SESSION_STATUS_T errorCode);
  char * getStatusCode(int statusCode);
  
  sessionRequest_t **read_config(char *filename, int *nsessions);
  int opof_fullTestSuite(const char *address, unsigned short port, const char *cert, bool verbose);
  int opof_test1(const char *address, int max_sessions, unsigned int pageSize,unsigned short port, const char *cert, bool verbose);
  int opof_test2(const char *address, int max_sessions, unsigned int pageSize,unsigned short port, const char *cert, bool verbose);
  int opof_test3(const char *address, int max_sessions, unsigned int pageSize,unsigned short port, const char *cert, bool verbose);
  int opof_test4(const char *address, int max_sessions, unsigned int pageSize,unsigned short port, const char *cert, bool verbose);
  int opof_test5(const char *address, int max_sessions, unsigned int pageSize,unsigned short port, const char *cert, bool verbose);
  int opof_test6(const char *address, int max_sessions, unsigned int pageSize,unsigned short port, const char *cert, bool verbose);
  int opof_test7(const char *address, int max_sessions, unsigned int pageSize,unsigned short port, const char *cert, bool verbose);
  int opof_test8(const char *address, int max_sessions, unsigned int pageSize,unsigned short port, const char *cert, bool verbose);
  int opof_test9(const char *address, int max_sessions, unsigned int pageSize,unsigned short port, const char *cert, bool verbose);
  int opof_test10(const char *address, int max_sessions, unsigned int pageSize,unsigned short port, const char *cert, bool verbose);

  void opof_list_tests(){
    printf("\n\nAvaialble Tests\n");
    printf("\tTest 1: Use addSession to create a large number of sessions, then call getClosedSessions\n");
    printf("\tTest 2: Use addSession to create a large number of sessions, then call getAllSessions\n");
    printf("\tTest 3: Use addSession to create a session using config file and then get the sessions\n");
    printf("\tTest 4: Use addSession to create a session using config file and then delete the sessions\n");
    printf("\tTest 5: Test that getClosedSessions works when there are no sessions in session table\n");
    printf("\tTest 6: Test duplicate session errors\n");
    printf("\tTest 7: Test get and delete session not found errors\n");
    printf("\tTest 8: Test delete session and reconnect\n");
    printf("\tTest 9: Test session client timeout and reconnect\n");
    printf("\tTest 10: Test session client timeout and reconnect\n");
    printf("\n");
  }

  int opof_run_tests(const char *address, int max_sessions, unsigned int pageSize,unsigned short port, const char *cert, char *test_config, int testid, bool verbose){

    int status;

    switch (testid){

    case -1:
      status = opof_fullTestSuite(address,port, cert, verbose);
      break;
    case 1:
      status = opof_test1(address,  max_sessions, pageSize,port, cert, verbose);
      break;
    case 2:
      status = opof_test2(address,  max_sessions, pageSize,port, cert, verbose);
      break;
    case 3:
      status = opof_test3(address,  max_sessions, pageSize,port, cert, verbose);
      break;
    case 4:
      status = opof_test4(address,  max_sessions, pageSize,port, cert, verbose);
      break;
    case 5:
      status = opof_test5(address,  0, pageSize,port, cert,test_config);
      break;
    case 6:
      status = opof_test6(address,  max_sessions, pageSize,port, cert,verbose);
      break;
    case 7:
      status = opof_test7(address,  max_sessions, pageSize,port, cert,verbose);
      break;
    case 8:
      status = opof_test8(address,  max_sessions, pageSize,port, cert,verbose);
      break;
    case 9:
      status = opof_test9(address,  max_sessions, pageSize,port, cert,verbose);
      break;
    case 10:
      status = opof_test10(address,  max_sessions, pageSize,port, cert,verbose);
      break;
    default:
      printf("ERROR: Unknown Test ID: %d\n", testid);
      status = FAILURE;
      return status;
    }
      if (status == SUCCESS){
        if (testid == -1){
          printf("\nINFO: Full Test Suite ran successfully\n\n");
        } else {
          printf("\nINFO: Test: %d ran successfully\n\n", testid);
        }
      } else {
        printf("ERROR: Test: %d Failed\n", testid);
      }
    return SUCCESS;
  }

  int opof_delete_all_sessions(void * handle, int pageSize){

    sessionResponse_t responses[BUFFER_MAX], resp;
    unsigned long nsessions = 1;
    int sessionId;
    int status = SUCCESS;
    uint64_t sessionStart = UINT_MAX;

  
    while (nsessions > 0 ){
      status = opof_get_all_sessions(handle, &sessionStart, pageSize, responses, &nsessions);
      if (status == 14){
        int retryInterval = getRetryInterval();
        int retryAttempts = getRetryAttempts();
        for(int i =0; i < retryAttempts; i++){
          sleep(retryInterval);
          status = opof_get_all_sessions(handle, &sessionStart, pageSize, responses, &nsessions);
          if (status != 14){
            break;
          }
        }
      }

      //printf("Number of sessions for get all: %d\n",nsessions);
      if (nsessions > 0){
        for (int i=0; i < nsessions; i++){
          sessionId = responses[i].sessionId;
          status = opof_del_session(handle, sessionId, &resp);
          if (status == 14){
            int retryInterval = getRetryInterval();
            int retryAttempts = getRetryAttempts();
            for(int i =0; i < retryAttempts; i++){
              sleep(retryInterval);
              status = opof_del_session(handle, sessionId, &resp);
              if (status != 14){
                break;
              } 
            }
          }
        }
      }
    }

    return status;
  }

int opof_fullTestSuite(const char *address, unsigned short port, const char *cert,bool verbose){

      int status;
      int pageSize = BUFFER_MAX;
      int max_sessions;

      max_sessions = 19;
      printf("\n\n\nSmall Buffer Tests: %d\n", max_sessions);
      status = opof_test1(address,  max_sessions, pageSize,port, cert, verbose);
      if (status == SUCCESS){
        printf("Passed Test 1 with max sessions: %d\n",max_sessions);
      } else {
        printf("Failed Test 1 with max sessions: %d\n",max_sessions);
        return FAILURE;
      }
      status = opof_test2(address,  max_sessions, pageSize,port, cert, verbose);
      if (status == SUCCESS){
        printf("Passed Test 2 with max sessions: %d\n",max_sessions);
      } else {
        printf("Failed Test 2 with max sessions: %d\n",max_sessions);
        return FAILURE;
      }
      status = opof_test3(address,  max_sessions, pageSize,port, cert, verbose);
      if (status == SUCCESS){
        printf("Passed Test 3 with max sessions: %d\n",max_sessions);
      } else {
        printf("Failed Test 3 with max sessions: %d\n",max_sessions);
        return FAILURE;
      }
      status = opof_test4(address,  max_sessions, pageSize,port, cert, verbose);
      if (status == SUCCESS){
        printf("Passed Test 4 with max sessions: %d\n",max_sessions);
      } else {
        printf("Failed Test 4 with max sessions: %d\n",max_sessions);
        return FAILURE;
      }
     
      max_sessions = 72;
      printf("\n\n\nMedium Buffer Tests: %d\n", max_sessions);
      status = opof_test1(address,  max_sessions, pageSize,port, cert, verbose);
      if (status == SUCCESS){
        printf("Passed Test 1 with max sessions: %d\n",max_sessions);
      } else {
        printf("Failed Test 1 with max sessions: %d\n",max_sessions);
        return FAILURE;
      }
      status = opof_test2(address,  max_sessions, pageSize,port, cert, verbose);
      if (status == SUCCESS){
        printf("Passed Test 2 with max sessions: %d\n",max_sessions);
      } else {
        printf("Failed Test 2 with max sessions: %d\n",max_sessions);
        return FAILURE;
      }
      status = opof_test3(address,  max_sessions, pageSize,port, cert, verbose);
      if (status == SUCCESS){
        printf("Passed Test 3 with max sessions: %d\n",max_sessions);
      } else {
        printf("Failed Test 3 with max sessions: %d\n",max_sessions);
        return FAILURE;
      }
      status = opof_test4(address,  max_sessions, pageSize,port, cert, verbose);
      if (status == SUCCESS){
        printf("Passed Test 4 with max sessions: %d\n",max_sessions);
      } else {
        printf("Failed Test 4 with max sessions: %d\n",max_sessions);
        return FAILURE;
      }
      status = opof_test5(address,  max_sessions, pageSize,port, cert, verbose);
      if (status == SUCCESS){
        printf("Passed Test 5 with max sessions: %d\n",max_sessions);
      } else {
        printf("Failed Test 5 with max sessions: %d\n",max_sessions);
        return FAILURE;
      }
      status = opof_test6(address,  max_sessions, pageSize,port, cert, verbose);
      if (status == SUCCESS){
        printf("Passed Test 6 with max sessions: %d\n",max_sessions);
      } else {
        printf("Failed Test 6 with max sessions: %d\n",max_sessions);
        return FAILURE;
      }
      status = opof_test7(address,  max_sessions, pageSize,port, cert, verbose);
      if (status == SUCCESS){
        printf("Passed Test 7 with max sessions: %d\n",max_sessions);
      } else {
        printf("Failed Test 7 with max sessions: %d\n",max_sessions);
        return FAILURE;
      }
      status = opof_test8(address,  max_sessions, pageSize,port, cert, verbose);
      if (status == SUCCESS){
        printf("Passed Test 8 with max sessions: %d\n",max_sessions);
      } else {
        printf("Failed Test 8 with max sessions: %d\n",max_sessions);
        return FAILURE;
      }

      max_sessions = 384;
      printf("\n\n\nLarge Buffer Tests: %d\n", max_sessions);
      status = opof_test1(address,  max_sessions, pageSize,port, cert, verbose);
      if (status == SUCCESS){
        printf("Passed Test 1 with max sessions: %d\n",max_sessions);
      } else {
        printf("Failed Test 1 with max sessions: %d\n",max_sessions);
        return FAILURE;
      }
      status = opof_test2(address,  max_sessions, pageSize,port, cert, verbose);
      if (status == SUCCESS){
        printf("Passed Test 2 with max sessions: %d\n",max_sessions);
      } else {
        printf("Failed Test 2 with max sessions: %d\n",max_sessions);
        return FAILURE;
      }
      status = opof_test3(address,  max_sessions, pageSize,port, cert, verbose);
      if (status == SUCCESS){
        printf("Passed Test 3 with max sessions: %d\n",max_sessions);
      } else {
        printf("Failed Test 3 with max sessions: %d\n",max_sessions);
        return FAILURE;
      }
      status = opof_test4(address,  max_sessions, pageSize,port, cert, verbose);
      if (status == SUCCESS){
        printf("Passed Test 4 with max sessions: %d\n",max_sessions);
      } else {
        printf("Failed Test 4 with max sessions: %d\n",max_sessions);
        return FAILURE;
      }
            status = opof_test5(address,  max_sessions, pageSize,port, cert, verbose);
      if (status == SUCCESS){
        printf("Passed Test 5 with max sessions: %d\n",max_sessions);
      } else {
        printf("Failed Test 5 with max sessions: %d\n",max_sessions);
        return FAILURE;
      }
      status = opof_test6(address,  max_sessions, pageSize,port, cert, verbose);
      if (status == SUCCESS){
        printf("Passed Test 6 with max sessions: %d\n",max_sessions);
      } else {
        printf("Failed Test 6 with max sessions: %d\n",max_sessions);
        return FAILURE;
      }
      status = opof_test7(address,  max_sessions, pageSize,port, cert, verbose);
      if (status == SUCCESS){
        printf("Passed Test 7 with max sessions: %d\n",max_sessions);
      } else {
        printf("Failed Test 7 with max sessions: %d\n",max_sessions);
        return FAILURE;
      }
      status = opof_test8(address,  max_sessions, pageSize,port, cert, verbose);
      if (status == SUCCESS){
        printf("Passed Test 8 with max sessions: %d\n",max_sessions);
      } else {
        printf("Failed Test 8 with max sessions: %d\n",max_sessions);
        return FAILURE;
      }

      max_sessions = 1019;
      printf("\n\n\nCache Overflow Tests: %d\n", max_sessions);
      status = opof_test1(address,  max_sessions, pageSize,port, cert, verbose);
      if (status == SUCCESS){
        printf("Passed Test 1 with max sessions: %d\n",max_sessions);
      } else {
        printf("Failed Test 1 with max sessions: %d\n",max_sessions);
        return FAILURE;
      }
      status = opof_test2(address,  max_sessions, pageSize,port, cert, verbose);
      if (status == SUCCESS){
        printf("Passed Test 2 with max sessions: %d\n",max_sessions);
      } else {
        printf("Failed Test 2 with max sessions: %d\n",max_sessions);
        return FAILURE;
      }
      status = opof_test3(address,  max_sessions, pageSize,port, cert, verbose);
      if (status == SUCCESS){
        printf("Passed Test 3 with max sessions: %d\n",max_sessions);
      } else {
        printf("Failed Test 3 with max sessions: %d\n",max_sessions);
        return FAILURE;
      }
      status = opof_test4(address,  max_sessions, pageSize,port, cert, verbose);
      if (status == SUCCESS){
        printf("Passed Test 4 with max sessions: %d\n",max_sessions);
      } else {
        printf("Failed Test 4 with max sessions: %d\n",max_sessions);
        return FAILURE;
      }
            status = opof_test5(address,  max_sessions, pageSize,port, cert, verbose);
      if (status == SUCCESS){
        printf("Passed Test 5 with max sessions: %d\n",max_sessions);
      } else {
        printf("Failed Test 5 with max sessions: %d\n",max_sessions);
        return FAILURE;
      }
      status = opof_test6(address,  max_sessions, pageSize,port, cert, verbose);
      if (status == SUCCESS){
        printf("Passed Test 6 with max sessions: %d\n",max_sessions);
      } else {
        printf("Failed Test 6 with max sessions: %d\n",max_sessions);
        return FAILURE;
      }
      status = opof_test7(address,  max_sessions, pageSize,port, cert, verbose);
      if (status == SUCCESS){
        printf("Passed Test 7 with max sessions: %d\n",max_sessions);
      } else {
        printf("Failed Test 7 with max sessions: %d\n",max_sessions);
        return FAILURE;
      }
      status = opof_test8(address,  max_sessions, pageSize,port, cert, verbose);
      if (status == SUCCESS){
        printf("Passed Test 8 with max sessions: %d\n",max_sessions);
      } else {
        printf("Failed Test 8 with max sessions: %d\n",max_sessions);
        return FAILURE;
      }
  return SUCCESS;

}

int opof_test1(const char *address, int max_sessions, unsigned int pageSize,unsigned short port, const char *cert, bool verbose){

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
  int total_sessions = max_sessions;

 
  handle = opof_create_sessionTable(address, port, cert);
  args.handle = handle;
  args.pageSize = pageSize;

  /*
  *  Clean up any exisitng data in cache
  */
  if (opof_delete_all_sessions(handle,pageSize) == FAILURE){
    return FAILURE;
  }
  printf("\n\nRunning Test 1: Testing get closed sessions");
  printf("\tNumber of Sessions: %d page size: %d\n",max_sessions, pageSize);
  //
  clock_t begin = clock();
  printf("\n\tAdding %d sessions to cache\n", max_sessions);
  while(max_sessions > 0){

    sessionCount = max_sessions - pageSize;
    if (sessionCount < 0){
      bufferSize = max_sessions;
    } else {
      bufferSize = pageSize;
    }
    request = createSessionRequest(bufferSize, sessionId);
    status = opof_add_session(bufferSize,handle, request, &addResp);
    if (status != _OK){
      printf("ERROR: Adding sessions: %d\n",status);
      return FAILURE;
    }
    if (addResp.number_errors > 0){
      printf("\n\nErrors in the following sessions\n");
      for (int i=0; i < addResp.number_errors; i++){
        printf("\tSessionId: %lu\t error: %s\n", addResp.sessionErrors[i].sessionId, getAddResponseError(addResp.sessionErrors[i].errorStatus));
      }
    }
     max_sessions = sessionCount;
     sessionId += bufferSize;
  }
  if (verbose == true){
    print_response_header();
  }
  printf("\n\tClosing sessions from cache\n");
  while(closed_sessions > 0){
    status = opof_get_closed_sessions(&args,responses,&closed_sessions);
    printf("Closed sessions: %lu\n", closed_sessions);
    if (verbose == true){
      if (closed_sessions > 0){
        for (int i=0; i < closed_sessions; i++){
          print_response( &responses[i]);
        }
      }
    }
  }
  clock_t end = clock();
  double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("\n\nSessions per second (add and close): %lf\n\n", ((double)total_sessions)/(time_spent));
  return SUCCESS;
}

int opof_test2(const char *address, int max_sessions, unsigned int pageSize,unsigned short port, const char *cert, bool verbose){

  unsigned long nsessions = 1;
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
  printf("\n\nRunning Test 2; Testing get all Sessions");
  printf("\tNumber of Sessions: %d page size: %d\n",max_sessions, pageSize);
  //
  printf("\n\tAdding %d sessions to cache\n", max_sessions);
  while(max_sessions > 0){

    sessionCount = max_sessions - pageSize;
    if (sessionCount < 0){
      bufferSize = max_sessions;
    } else {
      bufferSize = pageSize;
    }
    request = createSessionRequest(bufferSize, sessionId);
    status = opof_add_session(bufferSize,handle, request, &addResp);
    if (status != _OK){
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
  printf("\n\tGetting all sessions from cache\n");
  while (nsessions > 0 ){
    status = opof_get_all_sessions(handle, &sessionStart, pageSize, responses, &nsessions);
    if (nsessions > 0){
      if (verbose){
        printf("\n\nDisplaying Page: %d\n",page_number);
        print_response_header();
      }
      for (int i=0; i < nsessions; i++){
        if (verbose){
          print_response(&responses[i]);
        }
        sessionStart = responses[i].sessionId;
      }
      //printf("sessionStart: %lu\n",sessionStart);
      page_number++;
      //if (page_number >5){
      //return SUCCESS;
      //}
    }
  }

  return SUCCESS;
}

int opof_test3(const char *address, int max_sessions, unsigned int pageSize,unsigned short port, const char *cert, bool verbose){

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
  printf("\n\nRunning Test 3: Testing get sessions");
  printf("\tNumber of Sessions: %d page size: %d\n",max_sessions, pageSize);
 
  //
  total_sessions = max_sessions;
  printf("\n\tAdding %d sessions to cache\n", max_sessions);
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
  if (verbose){
    printf("\n\tGetting following sessions from cache\n");
    print_response_header();
  }
  for (int i = 0; i < total_sessions; i++){
    status = opof_get_session(handle, i, &resp);
    if (verbose){
      print_response(&resp);
    }
    if (status == FAILURE){
      printf("ERROR: getting session: %d\n", sessionId);
      exit(-1);
    }
  }
  return SUCCESS;
}


int opof_test4(const char *address, int max_sessions, unsigned int pageSize,unsigned short port, const char *cert, bool verbose){

  //unsigned long nsessions = 1;
  int status;
  
  sessionTable_t *handle;
 
  sessionRequest_t **request;
  sessionResponse_t resp;
  addSessionResponse_t addResp;
 
  //sessionResponse_t responses[BUFFER_MAX];
  int sessionCount =1;
  int bufferSize;
  handle = opof_create_sessionTable(address, port, cert);
  int sessionId=0;
  int total_sessions;
  
  //uint64_t sessionStart = UINT_MAX;
  /*
  *  Clean up any exisitng data in cache
  */
  if (opof_delete_all_sessions(handle,pageSize) == FAILURE){
    return FAILURE;
  }
  printf("\n\nRunning Test 4: Testing delete sessions ");
  printf("\tNumber of Sessions: %d page size: %d\n",max_sessions, pageSize);
  
  //
  total_sessions = max_sessions;
  printf("\n\tAdding %d sessions to cache\n", max_sessions);
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
  if (verbose){
    printf("\n\tDeleting following sessions from cache\n");
    print_response_header();
  }
  for (int i = 0; i < total_sessions; i++){
    status = opof_del_session(handle, i, &resp);
    if (verbose){
      print_response(&resp);
    }
    if (status == FAILURE){
      printf("ERROR: deleting session: %d\n", sessionId);
      exit(-1);
    }
  }
  
  return SUCCESS;
}
 int opof_test5(const char *address, int max_sessions, unsigned int pageSize,unsigned short port, const char *cert, bool verbose){

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
  printf("\n\nRunning Test 5: Testing calling get_closed_sessions with zero sessions");
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
      return FAILURE;
    }
    if (addResp.number_errors > 0){
      printf("\n\nErrors in the following sessions\n");
      for (int i=0; i < addResp.number_errors; i++){
        printf("\tSessionId: %lu\t error: %s\n", addResp.sessionErrors[i].sessionId, getAddResponseError(addResp.sessionErrors[i].errorStatus));
      }
    }
     max_sessions = sessionCount;
     sessionId += bufferSize;
  }
  if (verbose == true){
    print_response_header();
  }
  while(closed_sessions > 0){
    status = opof_get_closed_sessions(&args,responses, &closed_sessions);
    if (status != _OK){
      printf("Test 5: %s\n", getStatusCode(status));
    }
    if (verbose == true){
      if (closed_sessions > 0){
        for (int i=0; i < closed_sessions; i++){
          print_response( &responses[i]);
        }
      }
    }
  }
  return SUCCESS;
}

int opof_test6(const char *address, int max_sessions, unsigned int pageSize,unsigned short port, const char *cert, bool verbose){

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
  int total_sessions = max_sessions;

 
  handle = opof_create_sessionTable(address, port, cert);
  args.handle = handle;
  args.pageSize = pageSize;

  /*
  *  Clean up any exisitng data in cache
  */
  if (opof_delete_all_sessions(handle,pageSize) == FAILURE){
    return FAILURE;
  }
  printf("\n\nRunning Test 6: Testing duplicate session adds");
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
    if (status != _OK){
      printf("Test 6: %s\n", getStatusCode(status));
      return FAILURE;
    }
    if (addResp.number_errors > 0){
      printf("\n\nErrors in the following sessions\n");
      for (int i=0; i < addResp.number_errors; i++){
        printf("\tSessionId: %lu\t error: %s\n", addResp.sessionErrors[i].sessionId, getAddResponseError(addResp.sessionErrors[i].errorStatus));
      }
    }
     max_sessions = sessionCount;
     sessionId += bufferSize;
  }
   sessionId = 0;
   max_sessions = total_sessions;
   while(max_sessions > 0){

    sessionCount = max_sessions - pageSize;
    if (sessionCount < 0){
      bufferSize = max_sessions;
    } else {
      bufferSize = pageSize;
    }
    request = createSessionRequest(bufferSize, sessionId);
    status = opof_add_session(bufferSize,handle, request, &addResp);
    if (status != _OK){
      printf("Test 6: %s\n", getStatusCode(status));
      return FAILURE;
    }
    if (addResp.number_errors > 0){
      printf("\n\nErrors in the following sessions\n");
      for (int i=0; i < addResp.number_errors; i++){
        printf("\tSessionId: %lu\t error: %s\n", addResp.sessionErrors[i].sessionId, getAddResponseError(addResp.sessionErrors[i].errorStatus));
      }
    }
     max_sessions = sessionCount;
     sessionId += bufferSize;
  }
  if (verbose == true){
    print_response_header();
  }
  while(closed_sessions > 0){
    status = opof_get_closed_sessions(&args,responses,&closed_sessions);
    printf("Closed sessions: %lu\n", closed_sessions);
    if (verbose == true){
      if (closed_sessions > 0){
        for (int i=0; i < closed_sessions; i++){
          print_response( &responses[i]);
        }
      }
    }
  }
  return SUCCESS;
}


int opof_test7(const char *address, int max_sessions, unsigned int pageSize,unsigned short port, const char *cert, bool verbose){

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
  printf("\n\nRunning Test 7: Testing get and delete on invalid sessions");
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
  //if (verbose){
  //  print_response_header();
 // }
  status = opof_get_session(handle, total_sessions+1, &resp);
  printf("\nTest 7 Get session ID: %d response: %s\n", total_sessions+1, getStatusCode(status));
  status = opof_del_session(handle, total_sessions+1, &resp);
  printf("Test 7 Delete session ID: %d response %s\n", total_sessions+1, getStatusCode(status));
  //if (verbose){
  //  print_response(&resp);
  //}
  if (status == FAILURE){
    printf("ERROR: getting session: %d\n", sessionId);
    exit(-1);
  }
  return SUCCESS;
}

int opof_test8(const char *address, int max_sessions, unsigned int pageSize,unsigned short port, const char *cert, bool verbose){

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
  int total_sessions = max_sessions;

 
  handle = opof_create_sessionTable(address, port, cert);
  args.handle = handle;
  args.pageSize = pageSize;

  /*
  *  Clean up any exisitng data in cache
  */
  if (opof_delete_all_sessions(handle,pageSize) == FAILURE){
    return FAILURE;
  }
  printf("\n\nRunning Test 8: Test delete and recreating session table handle");
  printf("\tNumber of Sessions: %d page size: %d\n",max_sessions, pageSize);
  //
  clock_t begin = clock();
  while(max_sessions > 0){

    sessionCount = max_sessions - pageSize;
    if (sessionCount < 0){
      bufferSize = max_sessions;
    } else {
      bufferSize = pageSize;
    }
    request = createSessionRequest(bufferSize, sessionId);
    status = opof_add_session(bufferSize,handle, request, &addResp);
    if (status != _OK){
      printf("ERROR: Adding sessions: %d\n",status);
      return FAILURE;
    }
    if (addResp.number_errors > 0){
      printf("\n\nErrors in the following sessions\n");
      for (int i=0; i < addResp.number_errors; i++){
        printf("\tSessionId: %lu\t error: %s\n", addResp.sessionErrors[i].sessionId, getAddResponseError(addResp.sessionErrors[i].errorStatus));
      }
    }
     max_sessions = sessionCount;
     sessionId += bufferSize;
  }
  if (verbose == true){
    print_response_header();
  }

  opof_delete_sessionTable(handle);
  handle = opof_create_sessionTable(address, port, cert);
  args.handle = handle;
  args.pageSize = pageSize;

  while(closed_sessions > 0){
    status = opof_get_closed_sessions(&args,responses,&closed_sessions);
    printf("Closed sessions: %lu\n", closed_sessions);
    if (verbose == true){
      if (closed_sessions > 0){
        for (int i=0; i < closed_sessions; i++){
          print_response( &responses[i]);
        }
      }
    }
  }
  clock_t end = clock();
  double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("\n\nSessions per second (add and close): %lf\n\n", ((double)total_sessions)/(time_spent));
  return SUCCESS;
}


int opof_test9(const char *address, int max_sessions, unsigned int pageSize,unsigned short port, const char *cert, bool verbose){

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
  int total_sessions = max_sessions;

 
  handle = opof_create_sessionTable(address, port, cert);
  args.handle = handle;
  args.pageSize = pageSize;
  printf("Handle %p\n", handle);

  /*
  *  Clean up any exisitng data in cache
  */
  if (opof_delete_all_sessions(handle,pageSize) == FAILURE){
    return FAILURE;
  }
  printf("\n\nRunning Test 9: Test session opof_clientlib timeout and restart\n");
  printf("\tNumber of Sessions: %d page size: %d\n",max_sessions, pageSize);
  //
  clock_t begin = clock();
  if (kill_test_server() == SUCCESS){
    printf("Test server killed\n");
  }
  while(max_sessions > 0){

    sessionCount = max_sessions - pageSize;
    if (sessionCount < 0){
      bufferSize = max_sessions;
    } else {
      bufferSize = pageSize;
    }
    request = createSessionRequest(bufferSize, sessionId);
    status = opof_add_session(bufferSize,handle, request, &addResp);
    /*
    * Check server is up - if not wait for it to come up
    */
    if (status == 14){
      int retryInterval = getRetryInterval();
      int retryAttempts = getRetryAttempts();
      for(int i =0; i < retryAttempts; i++){
        sleep(retryInterval);
        status = opof_add_session(bufferSize,handle, request, &addResp);
        if (status != 14){
          break;
        }
      }
    }
    if (status != _OK){
      printf("ERROR: Adding sessions: %d\n",status);
      return FAILURE;
    } 

    
    if (addResp.number_errors > 0){
      printf("\n\nErrors in the following sessions\n");
      for (int i=0; i < addResp.number_errors; i++){
        printf("\tSessionId: %lu\t error: %s\n", addResp.sessionErrors[i].sessionId, getAddResponseError(addResp.sessionErrors[i].errorStatus));
      }
    }
     max_sessions = sessionCount;
     sessionId += bufferSize;
  }
  if (verbose == true){
    print_response_header();
  }

  opof_delete_sessionTable(handle);
  handle = opof_create_sessionTable(address, port, cert);
  args.handle = handle;
  args.pageSize = pageSize;

  while(closed_sessions > 0){
    status = opof_get_closed_sessions(&args,responses,&closed_sessions);
    printf("Closed sessions: %lu\n", closed_sessions);
    if (verbose == true){
      if (closed_sessions > 0){
        for (int i=0; i < closed_sessions; i++){
          print_response( &responses[i]);
        }
      }
    }
  }
  clock_t end = clock();
  double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("\n\nSessions per second (add and close): %lf\n\n", ((double)total_sessions)/(time_spent));
  return SUCCESS;
}


int opof_test10(const char *address, int max_sessions, unsigned int pageSize,unsigned short port, const char *cert, bool verbose){

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
  int total_sessions = max_sessions;

 
  handle = opof_create_sessionTable(address, port, cert);
  args.handle = handle;
  args.pageSize = pageSize;
  printf("Handle %p\n", handle);

  /*
  *  Clean up any exisitng data in cache
  */
  if (opof_delete_all_sessions(handle,pageSize) == FAILURE){
    return FAILURE;
  }
  printf("\n\nRunning Test 10: Test session deadline timeout and restart\n");
  printf("\tNumber of Sessions: %d page size: %d\n",max_sessions, pageSize);
  //
  clock_t begin = clock();

  g_set_deadline(100);
  while(max_sessions > 0){

    sessionCount = max_sessions - pageSize;
    if (sessionCount < 0){
      bufferSize = max_sessions;
    } else {
      bufferSize = pageSize;
    }
    request = createSessionRequest(bufferSize, sessionId);
    status = opof_add_session(bufferSize,handle, request, &addResp);
    /*
    * Check server is up - if not wait for it to come up
    */
     printf("Add Sessi{on Status: %d\n",status);
    if (status == 4){
      printf("Deadline exceeded retrying\n");
      g_set_deadline(4000); 
      //opof_delete_sessionTable(handle);
      //handle = opof_create_sessionTable(address, port, cert);
      status = opof_add_session(bufferSize,handle, request, &addResp);
      printf("Add Sessi{on Status: %d, deadline %d\n",status,g_get_deadline());
    }
    if (status == 14){
      int retryInterval = getRetryInterval();
      int retryAttempts = getRetryAttempts();
      for(int i =0; i < retryAttempts; i++){
        sleep(retryInterval);
        status = opof_add_session(bufferSize,handle, request, &addResp);

        if (status != 14){
          break;
        }
      }
    }
    if (status != _OK){
      printf("ERROR: Adding sessions: %d\n",status);
      return FAILURE;
    } 

    
    if (addResp.number_errors > 0){
      printf("\n\nErrors in the following sessions\n");
      for (int i=0; i < addResp.number_errors; i++){
        printf("\tSessionId: %lu\t error: %s\n", addResp.sessionErrors[i].sessionId, getAddResponseError(addResp.sessionErrors[i].errorStatus));
      }
    }
     max_sessions = sessionCount;
     sessionId += bufferSize;
  }
  if (verbose == true){
    print_response_header();
  }
  g_set_deadline(100); 
  opof_delete_sessionTable(handle);
  handle = opof_create_sessionTable(address, port, cert);
  args.handle = handle;
  args.pageSize = pageSize;

  while(closed_sessions > 0){
    status = opof_get_closed_sessions(&args,responses,&closed_sessions);
    if (status == 4){
      printf("Deadline exceeded for get closed sessions retrying\n");
      g_set_deadline(4000); 
      //opof_delete_sessionTable(handle);
      //handle = opof_create_sessionTable(address, port, cert);
       status = opof_get_closed_sessions(&args,responses,&closed_sessions);
      printf("get closed Sessi{on Status: %d, deadline %d\n",status,g_get_deadline());
    }
    printf("Closed sessions: %lu\n", closed_sessions);
    if (verbose == true){
      if (closed_sessions > 0){
        for (int i=0; i < closed_sessions; i++){
          print_response( &responses[i]);
        }
      }
    }
  }
  clock_t end = clock();
  double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("\n\nSessions per second (add and close): %lf\n\n", ((double)total_sessions)/(time_spent));
  return SUCCESS;
}




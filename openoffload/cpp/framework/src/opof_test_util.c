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

#include <arpa/inet.h>
#include <sys/types.h>          
#include <sys/socket.h>
#include <netinet/in.h>


#include "opof_test_util.h"

/** \ingroup testlibrary
* \brief Utility function to read a SSL key from a local file system
*
* This function creates a "fake" array of sessionResponse_t structs. The statistics
* data is just created by a random function, that creates a value within a range. The 
* sessions are take from the hashtable. This is very insecure and should only be used 
* with test certificates and never in production.
*
* \param *filename        Filename of the key to read including the relative path.
* \param *key         The SSL key read from the file
* 
* \return SUCCESS or FAILURE
**/
int get_key(const char *filename, char *key){
    FILE *fp;
    int status  = SUCCESS;
  printf("Reading: %s\n", filename);
    fp = fopen(filename, "r");
    if(fp == NULL) {
      perror("Error opening file ");
      return(-1);
    }
    status = fread(key, sizeof(char), 2047,(FILE*)fp);
    if (status == -1){
        perror("Error reading file ");
        return(-1);
    } else {
        printf("Read %d characters\n",status);
        key[status++] = '\0'; /* Just to be safe. */
    }
   fclose(fp);
   printf("Returning: %d from reading %s\n", status, filename);
   return status;
};
/** \ingroup testlibrary
* \brief Utility function to create random dummy value in between a range of numbers
*
* \param lower   The lower value of the range within to create an int.
* \param upper   The upper value of the range within to create an int.
* 
* \return int Random value within the input range
**/
int range(int lower, int upper){
  return ((rand() %  (upper - lower + 1)) + lower); 
}

/** \ingroup testlibrary
* \brief Utility function to create an array of sessionResponse_t structs
*
* This function creates a "fake" array of sessionResponse_t structs. The statistics
* data is just created by a random function, that creates a value within a range. The 
* sessions are take from the hashtable.
*
* \param *size                 The size of the array to create
* \param *sessionCount   Returns the number of sessions returned from hashtable/
* 
* \return **sessionRequest_t
**/
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

/** \ingroup testlibrary
* \brief Utility function to create an array of sessionRequest_t structs
*
* This function creates a "fake" array of sessions. It is very simple and reuses
* the data for all teh sessions the only change is the session id which is incremented.
* It returns an array of sessionRequest_t suitable to pass to addSession.
*
* \param *size              The size of the array to create
* \param start_sessionId   The starting sessionId
* 
* \return **sessionRequest_t
**/
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
  status = inet_pton(AF_INET,src_addr, &srcip);
  if (status != 1) {
    fprintf(stderr,"ERROR: invalid address: %s %s\n",src_addr, strerror(errno));
  }
  status = inet_pton(AF_INET,dst_addr, &dstip);
  if (status != 1){
    fprintf(stderr,"ERROR: invalid address: %s %s\n",src_addr, strerror(errno));
  }
  status = inet_pton(AF_INET,nextHop, &nexthopip);
  if (status != 1){
    fprintf(stderr,"ERROR: invalid address: %s %s\n",src_addr, strerror(errno));
  }
  requests = (sessionRequest_t **)malloc(size * (sizeof(requests)));
  for (unsigned long i = 0; i < size; i++){
    request = (sessionRequest_t *)malloc(sizeof(*request));
    request->sessId = (i+start_sessionId);
    request->inlif = 1;
    request->outlif = 2;
    request->srcIP = srcip;
    request->dstIP = dstip;
    request->srcPort = 80;
    request->dstPort = 45678;
    request->proto = proto;
    request->ipver = ipver;
    request->nextHop = nexthopip;
    request->actType = action;
    requests[i] = request;
  }
return requests;
}
/** \ingroup testlibrary
* \brief Utility function to print out the sessionResponse_t structure to stdout
*
* This function prints out the contents of the sessionResponse_t structure in a human
* readable format. It converts the enums to strings and the IP Addresses to strings
* for both IPV4 and IPV$
*
* \param *response  The sessionResponse_t to print
* \param *message   A user defiend message to print with the contents
* 
* \return void
**/
void display_session_response(sessionResponse_t *response, const char *message){
    printf("\n\nSession Response: %s\n",message);
    printf("Session ID: %ld\n",response->sessionId);
    printf("In Packets %ld\n",response->inPackets);
    printf("Out Packets: %ld\n",response->outPackets);
    printf("In Bytes: %ld\n",response->inBytes);
    printf("Out Bytes: %ld\n",response->outBytes);
    if (response->sessionState == 0){
      printf("Session State (%d) _ESTABLSIHED \n",response->sessionState);
    } else if (response->sessionState == 1){
      printf("Session State (%d) _CLOSING_1 \n",response->sessionState);
    } else if (response->sessionState == 2){
      printf("Session State (%d) _CLOSING_2 \n",response->sessionState);
    } else if (response->sessionState == 3){
      printf("Session State (%d) _CLOSED \n",response->sessionState);
    } else {
      printf("Session State Invalid value(%d) \n",response->sessionState);
    }
    if (response->sessionCloseCode == 0){
       printf("Session Close Code (%d) _NOT_CLOSED \n",response->sessionCloseCode);
    } else if (response->sessionCloseCode == 1){
       printf("Session Close Code (%d) _FINACK \n",response->sessionCloseCode);
    } else if (response->sessionCloseCode == 2){
       printf("Session Close Code (%d) _RST \n",response->sessionCloseCode);
    } else if (response->sessionCloseCode == 3){
       printf("Session Close Code (%d) _TIMEOUT \n",response->sessionCloseCode);
    } else {
      printf("Session Close Code Invalid value (%d) \n",response->sessionCloseCode);
    }
    if (response->requestStatus == 0){
      printf("Request Status (%d) _ACCEPTED\n",response->requestStatus);
    } else if (response->requestStatus == 1){
      printf("Request Status (%d) _REJECTED\n",response->requestStatus);
    } else if (response->requestStatus == 2){
      printf("Request Status (%d) _REJECTED_SESSION_NOTEXISTANT\n",response->requestStatus);
    } else if (response->requestStatus == 3){
      printf("Request Status (%d) _REJECTED_SESSION_TABLE_FULL\n",response->requestStatus);
    } else if (response->requestStatus == 4){
      printf("Request Status (%d) _REJECTED_SESSION_ALREADY_EXISTS\n",response->requestStatus);
    } else if (response->requestStatus == 5){
      printf("Request Status (%d) _NO_CLOSED_SESSIONS\n",response->requestStatus);
    } else {
      printf("Request Status Invalid Value: %d\n",response->requestStatus);
    }
}
/** \ingroup testlibrary
* \brief Utility function to print out the sessionRequest_t structure to stdout
*
* This function prints out the contents of the sessionRequest_t structure in a human
* readable format. It converts the enums to strings.
*
* \param *response  The sessionRequest_t to print
* \param *message   A user defiend message to print with the contents
* 
* \return void
**/
void display_session_request(sessionRequest_t *request, const char * message){
    char str[INET6_ADDRSTRLEN];
    printf("\n\nSession Request: %s\n",message);
    printf("Session ID: %ld\n",request->sessId);
    printf( "Inlif: %d\n",request->inlif);
    printf( "Outlif: %d\n",request->outlif);
    if ((request->ipver) == _IPV6){
      printf( "IP Version: V6\n");
      printf( "Source IP: %s\n", inet_ntop(AF_INET6, &request->srcIPV6, str, INET6_ADDRSTRLEN));
      printf( "Destination IP: %s\n",inet_ntop(AF_INET6, &request->dstIPV6, str, INET6_ADDRSTRLEN));
      printf( "NextHop IP: %s\n",inet_ntop(AF_INET6, &request->nextHopV6, str, INET6_ADDRSTRLEN));
    } else {
      printf( "IP Version: V4\n");
      printf( "Source IP: %s\n", inet_ntop(AF_INET, &request->srcIP, str, INET6_ADDRSTRLEN));
      printf( "Destination IP: %s\n",inet_ntop(AF_INET, &request->dstIP, str, INET6_ADDRSTRLEN));
      printf( "NextHop IP: %s\n",inet_ntop(AF_INET, &request->nextHop, str, INET6_ADDRSTRLEN));
    }
    printf( "Source Port: %d\n",request->srcPort);
    printf( "Destination Port: %d\n",request->dstPort);
    if (request->proto == 6){
      printf( "Protocol Type (%d): TCP\n", request->proto);
    } else if (request->proto ==17 ){
      printf( "Protocol Type (%d): UDP\n",request->proto);
    } else {
      printf( "request->proto is: unknown\n");
    }
    if (request->actType == 0){
      printf( "Action Value (%d): DROP\n",request->actType);
    } else if (request->actType == 1) {
    printf( "Action Value (%d):  FORWARD\n",request->actType);
    } else if (request->actType == 2) {
    printf( "Action Value (%d):  MIRROR\n",request->actType);
    } else if (request->actType == 3) {
    printf( "Action Value (%d):  SNOOP\n",request->actType);
    } else {
       printf( "Action Value (%d): UNKNOWN\n",request->actType);
    }
}

void print_response_header(){
  printf("\n\n  Session ID\t In Packets\t In Bytes\t Out Packets\t  Out Bytes \t\t Session State \t  Close Code\t Request Status\n");
}

void print_response(sessionResponse_t *response){
 
  printf("%12lu\t %12ld\t %12ld\t %12ld\t %12ld\t", 
          response->sessionId,response->inPackets, response->inBytes, response->outPackets, 
          response->outBytes);
 printf("\t");
 if (response->sessionState == 0){
      printf( "_ESTABLISHED");
    } else if (response->sessionState == 1) {
     printf( "__CLOSING_1");
    } else if (response->sessionState == 2) {
     printf( "_CLOSING_2");
    } else if (response->sessionState == 3) {
     printf( "_CLOSED");
    } else {
      printf( "ERROR %d",response->sessionState);
    }
  printf("\t\t");
  if (response->sessionCloseCode == 0){
      printf( "_NOT_CLOSED");
    } else if (response->sessionCloseCode == 1) {
     printf( "_FINACK");
    } else if (response->sessionCloseCode == 2) {
     printf( "_RST");
    } else if (response->sessionCloseCode == 3) {
     printf( "_TIMEOUT");
    } else {
      printf( "ERROR %d",response->sessionCloseCode);
    }
  printf("\t");
  if (response->requestStatus == 0){
      printf( "_ACCEPTED");
    } else if (response->requestStatus == 1) {
     printf( "_REJECTED");
    } else if (response->requestStatus == 2) {
     printf( "_REJECTED_SESSION_NONEXISTANT");
    } else if (response->requestStatus == 3) {
     printf( "_REJECTED_SESSION_TABLE_FULL");
    } else if (response->requestStatus == 4) {
     printf( "_REJECTED_SESSION_AREADY_EXISTS");
    } else if (response->requestStatus == 5) {
     printf( "_NO_CLOSED_SESSIONS");
    } else {
      printf( "ERROR %d",response->requestStatus );
    }
    printf("\n");
}
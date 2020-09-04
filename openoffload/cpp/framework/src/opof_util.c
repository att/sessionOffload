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

#include <stdio.h>
#include "opof.h"
#include "opof_util.h"

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

int create_address(char *address, unsigned short port){
  int status = SUCCESS;
  char port_str[12];
  sprintf(port_str, "%u", port);
  strcat(address,":");
  strcat(address,port_str);
  printf("Address: %s\n",address);

  return status;
}

int range(int lower, int upper){
  return ((rand() %  (upper - lower + 1)) + lower); 
}
void display_session_response(sessionResponse_t *response){

    printf("\n\nSession Response\n");
    printf("Session ID: %ld\n",response->sessionId);
    printf("In Packets %ld\n",response->inPackets);
    printf("Out Packets: %ld\n",response->outPackets);
    printf("In Bytes: %ld\n",response->inBytes);
    printf("Out Bytes: %ld\n",response->outBytes);
    printf("Session State: %d\n",response->sessionState);
    printf("Session Close Code; %d\n",response->sessionCloseCode);
    printf("Request Status: %d\n",response->requestStatus);
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
  unsigned int srcip;
  unsigned int dstip;
  unsigned int nexthopip;
  char *src_addr = "10.0.1.1";
  char *dst_addr= "10.1.0.1";
  char *nextHop = "192.168.0.1";

  status = inet_pton(AF_INET, src_addr, &srcip); 
  status = inet_pton(AF_INET, dst_addr, &dstip); 
  status = inet_pton(AF_INET, nextHop, &nexthopip); 
  

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


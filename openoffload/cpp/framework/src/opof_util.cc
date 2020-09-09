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

extern "C" {
#include <stdio.h>
#include "opof.h"
}

#include "opof_util.h"
#include "opof_grpc.h"
#include "opof_session_client.h"




int create_address(char *address, unsigned short port){
  int status = SUCCESS;
  char port_str[12];
  sprintf(port_str, "%u", port);
  strcat(address,":");
  strcat(address,port_str);
  printf("Address: %s\n",address);

  return status;
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

void display_session_request(sessionRequest_t *request){

    printf("\n\nSession Response\n");
    printf("Session ID: %ld\n",request->sessId);
    printf( "Inlif: %d\n",request->inlif);
    printf( "Outlif: %d\n",request->outlif);
    printf( "Source Port: %d\n",request->srcPort);
    printf( "Source IP: %d\n", request->srcIP);
    printf( "Destination IP: %d\n",request->dstIP);
    printf( "Destination Port: %d\n",request->dstPort);
    printf( "Protocol ID: %d\n",request->proto);
    printf( "IP Version: %d\n",request->ipver);
    printf( "Action Value: %d\n",request->actType);
}

void convertSessionRequest2cpp(sessionRequest_t *request_c, sessionRequest *request){
    actionParameters action;

    request->set_sessionid(request_c->sessId);
    request->set_inlif(request_c->inlif);
    request->set_outlif(request_c->outlif);
    request->set_ipversion((IP_VERSION)request_c->ipver);
    request->set_sourceport(request_c->srcPort);
    request->set_sourceip(request_c->srcIP);
    request->set_destinationip(request_c->dstIP);
    request->set_destinationport(request_c->dstPort);
    request->set_protocolid((PROTOCOL_ID)request_c->proto);
    action.set_actiontype((ACTION_TYPE)request_c->actType);
    action.set_actionnexthop(request_c->nextHop);
    request->mutable_action()->CopyFrom(action);

}

void convertSessionResponse2c(sessionResponse *responsecpp, sessionResponse_t *responsec){

  responsec->sessionId = responsecpp->sessionid();
  responsec->requestStatus = (REQUEST_STATUS_T)responsecpp->requeststatus();
  responsec->sessionState = (SESSION_STATE_T)responsecpp->sessionstate();
  responsec->sessionCloseCode = (SESSION_CLOSE_T)responsecpp->sessionclosecode();
  responsec->inPackets = responsecpp->inpackets();
  responsec->outPackets = responsecpp->outpackets();
  responsec->inBytes = responsecpp->inbytes();
  responsec->outBytes = responsecpp->outbytes();
}




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

    printf("\n\nSession Request\n");
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
#ifdef DEBUG
    char address[INET_ADDRSTRLEN];
    struct in_addr ip;
    printf("DEBUG: Source IP as int: %d\n",request_c->srcIP);
    ip.s_addr = request_c->srcIP;
    printf("DEBUG: Source IP: %s\n", inet_ntop(AF_INET,(void *)&ip,address,INET_ADDRSTRLEN));
    ip.s_addr = request_c->dstIP;
    printf("DEBUG: Dest IP: %s\n", inet_ntop(AF_INET,(void *)&ip,address,INET_ADDRSTRLEN));
    ip.s_addr = request_c->nextHop;
    printf("DEBUG: NextHop IP: %s\n", inet_ntop(AF_INET,(void *)&ip,address,INET_ADDRSTRLEN));
#endif

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

void convertSessionResponse2cpp(sessionResponse *responsecpp, sessionResponse_t *responsec){
  responsecpp->set_sessionid(responsec->sessionId);
  responsecpp->set_requeststatus((REQUEST_STATUS)responsec->requestStatus);
  responsecpp->set_sessionstate((SESSION_STATE)responsec->sessionState);
  responsecpp->set_sessionclosecode((SESSION_CLOSE_CODE)responsec->sessionCloseCode);
  responsecpp->set_inpackets(responsec->inPackets);
  responsecpp->set_outpackets(responsec->outPackets);
  responsecpp->set_inbytes(responsec->inBytes);
  responsecpp->set_outbytes(responsec->outBytes);
}
void convertSessionRequest2c(sessionRequest request, sessionRequest_t *request_c){
    actionParameters action;

    request_c->sessId = request.sessionid();
    request_c->inlif = request.inlif();
    request_c->outlif = request.outlif();
    request_c->ipver = (IP_VERSION_T)request.ipversion();
    request_c->srcIP = request.sourceip();
    request_c->dstIP = request.destinationip();
    request_c->srcPort = request.sourceport();
    request_c->dstPort = request.destinationport();
    request_c->proto = (PROTOCOL_ID_T)request.protocolid();
    action = request.action();
    request_c->actType= (ACTION_VALUE_T)action.actiontype();
    request_c->nextHop= action.actionnexthop();
#ifdef DEBUG
    char address[INET_ADDRSTRLEN];
    struct in_addr ip;
    printf("DEBUG: Source IP as int: %d\n",request_c->srcIP);
    ip.s_addr = request_c->srcIP;
    printf("DEBUG: Source IP: %s\n", inet_ntop(AF_INET,(void *)&ip,address,INET_ADDRSTRLEN));
    ip.s_addr = request_c->dstIP;
    printf("DEBUG: Dest IP: %s\n", inet_ntop(AF_INET,(void *)&ip,address,INET_ADDRSTRLEN));
    ip.s_addr = request_c->nextHop;
    printf("DEBUG: NextHop IP: %s\n", inet_ntop(AF_INET,(void *)&ip,address,INET_ADDRSTRLEN));
#endif

 }




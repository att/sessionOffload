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
//#include "opof_session_client.h"




int create_address(char *address, unsigned short port){
  int status = SUCCESS;
  char port_str[12];
  sprintf(port_str, "%u", port);
  strcat(address,":");
  strcat(address,port_str);
  printf("Address: %s\n",address);

  return status;
}




void convertSessionRequest2cpp(sessionRequest_t *request_c, sessionRequest *request){
    actionParameters action;
    std::string s;
    request->set_sessionid(request_c->sessId);
    request->set_inlif(request_c->inlif);
    request->set_outlif(request_c->outlif);
    request->set_ipversion((IP_VERSION)request_c->ipver);
    request->set_sourceport(request_c->srcPort);
    if (request_c->ipver == _IPV6){
       s.assign(request_c->srcIPV6.s6_addr, request_c->srcIPV6.s6_addr+ 16);
      request->set_sourceipv6(s);
    } else {
      request->set_sourceip(request_c->srcIP.s_addr);
    } 
    if (request_c->ipver == _IPV6){
      s.assign(request_c->dstIPV6.s6_addr, request_c->dstIPV6.s6_addr+ 16);
      request->set_destinationipv6(s);
    } else {
      request->set_destinationip(request_c->dstIP.s_addr);
    } 
    request->set_destinationport(request_c->dstPort);
    request->set_protocolid((PROTOCOL_ID)request_c->proto);
    action.set_actiontype((ACTION_TYPE)request_c->actType);
    action.set_actionnexthop(request_c->nextHop);
    request->mutable_action()->CopyFrom(action);

}
void convertAddSessionResponse2c(addSessionResponse_t *response_c, addSessionResponse *response){

  response_c->requestStatus = (REQUEST_STATUS_T)response->requeststatus();
  response_c->errorStatus = response->errorstatus();
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
    std::vector<uint8_t> char_array(16, 0);
    request_c->sessId = request.sessionid();
    request_c->inlif = request.inlif();
    request_c->outlif = request.outlif();
    request_c->ipver = (IP_VERSION_T)request.ipversion();
    if (request_c->ipver == _IPV6){
      char_array.assign(request.sourceipv6().begin(), request.sourceipv6().end());
      memcpy(request_c->srcIPV6.s6_addr,&char_array[0],16);
    } else {
      request_c->srcIP.s_addr = request.sourceip();
    }
    if (request_c->ipver == _IPV6){
      char_array.assign(request.destinationipv6().begin(), request.destinationipv6().end());
       memcpy(request_c->dstIPV6.s6_addr,&char_array[0],16);
    } else {
      request_c->dstIP.s_addr = request.destinationip();
    }
    request_c->srcPort = request.sourceport();
    request_c->dstPort = request.destinationport();
    request_c->proto = (PROTOCOL_ID_T)request.protocolid();
    action = request.action();
    request_c->actType= (ACTION_VALUE_T)action.actiontype();
    request_c->nextHop= action.actionnexthop();


 }




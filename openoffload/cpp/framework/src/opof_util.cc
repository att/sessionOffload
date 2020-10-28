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
* \defgroup utilities
*
* \brief gRPC Utilities for C/C++ Library
*
* The utilities are used to map clases and structs between C/C++. In addition any 
* common operations between the client and server implementations.
*
*/

extern "C" {
#include <stdio.h>
#include "opof.h"
}

#include "opof_util.h"
#include "opof_grpc.h"



/** \ingroup utilities
*
* \brief Append the port to the address string and return as string.
*
* \param *address   A string containing the address of the server. The string must
*                   be long enough to append the port too.
* \param prot       The listening port as an int, it will be converted to a string
*                   and appended to the address string.
*
* \return SUCCESS or FAILURE
*/
int create_address(char *address, unsigned short port){
  int status = SUCCESS;
  char port_str[12];
  sprintf(port_str, "%u", port);
  strcat(address,":");
  strcat(address,port_str);
  printf("Address: %s\n",address);

  return status;
}
/** \ingroup utilities
*
* \brief Covert a C SessionRequest_t to a C++ sessionRequest Class instance
*
* \param *request_c   The sessionRequest_t struct to convert
*
* \param *request     The sessionRequest object to create
*
* \return void
*/
void convertSessionRequest2cpp(sessionRequest_t *request_c, sessionRequest *request){
    actionParameters action;
    std::string s;
    request->set_sessionid(request_c->sessId);
    request->set_inlif(request_c->inlif);
    request->set_outlif(request_c->outlif);
    request->set_ipversion((IP_VERSION)request_c->ipver);
    request->set_sourceport((unsigned int)request_c->srcPort);
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
    request->set_destinationport((unsigned int)request_c->dstPort);
    request->set_protocolid((PROTOCOL_ID)request_c->proto);
    action.set_actiontype((ACTION_TYPE)request_c->actType);
    if (request_c->ipver == _IPV6){
      s.assign(request_c->nextHopV6.s6_addr, request_c->nextHopV6.s6_addr+ 16);
      action.set_actionnexthopv6(s);
    } else {
      action.set_actionnexthop(request_c->nextHop.s_addr);
    }
    request->mutable_action()->CopyFrom(action);

}
/** \ingroup utilities
*
* \brief Covert a C SessionRequest_t to a C++ sessionRequest Class instance
*
* \param *request_c   The sessionRequest_t struct to convert
*
* \param *request     The sessionRequest object to create
*
* \return void
*/
void convertAddSessionResponse2c(addSessionResponse_t *response_c, addSessionResponse *response){
  sessionResponseError responseError;
  response_c->requestStatus = (REQUEST_STATUS_T)response->requeststatus();
  if (response->responseerror_size() > 0){
    response_c->number_errors = response->responseerror_size();
    for (int i=0; i< response_c->number_errors; i++){
      responseError = response->responseerror(i);
      response_c->sessionErrors[i].sessionId = responseError.sessionid();
      response_c->sessionErrors[i].errorStatus = (ADD_SESSION_STATUS_T)responseError.errorstatus();
    }
  } else {
    response_c->number_errors = 0;
  }
}
/** \ingroup utilities
*
* \brief Covert a C SessionRequest_t to a C++ sessionRequest Class instance
*
* \param *request_c   The sessionRequest_t struct to convert
*
* \param *request     The sessionRequest object to create
*
* \return void
*/
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
    request_c->srcPort = (unsigned short)request.sourceport();
    request_c->dstPort = (unsigned short)request.destinationport();
    request_c->proto = (PROTOCOL_ID_T)request.protocolid();
    action = request.action();
    request_c->actType= (ACTION_VALUE_T)action.actiontype();

     if (request_c->ipver == _IPV6){
      char_array.assign(action.actionnexthopv6().begin(), action.actionnexthopv6().end());
      memcpy(request_c->nextHopV6.s6_addr,&char_array[0],16);
    } else {
      request_c->nextHop.s_addr = action.actionnexthop();
    }
 }

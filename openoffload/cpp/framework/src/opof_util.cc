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
* \brief Covert a C SessionRequest_t to a C++ sessionRequest Class instance
*
* \param *request_c   The sessionRequest_t struct to convert
*
* \param *request     The sessionRequest object to create
*
* \return void
*/
void convertSessionRequest2cpp(sessionRequest_t *request_c, SessionRequest *request){
    ActionParameters action;
    std::string s;
    request->set_session_id(request_c->sessId);
    request->set_in_lif(request_c->inlif);
    request->set_out_lif(request_c->outlif);
    request->set_ip_version((IpVersion)request_c->ipver);
    request->set_source_port((unsigned int)request_c->srcPort);
    if (request_c->ipver == _IPV6){
       s.assign(request_c->srcIPV6.s6_addr, request_c->srcIPV6.s6_addr+ 16);
      request->set_source_ipv6(s);
    } else {
      request->set_source_ip(request_c->srcIP.s_addr);
    } 
    if (request_c->ipver == _IPV6){
      s.assign(request_c->dstIPV6.s6_addr, request_c->dstIPV6.s6_addr+ 16);
      request->set_destination_ipv6(s);
    } else {
      request->set_destination_ip(request_c->dstIP.s_addr);
    } 
    request->set_destination_port((unsigned int)request_c->dstPort);
    request->set_protocol_id((ProtocolId)request_c->proto);
    action.set_action_type((ActionType)request_c->actType);
    if (request_c->ipver == _IPV6){
      s.assign(request_c->nextHopV6.s6_addr, request_c->nextHopV6.s6_addr+ 16);
      action.set_action_next_hop_v6(s);
    } else {
      action.set_action_next_hop(request_c->nextHop.s_addr);
    }
    request->mutable_action()->CopyFrom(action);
    request->set_cache_timeout(request_c->cacheTimeout);

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
void convertAddSessionResponse2c(addSessionResponse_t *response_c, AddSessionResponse *response){
  SessionResponseError responseError;
  //response_c->requestStatus = (REQUEST_STATUS_T)response->requeststatus();
  if (response->response_error_size() > 0){
    response_c->number_errors = response->response_error_size();
    for (int i=0; i< response_c->number_errors; i++){
      responseError = response->response_error(i);
      response_c->sessionErrors[i].sessionId = responseError.session_id();
      response_c->sessionErrors[i].errorStatus = responseError.error_status();
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
void convertSessionResponse2c(SessionResponse *responsecpp, sessionResponse_t *responsec){

  responsec->sessionId = responsecpp->session_id();
  responsec->requestStatus = (REQUEST_STATUS_T)responsecpp->request_status();
  responsec->sessionState = (SESSION_STATE_T)responsecpp->session_state();
  responsec->sessionCloseCode = (SESSION_CLOSE_T)responsecpp->session_close_code();
  responsec->inPackets = responsecpp->in_packets();
  responsec->outPackets = responsecpp->out_packets();
  responsec->inBytes = responsecpp->in_bytes();
  responsec->outBytes = responsecpp->out_bytes();
}

void convertSessionResponse2cpp(SessionResponse *responsecpp, sessionResponse_t *responsec){
  responsecpp->set_session_id(responsec->sessionId);
  responsecpp->set_request_status((RequestStatus)responsec->requestStatus);
  responsecpp->set_session_state((SessionState)responsec->sessionState);
  responsecpp->set_session_close_code((SessionCloseCode)responsec->sessionCloseCode);
  responsecpp->set_in_packets(responsec->inPackets);
  responsecpp->set_out_packets(responsec->outPackets);
  responsecpp->set_in_bytes(responsec->inBytes);
  responsecpp->set_out_bytes(responsec->outBytes);
}
void convertSessionRequest2c(SessionRequest request, sessionRequest_t *request_c){
    ActionParameters action;
    std::vector<uint8_t> char_array(16, 0);
    request_c->sessId = request.session_id();
    request_c->inlif = request.in_lif();
    request_c->outlif = request.out_lif();
    request_c->ipver = (IP_VERSION_T)request.ip_version();
    if (request_c->ipver == _IPV6){
      char_array.assign(request.source_ipv6().begin(), request.source_ipv6().end());
      memcpy(request_c->srcIPV6.s6_addr,&char_array[0],16);
    } else {
      request_c->srcIP.s_addr = request.source_ip();
    }
    if (request_c->ipver == _IPV6){
      char_array.assign(request.destination_ipv6().begin(), request.destination_ipv6().end());
       memcpy(request_c->dstIPV6.s6_addr,&char_array[0],16);
    } else {
      request_c->dstIP.s_addr = request.destination_ip();
    }
    request_c->srcPort = (unsigned short)request.source_port();
    request_c->dstPort = (unsigned short)request.destination_port();
    request_c->proto = (PROTOCOL_ID_T)request.protocol_id();
    action = request.action();
    request_c->actType= (ACTION_VALUE_T)action.action_type();
    request_c->cacheTimeout = request.cache_timeout();
     if (request_c->ipver == _IPV6){
      char_array.assign(action.action_next_hop_v6().begin(), action.action_next_hop_v6().end());
      memcpy(request_c->nextHopV6.s6_addr,&char_array[0],16);
    } else {
      request_c->nextHop.s_addr = action.action_next_hop();
    }
 }

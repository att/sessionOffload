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

extern "C" {
#include "opof.h"
#include "opof_error.h"
#include "opof_serverlib.h"
#include "opof_util.h"
}

#include "opof_grpc.h"
#include "opof_session_server.h"

 void convertSessionRequest2cpp(sessionRequest_t *request_c, sessionRequest *request);

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
    request_c->srcPort = request.sourceport();
    request_c->dstPort = request.destinationport();
    request_c->proto = (PROTOCOL_ID_T)request.protocolid();
    action = request.action();
    request_c->actType= (ACTION_VALUE_T)action.actiontype();
    request_c->nextHop= action.actionnexthop();

 }

 
#ifdef RPC

  Status SessionTableImpl::addSession(ServerContext* context, const sessionRequest* request,
                  addSessionResponse* response){
    addSessionResponse_t addResponse_c;
    sessionRequest_t request_c;
    int status;
    actionParameters action;
    int sessId = 1000;
   
    REQUEST_STATUS reqStatus = REQUEST_STATUS::_ACCEPTED;
    request_c.sessId = request->sessionid();
    request_c.inlif = request->inlif();
    request_c.outlif = request->outlif();
    request_c.ipver = (IP_VERSION_T)request->ipversion();
    request_c.srcPort = request->sourceport();
    request_c.dstPort = request->destinationport();
    request_c.proto = (PROTOCOL_ID_T)request->protocolid();
    action = request->action();
    request_c.actType= (ACTION_VALUE_T)action.actiontype();
    request_c.nextHop= action.actionnexthop();
    request.mutable_action()->CopyFrom(action);
#ifdef DEBUG
    std::cout << "In add Session Server " << std::endl;
    std::cout << "offload flow with: " << std::endl;
    std::cout << "session id: " << request->sessionid() << std::endl;
  	std::cout << "inlif: " << request->inlif() << std::endl;
  	std::cout << "outlif: " << request->outlif() << std::endl;  
  	std::cout << "source IP: " << request->sourceip() << std::endl;
  	std::cout << "destination IP: " << request->destinationip() << std::endl;
  	std::cout << "IP version: " << request->ipversion() << std::endl; 
  	std::cout << "source port: " << request->sourceport() << std::endl;
  	std::cout << "destination port: " << request->destinationport() << std::endl;    
  	std::cout << "protocol id: " << request->protocolid() << std::endl;   
  	std::cout << "nexthop is: " << action.actionnexthop() << std::endl;
  	std::cout << "actionType: " << action.actiontype() << std::endl;
#endif
    /*
    * Call C function (User defined)
    */
    status = opof_add_session_server(&request_c, &addResponse_c);
    if (status != 0){
      return grpc::Status(grpc::StatusCode::CANCELLED, "call to opof_add_session_server failed");
    }
    //response->set_sessionid(addResponse_c.sessionId);
    response->set_requeststatus((REQUEST_STATUS)addResponse_c.requestStatus);
  // Add an IPv4 session
    return Status::OK;
  }

#endif

  Status SessionTableImpl::addSession(ServerContext* context, ServerReader<sessionRequest>* reader, addSessionResponse* response) {
    int status;
    addSessionResponse_t addResponse_c;
    sessionRequest_t request_c;
    sessionRequest request;
    while(reader->Read(&request)){
      convertSessionRequest2c(request, &request_c);
      status = opof_add_session_server(&request_c, &addResponse_c);     
    }
    return Status::OK;
  }


  Status SessionTableImpl::getSession(ServerContext* context, const sessionId* sid,
                  sessionResponse* response) {
        sessionResponse_t response_c;
        int status;

        status = opof_get_session_server(sid->sessionid(), &response_c);
        convertSessionResponse2cpp(response, &response_c);

  // Add an IPv4 session
    return Status::OK;
  }

  Status SessionTableImpl::deleteSession(ServerContext* context, const sessionId* sid,
                  sessionResponse* response) {
      int status;
      sessionResponse_t response_c;
        //std::cout << "sessionID to delete is: " << sid->sessionid() << std::endl;
        status = opof_del_session_server(sid->sessionid(), &response_c);
        convertSessionResponse2cpp(response, &response_c);
        

  // Add an IPv4 session
    return Status::OK;
  }

  Status SessionTableImpl::getAllSessions(ServerContext* context, const statisticsRequestArgs* request, ServerWriter<sessionResponse>* writer) {
  
  sessionResponse response;

  response.set_sessionid(1001);
  response.set_sessionstate(SESSION_STATE::_CLOSED);
  response.set_inpackets(2000);
  response.set_outpackets(2200);
  response.set_requeststatus(REQUEST_STATUS::_ACCEPTED);

  writer->Write(response);
  return Status::OK;
 }

Status SessionTableImpl::getClosedSessions(ServerContext* context, const statisticsRequestArgs* request, ServerWriter<sessionResponse>* writer) {
  
  sessionResponse response;
  sessionResponse_t **closedSessions= NULL;
  sessionResponse_t *closedResponse;
  int nresponses = request->pagesize();
  closedSessions =createSessionResponse(nresponses);
  if (closedSessions == NULL){
    return Status::OK;
  }
  for (int i=0; i < nresponses; i++){
    closedResponse = closedSessions[i];
    response.set_sessionid(closedResponse->sessionId);
    response.set_sessionstate((SESSION_STATE)closedResponse->sessionState);
    response.set_inpackets(closedResponse->inPackets);
    response.set_outpackets(closedResponse->outPackets);
    response.set_inbytes(closedResponse->inBytes);
    response.set_outbytes(closedResponse->outBytes);
    response.set_sessionclosecode((SESSION_CLOSE_CODE)closedResponse->sessionCloseCode);
    response.set_requeststatus(REQUEST_STATUS::_ACCEPTED);
    writer->Write(response);
    free(closedResponse);
  }
  free(closedSessions);

  return Status::OK;
 }
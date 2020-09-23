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
* \ingroup serverlibrary
*
* \brief gRPC Server implementation
*
*/

extern "C" {
#include "opof.h"
#include "opof_error.h"
#include "opof_serverlib.h"
}

#include "opof_util.h"
#include "opof_grpc.h"
#include "opof_session_server.h"

/**
* \brief addSession
*
* \param context
* \param reader
* \param response
*/
Status SessionTableImpl::addSession(ServerContext* context, ServerReader<sessionRequest>* reader, addSessionResponse* response) {
    int status;
    unsigned long errorCode;
    unsigned long error = 1;
    int index=0;
    ADD_SESSION_STATUS reqStatus;
    addSessionResponse_t addResponse_c;
    sessionRequest_t request_c;
    sessionRequest request;
    while(reader->Read(&request)){
      convertSessionRequest2c(request, &request_c);
      status = opof_add_session_server(&request_c, &addResponse_c);
      if (status == FAILURE){
        errorCode = errorCode ^ (error << index);
      }
      index++;
    }
    if (errorCode > 0){
      reqStatus = ADD_SESSION_STATUS::_SESSION_REJECTED;
    }else {
      reqStatus = ADD_SESSION_STATUS::_SESSION_ACCEPTED;
    }
    response->set_requeststatus(reqStatus);
    response->set_errorstatus(errorCode);
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
  statisticsRequestArgs_t request_c;
  int sessionCount = 0;
  int nresponses = request->pagesize();
  request_c.pageSize = nresponses;
  closedSessions = opof_get_closed_sessions_server(&request_c, &sessionCount);
  if (closedSessions == NULL){
    response.set_requeststatus(REQUEST_STATUS::_NO_CLOSED_SESSIONS);
    return Status::OK;
  }
  for (int i=0; i < sessionCount; i++){
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
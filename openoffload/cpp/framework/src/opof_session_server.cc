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

/** \ingroup serverlibrary
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
  sessionResponseError *errorMessage;
  sessionRequest_t request_c;
  sessionRequest request;
  while(reader->Read(&request)){
    convertSessionRequest2c(request, &request_c);
    status = opof_add_session_server(&request_c, &addResponse_c);
    if (status != _OK){
      errorMessage = response->add_responseerror();
      errorMessage->set_sessionid(request.sessionid());
      errorMessage->set_errorstatus(status);
    }
    index++;
  }
  response->set_requeststatus(reqStatus);
  return Status::OK;
}

/** \ingroup serverlibrary
* \brief getSession
*
* \param context
* \param reader
* \param response
*/
Status SessionTableImpl::getSession(ServerContext* context, const sessionId* sid,
  sessionResponse* response) {
  sessionResponse_t response_c;
  int status;
  uint64_t session;
  session = sid->sessionid();
  status = opof_get_session_server(session, &response_c);
  if (status == _OK){
    convertSessionResponse2cpp(response, &response_c);
    return Status::OK;
  } else if (status == _NOT_FOUND){
    return Status(grpc::StatusCode::NOT_FOUND, "Get Session Not Found");
  } else {
    return Status(grpc::StatusCode::UNKNOWN, "Internal Open Offload Server Error");
  }
}

/** \ingroup serverlibrary
* \brief deleteSession
*
* \param context
* \param reader
* \param response
*/
Status SessionTableImpl::deleteSession(ServerContext* context, const sessionId* sid,
  sessionResponse* response) {
  int status;
  sessionResponse_t response_c;
  status = opof_del_session_server(sid->sessionid(), &response_c);
  if (status == _OK){
    convertSessionResponse2cpp(response, &response_c);
    return Status::OK;
  } else if (status == _NOT_FOUND){
    return Status(grpc::StatusCode::NOT_FOUND, "Delete Session Not Found");
  } else {
    return Status(grpc::StatusCode::UNKNOWN, "Internal Open Offload Server Error");
  }
}

/** \ingroup serverlibrary
* \brief getAllSessions
*
* \param context
* \param reader
* \param response
*/
Status SessionTableImpl::getAllSessions(ServerContext* context, const statisticsRequestArgs* request, sessionResponseArray *responses) {
  
  Status status;
  sessionResponse_t **allSessions= NULL;
  sessionResponse_t *closedResponse;
  sessionResponse *response;
  int sessionCount;
  int pageCount = 0;
  
  int nresponses = BUFFER_MAX;
  uint64_t start_session;
  start_session = request->startsession();
  
  allSessions = (sessionResponse_t **)malloc(nresponses * sizeof(sessionResponse_t *));
  for (int i = 0; i < nresponses; i++){
    allSessions[i] = (sessionResponse_t *)malloc(sizeof(sessionResponse_t));
  }
  
  sessionCount = opof_get_all_sessions_server(nresponses, &start_session, pageCount, allSessions);

  responses->set_nextkey(start_session);
  
  if (sessionCount > 0){
    pageCount++;
    for (int i=0; i < sessionCount; i++){
      closedResponse = allSessions[i];
      response = responses->mutable_responsearray(i);
      response->set_sessionid(closedResponse->sessionId);
      response->set_sessionstate((SESSION_STATE)closedResponse->sessionState);
      response->set_inpackets(closedResponse->inPackets);
      response->set_outpackets(closedResponse->outPackets);
      response->set_inbytes(closedResponse->inBytes);
      response->set_outbytes(closedResponse->outBytes);
      response->set_sessionclosecode((SESSION_CLOSE_CODE)closedResponse->sessionCloseCode);
      response->set_requeststatus((REQUEST_STATUS)closedResponse->requestStatus);
    }
  }
  for (int i=0; i <nresponses; i++){
    free(allSessions[i]);
  }
  free(allSessions);
  return Status::OK;
}

/** \ingroup serverlibrary
* \brief getClosedSessions
*
* \param context
* \param reader
* \param response
*/
Status SessionTableImpl::getClosedSessions(ServerContext* context, const statisticsRequestArgs* request, ServerWriter<sessionResponse>* writer) {
  
  sessionResponse response;
  sessionResponse_t closedResponse;
  statisticsRequestArgs_t request_c;
  int sessionCount = 0;
  //Status status;
  int nresponses = request->pagesize();
  request_c.pageSize = nresponses;
  sessionResponse_t closedSessions[BUFFER_MAX];

  sessionCount = opof_get_closed_sessions_server(&request_c, closedSessions);
  if (sessionCount == 0){
    return Status(grpc::StatusCode::NOT_FOUND,"No Closed Sessions");
  }
  for (int i=0; i < sessionCount; i++){
    closedResponse = closedSessions[i];
    response.set_sessionid(closedResponse.sessionId);
    response.set_sessionstate((SESSION_STATE)closedResponse.sessionState);
    response.set_inpackets(closedResponse.inPackets);
    response.set_outpackets(closedResponse.outPackets);
    response.set_inbytes(closedResponse.inBytes);
    response.set_outbytes(closedResponse.outBytes);
    response.set_sessionclosecode((SESSION_CLOSE_CODE)closedResponse.sessionCloseCode);
    response.set_requeststatus(REQUEST_STATUS::_ACCEPTED);
    writer->Write(response);
    
  }
  
  return Status::OK;
}
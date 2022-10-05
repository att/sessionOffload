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
Status SessionTableImpl::AddSession(ServerContext* context, ServerReader<SessionRequest>* reader, AddSessionResponse* response) {
  int status;
  //int index=0;
  //ADD_SESSION_STATUS reqStatus = ADD_SESSION_STATUS::_SESSION_ACCEPTED;
  addSessionResponse_t addResponse_c;
  SessionResponseError *errorMessage;
  sessionRequest_t request_c;
  SessionRequest request;
  response->clear_response_error();
  //std::cout << "response size: " << response->responseerror_size() << std::endl;
  //if (context->IsCancelled()) {
  //  return Status(StatusCode::CANCELLED, "Deadline exceeded or Client cancelled, abandoning.");
  //}
#ifdef TESTS
  sleep(1);
#endif
  while(reader->Read(&request)){
    convertSessionRequest2c(request, &request_c);
    if (context->IsCancelled()) {
      return Status(StatusCode::CANCELLED, "Deadline exceeded or Client cancelled, abandoning.");
    }
    status = opof_add_session_server(&request_c, &addResponse_c);
    if (status != _OK){
      errorMessage = response->add_response_error();
      errorMessage->set_session_id(request.session_id());
      errorMessage->set_error_status(status);
    }
    //index++;
  }
  //response->set_requeststatus(reqStatus);
  return Status::OK;
}

/** \ingroup serverlibrary
* \brief getSession
*
* \param context
* \param reader
* \param response
*/
Status SessionTableImpl::GetSession(ServerContext* context, const SessionId* sid,
  SessionResponse* response) {
  sessionResponse_t response_c;
  int status;
  uint64_t session;
  if (context->IsCancelled()) {
      return Status(StatusCode::CANCELLED, "Deadline exceeded or Client cancelled, abandoning.");
  }
  session = sid->session_id();
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
Status SessionTableImpl::DeleteSession(ServerContext* context, const SessionId* sid,
  SessionResponse* response) {
  int status;
  sessionResponse_t response_c;
  if (context->IsCancelled()) {
    return Status(StatusCode::CANCELLED, "Deadline exceeded or Client cancelled, abandoning.");
  }
  status = opof_del_session_server(sid->session_id(), &response_c);
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
Status SessionTableImpl::GetAllSessions(ServerContext* context, const SessionRequestArgs* request, SessionResponses *responses) {
  
  Status status;
  sessionResponse_t **allSessions= NULL;
  sessionResponse_t *closedResponse;
  SessionResponse *response;
  int sessionCount;
  int pageCount = 0;
  
  int nresponses = BUFFER_MAX;
  uint64_t start_session;
  if (context->IsCancelled()) {
      return Status(StatusCode::CANCELLED, "Deadline exceeded or Client cancelled, abandoning.");
  }
  start_session = request->start_session();
  
  allSessions = (sessionResponse_t **)malloc(nresponses * sizeof(sessionResponse_t *));
  for (int i = 0; i < nresponses; i++){
    allSessions[i] = (sessionResponse_t *)malloc(sizeof(sessionResponse_t));
  }
  
  sessionCount = opof_get_all_sessions_server(nresponses, &start_session, pageCount, allSessions);

  responses->set_next_key(start_session);
  
  if (sessionCount > 0){
    pageCount++;
    for (int i=0; i < sessionCount; i++){
      closedResponse = allSessions[i];
      response = responses->add_session_info();
      response->set_session_id(closedResponse->sessionId);
      response->set_session_state((SessionState)closedResponse->sessionState);
      response->set_in_packets(closedResponse->inPackets);
      response->set_out_packets(closedResponse->outPackets);
      response->set_in_bytes(closedResponse->inBytes);
      response->set_out_bytes(closedResponse->outBytes);
      response->set_session_close_code((SessionCloseCode)closedResponse->sessionCloseCode);
      response->set_request_status((RequestStatus)closedResponse->requestStatus);
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
Status SessionTableImpl::GetClosedSessions(ServerContext* context, const SessionRequestArgs* request, ServerWriter<SessionResponse>* writer) {
  
  SessionResponse response;
  sessionResponse_t closedResponse;
  statisticsRequestArgs_t request_c;
  int sessionCount = 0;
  //Status status;
  if (context->IsCancelled()) {
      return Status(StatusCode::CANCELLED, "Deadline exceeded or Client cancelled, abandoning.");
  }
  int nresponses = request->page_size();
  request_c.pageSize = nresponses;
  sessionResponse_t closedSessions[BUFFER_MAX];
#ifdef TESTS
  sleep(1);
#endif
  sessionCount = opof_get_closed_sessions_server(&request_c, closedSessions);
  if (sessionCount == 0){
    return Status(grpc::StatusCode::NOT_FOUND,"No Closed Sessions");
  }
  for (int i=0; i < sessionCount; i++){
    closedResponse = closedSessions[i];
    response.set_session_id(closedResponse.sessionId);
    response.set_session_state((SessionState)closedResponse.sessionState);
    response.set_in_packets(closedResponse.inPackets);
    response.set_out_packets(closedResponse.outPackets);
    response.set_in_bytes(closedResponse.inBytes);
    response.set_out_bytes(closedResponse.outBytes);
    response.set_session_close_code((SessionCloseCode)closedResponse.sessionCloseCode);
    response.set_request_status(RequestStatus::_ACCEPTED);
    writer->Write(response);
    
  }
  
  return Status::OK;
}

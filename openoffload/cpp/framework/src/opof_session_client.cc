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
}
#include "opof_util.h"
#include "opof_grpc.h"
#include "opof_session_client.h"



Status SessionTableClient::addSessionClient(int size, sessionRequest_t **s, addSessionResponse_t *resp){

sessionRequest_t *request_c;
sessionRequest request;
addSessionResponse response;
ClientContext context;
Status status;
std::unique_ptr<ClientWriter <sessionRequest> > writer(
        stub_->addSession(&context, &response));

for (int i=0; i< size; i++){
  request_c = s[i];
  convertSessionRequest2cpp(request_c, &request);
  writer->Write(request);
  free(request_c);
}

free(s);
writer->WritesDone();
status = writer->Finish();
return status;
}
// getSession
std::string SessionTableClient::getSessionClient(int sessionid,sessionResponse_t *resp){

  sessionId sid;
  sessionResponse response;
  sid.set_sessionid(sessionid);
  ClientContext context;

  Status status = stub_->getSession(&context, sid, &response);
  convertSessionResponse2c(&response, resp);
  if (status.ok()) {
    return "Success";
  } else {
    std::cout << "RPC getSession failed with error code: " << status.error_code() << ": " << status.error_message()
              << std::endl;
    return "RPC failed";
  }
  //TODO: print timestamps
  //std::cout << "session starttime is: " << response.starttime() << std::endl;
  //std::cout << "session endtime is: " << response.endtime() << std::endl;
}

// deleteSession
std::string SessionTableClient::deleteSessionClient(int sessionid,sessionResponse_t *resp){

sessionId sid;
sessionResponse response;
sid.set_sessionid(sessionid);
ClientContext context;
Status status = stub_->deleteSession(&context, sid, &response);

if (status.ok()) {
  resp->sessionId = response.sessionid();
  resp->requestStatus = (REQUEST_STATUS_T)response.requeststatus();
  resp->sessionState = (SESSION_STATE_T)response.sessionstate();
  resp->inPackets = response.inpackets();
  resp->outPackets = response.outpackets();
  return "Success";
} else {
    std::cout << "RPC Delete Session Failed: " << status.error_code() << ": " << status.error_message()
              << std::endl;
    return "RPC failed";
  }

}

unsigned long SessionTableClient::getClosedSessions(statisticsRequestArgs_t *args){
  unsigned long sessionCount = 0;
  sessionResponse response;
  statisticsRequestArgs request;
  ClientContext context;
  sessionResponse_t responsec;
  request.set_pagesize(args->pageSize);

  std::unique_ptr<ClientReader <sessionResponse> > reader(
        stub_->getClosedSessions(&context, request));
    while (reader->Read(&response)) {
      if (response.requeststatus() == REQUEST_STATUS::_NO_CLOSED_SESSIONS){
        return sessionCount;
      }
      convertSessionResponse2c(&response, &responsec);
      //display_session_response(&responsec);
      sessionCount++;
    }

  Status status = reader->Finish();
  if (status.ok()) {
    //std::cout << "getClosedSessions rpc succeeded." << std::endl;
  } else {
    std::cout << "getClosedSessions rpc failed." << std::endl;
  }
  return sessionCount;
}

void SessionTableClient::getAllSessions(){
  sessionResponse response;
  statisticsRequestArgs request;
  ClientContext context;
  sessionResponse_t responsec;

  std::cout << "Getting all Sessions" << std::endl;

  std::unique_ptr<ClientReader <sessionResponse> > reader(
        stub_->getAllSessions(&context, request));
    while (reader->Read(&response)) {
      std::cout << "Found feature called "  << std::endl;
      std::cout << "Session ID: " << response.sessionid() << std::endl;
      convertSessionResponse2c(&response, &responsec);
               // << feature.name() << " at "
               // << feature.location().latitude()/kCoordFactor_ << ", "
               // << feature.location().longitude()/kCoordFactor_ << std::endl;
    }

  Status status = reader->Finish();
  if (status.ok()) {
    std::cout << "getAllSessions rpc succeeded." << std::endl;
  } else {
    std::cout << "getAllSessions rpc failed no closed sessions." << std::endl;
  }
}
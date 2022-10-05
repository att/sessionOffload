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
#ifdef DEBUG
#include "opof_test_util.h"
#endif
  //
  // Set default deadline on API calls to 100 milli seconds
  //
  unsigned int g_deadline = 100;
  /**  \ingroup clientcinterface
  * \brief gets the deadline value in milli-seconds
  *
  * \param void
  * \return value of global value deadline
  *
  */
  unsigned int opof_get_deadline(void){
    return g_deadline;
  }
 /**  \ingroup clientcinterface
  * \brief sets the deadline value in milli-seconds
  *
  * \param int
  * \return value of global value deadline
  *
  */
  unsigned int opof_set_deadline(int deadline){
    g_deadline = deadline;
    return g_deadline;
  }
} // extern C

#include "opof_util.h"
#include "opof_grpc.h"
#include "opof_session_client.h"


/**  \ingroup clientlibrary
* \brief
*
* \param size
* \param sessionRequest_t
* \param addSeesionResponse_t
*
*/
int SessionTableClient::addSessionClient(int size, sessionRequest_t **s, addSessionResponse_t *resp){

  sessionRequest_t *request_c;
  SessionRequest request;
  AddSessionResponse response;
  ClientContext context;
  Status status;
  std::chrono::system_clock::time_point deadline = std::chrono::system_clock::now() + std::chrono::milliseconds(opof_get_deadline());
  context.set_deadline(deadline);
  #ifdef DEBUG
  std::cout << "Deadline set for add session: " << opof_get_deadline() << " milli seconds" << endl;
  #endif
  std::unique_ptr<ClientWriter <SessionRequest> > writer(
          stub_->AddSession(&context, &response));

  for (int i=0; i< size; i++){
    request_c = s[i];
  #ifdef DEBUG
    display_session_request(request_c, "addSessionClient");
  #endif
    convertSessionRequest2cpp(request_c, &request);
    writer->Write(request);
  }
  writer->WritesDone();
  status = writer->Finish();
  convertAddSessionResponse2c(resp,&response);
  //std::cout << "Status code: " <<  static_cast<int>(status.error_code()) << endl;
  return static_cast<int>(status.error_code());
}
/**  \ingroup clientlibrary
* \brief getSessionClient
*
* \param size
* \param sessionRequest_t
* \param addSeesionResponse_t
*
*/
int SessionTableClient::getSessionClient(int sessionid,sessionResponse_t *resp){

  SessionId sid;
  SessionResponse response;
  sid.set_session_id(sessionid);
  ClientContext context;
  std::chrono::system_clock::time_point deadline = std::chrono::system_clock::now() + std::chrono::milliseconds(opof_get_deadline());
  context.set_deadline(deadline);
  #ifdef DEBUG
  std::cout << "Deadline set for get session: " << opof_get_deadline() << " milli seconds" << endl;
  #endif
  Status status = stub_->GetSession(&context, sid, &response);
  convertSessionResponse2c(&response, resp);
  return static_cast<int>(status.error_code());
}

/**  \ingroup clientlibrary
* \brief deleteSessionClient
*
* \param size
* \param sessionRequest_t
* \param addSeesionResponse_t
*
*/
int SessionTableClient::deleteSessionClient(int sessionid,sessionResponse_t *resp){

  SessionId sid;
  SessionResponse response;
  sid.set_session_id(sessionid);
  ClientContext context;
  std::chrono::system_clock::time_point deadline = std::chrono::system_clock::now() + std::chrono::milliseconds(opof_get_deadline());
  context.set_deadline(deadline);
  #ifdef DEBUG
  std::cout << "Deadline set for delete session: " << opof_get_deadline() << " milli seconds" << endl;
  #endif
  Status status = stub_->DeleteSession(&context, sid, &response);

  convertSessionResponse2c(&response, resp);
  #ifdef DEBUG
    display_session_response(resp, "delSessionClient");
  #endif

  return static_cast<int>(status.error_code());
}
/**  \ingroup clientlibrary
* \brief getClosedSessions
*
* \param size
* \param sessionRequest_t
* \param addSeesionResponse_t
*
*/
int SessionTableClient::getClosedSessions(statisticsRequestArgs_t *args, sessionResponse_t responses[], unsigned long *sessionCount){
  SessionResponse response;
  SessionRequestArgs request;
  ClientContext context;
  request.set_page_size(args->pageSize);
  std::chrono::system_clock::time_point deadline = std::chrono::system_clock::now() + std::chrono::milliseconds(opof_get_deadline());
  context.set_deadline(deadline);
  #ifdef DEBUG
  std::cout << "Deadline set for get closed sessions: " << opof_get_deadline() << " milli seconds" << endl;
  #endif
  *sessionCount = 0;
  std::unique_ptr<ClientReader <SessionResponse> > reader(
        stub_->GetClosedSessions(&context, request));
  while (reader->Read(&response)) {
    convertSessionResponse2c(&response, &responses[*sessionCount]);
    (*sessionCount)++;
  }
  Status status = reader->Finish();
  return static_cast<int>(status.error_code());
}
/**  \ingroup clientlibrary
* \brief getAllSessions
*
* \param size
* \param sessionRequest_t
* \param addSeesionResponse_t
*
*/
int  SessionTableClient::getAllSessions(int pageSize, uint64_t *session_start_id, uint64_t *session_count, sessionResponse_t responses[], unsigned long *sessionCount){
  
  Status status;
  SessionResponses response;
  SessionRequestArgs request;
  ClientContext context;
 
  int array_size;
  std::chrono::system_clock::time_point deadline = std::chrono::system_clock::now() + std::chrono::milliseconds(opof_get_deadline());
  context.set_deadline(deadline);
  #ifdef DEBUG
  std::cout << "Deadline set for get all sessions: " << opof_get_deadline() << " milli seconds" << endl;
  #endif
  request.set_page_size(pageSize);
  request.set_start_session(*session_start_id);
  
  status = stub_->GetAllSessions(&context, request, &response);
  array_size = response.session_info_size();
  *session_start_id = response.next_key();
 

  for (int i = 0; i < array_size; i++ ){
    convertSessionResponse2c(response.mutable_session_info(i), &responses[i]);
  }

  *session_count = array_size;
  
  return static_cast<int>(status.error_code());
}

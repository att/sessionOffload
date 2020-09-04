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

#include "opof_test.h"
#include "opof_util.h"
#include "opof_stats_server.h"

using grpc::ServerWriter;
using openoffload::v1alpha2::REQUEST_STATUS;
using openoffload::v1alpha2::SESSION_STATE;




 Status SessionStatsImpl::getAllSessions(ServerContext* context, const statisticsRequestArgs* request, ServerWriter<sessionResponse>* writer) {
  
  sessionResponse response;

  response.set_sessionid(1001);
  response.set_sessionstate(SESSION_STATE::_CLOSED);
  response.set_inpackets(2000);
  response.set_outpackets(2200);
  response.set_requeststatus(REQUEST_STATUS::_ACCEPTED);

  writer->Write(response);
  return Status::OK;
 }

Status SessionStatsImpl::getClosedSessions(ServerContext* context, const statisticsRequestArgs* request, ServerWriter<sessionResponse>* writer) {
  
  sessionResponse response;

  response.set_sessionid(1001);
  response.set_sessionstate(SESSION_STATE::_CLOSED);
  response.set_inpackets(2000);
  response.set_outpackets(2200);
  response.set_requeststatus(REQUEST_STATUS::_ACCEPTED);

  writer->Write(response);
  return Status::OK;
 }

/*

  Status SessionTableImpl::addSession(ServerContext* context, const sessionRequest* request,
                  addSessionResponse* response) {
    actionType action;
    int sessId = 1000;
    REQUEST_STATUS reqStatus = REQUEST_STATUS::_ACCEPTED;
    std::cout << "offload flow with: " << std::endl;
    std::cout << "inlif: " << request->inlif() << std::endl;
    std::cout << "outlif: " << request->outlif() << std::endl;
    std::cout << "source IP: " << request->sourceip() << std::endl;
    std::cout << "destination IP: " << request->destinationip() << std::endl;
    std::cout << "IP version: " << request->ipversion() << std::endl;
    std::cout << "source port: " << request->sourceport() << std::endl;
    std::cout << "destination port: " << request->destinationport() << std::endl;
    std::cout << "protocol id: " << request->protocolid() << std::endl;
    action = request->action();
    std::cout << "nexthop is: " << action.actionnexthop() << std::endl;
    std::cout << "actionType: " << action.actionvalue() << std::endl;
    response->set_sessionid(sessId);
    response->set_requeststatus(reqStatus);
  // Add an IPv4 session
    return Status::OK;
  }

  Status SessionTableImpl::getSession(ServerContext* context, const sessionId* sid,
                  sessionResponse* response) {

        std::cout << "sessionID queried is: " << sid->sessionid() << std::endl;
        response->set_sessionid(sid->sessionid());
        response->set_sessionstate(SESSION_STATE::_ESTABLISHED);
        response->set_requeststatus(REQUEST_STATUS::_ACCEPTED);
        response->set_inpackets(1000);
        response->set_outpackets(20000);

  // Add an IPv4 session
    return Status::OK;
  }

  Status SessionTableImpl::deleteSession(ServerContext* context, const sessionId* sid,
                  sessionResponse* response)  {

        std::cout << "sessionID to delete is: " << sid->sessionid() << std::endl;
        response->set_sessionid(sid->sessionid());
        response->set_sessionstate(SESSION_STATE::_CLOSING_1);
        response->set_requeststatus(REQUEST_STATUS::_ACCEPTED);
        response->set_inpackets(2000);
        response->set_outpackets(40000);

  // Add an IPv4 session
    return Status::OK;
  }

*/
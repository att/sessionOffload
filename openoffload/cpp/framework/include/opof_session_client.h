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

#ifndef __OPOF_SESSION_CLIENT_H
#define __OPOF_SESSION_CLIENT_H

/**
* \defgroup clientlibrary  C++ Client Interfaces
*
* \brief Internal C++ Client Interfaces called by the external C Interfaces
*
*/

extern "C" {
#include "opof.h"
#include "opof_error.h"
}
#include "opof_grpc.h"

class SessionTableClient {
public: 
    SessionTableClient(std::shared_ptr<Channel> channel)
    : stub_(SessionTable::NewStub(channel)) {};
    int addSessionClient(int size, sessionRequest_t **s, addSessionResponse_t *resp);
    int getSessionClient(int session, sessionResponse_t *resp);
    int deleteSessionClient(int session, sessionResponse_t *resp);
    int getAllSessions(int pageSize, uint64_t *start_session, uint64_t *sessions, sessionResponse_t responses[],unsigned long *sessionCount);
    int getClosedSessions(statisticsRequestArgs_t *args, sessionResponse_t responses[], unsigned long *sessionCount);
private:
    std::unique_ptr<SessionTable::Stub> stub_;
};


#endif
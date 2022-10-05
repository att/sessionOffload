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

#ifndef __OPOF_SESSION_SERVER_H
#define __OPOF_SESSION_SERVER_H

/**
* \defgroup serverlibrary  C++ Server Interfaces
*
* \brief Internal C++ Server Interfaces called by the external C Interfaces
*
*/
extern "C" {
#include "opof.h"
}
#include "opof_grpc.h"


class SessionTableImpl final : public SessionTable::Service {
public:  
    Status AddSession(ServerContext* context, ServerReader<SessionRequest>* reader, AddSessionResponse* response) override;
    Status GetSession(ServerContext* context, const SessionId* sid, SessionResponse* response) override;
    Status DeleteSession(ServerContext* context, const SessionId* sid, SessionResponse* response) override;
    Status GetAllSessions(ServerContext* context, const SessionRequestArgs* request, SessionResponses *responseArray) override;
    Status GetClosedSessions(ServerContext* context,  const SessionRequestArgs* response,ServerWriter<SessionResponse>* writer) override;
};


#endif

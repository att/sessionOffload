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

extern "C" {
#include "opof.h"
#include "opof_error.h"
}
#include "opof_grpc.h"

class SessionTableClient {
public: 
    SessionTableClient(std::shared_ptr<Channel> channel)
    : stub_(SessionTable::NewStub(channel)) {};
    Status addSessionClient(int size, sessionRequest_t **s, addSessionResponse_t *resp);
    std::string getSessionClient(int session, sessionResponse_t *resp);
    std::string deleteSessionClient(int session, sessionResponse_t *resp);
    void getAllSessions();
    unsigned long getClosedSessions(statisticsRequestArgs_t *args);
private:
    std::unique_ptr<SessionTable::Stub> stub_;
};


#endif
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




#include "opof_test.h"

/*
typedef struct sessionTuple {
    long sessId;
    uint32_t inlif;
    uint32_t outlif;
    std::string srcIP;
    std::string dstIP;
    uint32_t srcPort;
    uint32_t dstPort;
    PROTOCOL_ID proto;
    IP_VERSION ipver;
    ACTION_VALUE actType;
    std::string nextHop;
} sessionTuple_t;
*/
class SessionTableClient {
public: 
    SessionTableClient(std::shared_ptr<Channel> channel)
    : stub_(SessionTable::NewStub(channel)) {};
    std::string addSession(sessionTuple_t s);
    std::string getSession(sessionTuple_t s);
    std::string deleteSession(sessionTuple_t s);
private:
    std::unique_ptr<SessionTable::Stub> stub_;
};


#endif //__OPOF_SESSION_CLIENT_H

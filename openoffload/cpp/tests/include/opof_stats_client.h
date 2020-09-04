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

#ifndef __OPOF_STATS_CLIENT_H
#define __OPOF_STATS_CLIENT_H

#include "opof_test.h"

class SessionStatsClient {
public: 
    SessionStatsClient(std::shared_ptr<Channel> channel)
    : stub_(SessionStatisticsTable::NewStub(channel)) {};
    void getAllSessions();
    void getClosedSessions();
private:
    std::unique_ptr<SessionStatisticsTable::Stub> stub_;
};


#endif //__OPOF_STATS_CLIENT_H


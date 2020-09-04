// Copyright (C) 2020 Palo Alto Networks Intellectual Property. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
#ifndef OPOF_SERVERLIB_H
#define OPOF_SERVERLIB_H

#include "opof.h"
#include "opof_error.h"



//sessionTable_t * opof_create_server(const char * host, unsigned int port, const char *public_key);
//
 int opof_add_session_server(sessionRequest_t *parameters, addSessionResponse_t *response);
 int opof_get_session_server(unsigned long sessionId, sessionResponse_t *response);
 int opof_del_session_server(unsigned long sessionId, sessionResponse_t *response);
 sessionResponse_t *getClosedSessionsFromHash();


#endif
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

#ifndef OPOF_CLIENTLIB_H
#define OPOF_CLIENTLIB_H
#ifdef __cplusplus
extern "C" {
#endif

//extern "C" {
#include "opof.h"
//
//#include "opof_sessiontable.h"
struct sessionTable;
typedef struct sessionTable sessionTable_t;

sessionTable_t * opof_create_sessionTable(const char * host, unsigned int port, const char *public_key);
void opof_delete_sessionTable( sessionTable_t *session);
int opof_add_session(int size, sessionTable_t *sessionHandle,  sessionRequest_t **parameters, addSessionResponse_t *resp);
int opof_del_session(sessionTable_t *sessionHandle,  unsigned long  sessionId, sessionResponse_t *resp);
int opof_get_session(sessionTable_t *sessionHandle,  unsigned long  sessionId , sessionResponse_t *resp);
void opof_get_all_sessions(sessionTable_t *sessionHandle);
void opof_get_closed_sessions(streamArgs_t *args);
//void opof_get_closed_sessions(sessionTable_t *sessionHandle, unsigned int size);
#ifdef __cplusplus
}
#endif
#endif
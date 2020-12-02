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

/** \defgroup clientcinterface C Client Interface
*
* \brief External C Client Interfaces called user applications
*
* These are the functions a C developer needs to use to access teh underlying 
* gRPC C++ implementation. The starting point is opof_create_sessionTable. This
* function calls the C++ constuctor for the sessionTable class as returns a hendle
* to the constructor as a void pointer. The otehr methods are wrapped with
* C functions that take the handle as an input.
*
* There are also a set of utility functiosn that convert C structs to C++ Classes and vice-versa.
* However the C developer only needs to work with the C structs defined in opof.h.
*/
#ifndef OPOF_CLIENTLIB_H
#define OPOF_CLIENTLIB_H
#ifdef __cplusplus
extern "C" {
#endif

#include "opof.h"



/** /ingroup clientcinterface
*
* Function prototypes for C Client Library
*
*/
struct sessionTable;
typedef struct sessionTable sessionTable_t;

sessionTable_t * opof_create_sessionTable(const char * host, unsigned int port, const char *public_key);
void opof_delete_sessionTable( sessionTable_t *session);
int opof_add_session(int size, sessionTable_t *sessionHandle,  sessionRequest_t **parameters, addSessionResponse_t *resp);
int opof_del_session(sessionTable_t *sessionHandle,  unsigned long  sessionId, sessionResponse_t *resp);
int opof_get_session(sessionTable_t *sessionHandle,  unsigned long  sessionId , sessionResponse_t *resp);
int opof_get_all_sessions(sessionTable_t *sessionHandle, uint64_t *sessionStart,int pageSize, sessionResponse_t responses[], unsigned long *sessionCount);
int opof_get_closed_sessions(streamArgs_t *args,sessionResponse_t responses[], unsigned long *sessionCount);
#ifdef __cplusplus
}
#endif
#endif
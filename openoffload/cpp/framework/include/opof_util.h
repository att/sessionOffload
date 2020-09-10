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
#ifndef OPOF_UTIL_H
#define OPOF_UTIL_H
#ifdef __cplusplus
extern "C" {
#endif
#include <arpa/inet.h>
#include <sys/types.h>          
#include <sys/socket.h>

#include "opof.h"
#include "opof_error.h"

#ifdef __cplusplus
}
#endif
#include "opof_grpc.h"

int get_key(const char *filename, char *key);
//int create_address(char *address, unsigned short port);

void display_session_response(sessionResponse_t *response);
void display_session_request(sessionRequest_t *request);
void convertSessionRequest2cpp(sessionRequest_t *request_c, sessionRequest *request);
void convertSessionResponse2c(sessionResponse *responsecpp, sessionResponse_t *responsec);
void convertSessionResponse2cpp(sessionResponse *responsecpp, sessionResponse_t *responsec);
void convertSessionRequest2c(sessionRequest request, sessionRequest_t *request_c);
  
#endif
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
#ifndef OPOF_TEST_UTIL_H
#define OPOF_TEST_UTIL_H



#ifdef __cplusplus
extern "C" {
#endif
#include <stdlib.h>
#include "opof.h"
#include "opof_error.h"
#include "opof_serverlib.h"
#include "opof_hash.h"

int range(int low, int high);
sessionResponse_t **createSessionResponse(int size, int *sessionCount);
sessionRequest_t **createSessionRequest(int size, unsigned long start_sessionId);
sessionResponse_t *getClosedSessionsFromHash();
#ifdef __cplusplus
}
#endif

#endif //OPOF_TEST_UTIL_H
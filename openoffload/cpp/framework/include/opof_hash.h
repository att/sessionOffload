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
#ifndef OPOF_HASH_H
#define OPOF_HASH_H
#include "opof.h"
#include "uthash.h"


#define HASHTABLE_SIZE 1000

typedef struct {
	unsigned long sessionId;
} record_key_t;

typedef struct {
	record_key_t key;
	unsigned int inLif;
	unsigned int outLif;
	IP_VERSION_T ipVersion;
	unsigned int sourceIp;
	unsigned int sourcePort;
	unsigned int dstIp;
	unsigned int dstPort;
	unsigned long inPackets;
	unsigned long outPackets;
	unsigned long inBytes;
	unsigned long outBytes;
	PROTOCOL_ID_T protocolId;
	ACTION_VALUE_T actionValue;
	SESSION_STATE_T sessionState;
	SESSION_CLOSE_T sessionClose;
	UT_hash_handle hh;
} record_t;

static record_t *sessions = NULL;

#endif  //OPOF_HASH_H
/*
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
#ifndef OPOF_TEST_H
#define OPOF_TEST_H

using namespace std;

#include <iostream>
#include <memory>
#include <string>
#include <sstream>
#include <fstream>
#include <grpcpp/grpcpp.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>
#include <grpcpp/channel_impl.h>

#include "openoffload.grpc.pb.h"
#include "openoffload.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using openoffload::v1alpha2::SessionTable;
using openoffload::v1alpha2::SessionStatisticsTable;
using openoffload::v1alpha2::sessionRequest;
using openoffload::v1alpha2::statisticsRequestArgs;
using openoffload::v1alpha2::addSessionResponse;
using openoffload::v1alpha2::sessionResponse;
using openoffload::v1alpha2::sessionId;
using openoffload::v1alpha2::IP_VERSION;
using openoffload::v1alpha2::PROTOCOL_ID;
using openoffload::v1alpha2::ACTION_VALUE;
using openoffload::v1alpha2::actionType;

#define SUCCESS 0
#define FAILURE -1

typedef struct sessionTuple {
    long sessId;
    int inlif;
    int outlif;
    std::string srcIP;
    std::string dstIP;
    int srcPort;
    int dstPort;
    PROTOCOL_ID proto;
    IP_VERSION ipver;
    ACTION_VALUE actType;
    std::string nextHop;
} sessionTuple_t;

#endif  /* OPOF_TEST_H */
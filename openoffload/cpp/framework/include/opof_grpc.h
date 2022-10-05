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

#ifndef __OPOF_GRPC_H
#define __OPOF_GRPC_H


using namespace std;

#include <iostream>
#include <memory>
#include <string>
#include <sstream>
#include <fstream>
#include <grpcpp/grpcpp.h>
#include <grpcpp/channel.h>
#include <grpcpp/server.h>
#include <grpcpp/client_context.h>
#include <grpcpp/server_context.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>

#include "openoffload.grpc.pb.h"
#include "openoffload.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using grpc::StatusCode;
using grpc::ClientReader;
using grpc::ServerReader;
using grpc::ServerWriter;
using grpc::ClientWriter;
//
using openoffload::v1::SessionTable;
using openoffload::v1::SessionRequest;
using openoffload::v1::SessionRequestArgs;
using openoffload::v1::AddSessionResponse;
using openoffload::v1::SessionResponse;
using openoffload::v1::SessionResponses;
using openoffload::v1::SessionResponseError;
using openoffload::v1::SessionId;
using openoffload::v1::IpVersion;
using openoffload::v1::ProtocolId;
using openoffload::v1::ActionType;
using openoffload::v1::RequestStatus;
using openoffload::v1::AddSessionStatus;
using openoffload::v1::SessionState;
using openoffload::v1::SessionCloseCode;
using openoffload::v1::ActionParameters;

#endif // _OPOF_GRPC_H

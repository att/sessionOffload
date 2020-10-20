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
#ifndef __OPOF_CLIENT
#define __OPOF_CLIENT

extern "C" {
#include "opof.h"
#include "opof_error.h"
#include "opof_clientlib.h"
}

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
//#include <grpcpp/channel_impl.h>

#include "openoffload.grpc.pb.h"
#include "openoffload.pb.h"

#include "opof_sessiontable.h"


using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using openoffload::v1alpha2::SessionTable;

class opof_client
{
public:
	opof_client(const std::string& address, unsigned short port, const std::string& cert);
	SessionTableClient getSessionHandle(void);
	//std::shared_ptr<grpc_impl::Channel> channelHandle;
	Channel channelHandle;
};



#endif
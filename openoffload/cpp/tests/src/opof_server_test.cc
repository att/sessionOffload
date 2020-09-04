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
#include "opof_test.h"
#include "opof_util.h"
#include "opof_session_server.h"
#include "opof_stats_server.h"


extern "C" void opof_server_test(const std::string& address, unsigned short port);

void opof_server_test(const std::string& address, unsigned short port){

  SessionTableImpl service;
  SessionStatsImpl statsService;
  grpc::SslCredentialsOptions sslOpts;
 
  std::string servercert;
  std::string serverkey;
  read("ssl/server.crt", servercert);
  read("ssl/server.key", serverkey);
  grpc::SslServerCredentialsOptions::PemKeyCertPair pkcp;
  pkcp.private_key = serverkey;
  pkcp.cert_chain = servercert;

  grpc::SslServerCredentialsOptions ssl_opts;
  ssl_opts.pem_root_certs="";
  ssl_opts.pem_key_cert_pairs.push_back(pkcp);

  std::shared_ptr<grpc::ServerCredentials> creds;
  creds = grpc::SslServerCredentials(ssl_opts);

 
  std::string server_address("localhost:3443");
 
  ServerBuilder builder;
  // Listen on the given address without any authentication mechanism.
  builder.AddListeningPort(server_address, creds);
  //std::unique_ptr<openoffload::v1alpha2::SessionTable::Stub> stub = openoffload::v1alpha2::SessionTable::NewStub(channel, stubOptions);
  builder.RegisterService(&service);
  builder.RegisterService(&statsService);
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;

  // Wait for the server to shutdown. Note that some other thread must be
  // responsible for shutting down the server for this call to ever return.
  server->Wait();
  
}

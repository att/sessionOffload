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
/**
* \defgroup serverlibrary
*
* \brief gRPC Server implementation
*
*/
extern "C" {
#include "opof.h"
}
#include "opof_util.h"
#include "opof_grpc.h"
#include "opof_session_server.h"

//
// Global Promise for graceful shutdown
//
std::promise<void> exit_requested;
extern "C" void opof_server(const char *address, unsigned short port, const char* cert, const char* key);



/**
* \brief  gRPC C++ Server Implementation
*
* \param address The address the server is listening on either the IP address or "localhost"
* \param port    The port the port the server is listening on
* \param cert    The pulbic key fo the TLS connection
* \parman key    The privaste key of the TLS connection
*
*/
void opof_server(const char* address, unsigned short port, const char* cert, const char* key){

  SessionTableImpl service;
  std::string cppaddress(address);
#ifdef SSL
  grpc::SslCredentialsOptions sslOpts;
  std::string cppkey(key);
  std::string cppcert(cert);

  
  
  grpc::SslServerCredentialsOptions::PemKeyCertPair pkcp;
  pkcp.private_key = cppkey;
  pkcp.cert_chain = cppcert;

  grpc::SslServerCredentialsOptions ssl_opts;
  ssl_opts.pem_root_certs="";
  ssl_opts.pem_key_cert_pairs.push_back(pkcp);

  std::shared_ptr<grpc::ServerCredentials> creds;
  creds = grpc::SslServerCredentials(ssl_opts);
#endif
  cppaddress.append(":");
  cppaddress.append(std::to_string(port));
  //std::cout << "creating address from: " << "cppaddress: " << address << " Port: " << port << std::endl;
  ServerBuilder builder;
  builder.SetSyncServerOption(ServerBuilder::SyncServerOption::NUM_CQS, 10);
  builder.SetSyncServerOption(ServerBuilder::SyncServerOption::MIN_POLLERS, 2);
  builder.SetSyncServerOption(ServerBuilder::SyncServerOption::MAX_POLLERS, 20);
  builder.SetSyncServerOption(ServerBuilder::SyncServerOption::CQ_TIMEOUT_MSEC, 100);

  // Listen on the given address without any authentication mechanism.
#ifdef SSL
  builder.AddListeningPort(cppaddress, creds);
#else
  builder.AddListeningPort(cppaddress, grpc::InsecureServerCredentials());
#endif
  //std::unique_ptr<openoffload::v1alpha2::SessionTable::Stub> stub = openoffload::v1alpha2::SessionTable::NewStub(channel, stubOptions);
  builder.RegisterService(&service);
 
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on: " << cppaddress << std::endl;

  // Wait for the server to shutdown. Note that some other thread must be
  // responsible for shutting down the server for this call to ever return.
  auto serveFn = [&]() {
    server->Wait();
  };
  std::thread serving_thread(serveFn);

  auto f = exit_requested.get_future();
  f.wait();
  server->Shutdown();
  serving_thread.join();
  std::cout << "Shutting down opof server " << std::endl;
  
}

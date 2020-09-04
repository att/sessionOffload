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
#include "opof_session_client.h"
#include "opof_stats_client.h"

extern "C" void opof_client_test(const std::string& address, unsigned short port);


void opof_client_test(const std::string& address, unsigned short port){
  grpc::SslCredentialsOptions sslOpts;
  std::string cert;
  read("ssl/server.crt", cert);
  sslOpts.pem_root_certs = cert; 
  cout << cert << endl;
  auto channel_creds = grpc::SslCredentials(grpc::SslCredentialsOptions(sslOpts));
  auto channel = grpc::CreateChannel("localhost:3443", channel_creds);
  //auto stubOptions = grpc::StubOptions();
  
  sessionTuple_t st;
  std::string reply;
  st.inlif=1;
  st.outlif=2;
  st.srcIP.assign("10.0.1.1");
  st.dstIP.assign("10.0.1.2");
  st.srcPort=22;
  st.dstPort=23;
  st.proto=PROTOCOL_ID::_TCP;
  st.ipver=IP_VERSION::_IPV4;
  st.actType=ACTION_VALUE::_FORWARD;
  st.nextHop.assign("10.0.1.5");
  st.sessId=1000;
  //std::unique_ptr<openoffload::v1alpha2::SessionTable::Stub> stub = openoffload::v1alpha2::SessionTable::NewStub(channel, stubOptions);

  // Add an IPv4 session
  //session_addSession(stub);
  //auto activationStub = Activation::Stub::Stub(channel);

  //std::unique_ptr<Activation::Stub> stub(Activation::NewStub(channel));
  SessionTableClient sessionClient(channel);
  SessionStatsClient statsClient(channel);
  // Wait for the server to shutdown. Note that some other thread must be
  // responsible for shutting down the server for this call to ever return.
  // server->Wait();
  std::cout << "Running client" << std::endl;
  reply = sessionClient.addSession(st);
  std::cout << "Add Session received: " << reply << std::endl;
  
  reply = sessionClient.getSession(st);
  std::cout << "Get Session received: " << reply << std::endl;

  reply = sessionClient.deleteSession(st);
  std::cout << "Delete Session received: " << reply << std::endl;

  statsClient.getAllSessions();
  std::cout << "Getting all sessions: " << std::endl;

  statsClient.getClosedSessions();
  std::cout << "Getting Closed sessions: " << std::endl;
 
}

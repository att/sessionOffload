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
#include <stdlib.h>


extern "C" {
#include "opof.h"
#include "opof_error.h"
}

#include "opof_grpc.h"

#include "opof_session_client.h"
#include "opof_clientlib.h"


struct sessionTable{
	void *obj;
};

sessionTable_t * opof_create_sessionTable(const char * host, unsigned int port, const char *public_key){
	
	sessionTable_t *sessionHandle;
	SessionTableClient *client;
	std::string address(host);
#ifdef SSL
	grpc::SslCredentialsOptions sslOpts;
  	sslOpts.pem_root_certs = public_key;
#endif
  	address.append(":");
  	address.append(std::to_string(port));
    //std::cout << "Public key: " << public_key << std::endl;

    sessionHandle  = (sessionTable_t *)malloc(sizeof(*sessionHandle));
 #ifdef SSL 
  	auto channel_creds = grpc::SslCredentials(grpc::SslCredentialsOptions(sslOpts));
  	//SessionTableClient client(grpc::CreateChannel("localhost:3443", channel_creds));
  	std::cout << "INFO: Creating Secure Client Connection to: " << address << std::endl;
  	client = new SessionTableClient(grpc::CreateChannel(address, channel_creds));
 #else
  	std::cout << "INFO: Creating Insecure Client Connection to: " << address << std::endl;
  	client = new SessionTableClient(grpc::CreateChannel(address, grpc::InsecureChannelCredentials()));
 #endif
  	sessionHandle->obj = client;
  	std::cout << "INFO: Created SessionTableClient" << std::endl;
  	return sessionHandle;
  	
}

int opof_delete_client(sessionTable_t *session){
	int status;

	return status;
}

int opof_add_session(int size, sessionTable_t *sessionHandle,  sessionRequest_t **req, addSessionResponse_t *resp){
	int status;
	SessionTableClient *client;
	std::string reply;
	//sessionRequest_t *param;
	//param = (sessionRequest_t *)parameters;

	client = static_cast<SessionTableClient *>(sessionHandle->obj);
	reply = client->addSessionClient(size,req,resp);
 	//std::cout << "Add Session received: " << reply << std::endl;
	return status;
}
int opof_get_session(sessionTable_t *sessionHandle, unsigned long  session, sessionResponse_t *resp){
	int status = SUCCESS;
	SessionTableClient *client;
	std::string reply;
	//sessionRequest_t *param;
	//sessionId sid;
    //sid.set_sessionid(sessionid);

	//param = (sessionTuple_t *)parameters;

	client = static_cast<SessionTableClient *>(sessionHandle->obj);
	reply = client->getSessionClient(session, resp);
 	//std::cout << "Get Session received: " << reply << std::endl;
	return status;
}

int opof_del_session(sessionTable_t *sessionHandle,  unsigned long  sessionId, sessionResponse_t *resp){
	int status= SUCCESS;
	SessionTableClient *client;
	std::string reply;

	client = static_cast<SessionTableClient *>(sessionHandle->obj);
	reply = client->deleteSessionClient(sessionId, resp);

	return status;
}

void opof_get_closed_sessions(streamArgs_t *args){
	//void opof_get_closed_sessions(sessionTable_t *sessionHandle, unsigned int size){
	int status= SUCCESS;
	sessionTable_t *sessionHandle;
	SessionTableClient *client;
	std::string reply;
	unsigned long closed_sessions =1;
	
	statisticsRequestArgs_t sessionArgs;
	sessionArgs.pageSize = args->pageSize;
	sessionHandle = (sessionTable_t *)args->handle;
	
	client = static_cast<SessionTableClient *>(sessionHandle->obj);
	while(closed_sessions > 0){
		closed_sessions = client->getClosedSessions(&sessionArgs);
	}
	//return status;
}

void opof_get_all_sessions(sessionTable_t *sessionHandle){
	int status= SUCCESS;
	SessionTableClient *client;
	std::string reply;

	client = static_cast<SessionTableClient *>(sessionHandle->obj);

	client->getAllSessions();

	//return status;
}
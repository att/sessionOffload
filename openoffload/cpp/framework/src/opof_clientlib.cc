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
}

#include "opof_grpc.h"

#include "opof_session_client.h"
#include "opof_clientlib.h"


struct sessionTable{
	void *obj;
};

/** 
* \ingroup clientcinterface
* \brief Entry point for C Inteface to C++ Class structure
*
* This method hides the C++ constructor and returns the C__ instance as a void pointer in the 
* sessionTable_t typedef struct. All methods of the sessionTable client class are passed this handle.
*
* \param host 		The address the server is listening on either the IP address or "localhost"
* \param port   	The port the port the server is listening on
* \param public_key The public key for the TLS connection 
* \return 	     	sessionTable_t, returns a handle to the C++ instance as a void handle.
*
*/
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
/**  \ingroup clientcinterface
* \brief Destroy client connection to server
*
* The opof_delete_sessionTable deletes the handle to the client. This cleans up the internal gRPC connections.
* This enables a new connection to be made cleanly to the server.  
** \param  *sessionHandle    Handle pointing to the C++ instance 
*
* \return  void
*
*/
void opof_delete_sessionTable(sessionTable_t *sessionHandle){
	SessionTableClient *client;
	client = static_cast<SessionTableClient *>(sessionHandle->obj);
	delete client;
	free(sessionHandle);
}
/**  \ingroup clientcinterface
* \brief Add sessions to the offload device in batches of up to 64.
*
* The opof_add_session interface is a C wrapper on the underlying gRPC C++ code. This function
* streams up to 64 sessionRequest_t structs to the server. The server either returns success or failure.
* On failure the server returns a bitmask in the addSessionResponse_t, sessions that were failed to be added
* to the hardware have a bit set. Any sessions that failed are set to 1 
* the bitmask.
*
* \param  size 				The number of sessions to be added, max 64.
* \param  *sessionHandle    Handle pointing to the C++ instance 
* \param  **req 			An array of pointers to sessionRequest_t objects (max 64). It is the responsiblity of
*                           the caller to allocate and free this array.
* \param  *resp             Response from the server as a addSessionResponse_t. The requestStatus field returns the
*							status of the server operation. On failure the user must check the errorStatus for sessions
*							that failed.
* \return  SUCCESS or FAILURE Returns the status from the infrastructure.
*
*/
int opof_add_session(int size, sessionTable_t *sessionHandle,  sessionRequest_t **req, addSessionResponse_t *resp){
	SessionTableClient *client;
	int status;

	client = static_cast<SessionTableClient *>(sessionHandle->obj);
	status = client->addSessionClient(size,req,resp);
	return status;
}
/**  \ingroup clientcinterface
* \brief Get a session from the offload device.
*
* THe opof_get_session interface is a C wrapper on the underlying gRPC C++ code. This function is used
* to get the statistics and state of an individual session. 
*
* \param sessionHandle 			 Handle pointing to the C++ instance 
* \param session                 The ID of the session to retrive from the server.
* \param *resp                   The response from the server as sessionResponse_t
* \return 	     SUCCESS or FAILURE
*
*/
int opof_get_session(sessionTable_t *sessionHandle, unsigned long  session, sessionResponse_t *resp){
	int status = SUCCESS;
	SessionTableClient *client;

	client = static_cast<SessionTableClient *>(sessionHandle->obj);
	status = client->getSessionClient(session, resp);
	return status;
}
/**  \ingroup clientcinterface
* \brief Delete a session from the offload device.
*
* THe opof_del_session interface is a C wrapper on the underlying gRPC C++ code. This function is used
* to delete a session on the offload device and return the statistics and state ofsession. 
*
* \param sessionHandle 			 Handle pointing to the C++ instance 
* \param session                 The ID of the session to delete from the server.
* \param *resp                   The response from the server as sessionResponse_t
* \return 	     SUCCESS or FAILURE
*
*/
int opof_del_session(sessionTable_t *sessionHandle,  unsigned long  sessionId, sessionResponse_t *resp){
	int status= SUCCESS;
	SessionTableClient *client;

	client = static_cast<SessionTableClient *>(sessionHandle->obj);
	status = client->deleteSessionClient(sessionId, resp);
	return status;
}
/**  \ingroup clientcinterface
* \brief Get a stream of closed sessions from the offload device
*
* THe opof_get_closed_sessions interface is a C wrapper on the underlying gRPC C++ code. This function
* streams sessions to the client as the server clsoes them.
*
* \param sessionHandle 			 Handle pointing to the C++ instance 
* \return void
*
*/
int opof_get_closed_sessions(streamArgs_t *args, sessionResponse_t responses[], unsigned long *sessionCount){
	int status;
	SessionTableClient *client;
	std::string reply;
	
	sessionTable_t *sessionHandle;
	statisticsRequestArgs_t sessionArgs;
	sessionArgs.pageSize = args->pageSize;
	sessionHandle = (sessionTable_t *)args->handle;
	
	client = static_cast<SessionTableClient *>(sessionHandle->obj);
	status = client->getClosedSessions(&sessionArgs, responses, sessionCount);
	return status;
}
/**  \ingroup clientcinterface
* \brief Get a stream of all sessions from the offload device
*
* THe opof_get_closed_sessions interface is a C wrapper on the underlying gRPC C++ code. This function
* streams all sessions to the client from the server.
*
* \param sessionHandle 			 Handle pointing to the C++ instance 
* \return void
*
*/
int opof_get_all_sessions(sessionTable_t *sessionHandle, uint64_t *startSession,int pageSize, sessionResponse_t responses[], unsigned long *sessionCount){
	int status;
	SessionTableClient *client;
	std::string reply;
	

	client = static_cast<SessionTableClient *>(sessionHandle->obj);

	status = client->getAllSessions(pageSize, startSession, sessionCount,responses, sessionCount);
	return status;
}
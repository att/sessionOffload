*** Settings ***
Documentation     The main interface for interacting with the Baic Python Tests.
Library           String
Library           Collections
Library           OperatingSystem
Library           Process

*** Variables ***
${projectRoot}        ${EXECDIR}



*** Keywords ***
# ~/sessionOffload/tests/robot
Run buildBasic Script 
     [Documentation]    Run scripts to create buildBasic directory 
     Run Process	bash     builditBasic.sh    cwd=${projectRoot}/..    

Start Offload Server 
     [Documentation]    Start gRPC Sessions Server 
     #Start Process	python3   	sessions_server.py 	cwd=${projectRoot}/../buildBasic   alias=offload
     Start Process	python3   	sessions_server.py 	cwd=${projectRoot}/../buildBasic   alias=offload    env:PYTHONUNBUFFERED=1     stdout=/tmp/session_offfload_stdout.txt

Run Client
     [Documentation]    Start gRPC Client 
     Run Process	python3   	sessions_client.py 	cwd=${projectRoot}/../buildBasic   alias=client
     ${result} =	Get Process Result	client
     Log       ${result.stderr}	
     Log       ${result.stdout}	

Run Add Session Ipv4
     [Documentation]    Add IPv4 Session
     Run Process	python3   	-c   import sessions_client; sessions_client.run_add_session_ipv4(); 	cwd=${projectRoot}/../buildBasic   alias=client
     ${result} =	Get Process Result	client
     Log       ${result.stderr}
     Log       ${result.stdout}
     Should Contain   ${result.stdout}    Request Status=   0

Run Add Session Ipv4 Error Session
     [Documentation]    Add IPv4 Session
     Run Process	python3   	-c   import sessions_client; sessions_client.run_add_session_ipv4_error(); 	cwd=${projectRoot}/../buildBasic   alias=client
     ${result} =	Get Process Result	client
     Log       ${result.stderr}
     Log       ${result.stdout}
     Should Contain   ${result.stdout}    addSessionErrorResponse.error_status: 2

Run Add Session Ipv6
     [Documentation]    Add IPv6 Session
     Run Process	python3   	-c   import sessions_client; sessions_client.run_add_session_ipv6(); 	cwd=${projectRoot}/../buildBasic   alias=client
     ${result} =	Get Process Result	client
     Log       ${result.stderr}	
     Log       ${result.stdout}	
     Should Contain   ${result.stdout}      RequestStatus=  0

Run Get Session 
     [Documentation]    Get Session
     Run Process	python3   	-c   import sessions_client; sessions_client.run_get_session(); 	cwd=${projectRoot}/../buildBasic   alias=client
     ${result} =	Get Process Result	client
     Log       ${result.stderr}	
     Log       ${result.stdout}	
     Should Contain   ${result.stdout}     SessionId: 1001
     Should Contain   ${result.stdout}     Session State:  _ESTABLISHED

Run Delete Session 
     [Documentation]    Delete Session
     Run Process	python3   	-c   import sessions_client; sessions_client.run_delete_session(); 	cwd=${projectRoot}/../buildBasic   alias=client
     ${result} =	Get Process Result	client
     Log       ${result.stderr}	
     Log       ${result.stdout}	
     Should Contain   ${result.stdout}     Session RequestStatus:   0

Run Add Mirror Session 
     [Documentation]    Add Mirror Session
     Run Process	python3   	-c   import sessions_client; sessions_client.run_add_mirror_session(); 	cwd=${projectRoot}/../buildBasic   alias=client
     ${result} =	Get Process Result	client
     Log       ${result.stderr}	
     Log       ${result.stdout}	
     Should Contain   ${result.stdout}     SessionResponse:  0

Run Get Closed Sessions
     [Documentation]    Get Closed Sessions
     Run Process	python3   	-c   import sessions_client; sessions_client.run_get_closed_sessions(); 	cwd=${projectRoot}/../buildBasic   alias=client
     ${result} =	Get Process Result	client
     Log       ${result.stderr}	
     Log       ${result.stdout}	
     Should Contain   ${result.stdout}     SessionId: 1001    
     Should Contain   ${result.stdout}     SessionId: 1002    
     Should Contain   ${result.stdout}     CLOSED 


Run Get All Sessions
     [Documentation]    Get All Sessions
     Run Process	python3   	-c   import sessions_client; sessions_client.run_get_all_sessions(); 	cwd=${projectRoot}/../buildBasic   alias=client
     ${result} =	Get Process Result	client
     Log       ${result.stderr}	
     Log       ${result.stdout}	
     Should Contain   ${result.stdout}     SessionId: 1001    
     Should Contain   ${result.stdout}     SessionId: 1002    
     Should Contain   ${result.stdout}     CLOSED 

Stop Offload Server
   [Documentation]  Stop Offload Server
   #Terminate Process   offload 
   ${result} =    Terminate Process    offload 
   Log    ${result.stdout}

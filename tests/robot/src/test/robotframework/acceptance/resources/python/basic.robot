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
     Start Process	bash     builditBasic.sh    cwd=${projectRoot}/..    

Start Offload Server 
     [Documentation]    Start gRPC Sessions Server 
     Start Process	python   	sessions_server.py 	cwd=${projectRoot}/../buildBasic   alias=offload

Start Stats Server
     [Documentation]    Start gRPC Stats Server 
     Start Process	python   	sessions_stats_server.py  	cwd=${projectRoot}/../buildBasic   alias=stats

Start Activation Server
     [Documentation]    Start gRPC Activation Server 
     Start Process  python    activation_server.py      cwd=${projectRoot}/../buildBasic   alias=activation 

Run Client
     [Documentation]    Start gRPC Client 
     Run Process	python   	sessions_client.py 	cwd=${projectRoot}/../buildBasic   alias=client
     ${result} =	Get Process Result	client
     Log       ${result.stderr}	
     Log       ${result.stdout}	

Run Add Session Ipv4
     [Documentation]    Add IPv4 Session
     Run Process	python   	-c   import sessions_client; sessions_client.run_add_session_ipv4(); 	cwd=${projectRoot}/../buildBasic   alias=client
     ${result} =	Get Process Result	client
     Log       ${result.stderr}	
     Log       ${result.stdout}	
     Should Contain   ${result.stdout}     SESSIONID= 1001    


Run Add Session Ipv6
     [Documentation]    Add IPv6 Session
     Run Process	python   	-c   import sessions_client; sessions_client.run_add_session_ipv6(); 	cwd=${projectRoot}/../buildBasic   alias=client
     ${result} =	Get Process Result	client
     Log       ${result.stderr}	
     Log       ${result.stdout}	
     Should Contain   ${result.stdout}     SESSIONID= 1001    

Run Get Session 
     [Documentation]    Get Session
     Run Process	python   	-c   import sessions_client; sessions_client.run_get_session(); 	cwd=${projectRoot}/../buildBasic   alias=client
     ${result} =	Get Process Result	client
     Log       ${result.stderr}	
     Log       ${result.stdout}	
     Should Contain   ${result.stdout}     SessionId: 1001
     Should Contain   ${result.stdout}     Session State: _ESTABLISHED

Run Delete Session 
     [Documentation]    Delete Session
     Run Process	python   	-c   import sessions_client; sessions_client.run_delete_session(); 	cwd=${projectRoot}/../buildBasic   alias=client
     ${result} =	Get Process Result	client
     Log       ${result.stderr}	
     Log       ${result.stdout}	
     Should Contain   ${result.stdout}     Session RequestStatus: 0

Run Add Mirror Session 
     [Documentation]    Add Mirror Session
     Run Process	python   	-c   import sessions_client; sessions_client.run_add_mirror_session(); 	cwd=${projectRoot}/../buildBasic   alias=client
     ${result} =	Get Process Result	client
     Log       ${result.stderr}	
     Log       ${result.stdout}	
     Should Contain   ${result.stdout}     SESSIONID= 1001    

Run Get Closed Sessions
     [Documentation]    Get Closed Sessions
     Run Process	python   	-c   import sessions_client; sessions_client.run_get_closed_sessions(); 	cwd=${projectRoot}/../buildBasic   alias=client
     ${result} =	Get Process Result	client
     Log       ${result.stderr}	
     Log       ${result.stdout}	
     Should Contain   ${result.stdout}     SessionId: 1001    
     Should Contain   ${result.stdout}     SessionId: 1002    
     Should Contain   ${result.stdout}     CLOSED 


Run Get All Sessions
     [Documentation]    Get All Sessions
     Run Process	python   	-c   import sessions_client; sessions_client.run_get_all_sessions(); 	cwd=${projectRoot}/../buildBasic   alias=client
     ${result} =	Get Process Result	client
     Log       ${result.stderr}	
     Log       ${result.stdout}	
     Should Contain   ${result.stdout}     SessionId: 1001    
     Should Contain   ${result.stdout}     SessionId: 1002    
     Should Contain   ${result.stdout}     CLOSED 

Run Activation Sequence
    [Documentation]   Run Activation Sequence
    Run Process        python    -c  import sessions_client; sessions_client.run_activation_sequence();     cwd=${projectRoot}/../buildBasic   alias=client  
     ${result} =	Get Process Result	client
     Log       ${result.stderr}	
     Log       ${result.stdout}	
     Should Contain   ${result.stdout}     Status:  _DEVICE_REGISTERED
     Should Contain   ${result.stdout}     Description:  Acme SmartNIC
     Should Contain   ${result.stdout}     Status:  _DEVICE_ACTIVATED

Stop Offload Server
   [Documentation]  Stop Offload Server
   Terminate Process   offload 

Stop Stats Server
   [Documentation]  Stop Stats Server
   Terminate Process   stats

Stop Activation Server
   [Documentation]  Stop Activation Server
   Terminate Process   activation

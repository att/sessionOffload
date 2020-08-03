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
Run buildPythonSimulator Script 
     [Documentation]    Run scripts to create buildPythonSimulator directory 
     ${result}=   Run Process 	./builditPythonSimulator.sh   shell=true   cwd=${projectRoot}/..    
     Log       ${result.stderr}	
     Log       ${result.stdout}	

Start Offload Server 
     [Documentation]    Start gRPC Sessions Server 
     Start Process	python3    nos_sessions_server_simulator.py   cwd=${projectRoot}/../buildPythonSimulator   alias=offloadS   stdout=simulator.stdout.txt   stderr=simulator.stderr.txt


Run Add Session Ipv4
     [Documentation]    Add IPv4 Session
     ${result}=    Is Process Running    offloadS
     #${result}=    Run Process	   /bin/ps    -Af     alias=bash
     #Log       ${result.stderr}	
     #Log       ${result.stdout}	
     Run Process	python3   	-c   import fw_sessions_client_simulator; fw_sessions_client_simulator.run_addSession(); 	cwd=${projectRoot}/../buildPythonSimulator   alias=client
     ${result} =	Get Process Result	client
     Log       ${result.stderr}	
     Log       ${result.stdout}	
     Should Contain   ${result.stdout}     SESSIONID=     


Run Add Session Ipv4 Until Full
     [Documentation]    Add IPv4 Sessions untill table full
     [Arguments]    ${endLoop}=25
     ${result}=    Is Process Running    offloadS
     FOR    ${INDEX}    IN RANGE    1    ${endLoop} 
     	Run Process	python3   	-c   import fw_sessions_client_simulator; fw_sessions_client_simulator.run_addSession(); 	cwd=${projectRoot}/../buildPythonSimulator   alias=client
        ${result} =	Get Process Result	client
        Log       ${result.stderr}	
        Log       ${result.stdout}	
        Should Contain   ${result.stdout}     SESSIONID=     
        Exit For Loop If    'offload session table is full' in '''${result.stdout}'''
     END
     Should Be True 	${INDEX}==19

Stop Server
   [Documentation]  Stop Server
   Terminate Process   offloadS
   ${result} =	Get Process Result	offloadS
   Log       ${result.stderr}	
   Log       ${result.stdout}	

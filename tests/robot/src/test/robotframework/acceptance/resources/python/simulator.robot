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
     Run Process	bash     builditPythonSimulator.sh     cwd=${projectRoot}/..    

Start Offload Simulator Server 
     [Documentation]    Start gRPC Sessions Server 
     Start Process	python3   	nos_sessions_server_simulator.py 	cwd=${projectRoot}/../buildPythonSimulator   alias=offload


Run Simulator Client
     [Documentation]    Start gRPC Client and loop through tests 2 times
     Run Process	python3   	fw_sessions_client_simulator.py     --loopmax     2      cwd=${projectRoot}/../buildPythonSimulator   alias=client
     ${result} =	Get Process Result	client
     Log       ${result.stderr}	
     Log       ${result.stdout}	

Stop Offload Server
   [Documentation]  Stop Offload Server
   Terminate Process   offload 


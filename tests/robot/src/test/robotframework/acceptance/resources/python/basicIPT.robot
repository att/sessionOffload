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
Run buildIPTBasic Script 
     [Documentation]    Run scripts to create buildIPTBasic directory 
     Run Process	bash     builditIPTBasic.sh    cwd=${projectRoot}/..    

Start IPTOffload Server 
     [Documentation]    Start gRPC Tunnel Server 
     #Start Process	python3   	tunnels_server.py 	cwd=${projectRoot}/../buildIPTBasic   alias=tunnel
     Start Process	python3   	tunnels_server.py 	cwd=${projectRoot}/../buildIPTBasic   alias=tunnel  senv:PYTHONUNBUFFERED=1     stdout=/tmp/tunnel_offfload_stdout.txt

Stop IPTOffload Server
   [Documentation]  Stop IPTOffload Server
   ${result} =    Terminate Process    tunnel 
   Log    ${result.stdout}

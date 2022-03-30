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
     Start Process	python3   	tunnels_server.py 	cwd=${projectRoot}/../buildIPTBasic   alias=tunnel    env:PYTHONUNBUFFERED=1     stdout=/tmp/tunnel_offfload_stdout.txt

Run IPT Client
     [Documentation]    Start gRPC IPT Client
     Run Process        python3         tunnels_client.py      cwd=${projectRoot}/../buildIPTBasic   alias=iptclient
     ${result} =        Get Process Result      iptclient
     Log       ${result.stderr}
     Log       ${result.stdout}

Stop IPTOffload Server
   [Documentation]  Stop IPTOffload Server
   ${result} =    Terminate Process    tunnel 
   Log    ${result.stdout}
   ${stdout} =        Get Process Result      tunnel   stdout=true
   Log       ${stdout}




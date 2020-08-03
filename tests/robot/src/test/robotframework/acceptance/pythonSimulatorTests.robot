*** Settings ***
Documentation     Executes the python basic gRPC Tests
Resource         ./resources/python/simulator.robot

*** Test Cases ***
Setup gRPC Tests
    Run buildPythonSimulator Script
    Start Offload Server 
Add IPv4 Session Test
    Run Add Session IPV4
    Run Add Session Ipv4 Until Full
Suite Teardown
    Stop Server

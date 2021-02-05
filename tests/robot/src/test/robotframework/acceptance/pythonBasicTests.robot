*** Settings ***
Documentation     Executes the python basic gRPC Tests
Resource         ./resources/python/basic.robot

*** Test Cases ***
Setup gRPC Tests
    Run buildBasic Script
    Start Offload Server 
    Start Activation Server 
Full Client Tests
    Run Client
Add IPv4 Session Test
    Run Add Session IPV4
Add IPv4 Session Error Test
    Run Add Session Ipv4 Error Session
Add IPV6 Session Test
    Run Add Session IPv6
Get Session Test
    Run Get Session
Delete Session Test
    Run Delete Session
Add Mirror Session Test
    Run Add Mirror Session
Get Closed Sessions Test
    Run Get Closed Sessions
Get All Closed Sessions Test
    Run Get All Sessions
Activation Tests
    Run Activation Sequence
Suite Teardown
    Stop Activation Server
    Stop Offload Server

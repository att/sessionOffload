*** Settings ***
Documentation     Executes the python basic gRPC Tests
Resource         ./resources/python/basic.robot

*** Test Cases ***
Setup gRPC Tests
    [Tags]   basic
    Run buildBasic Script
    Start Offload Server 
    Start Stats Server 
    Start Activation Server 
Full Client Tests
    [Tags]   basic
    Run Client
Add IPv4 Session Test
    [Tags]   basic
    Run Add Session IPV4
Add IPV6 Session Test
    [Tags]   basic
    Run Add Session IPv6
Get Session Test
    [Tags]   basic
    Run Get Session
Delete Session Test
    [Tags]   basic
    Run Delete Session
Add Mirror Session Test
    [Tags]   basic
    Run Add Mirror Session
Get Closed Sessions Test
    [Tags]   basic
    Run Get Closed Sessions
Get All Closed Sessions Test
    [Tags]   basic
    Run Get All Sessions
Activation Tests
    [Tags]   basic
    Run Activation Sequence
Suite Teardown
    [Tags]   basic
    Stop Activation Server
    Stop Stats Server
    Stop Offload Server

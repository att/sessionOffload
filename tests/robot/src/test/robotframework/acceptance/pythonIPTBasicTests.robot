*** Settings ***
Documentation     Executes the python basic gRPC Tests
Resource         ./resources/python/basicIPT.robot

*** Test Cases ***
Setup gRPC Tests
    Run buildIPTBasic Script
    Start IPTOffload Server 
Full IPT Client Tests
    Run IPT Client
Suite Teardown
    Stop IPTOffload Server

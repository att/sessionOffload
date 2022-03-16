*** Settings ***
Documentation     Executes the python basic gRPC Tests
Resource         ./resources/python/basicIPT.robot

*** Test Cases ***
Setup gRPC Tests
    Run buildIPTBasic Script
    Start IPTOffload Server 
Suite Teardown
    Stop IPTOffload Server

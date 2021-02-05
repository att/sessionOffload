*** Settings ***
Documentation     Executes the python basic gRPC Tests
Resource         ./resources/python/simulator.robot

*** Test Cases ***
Setup gRPC Tests
    Run buildPythonSimulator Script
    Start Offload Simulator Server
Run Client Tests
    Run Simulator Client
Suite Teardown
    Stop Offload Server

#============LICENSE_START=============================================================================================================
# Copyright (C) 2020 Palo Alto Netowrks Intellectual Property. All rights reserved.
#===================================================================
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#============LICENSE_END===============================================================================================================

"""The Python implementation of the activation server for OpenOffload."""

from concurrent import futures
import time
import logging
import socket
import struct

import grpc
import google.protobuf.timestamp_pb2

import openoffload_pb2
import openoffload_pb2_grpc


class ActivationServicer(openoffload_pb2_grpc.ActivationServicer):


	def __init__(self):
		self.allDevices={}

	def getRegisteredOffloadDevices(self, request, context):
		print("############ GetAllImplementations ##################")
		listofdevice = openoffload_pb2.deviceList()
		print('Devices: ', str(self.allDevices))
		for value in self.allDevices.values():
			listofdevice.devices.append(value)
		return listofdevice

	def activateOffload(self,request, context):
		print("############ activateOffload ##################")
		self.allDevices[request.name] = request
		return openoffload_pb2.activationStatus(status=openoffload_pb2._DEVICE_ACTIVATED, device=request)

	def deactivateOffload(self,request, context):
		print("############ deactivateOffload ##################")
		return openoffload_pb2.activationStatus(status=openoffload_pb2._DEVICE_DEACTIVATED, device=request)

	def registerOffloadDevice(self,request, context):
		print("############ registerOffloadDevice ##################")
		self.allDevices[request.name] = request
		return openoffload_pb2.registrationStatus(status=openoffload_pb2._DEVICE_REGISTERED)

	def deregisterOffloadDevice(self,request, context):
		print("############ deregisterOffloadDevice ##################")
		del self.allDevices[request.name]
		return openoffload_pb2.registrationStatus(status=openoffload_pb2._DEVICE_DEREGISTERED)

def activationService():
	server = grpc.server(futures.ThreadPoolExecutor(max_workers=5))
	openoffload_pb2_grpc.add_ActivationServicer_to_server(
		ActivationServicer(), server)
	with open('ssl/server.key', 'rb') as f:
		private_key = f.read()
	with open('ssl/server.crt', 'rb') as f:
		certificate_chain = f.read()
	server_credentials = grpc.ssl_server_credentials( ( (private_key, certificate_chain), ) )
	server.add_secure_port('localhost:3445', server_credentials)
	server.start()
	server.wait_for_termination()

if __name__ == '__main__':
	logging.basicConfig()
	activationService()
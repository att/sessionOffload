# Copyright (C) 2020 Palo Alto Networks Intellectual Property. All rights reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

PROTO_NAME := openoffload
GOCMD := go
PYTHONCMD := python
PROTOC:=protoc
PYTHONOUT := python
PROTOS_PATH = ../protos
vpath %.proto $(PROTOS_PATH)
GRPC_CPP_PLUGIN = grpc_cpp_plugin
GRPC_CPP_PLUGIN_PATH ?= `which $(GRPC_CPP_PLUGIN)`
GOOUT := golang

$(PROTO_NAME).pb.go: $(PROTO_NAME).proto
	mkdir -p golang
	protoc -I $(PROTOS_PATH)/ $(PROTO_NAME).proto  --go-grpc_out=$(GOOUT) --go_out=$(GOOUT)  $(PROTO_NAME).proto

$(PROTO_NAME).pb.py: $(PROTO_NAME).proto
	mkdir -p python
	$(PYTHONCMD) -m grpc_tools.protoc -I $(PROTOS_PATH) --python_out=$(PYTHONOUT) --grpc_python_out=$(PYTHONOUT) $(PROTO_NAME).proto

$(PROTO_NAME).pb.cc: $(PROTO_NAME).proto
	mkdir -p cpp
	$(PROTOC) -I $(PROTOS_PATH) --cpp_out=cpp $(PROTO_NAME).proto

$(PROTO_NAME).grpc.pb.cc: $(PROTO_NAME).proto
	$(PROTOC) -I $(PROTOS_PATH) --grpc_out=cpp --plugin=protoc-gen-grpc=$(GRPC_CPP_PLUGIN_PATH) $<

all-go:  $(PROTO_NAME).pb.go

all-py:  $(PROTO_NAME).pb.py

all-cpp:  $(PROTO_NAME).grpc.pb.cc $(PROTO_NAME).pb.cc

all: $(PROTO_NAME).pb.go $(PROTO_NAME).pb.py $(PROTO_NAME).pb.cc $(PROTO_NAME).grpc.pb.cc


.PHONY: clean
#
clean:
	rm -f python/*.py
	rm -f golang/*.go
	rm -f cpp/*.cc
	rm -f cpp/*.h
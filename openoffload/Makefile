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
MOUNT_PATH = ../
vpath %.proto $(PROTOS_PATH)
GOOUT := golang

$(PROTO_NAME).pb.go: $(PROTO_NAME).proto
	mkdir -p golang
	docker run -v "${PWD}/${MOUNT_PATH}":/defs namely/protoc-all --lint -d ./protos -l go -o ./openoffload/golang  --go-source-relative

$(PROTO_NAME).pb.py: $(PROTO_NAME).proto
	mkdir -p python
	docker run -v "${PWD}/${MOUNT_PATH}":/defs namely/protoc-all --lint -d ./protos -l python -o ./openoffload/python  --go-source-relative

$(PROTO_NAME).pb.cc: $(PROTO_NAME).proto
	mkdir -p cpp
	docker run -v "${PWD}/${MOUNT_PATH}":/defs namely/protoc-all --lint -d ./protos -l cpp -o ./openoffload/cpp  --go-source-relative

all-go:  $(PROTO_NAME).pb.go

all-py:  $(PROTO_NAME).pb.py

all-cpp:  $(PROTO_NAME).pb.cc

all: $(PROTO_NAME).pb.go $(PROTO_NAME).pb.py $(PROTO_NAME).pb.cc


.PHONY: clean
#
clean:
	rm -f python/*.py
	rm -f golang/*.go
	rm -f cpp/*.cc
	rm -f cpp/*.h

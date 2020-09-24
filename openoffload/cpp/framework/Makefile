# Copyright (C) 2020 Palo Alto Networks Intellectual Property. All rights reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at:
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
WORKING_DIR ?= $(shell pwd)
$(info Working directory is  $(WORKING_DIR))
##
DOXYGEN := doxygen
COMBINED_NAME := opof_test
CLIENT_NAME :=opof_client_test
SERVER_NAME :=opof_server_test
PROTO_NAME := openoffload
DIRECTORIES := directories
PROTOS := protos
VERSION=0.1
SERVER_LIB := libopof_server.a
CLIENT_LIB := libopof_client.a
#
ROOT_DIR ?= $(HOME)
#
# CPP gRPC Definitions
#
PROTOC:=protoc
GRPC_CPP_PLUGIN = grpc_cpp_plugin
GRPC_CPP_PLUGIN_PATH ?= `which $(GRPC_CPP_PLUGIN)`
#
# Source Directories
#
PROTOS_PATH = ../../../protos
vpath %.proto $(PROTOS_PATH)

C_SRC_DIR := src
vpath  %.c $(C_SRC_DIR)
vpath  %.cc $(C_SRC_DIR)
#
H_SRC_DIR := include
vpath  %.h $(H_SRC_DIR)
#vpath  %.h $(C_SRC_DIR)

OBJ_SRC_DIR := obj
vpath %.o $(OBJ_SRC_DIR)
#
# Output directories
#
##
INC_DIR := include
SRC_DIR := src
LIB_DIR := lib
OBJ_DIR := obj
BIN_DIR := bin
DOC_DIR := doc
INCLUDES := -I ./include \
	-I ./ -I /usr/include \
	-I $(ROOT_DIR)/local/include \
	-I /usr/include/c++/8 \
	-I /usr/include/linux \
	-I /usr/lib/gcc/x86_64-redhat-linux/8/include \
	-I /usr/include/c++/8/x86_64-redhat-linux/
##
#
# Compiler flags
#
##
CC := gcc -g

CPP := g++ -g

#CFLAGS := -c -O2 -I include -I /usr/local/include -I $(ROOT_DIR)/local/include -std=gnu99 -Wall -DDEBUG -DVISIBLE
CFLAGS := -c -O2 -I include -Wno-undef -I $(ROOT_DIR)/local/include -std=gnu99 -Wall -DDEBUG
#CPPFLAGS := -c -O2 -std=c++11 -I ./ -I include  -I $(ROOT_DIR)/local/include -Wno-deprecated-declarations  -DDEBUG -DVISIBLE
#CPPFLAGS := -c -O2 -std=c++11 -Wno-undef -I include -I $(ROOT_DIR)/local/include -Wno-deprecated-declarations -DSSL -DDEBUG
CPPFLAGS := -c -O2 -std=c++11 -Wno-undef -I include -I $(ROOT_DIR)/local/include -Wno-deprecated-decl -DDEBUG
#
#
# Link Flags
#
LIBDIR := $(ROOT_DIR)/local/lib
LIB64DIR := $(ROOT_DIR)/local/lib64
#
GPR_LIB := $(LIBDIR)/libgpr.a
CHANNELZ_LIB := $(LIBDIR)/libgrpcpp_channelz.a
GRPCPP_LIB := $(LIBDIR)/libgrpc++.a
GRPC_LIB := $(LIBDIR)/libgrpc.a
PROTOBUF_LIB := $(LIB64DIR)/libprotobuf.a
CARES_LIB := $(LIB64DIR)/libcares.a
REFLECTION_LIB := $(LIBDIR)/libgrpc++_reflection.a
UPB_LIB := $(LIBDIR)/libupb.a
ABSL_BASE_LIB := $(LIBDIR)/libabsl_base.a
ABSL_FORMAT_LIB := $(LIBDIR)/libabsl_str_format_internal.a
ABSL_INT28_LIB := $(LIBDIR)/libabsl_int128.a
ABSL_STRINGS_LIB := $(LIBDIR)/libabsl_strings.a 
ABSL_BAD_LIB := $(LIBDIR)/libabsl_bad_optional_access.a
SSL_LIB := $(LIBDIR)/libssl.a
CRYPTO_LIB := $(LIBDIR)/libcrypto.a
ABSL_STR_INTERNAL_LIB := $(LIBDIR)/libabsl_strings_internal.a 
ADDRESS_SORT_LIB := $(LIBDIR)/libaddress_sorting.a
Z_LIB := $(LIBDIR)/libz.a
ABSL_LOGGING_LIB := $(LIBDIR)/libabsl_raw_logging_internal.a 
ABSL_THROW := $(LIBDIR)/libabsl_throw_delegate.a
ABSL_TIME_LIB := $(LIBDIR)/libabsl_time.a
ABSL_TIME_ZONE_LIB := $(LIBDIR)/libabsl_time_zone.a


LD = g++ -g
#LDFLAGS = -lc  -lc++ -lgrpc++ -lprotobuf -L/usr/local/lib -L /opt/local/lib
LIBCONFIG := /usr/local/lib/libconfig.a
LDFLAGS = -lc  -lstdc++ -pthread  
SERVERFLAGS =  -L $(LIB_DIR) -lopof_server
CLIENTFLAGS =  -L $(LIB_DIR) -lopof_client

LIBS ?= \
	$(PROTOBUF_LIB) \
  	$(GRPCPP_LIB) \
  	$(GRPC_LIB) \
  	$(GPR_LIB) \
  	$(UPB_LIB) \
  	$(Z_LIB) \
  	$(SSL_LIB) \
  	$(CRYPTO_LIB) \
  	$(CARES_LIB) \
  	$(ADDRESS_SORT_LIB) \
  	$(ABSL_BAD_LIB) \
  	$(ABSL_BASE_LIB) \
  	$(ABSL_FORMAT_LIB) \
  	$(ABSL_INT28_LIB) \
  	$(ABSL_STRINGS_LIB) \
  	$(ABSL_STR_INTERNAL_LIB) \
  	$(ABSL_LOGGING_LIB) \
  	$(ABSL_THROW) \
  	$(ABSL_TIME_LIB) \
  	$(ABSL_TIME_ZONE_LIB)  
#           
all: $(DIRECTORIES) $(PROTOS) $(SERVER_LIB) $(CLIENT_LIB) $(COMBINED_NAME) $(SERVER_NAME) $(CLIENT_NAME) 
#
# Create documentation
#
doc:
	$(DOXYGEN) $(DOC_DIR)/Doxyfile
#
# Create directories
#
$(DIRECTORIES):
	mkdir -p obj
	mkdir -p lib
	mkdir -p bin
	mkdir -p doc
#
# Generate gPRC files
#
$(PROTO_NAME).pb.cc: $(PROTO_NAME).proto
	$(PROTOC) -I $(PROTOS_PATH) --cpp_out=src $(PROTO_NAME).proto
	mv src/$(PROTO_NAME).pb.h include/
#
$(PROTO_NAME).grpc.pb.cc: $(PROTO_NAME).proto
	$(PROTOC) -I $(PROTOS_PATH) --grpc_out=src --plugin=protoc-gen-grpc=$(GRPC_CPP_PLUGIN_PATH) $<
	mv src/$(PROTO_NAME).grpc.pb.h include/
#
$(PROTOS):  $(PROTO_NAME).grpc.pb.cc $(PROTO_NAME).pb.cc

#
OBJS = \
	$(OBJ_DIR)/opof_main.o \
	$(OBJ_DIR)/openoffload.pb.o \
	$(OBJ_DIR)/openoffload.grpc.pb.o \
	$(OBJ_DIR)/opof_error.o \
	$(OBJ_DIR)/opof_session_server.o \
	$(OBJ_DIR)/opof_session_client.o \
	$(OBJ_DIR)/opof_client_test.o \
	$(OBJ_DIR)/opof_run_tests.o \
	$(OBJ_DIR)/opof_server_test.o \
	$(OBJ_DIR)/opof_clientlib.o \
	$(OBJ_DIR)/opof_server.o \
	$(OBJ_DIR)/opof_util.o \
	$(OBJ_DIR)/opof_test_util.o \
	$(OBJ_DIR)/opof_config.o

SERVER_OBJS = \
	$(OBJ_DIR)/openoffload.pb.o \
	$(OBJ_DIR)/openoffload.grpc.pb.o \
	$(OBJ_DIR)/opof_session_server.o \
	$(OBJ_DIR)/opof_server.o \
	$(OBJ_DIR)/opof_util.o

CLIENT_OBJS = \
	$(OBJ_DIR)/openoffload.pb.o \
	$(OBJ_DIR)/openoffload.grpc.pb.o \
	$(OBJ_DIR)/opof_session_client.o \
	$(OBJ_DIR)/opof_clientlib.o \
	$(OBJ_DIR)/opof_util.o

#

openoffload.pb.o: $(SRC_DIR)/openoffload.pb.cc $(INC_DIR)/openoffload.pb.h
	$(CPP) $(CPPFLAGS) $< -o $(OBJ_DIR)/$@

openoffload.grpc.pb.o: $(SRC_DIR)/openoffload.grpc.pb.cc $(INC_DIR)/openoffload.grpc.pb.h $(INC_DIR)/openoffload.pb.h 
	$(CPP) $(CPPFLAGS) $< -o $(OBJ_DIR)/$@

opof_error.o: opof_error.c opof_error.h
	$(CC) $(CFLAGS) $< -o $(OBJ_DIR)/$@

opof_config.o: opof_config.c opof_error.h
	$(CC) $(CFLAGS) $< -o $(OBJ_DIR)/$@

opof_main.o: opof_main.c opof.h opof_error.h
	$(CC) $(CFLAGS) $< -o $(OBJ_DIR)/$@
#
opof_server_main.o: opof_server_main.c opof.h opof_error.h
	$(CC) $(CFLAGS) $< -o $(OBJ_DIR)/$@
#
opof_client_main.o: opof_client_main.c opof.h opof_error.h
	$(CC) $(CFLAGS) $< -o $(OBJ_DIR)/$@
#
opof_client_test.o: opof_client_test.c opof.h opof_error.h opof_clientlib.h
	$(CC) $(CFLAGS) $< -o $(OBJ_DIR)/$@

opof_run_tests.o: opof_run_tests.c opof.h opof_error.h opof_clientlib.h
	$(CC) $(CFLAGS) $< -o $(OBJ_DIR)/$@

opof_server_test.o: opof_server_test.c opof.h opof_error.h
	$(CC) $(CFLAGS) $< -o $(OBJ_DIR)/$@

opof_clientlib.o: opof_clientlib.cc opof.h opof_error.h opof_clientlib.h
	$(CPP) $(CPPFLAGS) $< -o $(OBJ_DIR)/$@

opof_server.o: opof_server.cc opof.h opof_error.h opof_serverlib.h
	$(CPP) $(CPPFLAGS) $< -o $(OBJ_DIR)/$@

opof_session_client.o: opof_session_client.cc opof.h opof_error.h opof_session_client.h openoffload.grpc.pb.h openoffload.pb.h
	$(CPP) $(CPPFLAGS) $< -o $(OBJ_DIR)/$@

opof_session_server.o: opof_session_server.cc opof.h opof_error.h opof_session_server.h  openoffload.grpc.pb.h openoffload.pb.h
	$(CPP) $(CPPFLAGS) $< -o $(OBJ_DIR)/$@

opof_util.o: opof_util.cc opof.h opof_error.h opof_util.h
	$(CPP) $(CPPFLAGS) $< -o $(OBJ_DIR)/$@

opof_test_util.o: opof_test_util.c opof.h opof_error.h opof_test_util.h
	$(CC) $(CFLAGS) $< -o $(OBJ_DIR)/$@

$(COMBINED_NAME): openoffload.pb.o openoffload.grpc.pb.o opof_error.o opof_main.o opof_session_server.o opof_session_client.o opof_client_test.o opof_server_test.o opof_clientlib.o opof_server.o opof_util.o opof_test_util.o opof_run_tests.o opof_config.o
	$(LD) $(LDFLAGS) $(OBJS) $(LIBCONFIG) $(LIBS) -o $(BIN_DIR)/$@
#
$(CLIENT_NAME): opof_client_main.o opof_client_test.o opof_config.o opof_error.o opof_test_util.o opof_run_tests.o
	$(LD) $(LDFLAGS)  $(OBJ_DIR)/opof_client_main.o $(OBJ_DIR)/opof_error.o $(OBJ_DIR)/opof_client_test.o $(OBJ_DIR)/opof_run_tests.o $(OBJ_DIR)/opof_config.o  $(OBJ_DIR)/opof_test_util.o  $(CLIENTFLAGS)  $(LIBCONFIG) $(LIBS) -o $(BIN_DIR)/$@
#
$(SERVER_NAME): opof_server_main.o opof_server_test.o opof_error.o opof_test_util.o
	$(LD) $(LDFLAGS)  $(OBJ_DIR)/opof_server_main.o $(OBJ_DIR)/opof_error.o $(OBJ_DIR)/opof_server_test.o  $(OBJ_DIR)/opof_test_util.o $(SERVERFLAGS) $(LIBCONFIG) $(LIBS)  -o $(BIN_DIR)/$@
#
$(SERVER_LIB): openoffload.pb.o openoffload.grpc.pb.o opof_session_server.o opof_server.o opof_util.o
	ar ruv $(LIB_DIR)/$@ $(SERVER_OBJS)
	ranlib $(LIB_DIR)/$@
#
$(CLIENT_LIB): openoffload.pb.o openoffload.grpc.pb.o opof_session_client.o opof_clientlib.o opof_util.o
	ar ruv $(LIB_DIR)/$@ $(CLIENT_OBJS)
	ranlib $(LIB_DIR)/$@
#
#
.PHONY: clean all doc
#
clean:
	rm -f obj/*.o \
	rm -f bin/* \
	rm -f lib/*.a \
	rm -f src/openoffload.grpc.pb.cc \
	rm -f src/openoffload.pb.cc \
	rm -f include/openoffload.grpc.pb.h \
	rm -f include/openoffload.pb.h
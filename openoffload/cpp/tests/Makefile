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
##
CLIENT_NAME := opof_test
VERSION := 0.1
#
# Source Directories
#
ROOT_DIR ?= /home/jmcdowall
#
C_SRC_DIR := src
vpath  %.c $(C_SRC_DIR)
vpath  %.cc $(C_SRC_DIR)
#
H_SRC_DIR := include
vpath  %.h $(H_SRC_DIR)
#
# Output directories
#
##
LIB_DIR := lib
OBJ_DIR := obj
BIN_DIR := bin
#SRC_DIR := src
##
#
# Compiler flags
#
##
CC := gcc
CPP := g++
CFLAGS := -g -c -O2 -I include -I /usr/local/include -std=gnu99 -Wall -DDEBUG
CPPFLAGS := -g -c -O2 -std=c++11 -I ./ -I include  -I $(ROOT_DIR)/local/include -Wno-deprecated-declarations  -DDEBUG
#
#
# Link Flags
#
LIBDIR := $(ROOT_DIR)/local/lib
LIB64DIR := $(ROOT_DIR)/local/lib64
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




LD := g++ -g
#LDFLAGS = -lc  -lc++ -lgrpc++ -lprotobuf -L/usr/local/lib -L /opt/local/lib
LDFLAGS := -lc  -lstdc++ -pthread
LIBS := \
  $(PROTOBUF_LIB) \
  $(GRPCPP_LIB) \
  $(UPB_LIB) \
  $(GRPC_LIB) \
  $(GPR_LIB) \
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
# Object files
#
OBJS = \
	$(OBJ_DIR)/openoffload.pb.o \
	$(OBJ_DIR)/openoffload.grpc.pb.o \
	$(OBJ_DIR)/opof_error.o \
	$(OBJ_DIR)/opof_main.o \
	$(OBJ_DIR)/opof_client.o \
	$(OBJ_DIR)/opof_activation.o \
	$(OBJ_DIR)/opof_session.o \
  $(OBJ_DIR)/opof_statistics.o

all: $(CLIENT_NAME)


openoffload.pb.o: openoffload.pb.cc openoffload.pb.h
	$(CPP) $(CPPFLAGS) $< -o $(OBJ_DIR)/$@

openoffload.grpc.pb.o: openoffload.grpc.pb.cc openoffload.grpc.pb.h
	$(CPP) $(CPPFLAGS) $< -o $(OBJ_DIR)/$@

opof_error.o: opof_error.c opof_error.h
	$(CC) $(CFLAGS) $< -o $(OBJ_DIR)/$@

opof_main.o: opof_main.c opof.h opof_error.h
	$(CC) $(CFLAGS) $< -o $(OBJ_DIR)/$@

opof_client.o: opof_client.cc opof.h opof_error.h openoffload.grpc.pb.h openoffload.pb.h
	$(CPP) $(CPPFLAGS) $< -o $(OBJ_DIR)/$@

opof_activation.o: opof_activation.cc opof.h opof_error.h openoffload.grpc.pb.h openoffload.pb.h
	$(CPP) $(CPPFLAGS) $< -o $(OBJ_DIR)/$@

opof_session.o: opof_session.cc opof.h opof_error.h openoffload.grpc.pb.h openoffload.pb.h
	$(CPP) $(CPPFLAGS) $< -o $(OBJ_DIR)/$@

opof_statistics.o: opof_statistics.c opof.h opof_error.h
	$(CC) $(CFLAGS) $< -o $(OBJ_DIR)/$@


#$(CLIENT_NAME): $(OBJS)

$(CLIENT_NAME): openoffload.pb.o openoffload.grpc.pb.o opof_error.o opof_main.o opof_client.o opof_activation.o opof_session.o opof_statistics.o
	$(LD) $(LDFLAGS) $(OBJS) $(LIBS) -o $(BIN_DIR)/$@

.PHONY: clean

clean:
	rm -f obj/*.o \
	rm -f bin/*
# DO NOT DELETE

openoffload.pb.o: openoffload.pb.h /usr/include/c++/8/limits
openoffload.pb.o: /usr/include/c++/8/x86_64-redhat-linux/bits/c++config.h
openoffload.pb.o: /usr/include/bits/wordsize.h
openoffload.pb.o: /usr/include/c++/8/x86_64-redhat-linux/bits/os_defines.h
openoffload.pb.o: /usr/include/features.h /usr/include/stdc-predef.h
openoffload.pb.o: /usr/include/sys/cdefs.h /usr/include/bits/long-double.h
openoffload.pb.o: /usr/include/gnu/stubs.h
openoffload.pb.o: /usr/include/c++/8/x86_64-redhat-linux/bits/cpu_defines.h
openoffload.pb.o: /usr/include/c++/8/string
openoffload.pb.o: /usr/include/c++/8/bits/stringfwd.h
openoffload.pb.o: /usr/include/c++/8/bits/memoryfwd.h
openoffload.pb.o: /usr/include/c++/8/bits/char_traits.h
openoffload.pb.o: /usr/include/c++/8/bits/stl_algobase.h
openoffload.pb.o: /usr/include/c++/8/bits/functexcept.h
openoffload.pb.o: /usr/include/c++/8/bits/exception_defines.h
openoffload.pb.o: /usr/include/c++/8/bits/cpp_type_traits.h
openoffload.pb.o: /usr/include/c++/8/ext/type_traits.h
openoffload.pb.o: /usr/include/c++/8/ext/numeric_traits.h
openoffload.pb.o: /usr/include/c++/8/bits/stl_pair.h
openoffload.pb.o: /usr/include/c++/8/bits/move.h
openoffload.pb.o: /usr/include/c++/8/bits/concept_check.h
openoffload.pb.o: /usr/include/c++/8/bits/stl_iterator_base_types.h
openoffload.pb.o: /usr/include/c++/8/bits/stl_iterator_base_funcs.h
openoffload.pb.o: /usr/include/c++/8/debug/assertions.h
openoffload.pb.o: /usr/include/c++/8/bits/stl_iterator.h
openoffload.pb.o: /usr/include/c++/8/bits/ptr_traits.h
openoffload.pb.o: /usr/include/c++/8/debug/debug.h
openoffload.pb.o: /usr/include/c++/8/bits/predefined_ops.h
openoffload.pb.o: /usr/include/c++/8/bits/postypes.h
openoffload.pb.o: /usr/include/c++/8/cwchar /usr/include/wchar.h
openoffload.pb.o: /usr/include/bits/libc-header-start.h
openoffload.pb.o: /usr/include/bits/floatn.h
openoffload.pb.o: /usr/include/bits/floatn-common.h
openoffload.pb.o: /usr/include/linux/stddef.h
openoffload.pb.o: /usr/lib/gcc/x86_64-redhat-linux/8/include/stdarg.h
openoffload.pb.o: /usr/include/bits/wchar.h /usr/include/bits/types/wint_t.h
openoffload.pb.o: /usr/include/bits/types/mbstate_t.h
openoffload.pb.o: /usr/include/bits/types/__mbstate_t.h
openoffload.pb.o: /usr/include/bits/types/__FILE.h
openoffload.pb.o: /usr/include/bits/types/FILE.h
openoffload.pb.o: /usr/include/bits/types/locale_t.h
openoffload.pb.o: /usr/include/bits/types/__locale_t.h
openoffload.pb.o: /usr/include/c++/8/bits/allocator.h
openoffload.pb.o: /usr/include/c++/8/x86_64-redhat-linux/bits/c++allocator.h
openoffload.pb.o: /usr/include/c++/8/ext/new_allocator.h
openoffload.pb.o: /usr/include/c++/8/new /usr/include/c++/8/exception
openoffload.pb.o: /usr/include/c++/8/bits/exception.h
openoffload.pb.o: /usr/include/c++/8/bits/localefwd.h
openoffload.pb.o: /usr/include/c++/8/x86_64-redhat-linux/bits/c++locale.h
openoffload.pb.o: /usr/include/c++/8/clocale /usr/include/locale.h
openoffload.pb.o: /usr/include/bits/locale.h /usr/include/c++/8/iosfwd
openoffload.pb.o: /usr/include/c++/8/cctype /usr/include/ctype.h
openoffload.pb.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
openoffload.pb.o: /usr/include/endian.h /usr/include/bits/endian.h
openoffload.pb.o: /usr/include/bits/byteswap.h
openoffload.pb.o: /usr/include/bits/uintn-identity.h
openoffload.pb.o: /usr/include/c++/8/bits/ostream_insert.h
openoffload.pb.o: /usr/include/c++/8/bits/cxxabi_forced.h
openoffload.pb.o: /usr/include/c++/8/bits/stl_function.h
openoffload.pb.o: /usr/include/c++/8/backward/binders.h
openoffload.pb.o: /usr/include/c++/8/bits/range_access.h
openoffload.pb.o: /usr/include/c++/8/bits/basic_string.h
openoffload.pb.o: /usr/include/c++/8/ext/atomicity.h
openoffload.pb.o: /usr/include/c++/8/x86_64-redhat-linux/bits/gthr.h
openoffload.pb.o: /usr/include/c++/8/x86_64-redhat-linux/bits/gthr-default.h
openoffload.pb.o: /usr/include/pthread.h /usr/include/sched.h
openoffload.pb.o: /usr/include/bits/types/time_t.h
openoffload.pb.o: /usr/include/bits/types/struct_timespec.h
openoffload.pb.o: /usr/include/bits/sched.h
openoffload.pb.o: /usr/include/bits/types/struct_sched_param.h
openoffload.pb.o: /usr/include/bits/cpu-set.h /usr/include/time.h
openoffload.pb.o: /usr/include/bits/time.h /usr/include/bits/types/clock_t.h
openoffload.pb.o: /usr/include/bits/types/struct_tm.h
openoffload.pb.o: /usr/include/bits/types/clockid_t.h
openoffload.pb.o: /usr/include/bits/types/timer_t.h
openoffload.pb.o: /usr/include/bits/types/struct_itimerspec.h
openoffload.pb.o: /usr/include/bits/pthreadtypes.h
openoffload.pb.o: /usr/include/bits/thread-shared-types.h
openoffload.pb.o: /usr/include/bits/pthreadtypes-arch.h
openoffload.pb.o: /usr/include/bits/setjmp.h
openoffload.pb.o: /usr/include/c++/8/x86_64-redhat-linux/bits/atomic_word.h
openoffload.pb.o: /usr/include/c++/8/ext/alloc_traits.h
openoffload.pb.o: /usr/include/c++/8/bits/basic_string.tcc
openoffload.pb.o: /home/jmcdowall/local/include/google/protobuf/port_def.inc
openoffload.pb.o: /home/jmcdowall/local/include/google/protobuf/port_undef.inc
openoffload.pb.o: /home/jmcdowall/local/include/google/protobuf/io/coded_stream.h
openoffload.pb.o: /usr/include/assert.h /usr/include/c++/8/atomic
openoffload.pb.o: /usr/include/c++/8/bits/c++0x_warning.h
openoffload.pb.o: /usr/include/c++/8/climits /usr/include/limits.h
openoffload.pb.o: /usr/include/linux/limits.h /usr/include/bits/posix1_lim.h
openoffload.pb.o: /usr/include/bits/local_lim.h /usr/include/linux/limits.h
openoffload.pb.o: /usr/include/bits/posix2_lim.h /usr/include/c++/8/cstddef
openoffload.pb.o: /usr/include/c++/8/cstring /usr/include/string.h
openoffload.pb.o: /usr/include/strings.h /usr/include/c++/8/type_traits
openoffload.pb.o: /usr/include/c++/8/utility
openoffload.pb.o: /usr/include/c++/8/bits/stl_relops.h
openoffload.pb.o: /usr/include/sys/param.h /usr/include/sys/types.h
openoffload.pb.o: /usr/include/bits/stdint-intn.h /usr/include/sys/select.h
openoffload.pb.o: /usr/include/bits/select.h
openoffload.pb.o: /usr/include/bits/types/sigset_t.h
openoffload.pb.o: /usr/include/bits/types/__sigset_t.h
openoffload.pb.o: /usr/include/bits/types/struct_timeval.h
openoffload.pb.o: /usr/include/signal.h /usr/include/bits/signum.h
openoffload.pb.o: /usr/include/bits/signum-generic.h
openoffload.pb.o: /usr/include/bits/types/sig_atomic_t.h
openoffload.pb.o: /usr/include/bits/types/siginfo_t.h
openoffload.pb.o: /usr/include/bits/types/__sigval_t.h
openoffload.pb.o: /usr/include/bits/siginfo-arch.h
openoffload.pb.o: /usr/include/bits/siginfo-consts.h
openoffload.pb.o: /usr/include/bits/types/sigval_t.h
openoffload.pb.o: /usr/include/bits/types/sigevent_t.h
openoffload.pb.o: /usr/include/bits/sigevent-consts.h
openoffload.pb.o: /usr/include/bits/sigaction.h
openoffload.pb.o: /usr/include/bits/sigcontext.h
openoffload.pb.o: /usr/include/bits/types/stack_t.h
openoffload.pb.o: /usr/include/sys/ucontext.h /usr/include/bits/sigstack.h
openoffload.pb.o: /usr/include/bits/ss_flags.h
openoffload.pb.o: /usr/include/bits/types/struct_sigstack.h
openoffload.pb.o: /usr/include/bits/sigthread.h /usr/include/bits/param.h
openoffload.pb.o: /usr/include/linux/param.h /usr/include/asm/param.h
openoffload.pb.o: /usr/include/asm-generic/param.h
openoffload.pb.o: /home/jmcdowall/local/include/google/protobuf/stubs/common.h
openoffload.pb.o: /usr/include/c++/8/algorithm
openoffload.pb.o: /usr/include/c++/8/bits/stl_algo.h
openoffload.pb.o: /usr/include/c++/8/cstdlib /usr/include/stdlib.h
openoffload.pb.o: /usr/include/bits/waitflags.h
openoffload.pb.o: /usr/include/bits/waitstatus.h /usr/include/alloca.h
openoffload.pb.o: /usr/include/bits/stdlib-float.h
openoffload.pb.o: /usr/include/c++/8/bits/std_abs.h /usr/include/stdlib.h
openoffload.pb.o: /usr/include/c++/8/bits/algorithmfwd.h
openoffload.pb.o: /usr/include/c++/8/bits/stl_heap.h
openoffload.pb.o: /usr/include/c++/8/bits/stl_tempbuf.h
openoffload.pb.o: /usr/include/c++/8/bits/stl_construct.h
openoffload.pb.o: /usr/include/c++/8/iostream /usr/include/c++/8/ostream
openoffload.pb.o: /usr/include/c++/8/ios /usr/include/c++/8/bits/ios_base.h
openoffload.pb.o: /usr/include/c++/8/bits/locale_classes.h
openoffload.pb.o: /usr/include/c++/8/bits/locale_classes.tcc
openoffload.pb.o: /usr/include/c++/8/stdexcept /usr/include/c++/8/streambuf
openoffload.pb.o: /usr/include/c++/8/bits/streambuf.tcc
openoffload.pb.o: /usr/include/c++/8/bits/basic_ios.h
openoffload.pb.o: /usr/include/c++/8/bits/locale_facets.h
openoffload.pb.o: /usr/include/c++/8/cwctype
openoffload.pb.o: /usr/include/c++/8/x86_64-redhat-linux/bits/ctype_base.h
openoffload.pb.o: /usr/include/c++/8/bits/streambuf_iterator.h
openoffload.pb.o: /usr/include/c++/8/x86_64-redhat-linux/bits/ctype_inline.h
openoffload.pb.o: /usr/include/c++/8/bits/locale_facets.tcc
openoffload.pb.o: /usr/include/c++/8/bits/basic_ios.tcc
openoffload.pb.o: /usr/include/c++/8/bits/ostream.tcc
openoffload.pb.o: /usr/include/c++/8/istream
openoffload.pb.o: /usr/include/c++/8/bits/istream.tcc /usr/include/c++/8/map
openoffload.pb.o: /usr/include/c++/8/bits/stl_tree.h
openoffload.pb.o: /usr/include/c++/8/bits/stl_map.h
openoffload.pb.o: /usr/include/c++/8/bits/stl_multimap.h
openoffload.pb.o: /usr/include/c++/8/memory
openoffload.pb.o: /usr/include/c++/8/bits/stl_uninitialized.h
openoffload.pb.o: /usr/include/c++/8/bits/stl_raw_storage_iter.h
openoffload.pb.o: /usr/include/c++/8/backward/auto_ptr.h
openoffload.pb.o: /usr/include/c++/8/set /usr/include/c++/8/bits/stl_set.h
openoffload.pb.o: /usr/include/c++/8/bits/stl_multiset.h
openoffload.pb.o: /usr/include/c++/8/vector
openoffload.pb.o: /usr/include/c++/8/bits/stl_vector.h
openoffload.pb.o: /usr/include/c++/8/bits/stl_bvector.h
openoffload.pb.o: /usr/include/c++/8/bits/vector.tcc
openoffload.pb.o: /home/jmcdowall/local/include/google/protobuf/stubs/port.h
openoffload.pb.o: /usr/include/c++/8/cstdint
openoffload.pb.o: /home/jmcdowall/local/include/google/protobuf/stubs/platform_macros.h
openoffload.pb.o: /usr/include/byteswap.h
openoffload.pb.o: /home/jmcdowall/local/include/google/protobuf/stubs/macros.h
openoffload.pb.o: /home/jmcdowall/local/include/google/protobuf/stubs/logging.h
openoffload.pb.o: /home/jmcdowall/local/include/google/protobuf/stubs/strutil.h
openoffload.pb.o: /home/jmcdowall/local/include/google/protobuf/stubs/stringpiece.h
openoffload.pb.o: /home/jmcdowall/local/include/google/protobuf/stubs/hash.h
openoffload.pb.o: /usr/include/c++/8/unordered_map
openoffload.pb.o: /usr/include/c++/8/unordered_set
openoffload.pb.o: /home/jmcdowall/local/include/google/protobuf/port.h
openoffload.pb.o: /home/jmcdowall/local/include/google/protobuf/arena.h
openoffload.pb.o: /usr/include/c++/8/typeinfo
openoffload.pb.o: /home/jmcdowall/local/include/google/protobuf/arena_impl.h
openoffload.pb.o: /home/jmcdowall/local/include/google/protobuf/arenastring.h
openoffload.pb.o: /home/jmcdowall/local/include/google/protobuf/stubs/fastmem.h
openoffload.pb.o: /usr/include/stdio.h /usr/include/bits/types/__fpos_t.h
openoffload.pb.o: /usr/include/bits/types/__fpos64_t.h
openoffload.pb.o: /usr/include/bits/types/struct_FILE.h
openoffload.pb.o: /usr/include/bits/stdio_lim.h
openoffload.pb.o: /usr/include/bits/sys_errlist.h
openoffload.pb.o: /home/jmcdowall/local/include/google/protobuf/generated_message_table_driven.h
openoffload.pb.o: /home/jmcdowall/local/include/google/protobuf/map.h
openoffload.pb.o: /usr/include/c++/8/initializer_list
openoffload.pb.o: /usr/include/c++/8/iterator
openoffload.pb.o: /usr/include/c++/8/bits/stream_iterator.h
openoffload.pb.o: /home/jmcdowall/local/include/google/protobuf/generated_enum_util.h
openoffload.pb.o: /home/jmcdowall/local/include/google/protobuf/message_lite.h
openoffload.pb.o: /home/jmcdowall/local/include/google/protobuf/metadata_lite.h
openoffload.pb.o: /home/jmcdowall/local/include/google/protobuf/stubs/once.h
openoffload.pb.o: /usr/include/c++/8/mutex
openoffload.pb.o: /home/jmcdowall/local/include/google/protobuf/map_type_handler.h
openoffload.pb.o: /home/jmcdowall/local/include/google/protobuf/parse_context.h
openoffload.pb.o: /home/jmcdowall/local/include/google/protobuf/io/zero_copy_stream.h
openoffload.pb.o: /home/jmcdowall/local/include/google/protobuf/implicit_weak_message.h
openoffload.pb.o: /home/jmcdowall/local/include/google/protobuf/repeated_field.h
openoffload.pb.o: /home/jmcdowall/local/include/google/protobuf/stubs/casts.h
openoffload.pb.o: /home/jmcdowall/local/include/google/protobuf/wire_format_lite.h
openoffload.pb.o: /home/jmcdowall/local/include/google/protobuf/map_entry_lite.h
openoffload.pb.o: /home/jmcdowall/local/include/google/protobuf/generated_message_util.h
openoffload.pb.o: /home/jmcdowall/local/include/google/protobuf/any.h
openoffload.pb.o: /home/jmcdowall/local/include/google/protobuf/has_bits.h
openoffload.pb.o: /home/jmcdowall/local/include/google/protobuf/map_field_lite.h
openoffload.pb.o: /home/jmcdowall/local/include/google/protobuf/inlined_string_field.h
openoffload.pb.o: /home/jmcdowall/local/include/google/protobuf/generated_message_reflection.h
openoffload.pb.o: /home/jmcdowall/local/include/google/protobuf/descriptor.h
openoffload.pb.o: /home/jmcdowall/local/include/google/protobuf/stubs/mutex.h
openoffload.pb.o: /home/jmcdowall/local/include/google/protobuf/generated_enum_reflection.h
openoffload.pb.o: /home/jmcdowall/local/include/google/protobuf/unknown_field_set.h
openoffload.pb.o: /home/jmcdowall/local/include/google/protobuf/io/zero_copy_stream_impl_lite.h
openoffload.pb.o: /home/jmcdowall/local/include/google/protobuf/stubs/callback.h
openoffload.pb.o: /home/jmcdowall/local/include/google/protobuf/stubs/stl_util.h
openoffload.pb.o: /home/jmcdowall/local/include/google/protobuf/message.h
openoffload.pb.o: /home/jmcdowall/local/include/google/protobuf/extension_set.h
openoffload.pb.o: /usr/include/c++/8/cassert
openoffload.pb.o: /home/jmcdowall/local/include/google/protobuf/timestamp.pb.h
openoffload.pb.o: /home/jmcdowall/local/include/google/protobuf/reflection_ops.h
openoffload.pb.o: /home/jmcdowall/local/include/google/protobuf/wire_format.h
openoffload.grpc.pb.o: openoffload.pb.h /usr/include/c++/8/limits
openoffload.grpc.pb.o: /usr/include/c++/8/x86_64-redhat-linux/bits/c++config.h
openoffload.grpc.pb.o: /usr/include/bits/wordsize.h
openoffload.grpc.pb.o: /usr/include/c++/8/x86_64-redhat-linux/bits/os_defines.h
openoffload.grpc.pb.o: /usr/include/features.h /usr/include/stdc-predef.h
openoffload.grpc.pb.o: /usr/include/sys/cdefs.h
openoffload.grpc.pb.o: /usr/include/bits/long-double.h
openoffload.grpc.pb.o: /usr/include/gnu/stubs.h
openoffload.grpc.pb.o: /usr/include/c++/8/x86_64-redhat-linux/bits/cpu_defines.h
openoffload.grpc.pb.o: /usr/include/c++/8/string
openoffload.grpc.pb.o: /usr/include/c++/8/bits/stringfwd.h
openoffload.grpc.pb.o: /usr/include/c++/8/bits/memoryfwd.h
openoffload.grpc.pb.o: /usr/include/c++/8/bits/char_traits.h
openoffload.grpc.pb.o: /usr/include/c++/8/bits/stl_algobase.h
openoffload.grpc.pb.o: /usr/include/c++/8/bits/functexcept.h
openoffload.grpc.pb.o: /usr/include/c++/8/bits/exception_defines.h
openoffload.grpc.pb.o: /usr/include/c++/8/bits/cpp_type_traits.h
openoffload.grpc.pb.o: /usr/include/c++/8/ext/type_traits.h
openoffload.grpc.pb.o: /usr/include/c++/8/ext/numeric_traits.h
openoffload.grpc.pb.o: /usr/include/c++/8/bits/stl_pair.h
openoffload.grpc.pb.o: /usr/include/c++/8/bits/move.h
openoffload.grpc.pb.o: /usr/include/c++/8/bits/concept_check.h
openoffload.grpc.pb.o: /usr/include/c++/8/bits/stl_iterator_base_types.h
openoffload.grpc.pb.o: /usr/include/c++/8/bits/stl_iterator_base_funcs.h
openoffload.grpc.pb.o: /usr/include/c++/8/debug/assertions.h
openoffload.grpc.pb.o: /usr/include/c++/8/bits/stl_iterator.h
openoffload.grpc.pb.o: /usr/include/c++/8/bits/ptr_traits.h
openoffload.grpc.pb.o: /usr/include/c++/8/debug/debug.h
openoffload.grpc.pb.o: /usr/include/c++/8/bits/predefined_ops.h
openoffload.grpc.pb.o: /usr/include/c++/8/bits/postypes.h
openoffload.grpc.pb.o: /usr/include/c++/8/cwchar /usr/include/wchar.h
openoffload.grpc.pb.o: /usr/include/bits/libc-header-start.h
openoffload.grpc.pb.o: /usr/include/bits/floatn.h
openoffload.grpc.pb.o: /usr/include/bits/floatn-common.h
openoffload.grpc.pb.o: /usr/include/linux/stddef.h
openoffload.grpc.pb.o: /usr/lib/gcc/x86_64-redhat-linux/8/include/stdarg.h
openoffload.grpc.pb.o: /usr/include/bits/wchar.h
openoffload.grpc.pb.o: /usr/include/bits/types/wint_t.h
openoffload.grpc.pb.o: /usr/include/bits/types/mbstate_t.h
openoffload.grpc.pb.o: /usr/include/bits/types/__mbstate_t.h
openoffload.grpc.pb.o: /usr/include/bits/types/__FILE.h
openoffload.grpc.pb.o: /usr/include/bits/types/FILE.h
openoffload.grpc.pb.o: /usr/include/bits/types/locale_t.h
openoffload.grpc.pb.o: /usr/include/bits/types/__locale_t.h
openoffload.grpc.pb.o: /usr/include/c++/8/bits/allocator.h
openoffload.grpc.pb.o: /usr/include/c++/8/x86_64-redhat-linux/bits/c++allocator.h
openoffload.grpc.pb.o: /usr/include/c++/8/ext/new_allocator.h
openoffload.grpc.pb.o: /usr/include/c++/8/new /usr/include/c++/8/exception
openoffload.grpc.pb.o: /usr/include/c++/8/bits/exception.h
openoffload.grpc.pb.o: /usr/include/c++/8/bits/localefwd.h
openoffload.grpc.pb.o: /usr/include/c++/8/x86_64-redhat-linux/bits/c++locale.h
openoffload.grpc.pb.o: /usr/include/c++/8/clocale /usr/include/locale.h
openoffload.grpc.pb.o: /usr/include/bits/locale.h /usr/include/c++/8/iosfwd
openoffload.grpc.pb.o: /usr/include/c++/8/cctype /usr/include/ctype.h
openoffload.grpc.pb.o: /usr/include/bits/types.h
openoffload.grpc.pb.o: /usr/include/bits/typesizes.h /usr/include/endian.h
openoffload.grpc.pb.o: /usr/include/bits/endian.h
openoffload.grpc.pb.o: /usr/include/bits/byteswap.h
openoffload.grpc.pb.o: /usr/include/bits/uintn-identity.h
openoffload.grpc.pb.o: /usr/include/c++/8/bits/ostream_insert.h
openoffload.grpc.pb.o: /usr/include/c++/8/bits/cxxabi_forced.h
openoffload.grpc.pb.o: /usr/include/c++/8/bits/stl_function.h
openoffload.grpc.pb.o: /usr/include/c++/8/backward/binders.h
openoffload.grpc.pb.o: /usr/include/c++/8/bits/range_access.h
openoffload.grpc.pb.o: /usr/include/c++/8/bits/basic_string.h
openoffload.grpc.pb.o: /usr/include/c++/8/ext/atomicity.h
openoffload.grpc.pb.o: /usr/include/c++/8/x86_64-redhat-linux/bits/gthr.h
openoffload.grpc.pb.o: /usr/include/c++/8/x86_64-redhat-linux/bits/gthr-default.h
openoffload.grpc.pb.o: /usr/include/pthread.h /usr/include/sched.h
openoffload.grpc.pb.o: /usr/include/bits/types/time_t.h
openoffload.grpc.pb.o: /usr/include/bits/types/struct_timespec.h
openoffload.grpc.pb.o: /usr/include/bits/sched.h
openoffload.grpc.pb.o: /usr/include/bits/types/struct_sched_param.h
openoffload.grpc.pb.o: /usr/include/bits/cpu-set.h /usr/include/time.h
openoffload.grpc.pb.o: /usr/include/bits/time.h
openoffload.grpc.pb.o: /usr/include/bits/types/clock_t.h
openoffload.grpc.pb.o: /usr/include/bits/types/struct_tm.h
openoffload.grpc.pb.o: /usr/include/bits/types/clockid_t.h
openoffload.grpc.pb.o: /usr/include/bits/types/timer_t.h
openoffload.grpc.pb.o: /usr/include/bits/types/struct_itimerspec.h
openoffload.grpc.pb.o: /usr/include/bits/pthreadtypes.h
openoffload.grpc.pb.o: /usr/include/bits/thread-shared-types.h
openoffload.grpc.pb.o: /usr/include/bits/pthreadtypes-arch.h
openoffload.grpc.pb.o: /usr/include/bits/setjmp.h
openoffload.grpc.pb.o: /usr/include/c++/8/x86_64-redhat-linux/bits/atomic_word.h
openoffload.grpc.pb.o: /usr/include/c++/8/ext/alloc_traits.h
openoffload.grpc.pb.o: /usr/include/c++/8/bits/basic_string.tcc
openoffload.grpc.pb.o: /home/jmcdowall/local/include/google/protobuf/port_def.inc
openoffload.grpc.pb.o: /home/jmcdowall/local/include/google/protobuf/port_undef.inc
openoffload.grpc.pb.o: /home/jmcdowall/local/include/google/protobuf/io/coded_stream.h
openoffload.grpc.pb.o: /usr/include/assert.h /usr/include/c++/8/atomic
openoffload.grpc.pb.o: /usr/include/c++/8/bits/c++0x_warning.h
openoffload.grpc.pb.o: /usr/include/c++/8/climits /usr/include/limits.h
openoffload.grpc.pb.o: /usr/include/linux/limits.h
openoffload.grpc.pb.o: /usr/include/bits/posix1_lim.h
openoffload.grpc.pb.o: /usr/include/bits/local_lim.h
openoffload.grpc.pb.o: /usr/include/linux/limits.h
openoffload.grpc.pb.o: /usr/include/bits/posix2_lim.h
openoffload.grpc.pb.o: /usr/include/c++/8/cstddef /usr/include/c++/8/cstring
openoffload.grpc.pb.o: /usr/include/string.h /usr/include/strings.h
openoffload.grpc.pb.o: /usr/include/c++/8/type_traits
openoffload.grpc.pb.o: /usr/include/c++/8/utility
openoffload.grpc.pb.o: /usr/include/c++/8/bits/stl_relops.h
openoffload.grpc.pb.o: /usr/include/sys/param.h /usr/include/sys/types.h
openoffload.grpc.pb.o: /usr/include/bits/stdint-intn.h
openoffload.grpc.pb.o: /usr/include/sys/select.h /usr/include/bits/select.h
openoffload.grpc.pb.o: /usr/include/bits/types/sigset_t.h
openoffload.grpc.pb.o: /usr/include/bits/types/__sigset_t.h
openoffload.grpc.pb.o: /usr/include/bits/types/struct_timeval.h
openoffload.grpc.pb.o: /usr/include/signal.h /usr/include/bits/signum.h
openoffload.grpc.pb.o: /usr/include/bits/signum-generic.h
openoffload.grpc.pb.o: /usr/include/bits/types/sig_atomic_t.h
openoffload.grpc.pb.o: /usr/include/bits/types/siginfo_t.h
openoffload.grpc.pb.o: /usr/include/bits/types/__sigval_t.h
openoffload.grpc.pb.o: /usr/include/bits/siginfo-arch.h
openoffload.grpc.pb.o: /usr/include/bits/siginfo-consts.h
openoffload.grpc.pb.o: /usr/include/bits/types/sigval_t.h
openoffload.grpc.pb.o: /usr/include/bits/types/sigevent_t.h
openoffload.grpc.pb.o: /usr/include/bits/sigevent-consts.h
openoffload.grpc.pb.o: /usr/include/bits/sigaction.h
openoffload.grpc.pb.o: /usr/include/bits/sigcontext.h
openoffload.grpc.pb.o: /usr/include/bits/types/stack_t.h
openoffload.grpc.pb.o: /usr/include/sys/ucontext.h
openoffload.grpc.pb.o: /usr/include/bits/sigstack.h
openoffload.grpc.pb.o: /usr/include/bits/ss_flags.h
openoffload.grpc.pb.o: /usr/include/bits/types/struct_sigstack.h
openoffload.grpc.pb.o: /usr/include/bits/sigthread.h
openoffload.grpc.pb.o: /usr/include/bits/param.h /usr/include/linux/param.h
openoffload.grpc.pb.o: /usr/include/asm/param.h
openoffload.grpc.pb.o: /usr/include/asm-generic/param.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/google/protobuf/stubs/common.h
openoffload.grpc.pb.o: /usr/include/c++/8/algorithm
openoffload.grpc.pb.o: /usr/include/c++/8/bits/stl_algo.h
openoffload.grpc.pb.o: /usr/include/c++/8/cstdlib /usr/include/stdlib.h
openoffload.grpc.pb.o: /usr/include/bits/waitflags.h
openoffload.grpc.pb.o: /usr/include/bits/waitstatus.h /usr/include/alloca.h
openoffload.grpc.pb.o: /usr/include/bits/stdlib-float.h
openoffload.grpc.pb.o: /usr/include/c++/8/bits/std_abs.h
openoffload.grpc.pb.o: /usr/include/stdlib.h
openoffload.grpc.pb.o: /usr/include/c++/8/bits/algorithmfwd.h
openoffload.grpc.pb.o: /usr/include/c++/8/bits/stl_heap.h
openoffload.grpc.pb.o: /usr/include/c++/8/bits/stl_tempbuf.h
openoffload.grpc.pb.o: /usr/include/c++/8/bits/stl_construct.h
openoffload.grpc.pb.o: /usr/include/c++/8/iostream /usr/include/c++/8/ostream
openoffload.grpc.pb.o: /usr/include/c++/8/ios
openoffload.grpc.pb.o: /usr/include/c++/8/bits/ios_base.h
openoffload.grpc.pb.o: /usr/include/c++/8/bits/locale_classes.h
openoffload.grpc.pb.o: /usr/include/c++/8/bits/locale_classes.tcc
openoffload.grpc.pb.o: /usr/include/c++/8/stdexcept
openoffload.grpc.pb.o: /usr/include/c++/8/streambuf
openoffload.grpc.pb.o: /usr/include/c++/8/bits/streambuf.tcc
openoffload.grpc.pb.o: /usr/include/c++/8/bits/basic_ios.h
openoffload.grpc.pb.o: /usr/include/c++/8/bits/locale_facets.h
openoffload.grpc.pb.o: /usr/include/c++/8/cwctype
openoffload.grpc.pb.o: /usr/include/c++/8/x86_64-redhat-linux/bits/ctype_base.h
openoffload.grpc.pb.o: /usr/include/c++/8/bits/streambuf_iterator.h
openoffload.grpc.pb.o: /usr/include/c++/8/x86_64-redhat-linux/bits/ctype_inline.h
openoffload.grpc.pb.o: /usr/include/c++/8/bits/locale_facets.tcc
openoffload.grpc.pb.o: /usr/include/c++/8/bits/basic_ios.tcc
openoffload.grpc.pb.o: /usr/include/c++/8/bits/ostream.tcc
openoffload.grpc.pb.o: /usr/include/c++/8/istream
openoffload.grpc.pb.o: /usr/include/c++/8/bits/istream.tcc
openoffload.grpc.pb.o: /usr/include/c++/8/map
openoffload.grpc.pb.o: /usr/include/c++/8/bits/stl_tree.h
openoffload.grpc.pb.o: /usr/include/c++/8/bits/stl_map.h
openoffload.grpc.pb.o: /usr/include/c++/8/bits/stl_multimap.h
openoffload.grpc.pb.o: /usr/include/c++/8/memory
openoffload.grpc.pb.o: /usr/include/c++/8/bits/stl_uninitialized.h
openoffload.grpc.pb.o: /usr/include/c++/8/bits/stl_raw_storage_iter.h
openoffload.grpc.pb.o: /usr/include/c++/8/backward/auto_ptr.h
openoffload.grpc.pb.o: /usr/include/c++/8/set
openoffload.grpc.pb.o: /usr/include/c++/8/bits/stl_set.h
openoffload.grpc.pb.o: /usr/include/c++/8/bits/stl_multiset.h
openoffload.grpc.pb.o: /usr/include/c++/8/vector
openoffload.grpc.pb.o: /usr/include/c++/8/bits/stl_vector.h
openoffload.grpc.pb.o: /usr/include/c++/8/bits/stl_bvector.h
openoffload.grpc.pb.o: /usr/include/c++/8/bits/vector.tcc
openoffload.grpc.pb.o: /home/jmcdowall/local/include/google/protobuf/stubs/port.h
openoffload.grpc.pb.o: /usr/include/c++/8/cstdint
openoffload.grpc.pb.o: /home/jmcdowall/local/include/google/protobuf/stubs/platform_macros.h
openoffload.grpc.pb.o: /usr/include/byteswap.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/google/protobuf/stubs/macros.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/google/protobuf/stubs/logging.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/google/protobuf/stubs/strutil.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/google/protobuf/stubs/stringpiece.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/google/protobuf/stubs/hash.h
openoffload.grpc.pb.o: /usr/include/c++/8/unordered_map
openoffload.grpc.pb.o: /usr/include/c++/8/unordered_set
openoffload.grpc.pb.o: /home/jmcdowall/local/include/google/protobuf/port.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/google/protobuf/arena.h
openoffload.grpc.pb.o: /usr/include/c++/8/typeinfo
openoffload.grpc.pb.o: /home/jmcdowall/local/include/google/protobuf/arena_impl.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/google/protobuf/arenastring.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/google/protobuf/stubs/fastmem.h
openoffload.grpc.pb.o: /usr/include/stdio.h
openoffload.grpc.pb.o: /usr/include/bits/types/__fpos_t.h
openoffload.grpc.pb.o: /usr/include/bits/types/__fpos64_t.h
openoffload.grpc.pb.o: /usr/include/bits/types/struct_FILE.h
openoffload.grpc.pb.o: /usr/include/bits/stdio_lim.h
openoffload.grpc.pb.o: /usr/include/bits/sys_errlist.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/google/protobuf/generated_message_table_driven.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/google/protobuf/map.h
openoffload.grpc.pb.o: /usr/include/c++/8/initializer_list
openoffload.grpc.pb.o: /usr/include/c++/8/iterator
openoffload.grpc.pb.o: /usr/include/c++/8/bits/stream_iterator.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/google/protobuf/generated_enum_util.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/google/protobuf/message_lite.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/google/protobuf/metadata_lite.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/google/protobuf/stubs/once.h
openoffload.grpc.pb.o: /usr/include/c++/8/mutex
openoffload.grpc.pb.o: /home/jmcdowall/local/include/google/protobuf/map_type_handler.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/google/protobuf/parse_context.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/google/protobuf/io/zero_copy_stream.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/google/protobuf/implicit_weak_message.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/google/protobuf/repeated_field.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/google/protobuf/stubs/casts.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/google/protobuf/wire_format_lite.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/google/protobuf/map_entry_lite.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/google/protobuf/generated_message_util.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/google/protobuf/any.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/google/protobuf/has_bits.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/google/protobuf/map_field_lite.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/google/protobuf/inlined_string_field.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/google/protobuf/generated_message_reflection.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/google/protobuf/descriptor.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/google/protobuf/stubs/mutex.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/google/protobuf/generated_enum_reflection.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/google/protobuf/unknown_field_set.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/google/protobuf/io/zero_copy_stream_impl_lite.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/google/protobuf/stubs/callback.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/google/protobuf/stubs/stl_util.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/google/protobuf/message.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/google/protobuf/extension_set.h
openoffload.grpc.pb.o: /usr/include/c++/8/cassert
openoffload.grpc.pb.o: /home/jmcdowall/local/include/google/protobuf/timestamp.pb.h
openoffload.grpc.pb.o: openoffload.grpc.pb.h /usr/include/c++/8/functional
openoffload.grpc.pb.o: /home/jmcdowall/local/include/grpc/impl/codegen/port_platform.h
openoffload.grpc.pb.o: /usr/include/stdint.h /usr/include/bits/stdint-uintn.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/async_generic_service.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/async_stream_impl.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/call.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/grpc/impl/codegen/grpc_types.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/grpc/impl/codegen/compression_types.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/grpc/impl/codegen/gpr_types.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/grpc/impl/codegen/slice.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/grpc/impl/codegen/gpr_slice.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/grpc/impl/codegen/status.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/call_hook.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/channel_interface.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/grpc/impl/codegen/connectivity_state.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/status.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/config.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/status_code_enum.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/time.h
openoffload.grpc.pb.o: /usr/include/c++/8/chrono
openoffload.grpc.pb.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/core_codegen_interface.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/grpc/impl/codegen/byte_buffer.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/grpc/impl/codegen/byte_buffer_reader.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/grpc/impl/codegen/sync.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/grpc/impl/codegen/sync_generic.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/grpc/impl/codegen/atm.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/server_context_impl.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/call_op_set.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/byte_buffer.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/serialization_traits.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/slice.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/string_ref.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/call_op_set_interface.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/completion_queue_tag.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/client_context_impl.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/grpc/impl/codegen/propagation_bits.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/client_interceptor.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/interceptor.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/metadata_map.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/grpc/impl/codegen/log.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/rpc_method.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/create_auth_context.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/security/auth_context.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/sync.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/completion_queue_impl.h
openoffload.grpc.pb.o: /usr/include/c++/8/list
openoffload.grpc.pb.o: /usr/include/c++/8/bits/stl_list.h
openoffload.grpc.pb.o: /usr/include/c++/8/bits/list.tcc
openoffload.grpc.pb.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/grpc_library.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/intercepted_channel.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/interceptor_common.h
openoffload.grpc.pb.o: /usr/include/c++/8/array
openoffload.grpc.pb.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/server_interceptor.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/callback_common.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/message_allocator.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/server_callback_impl.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/service_type.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/rpc_service_method.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/server_interface.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/server_callback_handlers.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/async_stream.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/async_unary_call.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/async_unary_call_impl.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/client_callback.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/client_callback_impl.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/client_context.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/completion_queue.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/method_handler.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/method_handler_impl.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/sync_stream_impl.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/proto_utils.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/config_protobuf.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/google/protobuf/descriptor.pb.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/google/protobuf/descriptor_database.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/google/protobuf/util/json_util.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/google/protobuf/util/type_resolver.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/google/protobuf/type.pb.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/google/protobuf/any.pb.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/google/protobuf/source_context.pb.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/google/protobuf/stubs/status.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/google/protobuf/stubs/bytestream.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/proto_buffer_reader.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/proto_buffer_writer.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/server_callback.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/server_context.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/stub_options.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/sync_stream.h
openoffload.grpc.pb.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/client_unary_call.h
src/opof_error.o: include/opof_error.h /usr/include/stdio.h
src/opof_error.o: /usr/include/bits/libc-header-start.h
src/opof_error.o: /usr/include/features.h /usr/include/stdc-predef.h
src/opof_error.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
src/opof_error.o: /usr/include/bits/long-double.h /usr/include/gnu/stubs.h
src/opof_error.o: /usr/include/linux/stddef.h
src/opof_error.o: /usr/lib/gcc/x86_64-redhat-linux/8/include/stdarg.h
src/opof_error.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
src/opof_error.o: /usr/include/bits/types/__fpos_t.h
src/opof_error.o: /usr/include/bits/types/__mbstate_t.h
src/opof_error.o: /usr/include/bits/types/__fpos64_t.h
src/opof_error.o: /usr/include/bits/types/__FILE.h
src/opof_error.o: /usr/include/bits/types/FILE.h
src/opof_error.o: /usr/include/bits/types/struct_FILE.h
src/opof_error.o: /usr/include/bits/stdio_lim.h
src/opof_error.o: /usr/include/bits/sys_errlist.h /usr/include/string.h
src/opof_error.o: /usr/include/bits/types/locale_t.h
src/opof_error.o: /usr/include/bits/types/__locale_t.h /usr/include/strings.h
src/opof_error.o: /usr/include/unistd.h /usr/include/bits/posix_opt.h
src/opof_error.o: /usr/include/bits/environments.h
src/opof_error.o: /usr/include/bits/confname.h
src/opof_error.o: /usr/include/bits/getopt_posix.h
src/opof_error.o: /usr/include/bits/getopt_core.h /usr/include/stdlib.h
src/opof_error.o: /usr/include/bits/waitflags.h
src/opof_error.o: /usr/include/bits/waitstatus.h /usr/include/bits/floatn.h
src/opof_error.o: /usr/include/bits/floatn-common.h /usr/include/sys/types.h
src/opof_error.o: /usr/include/bits/types/clock_t.h
src/opof_error.o: /usr/include/bits/types/clockid_t.h
src/opof_error.o: /usr/include/bits/types/time_t.h
src/opof_error.o: /usr/include/bits/types/timer_t.h
src/opof_error.o: /usr/include/bits/stdint-intn.h /usr/include/endian.h
src/opof_error.o: /usr/include/bits/endian.h /usr/include/bits/byteswap.h
src/opof_error.o: /usr/include/bits/uintn-identity.h
src/opof_error.o: /usr/include/sys/select.h /usr/include/bits/select.h
src/opof_error.o: /usr/include/bits/types/sigset_t.h
src/opof_error.o: /usr/include/bits/types/__sigset_t.h
src/opof_error.o: /usr/include/bits/types/struct_timeval.h
src/opof_error.o: /usr/include/bits/types/struct_timespec.h
src/opof_error.o: /usr/include/bits/pthreadtypes.h
src/opof_error.o: /usr/include/bits/thread-shared-types.h
src/opof_error.o: /usr/include/bits/pthreadtypes-arch.h /usr/include/alloca.h
src/opof_error.o: /usr/include/bits/stdlib-float.h /usr/include/syslog.h
src/opof_error.o: /usr/include/sys/syslog.h /usr/include/bits/syslog-path.h
src/opof_error.o: /usr/include/errno.h /usr/include/bits/errno.h
src/opof_error.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
src/opof_error.o: /usr/include/asm-generic/errno.h
src/opof_error.o: /usr/include/asm-generic/errno-base.h /usr/include/signal.h
src/opof_error.o: /usr/include/bits/signum.h
src/opof_error.o: /usr/include/bits/signum-generic.h
src/opof_error.o: /usr/include/bits/types/sig_atomic_t.h
src/opof_error.o: /usr/include/bits/types/siginfo_t.h
src/opof_error.o: /usr/include/bits/types/__sigval_t.h
src/opof_error.o: /usr/include/bits/siginfo-arch.h
src/opof_error.o: /usr/include/bits/siginfo-consts.h
src/opof_error.o: /usr/include/bits/types/sigval_t.h
src/opof_error.o: /usr/include/bits/types/sigevent_t.h
src/opof_error.o: /usr/include/bits/sigevent-consts.h
src/opof_error.o: /usr/include/bits/sigaction.h
src/opof_error.o: /usr/include/bits/sigcontext.h
src/opof_error.o: /usr/include/bits/types/stack_t.h
src/opof_error.o: /usr/include/sys/ucontext.h /usr/include/bits/sigstack.h
src/opof_error.o: /usr/include/bits/ss_flags.h
src/opof_error.o: /usr/include/bits/types/struct_sigstack.h
src/opof_error.o: /usr/include/bits/sigthread.h
src/opof_main.o: /usr/include/stdio.h /usr/include/bits/libc-header-start.h
src/opof_main.o: /usr/include/features.h /usr/include/stdc-predef.h
src/opof_main.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
src/opof_main.o: /usr/include/bits/long-double.h /usr/include/gnu/stubs.h
src/opof_main.o: /usr/include/linux/stddef.h
src/opof_main.o: /usr/lib/gcc/x86_64-redhat-linux/8/include/stdarg.h
src/opof_main.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
src/opof_main.o: /usr/include/bits/types/__fpos_t.h
src/opof_main.o: /usr/include/bits/types/__mbstate_t.h
src/opof_main.o: /usr/include/bits/types/__fpos64_t.h
src/opof_main.o: /usr/include/bits/types/__FILE.h
src/opof_main.o: /usr/include/bits/types/FILE.h
src/opof_main.o: /usr/include/bits/types/struct_FILE.h
src/opof_main.o: /usr/include/bits/stdio_lim.h
src/opof_main.o: /usr/include/bits/sys_errlist.h
src/opof_main.o: /usr/lib/gcc/x86_64-redhat-linux/8/include/stdbool.h
src/opof_main.o: /usr/include/stdlib.h /usr/include/bits/waitflags.h
src/opof_main.o: /usr/include/bits/waitstatus.h /usr/include/bits/floatn.h
src/opof_main.o: /usr/include/bits/floatn-common.h /usr/include/sys/types.h
src/opof_main.o: /usr/include/bits/types/clock_t.h
src/opof_main.o: /usr/include/bits/types/clockid_t.h
src/opof_main.o: /usr/include/bits/types/time_t.h
src/opof_main.o: /usr/include/bits/types/timer_t.h
src/opof_main.o: /usr/include/bits/stdint-intn.h /usr/include/endian.h
src/opof_main.o: /usr/include/bits/endian.h /usr/include/bits/byteswap.h
src/opof_main.o: /usr/include/bits/uintn-identity.h /usr/include/sys/select.h
src/opof_main.o: /usr/include/bits/select.h
src/opof_main.o: /usr/include/bits/types/sigset_t.h
src/opof_main.o: /usr/include/bits/types/__sigset_t.h
src/opof_main.o: /usr/include/bits/types/struct_timeval.h
src/opof_main.o: /usr/include/bits/types/struct_timespec.h
src/opof_main.o: /usr/include/bits/pthreadtypes.h
src/opof_main.o: /usr/include/bits/thread-shared-types.h
src/opof_main.o: /usr/include/bits/pthreadtypes-arch.h /usr/include/alloca.h
src/opof_main.o: /usr/include/bits/stdlib-float.h /usr/include/getopt.h
src/opof_main.o: /usr/include/bits/getopt_core.h
src/opof_main.o: /usr/include/bits/getopt_ext.h /usr/include/time.h
src/opof_main.o: /usr/include/bits/time.h /usr/include/bits/types/struct_tm.h
src/opof_main.o: /usr/include/bits/types/struct_itimerspec.h
src/opof_main.o: /usr/include/bits/types/locale_t.h
src/opof_main.o: /usr/include/bits/types/__locale_t.h /usr/include/string.h
src/opof_main.o: /usr/include/strings.h /usr/include/signal.h
src/opof_main.o: /usr/include/bits/signum.h
src/opof_main.o: /usr/include/bits/signum-generic.h
src/opof_main.o: /usr/include/bits/types/sig_atomic_t.h
src/opof_main.o: /usr/include/bits/types/siginfo_t.h
src/opof_main.o: /usr/include/bits/types/__sigval_t.h
src/opof_main.o: /usr/include/bits/siginfo-arch.h
src/opof_main.o: /usr/include/bits/siginfo-consts.h
src/opof_main.o: /usr/include/bits/types/sigval_t.h
src/opof_main.o: /usr/include/bits/types/sigevent_t.h
src/opof_main.o: /usr/include/bits/sigevent-consts.h
src/opof_main.o: /usr/include/bits/sigaction.h /usr/include/bits/sigcontext.h
src/opof_main.o: /usr/include/bits/types/stack_t.h
src/opof_main.o: /usr/include/sys/ucontext.h /usr/include/bits/sigstack.h
src/opof_main.o: /usr/include/bits/ss_flags.h
src/opof_main.o: /usr/include/bits/types/struct_sigstack.h
src/opof_main.o: /usr/include/bits/sigthread.h /usr/include/syslog.h
src/opof_main.o: /usr/include/sys/syslog.h /usr/include/bits/syslog-path.h
src/opof_main.o: /usr/include/inttypes.h /usr/include/stdint.h
src/opof_main.o: /usr/include/bits/wchar.h /usr/include/bits/stdint-uintn.h
src/opof_main.o: /usr/include/sys/stat.h /usr/include/bits/stat.h
src/opof_main.o: ./include/opof.h ./include/opof_error.h
src/opof_main.o: /usr/include/unistd.h /usr/include/bits/posix_opt.h
src/opof_main.o: /usr/include/bits/environments.h
src/opof_main.o: /usr/include/bits/confname.h
src/opof_main.o: /usr/include/bits/getopt_posix.h /usr/include/errno.h
src/opof_main.o: /usr/include/bits/errno.h /usr/include/linux/errno.h
src/opof_main.o: /usr/include/asm/errno.h /usr/include/asm-generic/errno.h
src/opof_main.o: /usr/include/asm-generic/errno-base.h
src/opof_client.o: ./include/opof.h /usr/include/stdio.h
src/opof_client.o: /usr/include/bits/libc-header-start.h
src/opof_client.o: /usr/include/features.h /usr/include/stdc-predef.h
src/opof_client.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
src/opof_client.o: /usr/include/bits/long-double.h /usr/include/gnu/stubs.h
src/opof_client.o: /usr/include/linux/stddef.h
src/opof_client.o: /usr/lib/gcc/x86_64-redhat-linux/8/include/stdarg.h
src/opof_client.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
src/opof_client.o: /usr/include/bits/types/__fpos_t.h
src/opof_client.o: /usr/include/bits/types/__mbstate_t.h
src/opof_client.o: /usr/include/bits/types/__fpos64_t.h
src/opof_client.o: /usr/include/bits/types/__FILE.h
src/opof_client.o: /usr/include/bits/types/FILE.h
src/opof_client.o: /usr/include/bits/types/struct_FILE.h
src/opof_client.o: /usr/include/bits/stdio_lim.h
src/opof_client.o: /usr/include/bits/sys_errlist.h ./include/opof_error.h
src/opof_client.o: /usr/include/string.h /usr/include/bits/types/locale_t.h
src/opof_client.o: /usr/include/bits/types/__locale_t.h
src/opof_client.o: /usr/include/strings.h /usr/include/unistd.h
src/opof_client.o: /usr/include/bits/posix_opt.h
src/opof_client.o: /usr/include/bits/environments.h
src/opof_client.o: /usr/include/bits/confname.h
src/opof_client.o: /usr/include/bits/getopt_posix.h
src/opof_client.o: /usr/include/bits/getopt_core.h /usr/include/stdlib.h
src/opof_client.o: /usr/include/bits/waitflags.h
src/opof_client.o: /usr/include/bits/waitstatus.h /usr/include/bits/floatn.h
src/opof_client.o: /usr/include/bits/floatn-common.h /usr/include/sys/types.h
src/opof_client.o: /usr/include/bits/types/clock_t.h
src/opof_client.o: /usr/include/bits/types/clockid_t.h
src/opof_client.o: /usr/include/bits/types/time_t.h
src/opof_client.o: /usr/include/bits/types/timer_t.h
src/opof_client.o: /usr/include/bits/stdint-intn.h /usr/include/endian.h
src/opof_client.o: /usr/include/bits/endian.h /usr/include/bits/byteswap.h
src/opof_client.o: /usr/include/bits/uintn-identity.h
src/opof_client.o: /usr/include/sys/select.h /usr/include/bits/select.h
src/opof_client.o: /usr/include/bits/types/sigset_t.h
src/opof_client.o: /usr/include/bits/types/__sigset_t.h
src/opof_client.o: /usr/include/bits/types/struct_timeval.h
src/opof_client.o: /usr/include/bits/types/struct_timespec.h
src/opof_client.o: /usr/include/bits/pthreadtypes.h
src/opof_client.o: /usr/include/bits/thread-shared-types.h
src/opof_client.o: /usr/include/bits/pthreadtypes-arch.h
src/opof_client.o: /usr/include/alloca.h /usr/include/bits/stdlib-float.h
src/opof_client.o: /usr/include/syslog.h /usr/include/sys/syslog.h
src/opof_client.o: /usr/include/bits/syslog-path.h /usr/include/errno.h
src/opof_client.o: /usr/include/bits/errno.h /usr/include/linux/errno.h
src/opof_client.o: /usr/include/asm/errno.h /usr/include/asm-generic/errno.h
src/opof_client.o: /usr/include/asm-generic/errno-base.h
src/opof_client.o: /usr/include/signal.h /usr/include/bits/signum.h
src/opof_client.o: /usr/include/bits/signum-generic.h
src/opof_client.o: /usr/include/bits/types/sig_atomic_t.h
src/opof_client.o: /usr/include/bits/types/siginfo_t.h
src/opof_client.o: /usr/include/bits/types/__sigval_t.h
src/opof_client.o: /usr/include/bits/siginfo-arch.h
src/opof_client.o: /usr/include/bits/siginfo-consts.h
src/opof_client.o: /usr/include/bits/types/sigval_t.h
src/opof_client.o: /usr/include/bits/types/sigevent_t.h
src/opof_client.o: /usr/include/bits/sigevent-consts.h
src/opof_client.o: /usr/include/bits/sigaction.h
src/opof_client.o: /usr/include/bits/sigcontext.h
src/opof_client.o: /usr/include/bits/types/stack_t.h
src/opof_client.o: /usr/include/sys/ucontext.h /usr/include/bits/sigstack.h
src/opof_client.o: /usr/include/bits/ss_flags.h
src/opof_client.o: /usr/include/bits/types/struct_sigstack.h
src/opof_client.o: /usr/include/bits/sigthread.h /usr/include/c++/8/iostream
src/opof_client.o: /usr/include/c++/8/x86_64-redhat-linux/bits/c++config.h
src/opof_client.o: /usr/include/c++/8/x86_64-redhat-linux/bits/os_defines.h
src/opof_client.o: /usr/include/c++/8/x86_64-redhat-linux/bits/cpu_defines.h
src/opof_client.o: /usr/include/c++/8/ostream /usr/include/c++/8/ios
src/opof_client.o: /usr/include/c++/8/iosfwd
src/opof_client.o: /usr/include/c++/8/bits/stringfwd.h
src/opof_client.o: /usr/include/c++/8/bits/memoryfwd.h
src/opof_client.o: /usr/include/c++/8/bits/postypes.h
src/opof_client.o: /usr/include/c++/8/cwchar /usr/include/wchar.h
src/opof_client.o: /usr/include/bits/wchar.h /usr/include/bits/types/wint_t.h
src/opof_client.o: /usr/include/bits/types/mbstate_t.h
src/opof_client.o: /usr/include/c++/8/exception
src/opof_client.o: /usr/include/c++/8/bits/exception.h
src/opof_client.o: /usr/include/c++/8/bits/char_traits.h
src/opof_client.o: /usr/include/c++/8/bits/stl_algobase.h
src/opof_client.o: /usr/include/c++/8/bits/functexcept.h
src/opof_client.o: /usr/include/c++/8/bits/exception_defines.h
src/opof_client.o: /usr/include/c++/8/bits/cpp_type_traits.h
src/opof_client.o: /usr/include/c++/8/ext/type_traits.h
src/opof_client.o: /usr/include/c++/8/ext/numeric_traits.h
src/opof_client.o: /usr/include/c++/8/bits/stl_pair.h
src/opof_client.o: /usr/include/c++/8/bits/move.h
src/opof_client.o: /usr/include/c++/8/bits/concept_check.h
src/opof_client.o: /usr/include/c++/8/bits/stl_iterator_base_types.h
src/opof_client.o: /usr/include/c++/8/bits/stl_iterator_base_funcs.h
src/opof_client.o: /usr/include/c++/8/debug/assertions.h
src/opof_client.o: /usr/include/c++/8/bits/stl_iterator.h
src/opof_client.o: /usr/include/c++/8/bits/ptr_traits.h
src/opof_client.o: /usr/include/c++/8/debug/debug.h
src/opof_client.o: /usr/include/c++/8/bits/predefined_ops.h
src/opof_client.o: /usr/include/c++/8/bits/localefwd.h
src/opof_client.o: /usr/include/c++/8/x86_64-redhat-linux/bits/c++locale.h
src/opof_client.o: /usr/include/c++/8/clocale /usr/include/locale.h
src/opof_client.o: /usr/include/bits/locale.h /usr/include/c++/8/cctype
src/opof_client.o: /usr/include/ctype.h /usr/include/c++/8/bits/ios_base.h
src/opof_client.o: /usr/include/c++/8/ext/atomicity.h
src/opof_client.o: /usr/include/c++/8/x86_64-redhat-linux/bits/gthr.h
src/opof_client.o: /usr/include/c++/8/x86_64-redhat-linux/bits/gthr-default.h
src/opof_client.o: /usr/include/pthread.h /usr/include/sched.h
src/opof_client.o: /usr/include/bits/sched.h
src/opof_client.o: /usr/include/bits/types/struct_sched_param.h
src/opof_client.o: /usr/include/bits/cpu-set.h /usr/include/time.h
src/opof_client.o: /usr/include/bits/time.h
src/opof_client.o: /usr/include/bits/types/struct_tm.h
src/opof_client.o: /usr/include/bits/types/struct_itimerspec.h
src/opof_client.o: /usr/include/bits/setjmp.h
src/opof_client.o: /usr/include/c++/8/x86_64-redhat-linux/bits/atomic_word.h
src/opof_client.o: /usr/include/c++/8/bits/locale_classes.h
src/opof_client.o: /usr/include/c++/8/string
src/opof_client.o: /usr/include/c++/8/bits/allocator.h
src/opof_client.o: /usr/include/c++/8/x86_64-redhat-linux/bits/c++allocator.h
src/opof_client.o: /usr/include/c++/8/ext/new_allocator.h
src/opof_client.o: /usr/include/c++/8/new
src/opof_client.o: /usr/include/c++/8/bits/ostream_insert.h
src/opof_client.o: /usr/include/c++/8/bits/cxxabi_forced.h
src/opof_client.o: /usr/include/c++/8/bits/stl_function.h
src/opof_client.o: /usr/include/c++/8/backward/binders.h
src/opof_client.o: /usr/include/c++/8/bits/range_access.h
src/opof_client.o: /usr/include/c++/8/bits/basic_string.h
src/opof_client.o: /usr/include/c++/8/ext/alloc_traits.h
src/opof_client.o: /usr/include/c++/8/bits/basic_string.tcc
src/opof_client.o: /usr/include/c++/8/bits/locale_classes.tcc
src/opof_client.o: /usr/include/c++/8/stdexcept /usr/include/c++/8/streambuf
src/opof_client.o: /usr/include/c++/8/bits/streambuf.tcc
src/opof_client.o: /usr/include/c++/8/bits/basic_ios.h
src/opof_client.o: /usr/include/c++/8/bits/locale_facets.h
src/opof_client.o: /usr/include/c++/8/cwctype
src/opof_client.o: /usr/include/c++/8/x86_64-redhat-linux/bits/ctype_base.h
src/opof_client.o: /usr/include/c++/8/bits/streambuf_iterator.h
src/opof_client.o: /usr/include/c++/8/x86_64-redhat-linux/bits/ctype_inline.h
src/opof_client.o: /usr/include/c++/8/bits/locale_facets.tcc
src/opof_client.o: /usr/include/c++/8/bits/basic_ios.tcc
src/opof_client.o: /usr/include/c++/8/bits/ostream.tcc
src/opof_client.o: /usr/include/c++/8/istream
src/opof_client.o: /usr/include/c++/8/bits/istream.tcc
src/opof_client.o: /usr/include/c++/8/memory
src/opof_client.o: /usr/include/c++/8/bits/stl_construct.h
src/opof_client.o: /usr/include/c++/8/bits/stl_uninitialized.h
src/opof_client.o: /usr/include/c++/8/bits/stl_tempbuf.h
src/opof_client.o: /usr/include/c++/8/bits/stl_raw_storage_iter.h
src/opof_client.o: /usr/include/c++/8/backward/auto_ptr.h
src/opof_client.o: /usr/include/c++/8/sstream
src/opof_client.o: /usr/include/c++/8/bits/sstream.tcc
src/opof_client.o: /usr/include/c++/8/fstream
src/opof_client.o: /usr/include/c++/8/bits/codecvt.h
src/opof_client.o: /usr/include/c++/8/cstdio
src/opof_client.o: /usr/include/c++/8/x86_64-redhat-linux/bits/basic_file.h
src/opof_client.o: /usr/include/c++/8/x86_64-redhat-linux/bits/c++io.h
src/opof_client.o: /usr/include/c++/8/bits/fstream.tcc
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/grpcpp.h
src/opof_client.o: /home/jmcdowall/local/include/grpc/grpc.h
src/opof_client.o: /home/jmcdowall/local/include/grpc/support/port_platform.h
src/opof_client.o: /home/jmcdowall/local/include/grpc/impl/codegen/port_platform.h
src/opof_client.o: /usr/include/stdint.h /usr/include/bits/stdint-uintn.h
src/opof_client.o: /home/jmcdowall/local/include/grpc/status.h
src/opof_client.o: /home/jmcdowall/local/include/grpc/impl/codegen/status.h
src/opof_client.o: /home/jmcdowall/local/include/grpc/byte_buffer.h
src/opof_client.o: /home/jmcdowall/local/include/grpc/impl/codegen/byte_buffer.h
src/opof_client.o: /home/jmcdowall/local/include/grpc/impl/codegen/grpc_types.h
src/opof_client.o: /home/jmcdowall/local/include/grpc/impl/codegen/compression_types.h
src/opof_client.o: /home/jmcdowall/local/include/grpc/impl/codegen/gpr_types.h
src/opof_client.o: /home/jmcdowall/local/include/grpc/impl/codegen/slice.h
src/opof_client.o: /home/jmcdowall/local/include/grpc/impl/codegen/gpr_slice.h
src/opof_client.o: /home/jmcdowall/local/include/grpc/slice_buffer.h
src/opof_client.o: /home/jmcdowall/local/include/grpc/slice.h
src/opof_client.o: /home/jmcdowall/local/include/grpc/support/sync.h
src/opof_client.o: /home/jmcdowall/local/include/grpc/impl/codegen/sync.h
src/opof_client.o: /home/jmcdowall/local/include/grpc/impl/codegen/sync_generic.h
src/opof_client.o: /home/jmcdowall/local/include/grpc/impl/codegen/atm.h
src/opof_client.o: /home/jmcdowall/local/include/grpc/impl/codegen/connectivity_state.h
src/opof_client.o: /home/jmcdowall/local/include/grpc/impl/codegen/propagation_bits.h
src/opof_client.o: /home/jmcdowall/local/include/grpc/support/time.h
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/channel.h
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/channel_impl.h
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/impl/call.h
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/call.h
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/call_hook.h
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/channel_interface.h
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/status.h
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/config.h
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/status_code_enum.h
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/time.h
src/opof_client.o: /usr/include/c++/8/chrono
src/opof_client.o: /usr/include/c++/8/bits/c++0x_warning.h
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/client_interceptor.h
src/opof_client.o: /usr/include/c++/8/vector
src/opof_client.o: /usr/include/c++/8/bits/stl_vector.h
src/opof_client.o: /usr/include/c++/8/bits/stl_bvector.h
src/opof_client.o: /usr/include/c++/8/bits/vector.tcc
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/interceptor.h
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/byte_buffer.h
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/core_codegen_interface.h
src/opof_client.o: /home/jmcdowall/local/include/grpc/impl/codegen/byte_buffer_reader.h
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/serialization_traits.h
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/slice.h
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/string_ref.h
src/opof_client.o: /usr/include/c++/8/algorithm /usr/include/c++/8/utility
src/opof_client.o: /usr/include/c++/8/bits/stl_relops.h
src/opof_client.o: /usr/include/c++/8/bits/stl_algo.h
src/opof_client.o: /usr/include/c++/8/cstdlib
src/opof_client.o: /usr/include/c++/8/bits/std_abs.h /usr/include/stdlib.h
src/opof_client.o: /usr/include/c++/8/bits/algorithmfwd.h
src/opof_client.o: /usr/include/c++/8/bits/stl_heap.h
src/opof_client.o: /usr/include/c++/8/iterator
src/opof_client.o: /usr/include/c++/8/bits/stream_iterator.h
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/metadata_map.h
src/opof_client.o: /usr/include/c++/8/map /usr/include/c++/8/bits/stl_tree.h
src/opof_client.o: /usr/include/c++/8/bits/stl_map.h
src/opof_client.o: /usr/include/c++/8/bits/stl_multimap.h
src/opof_client.o: /home/jmcdowall/local/include/grpc/impl/codegen/log.h
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/rpc_method.h
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/completion_queue_impl.h
src/opof_client.o: /usr/include/c++/8/list /usr/include/c++/8/bits/stl_list.h
src/opof_client.o: /usr/include/c++/8/bits/list.tcc
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/completion_queue_tag.h
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/grpc_library.h
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/sync.h
src/opof_client.o: /usr/include/c++/8/mutex
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/client_context.h
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/client_context.h
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/client_context_impl.h
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/create_auth_context.h
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/security/auth_context.h
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/completion_queue.h
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/completion_queue.h
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/create_channel.h
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/create_channel_impl.h
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/security/credentials.h
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/security/credentials_impl.h
src/opof_client.o: /home/jmcdowall/local/include/grpc/grpc_security_constants.h
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/security/auth_context.h
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/security/tls_credentials_options.h
src/opof_client.o: /home/jmcdowall/local/include/grpc/support/log.h
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/support/config.h
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/support/channel_arguments_impl.h
src/opof_client.o: /home/jmcdowall/local/include/grpc/compression.h
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/resource_quota.h
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/resource_quota_impl.h
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/support/status.h
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/support/string_ref.h
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/support/channel_arguments.h
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/create_channel_posix.h
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/create_channel_posix_impl.h
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/server.h
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/server_impl.h
src/opof_client.o: /home/jmcdowall/local/include/grpc/support/atm.h
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/completion_queue_impl.h
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/health_check_service_interface.h
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/health_check_service_interface_impl.h
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/server_interface.h
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/interceptor_common.h
src/opof_client.o: /usr/include/c++/8/array /usr/include/c++/8/functional
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/call_op_set_interface.h
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/intercepted_channel.h
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/server_interceptor.h
src/opof_client.o: /usr/include/c++/8/atomic
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/rpc_service_method.h
src/opof_client.o: /usr/include/c++/8/climits /usr/include/limits.h
src/opof_client.o: /usr/include/linux/limits.h /usr/include/bits/posix1_lim.h
src/opof_client.o: /usr/include/bits/local_lim.h /usr/include/linux/limits.h
src/opof_client.o: /usr/include/bits/posix2_lim.h
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/server_context_impl.h
src/opof_client.o: /usr/include/c++/8/cassert /usr/include/assert.h
src/opof_client.o: /usr/include/c++/8/type_traits
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/call_op_set.h
src/opof_client.o: /usr/include/c++/8/cstring
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/callback_common.h
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/message_allocator.h
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/server_callback_impl.h
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/impl/rpc_service_method.h
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/security/server_credentials.h
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/security/server_credentials_impl.h
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/security/auth_metadata_processor.h
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/security/auth_metadata_processor_impl.h
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/server_builder.h
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/server_builder_impl.h
src/opof_client.o: /home/jmcdowall/local/include/grpc/support/cpu.h
src/opof_client.o: /home/jmcdowall/local/include/grpc/support/workaround_list.h
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/impl/channel_argument_option.h
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/impl/server_builder_option.h
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/impl/server_builder_option_impl.h
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/impl/server_builder_plugin.h
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/server_context.h
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/server_context.h
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/server_posix.h
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/server_posix_impl.h
src/opof_client.o: openoffload.grpc.pb.h openoffload.pb.h
src/opof_client.o: /usr/include/c++/8/limits
src/opof_client.o: /home/jmcdowall/local/include/google/protobuf/port_def.inc
src/opof_client.o: /home/jmcdowall/local/include/google/protobuf/port_undef.inc
src/opof_client.o: /home/jmcdowall/local/include/google/protobuf/io/coded_stream.h
src/opof_client.o: /usr/include/c++/8/cstddef /usr/include/sys/param.h
src/opof_client.o: /usr/include/bits/param.h /usr/include/linux/param.h
src/opof_client.o: /usr/include/asm/param.h /usr/include/asm-generic/param.h
src/opof_client.o: /home/jmcdowall/local/include/google/protobuf/stubs/common.h
src/opof_client.o: /usr/include/c++/8/set /usr/include/c++/8/bits/stl_set.h
src/opof_client.o: /usr/include/c++/8/bits/stl_multiset.h
src/opof_client.o: /home/jmcdowall/local/include/google/protobuf/stubs/port.h
src/opof_client.o: /usr/include/c++/8/cstdint
src/opof_client.o: /home/jmcdowall/local/include/google/protobuf/stubs/platform_macros.h
src/opof_client.o: /usr/include/byteswap.h
src/opof_client.o: /home/jmcdowall/local/include/google/protobuf/stubs/macros.h
src/opof_client.o: /home/jmcdowall/local/include/google/protobuf/stubs/logging.h
src/opof_client.o: /home/jmcdowall/local/include/google/protobuf/stubs/strutil.h
src/opof_client.o: /home/jmcdowall/local/include/google/protobuf/stubs/stringpiece.h
src/opof_client.o: /home/jmcdowall/local/include/google/protobuf/stubs/hash.h
src/opof_client.o: /usr/include/c++/8/unordered_map
src/opof_client.o: /usr/include/c++/8/unordered_set
src/opof_client.o: /home/jmcdowall/local/include/google/protobuf/port.h
src/opof_client.o: /home/jmcdowall/local/include/google/protobuf/arena.h
src/opof_client.o: /usr/include/c++/8/typeinfo
src/opof_client.o: /home/jmcdowall/local/include/google/protobuf/arena_impl.h
src/opof_client.o: /home/jmcdowall/local/include/google/protobuf/arenastring.h
src/opof_client.o: /home/jmcdowall/local/include/google/protobuf/stubs/fastmem.h
src/opof_client.o: /home/jmcdowall/local/include/google/protobuf/generated_message_table_driven.h
src/opof_client.o: /home/jmcdowall/local/include/google/protobuf/map.h
src/opof_client.o: /usr/include/c++/8/initializer_list
src/opof_client.o: /home/jmcdowall/local/include/google/protobuf/generated_enum_util.h
src/opof_client.o: /home/jmcdowall/local/include/google/protobuf/message_lite.h
src/opof_client.o: /home/jmcdowall/local/include/google/protobuf/metadata_lite.h
src/opof_client.o: /home/jmcdowall/local/include/google/protobuf/stubs/once.h
src/opof_client.o: /home/jmcdowall/local/include/google/protobuf/map_type_handler.h
src/opof_client.o: /home/jmcdowall/local/include/google/protobuf/parse_context.h
src/opof_client.o: /home/jmcdowall/local/include/google/protobuf/io/zero_copy_stream.h
src/opof_client.o: /home/jmcdowall/local/include/google/protobuf/implicit_weak_message.h
src/opof_client.o: /home/jmcdowall/local/include/google/protobuf/repeated_field.h
src/opof_client.o: /home/jmcdowall/local/include/google/protobuf/stubs/casts.h
src/opof_client.o: /home/jmcdowall/local/include/google/protobuf/wire_format_lite.h
src/opof_client.o: /home/jmcdowall/local/include/google/protobuf/map_entry_lite.h
src/opof_client.o: /home/jmcdowall/local/include/google/protobuf/generated_message_util.h
src/opof_client.o: /home/jmcdowall/local/include/google/protobuf/any.h
src/opof_client.o: /home/jmcdowall/local/include/google/protobuf/has_bits.h
src/opof_client.o: /home/jmcdowall/local/include/google/protobuf/map_field_lite.h
src/opof_client.o: /home/jmcdowall/local/include/google/protobuf/inlined_string_field.h
src/opof_client.o: /home/jmcdowall/local/include/google/protobuf/generated_message_reflection.h
src/opof_client.o: /home/jmcdowall/local/include/google/protobuf/descriptor.h
src/opof_client.o: /home/jmcdowall/local/include/google/protobuf/stubs/mutex.h
src/opof_client.o: /home/jmcdowall/local/include/google/protobuf/generated_enum_reflection.h
src/opof_client.o: /home/jmcdowall/local/include/google/protobuf/unknown_field_set.h
src/opof_client.o: /home/jmcdowall/local/include/google/protobuf/io/zero_copy_stream_impl_lite.h
src/opof_client.o: /home/jmcdowall/local/include/google/protobuf/stubs/callback.h
src/opof_client.o: /home/jmcdowall/local/include/google/protobuf/stubs/stl_util.h
src/opof_client.o: /home/jmcdowall/local/include/google/protobuf/message.h
src/opof_client.o: /home/jmcdowall/local/include/google/protobuf/extension_set.h
src/opof_client.o: /home/jmcdowall/local/include/google/protobuf/timestamp.pb.h
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/async_generic_service.h
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/async_stream_impl.h
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/service_type.h
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/server_callback_handlers.h
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/async_stream.h
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/async_unary_call.h
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/async_unary_call_impl.h
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/client_callback.h
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/client_callback_impl.h
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/method_handler.h
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/method_handler_impl.h
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/sync_stream_impl.h
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/proto_utils.h
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/config_protobuf.h
src/opof_client.o: /home/jmcdowall/local/include/google/protobuf/descriptor.pb.h
src/opof_client.o: /home/jmcdowall/local/include/google/protobuf/descriptor_database.h
src/opof_client.o: /home/jmcdowall/local/include/google/protobuf/util/json_util.h
src/opof_client.o: /home/jmcdowall/local/include/google/protobuf/util/type_resolver.h
src/opof_client.o: /home/jmcdowall/local/include/google/protobuf/type.pb.h
src/opof_client.o: /home/jmcdowall/local/include/google/protobuf/any.pb.h
src/opof_client.o: /home/jmcdowall/local/include/google/protobuf/source_context.pb.h
src/opof_client.o: /home/jmcdowall/local/include/google/protobuf/stubs/status.h
src/opof_client.o: /home/jmcdowall/local/include/google/protobuf/stubs/bytestream.h
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/proto_buffer_reader.h
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/proto_buffer_writer.h
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/server_callback.h
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/stub_options.h
src/opof_client.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/sync_stream.h
src/opof_activation.o: ./include/opof.h /usr/include/stdio.h
src/opof_activation.o: /usr/include/bits/libc-header-start.h
src/opof_activation.o: /usr/include/features.h /usr/include/stdc-predef.h
src/opof_activation.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
src/opof_activation.o: /usr/include/bits/long-double.h
src/opof_activation.o: /usr/include/gnu/stubs.h /usr/include/linux/stddef.h
src/opof_activation.o: /usr/lib/gcc/x86_64-redhat-linux/8/include/stdarg.h
src/opof_activation.o: /usr/include/bits/types.h
src/opof_activation.o: /usr/include/bits/typesizes.h
src/opof_activation.o: /usr/include/bits/types/__fpos_t.h
src/opof_activation.o: /usr/include/bits/types/__mbstate_t.h
src/opof_activation.o: /usr/include/bits/types/__fpos64_t.h
src/opof_activation.o: /usr/include/bits/types/__FILE.h
src/opof_activation.o: /usr/include/bits/types/FILE.h
src/opof_activation.o: /usr/include/bits/types/struct_FILE.h
src/opof_activation.o: /usr/include/bits/stdio_lim.h
src/opof_activation.o: /usr/include/bits/sys_errlist.h ./include/opof_error.h
src/opof_activation.o: /usr/include/string.h
src/opof_activation.o: /usr/include/bits/types/locale_t.h
src/opof_activation.o: /usr/include/bits/types/__locale_t.h
src/opof_activation.o: /usr/include/strings.h /usr/include/unistd.h
src/opof_activation.o: /usr/include/bits/posix_opt.h
src/opof_activation.o: /usr/include/bits/environments.h
src/opof_activation.o: /usr/include/bits/confname.h
src/opof_activation.o: /usr/include/bits/getopt_posix.h
src/opof_activation.o: /usr/include/bits/getopt_core.h /usr/include/stdlib.h
src/opof_activation.o: /usr/include/bits/waitflags.h
src/opof_activation.o: /usr/include/bits/waitstatus.h
src/opof_activation.o: /usr/include/bits/floatn.h
src/opof_activation.o: /usr/include/bits/floatn-common.h
src/opof_activation.o: /usr/include/sys/types.h
src/opof_activation.o: /usr/include/bits/types/clock_t.h
src/opof_activation.o: /usr/include/bits/types/clockid_t.h
src/opof_activation.o: /usr/include/bits/types/time_t.h
src/opof_activation.o: /usr/include/bits/types/timer_t.h
src/opof_activation.o: /usr/include/bits/stdint-intn.h /usr/include/endian.h
src/opof_activation.o: /usr/include/bits/endian.h
src/opof_activation.o: /usr/include/bits/byteswap.h
src/opof_activation.o: /usr/include/bits/uintn-identity.h
src/opof_activation.o: /usr/include/sys/select.h /usr/include/bits/select.h
src/opof_activation.o: /usr/include/bits/types/sigset_t.h
src/opof_activation.o: /usr/include/bits/types/__sigset_t.h
src/opof_activation.o: /usr/include/bits/types/struct_timeval.h
src/opof_activation.o: /usr/include/bits/types/struct_timespec.h
src/opof_activation.o: /usr/include/bits/pthreadtypes.h
src/opof_activation.o: /usr/include/bits/thread-shared-types.h
src/opof_activation.o: /usr/include/bits/pthreadtypes-arch.h
src/opof_activation.o: /usr/include/alloca.h /usr/include/bits/stdlib-float.h
src/opof_activation.o: /usr/include/syslog.h /usr/include/sys/syslog.h
src/opof_activation.o: /usr/include/bits/syslog-path.h /usr/include/errno.h
src/opof_activation.o: /usr/include/bits/errno.h /usr/include/linux/errno.h
src/opof_activation.o: /usr/include/asm/errno.h
src/opof_activation.o: /usr/include/asm-generic/errno.h
src/opof_activation.o: /usr/include/asm-generic/errno-base.h
src/opof_activation.o: /usr/include/signal.h /usr/include/bits/signum.h
src/opof_activation.o: /usr/include/bits/signum-generic.h
src/opof_activation.o: /usr/include/bits/types/sig_atomic_t.h
src/opof_activation.o: /usr/include/bits/types/siginfo_t.h
src/opof_activation.o: /usr/include/bits/types/__sigval_t.h
src/opof_activation.o: /usr/include/bits/siginfo-arch.h
src/opof_activation.o: /usr/include/bits/siginfo-consts.h
src/opof_activation.o: /usr/include/bits/types/sigval_t.h
src/opof_activation.o: /usr/include/bits/types/sigevent_t.h
src/opof_activation.o: /usr/include/bits/sigevent-consts.h
src/opof_activation.o: /usr/include/bits/sigaction.h
src/opof_activation.o: /usr/include/bits/sigcontext.h
src/opof_activation.o: /usr/include/bits/types/stack_t.h
src/opof_activation.o: /usr/include/sys/ucontext.h
src/opof_activation.o: /usr/include/bits/sigstack.h
src/opof_activation.o: /usr/include/bits/ss_flags.h
src/opof_activation.o: /usr/include/bits/types/struct_sigstack.h
src/opof_activation.o: /usr/include/bits/sigthread.h
src/opof_activation.o: /usr/include/c++/8/iostream
src/opof_activation.o: /usr/include/c++/8/x86_64-redhat-linux/bits/c++config.h
src/opof_activation.o: /usr/include/c++/8/x86_64-redhat-linux/bits/os_defines.h
src/opof_activation.o: /usr/include/c++/8/x86_64-redhat-linux/bits/cpu_defines.h
src/opof_activation.o: /usr/include/c++/8/ostream /usr/include/c++/8/ios
src/opof_activation.o: /usr/include/c++/8/iosfwd
src/opof_activation.o: /usr/include/c++/8/bits/stringfwd.h
src/opof_activation.o: /usr/include/c++/8/bits/memoryfwd.h
src/opof_activation.o: /usr/include/c++/8/bits/postypes.h
src/opof_activation.o: /usr/include/c++/8/cwchar /usr/include/wchar.h
src/opof_activation.o: /usr/include/bits/wchar.h
src/opof_activation.o: /usr/include/bits/types/wint_t.h
src/opof_activation.o: /usr/include/bits/types/mbstate_t.h
src/opof_activation.o: /usr/include/c++/8/exception
src/opof_activation.o: /usr/include/c++/8/bits/exception.h
src/opof_activation.o: /usr/include/c++/8/bits/char_traits.h
src/opof_activation.o: /usr/include/c++/8/bits/stl_algobase.h
src/opof_activation.o: /usr/include/c++/8/bits/functexcept.h
src/opof_activation.o: /usr/include/c++/8/bits/exception_defines.h
src/opof_activation.o: /usr/include/c++/8/bits/cpp_type_traits.h
src/opof_activation.o: /usr/include/c++/8/ext/type_traits.h
src/opof_activation.o: /usr/include/c++/8/ext/numeric_traits.h
src/opof_activation.o: /usr/include/c++/8/bits/stl_pair.h
src/opof_activation.o: /usr/include/c++/8/bits/move.h
src/opof_activation.o: /usr/include/c++/8/bits/concept_check.h
src/opof_activation.o: /usr/include/c++/8/bits/stl_iterator_base_types.h
src/opof_activation.o: /usr/include/c++/8/bits/stl_iterator_base_funcs.h
src/opof_activation.o: /usr/include/c++/8/debug/assertions.h
src/opof_activation.o: /usr/include/c++/8/bits/stl_iterator.h
src/opof_activation.o: /usr/include/c++/8/bits/ptr_traits.h
src/opof_activation.o: /usr/include/c++/8/debug/debug.h
src/opof_activation.o: /usr/include/c++/8/bits/predefined_ops.h
src/opof_activation.o: /usr/include/c++/8/bits/localefwd.h
src/opof_activation.o: /usr/include/c++/8/x86_64-redhat-linux/bits/c++locale.h
src/opof_activation.o: /usr/include/c++/8/clocale /usr/include/locale.h
src/opof_activation.o: /usr/include/bits/locale.h /usr/include/c++/8/cctype
src/opof_activation.o: /usr/include/ctype.h
src/opof_activation.o: /usr/include/c++/8/bits/ios_base.h
src/opof_activation.o: /usr/include/c++/8/ext/atomicity.h
src/opof_activation.o: /usr/include/c++/8/x86_64-redhat-linux/bits/gthr.h
src/opof_activation.o: /usr/include/c++/8/x86_64-redhat-linux/bits/gthr-default.h
src/opof_activation.o: /usr/include/pthread.h /usr/include/sched.h
src/opof_activation.o: /usr/include/bits/sched.h
src/opof_activation.o: /usr/include/bits/types/struct_sched_param.h
src/opof_activation.o: /usr/include/bits/cpu-set.h /usr/include/time.h
src/opof_activation.o: /usr/include/bits/time.h
src/opof_activation.o: /usr/include/bits/types/struct_tm.h
src/opof_activation.o: /usr/include/bits/types/struct_itimerspec.h
src/opof_activation.o: /usr/include/bits/setjmp.h
src/opof_activation.o: /usr/include/c++/8/x86_64-redhat-linux/bits/atomic_word.h
src/opof_activation.o: /usr/include/c++/8/bits/locale_classes.h
src/opof_activation.o: /usr/include/c++/8/string
src/opof_activation.o: /usr/include/c++/8/bits/allocator.h
src/opof_activation.o: /usr/include/c++/8/x86_64-redhat-linux/bits/c++allocator.h
src/opof_activation.o: /usr/include/c++/8/ext/new_allocator.h
src/opof_activation.o: /usr/include/c++/8/new
src/opof_activation.o: /usr/include/c++/8/bits/ostream_insert.h
src/opof_activation.o: /usr/include/c++/8/bits/cxxabi_forced.h
src/opof_activation.o: /usr/include/c++/8/bits/stl_function.h
src/opof_activation.o: /usr/include/c++/8/backward/binders.h
src/opof_activation.o: /usr/include/c++/8/bits/range_access.h
src/opof_activation.o: /usr/include/c++/8/bits/basic_string.h
src/opof_activation.o: /usr/include/c++/8/ext/alloc_traits.h
src/opof_activation.o: /usr/include/c++/8/bits/basic_string.tcc
src/opof_activation.o: /usr/include/c++/8/bits/locale_classes.tcc
src/opof_activation.o: /usr/include/c++/8/stdexcept
src/opof_activation.o: /usr/include/c++/8/streambuf
src/opof_activation.o: /usr/include/c++/8/bits/streambuf.tcc
src/opof_activation.o: /usr/include/c++/8/bits/basic_ios.h
src/opof_activation.o: /usr/include/c++/8/bits/locale_facets.h
src/opof_activation.o: /usr/include/c++/8/cwctype
src/opof_activation.o: /usr/include/c++/8/x86_64-redhat-linux/bits/ctype_base.h
src/opof_activation.o: /usr/include/c++/8/bits/streambuf_iterator.h
src/opof_activation.o: /usr/include/c++/8/x86_64-redhat-linux/bits/ctype_inline.h
src/opof_activation.o: /usr/include/c++/8/bits/locale_facets.tcc
src/opof_activation.o: /usr/include/c++/8/bits/basic_ios.tcc
src/opof_activation.o: /usr/include/c++/8/bits/ostream.tcc
src/opof_activation.o: /usr/include/c++/8/istream
src/opof_activation.o: /usr/include/c++/8/bits/istream.tcc
src/opof_activation.o: /usr/include/c++/8/memory
src/opof_activation.o: /usr/include/c++/8/bits/stl_construct.h
src/opof_activation.o: /usr/include/c++/8/bits/stl_uninitialized.h
src/opof_activation.o: /usr/include/c++/8/bits/stl_tempbuf.h
src/opof_activation.o: /usr/include/c++/8/bits/stl_raw_storage_iter.h
src/opof_activation.o: /usr/include/c++/8/backward/auto_ptr.h
src/opof_activation.o: /usr/include/c++/8/sstream
src/opof_activation.o: /usr/include/c++/8/bits/sstream.tcc
src/opof_activation.o: /usr/include/c++/8/fstream
src/opof_activation.o: /usr/include/c++/8/bits/codecvt.h
src/opof_activation.o: /usr/include/c++/8/cstdio
src/opof_activation.o: /usr/include/c++/8/x86_64-redhat-linux/bits/basic_file.h
src/opof_activation.o: /usr/include/c++/8/x86_64-redhat-linux/bits/c++io.h
src/opof_activation.o: /usr/include/c++/8/bits/fstream.tcc
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/grpcpp.h
src/opof_activation.o: /home/jmcdowall/local/include/grpc/grpc.h
src/opof_activation.o: /home/jmcdowall/local/include/grpc/support/port_platform.h
src/opof_activation.o: /home/jmcdowall/local/include/grpc/impl/codegen/port_platform.h
src/opof_activation.o: /usr/include/stdint.h /usr/include/bits/stdint-uintn.h
src/opof_activation.o: /home/jmcdowall/local/include/grpc/status.h
src/opof_activation.o: /home/jmcdowall/local/include/grpc/impl/codegen/status.h
src/opof_activation.o: /home/jmcdowall/local/include/grpc/byte_buffer.h
src/opof_activation.o: /home/jmcdowall/local/include/grpc/impl/codegen/byte_buffer.h
src/opof_activation.o: /home/jmcdowall/local/include/grpc/impl/codegen/grpc_types.h
src/opof_activation.o: /home/jmcdowall/local/include/grpc/impl/codegen/compression_types.h
src/opof_activation.o: /home/jmcdowall/local/include/grpc/impl/codegen/gpr_types.h
src/opof_activation.o: /home/jmcdowall/local/include/grpc/impl/codegen/slice.h
src/opof_activation.o: /home/jmcdowall/local/include/grpc/impl/codegen/gpr_slice.h
src/opof_activation.o: /home/jmcdowall/local/include/grpc/slice_buffer.h
src/opof_activation.o: /home/jmcdowall/local/include/grpc/slice.h
src/opof_activation.o: /home/jmcdowall/local/include/grpc/support/sync.h
src/opof_activation.o: /home/jmcdowall/local/include/grpc/impl/codegen/sync.h
src/opof_activation.o: /home/jmcdowall/local/include/grpc/impl/codegen/sync_generic.h
src/opof_activation.o: /home/jmcdowall/local/include/grpc/impl/codegen/atm.h
src/opof_activation.o: /home/jmcdowall/local/include/grpc/impl/codegen/connectivity_state.h
src/opof_activation.o: /home/jmcdowall/local/include/grpc/impl/codegen/propagation_bits.h
src/opof_activation.o: /home/jmcdowall/local/include/grpc/support/time.h
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/channel.h
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/channel_impl.h
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/impl/call.h
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/call.h
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/call_hook.h
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/channel_interface.h
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/status.h
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/config.h
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/status_code_enum.h
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/time.h
src/opof_activation.o: /usr/include/c++/8/chrono
src/opof_activation.o: /usr/include/c++/8/bits/c++0x_warning.h
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/client_interceptor.h
src/opof_activation.o: /usr/include/c++/8/vector
src/opof_activation.o: /usr/include/c++/8/bits/stl_vector.h
src/opof_activation.o: /usr/include/c++/8/bits/stl_bvector.h
src/opof_activation.o: /usr/include/c++/8/bits/vector.tcc
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/interceptor.h
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/byte_buffer.h
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/core_codegen_interface.h
src/opof_activation.o: /home/jmcdowall/local/include/grpc/impl/codegen/byte_buffer_reader.h
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/serialization_traits.h
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/slice.h
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/string_ref.h
src/opof_activation.o: /usr/include/c++/8/algorithm
src/opof_activation.o: /usr/include/c++/8/utility
src/opof_activation.o: /usr/include/c++/8/bits/stl_relops.h
src/opof_activation.o: /usr/include/c++/8/bits/stl_algo.h
src/opof_activation.o: /usr/include/c++/8/cstdlib
src/opof_activation.o: /usr/include/c++/8/bits/std_abs.h
src/opof_activation.o: /usr/include/stdlib.h
src/opof_activation.o: /usr/include/c++/8/bits/algorithmfwd.h
src/opof_activation.o: /usr/include/c++/8/bits/stl_heap.h
src/opof_activation.o: /usr/include/c++/8/iterator
src/opof_activation.o: /usr/include/c++/8/bits/stream_iterator.h
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/metadata_map.h
src/opof_activation.o: /usr/include/c++/8/map
src/opof_activation.o: /usr/include/c++/8/bits/stl_tree.h
src/opof_activation.o: /usr/include/c++/8/bits/stl_map.h
src/opof_activation.o: /usr/include/c++/8/bits/stl_multimap.h
src/opof_activation.o: /home/jmcdowall/local/include/grpc/impl/codegen/log.h
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/rpc_method.h
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/completion_queue_impl.h
src/opof_activation.o: /usr/include/c++/8/list
src/opof_activation.o: /usr/include/c++/8/bits/stl_list.h
src/opof_activation.o: /usr/include/c++/8/bits/list.tcc
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/completion_queue_tag.h
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/grpc_library.h
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/sync.h
src/opof_activation.o: /usr/include/c++/8/mutex
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/client_context.h
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/client_context.h
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/client_context_impl.h
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/create_auth_context.h
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/security/auth_context.h
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/completion_queue.h
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/completion_queue.h
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/create_channel.h
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/create_channel_impl.h
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/security/credentials.h
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/security/credentials_impl.h
src/opof_activation.o: /home/jmcdowall/local/include/grpc/grpc_security_constants.h
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/security/auth_context.h
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/security/tls_credentials_options.h
src/opof_activation.o: /home/jmcdowall/local/include/grpc/support/log.h
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/support/config.h
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/support/channel_arguments_impl.h
src/opof_activation.o: /home/jmcdowall/local/include/grpc/compression.h
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/resource_quota.h
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/resource_quota_impl.h
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/support/status.h
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/support/string_ref.h
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/support/channel_arguments.h
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/create_channel_posix.h
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/create_channel_posix_impl.h
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/server.h
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/server_impl.h
src/opof_activation.o: /home/jmcdowall/local/include/grpc/support/atm.h
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/completion_queue_impl.h
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/health_check_service_interface.h
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/health_check_service_interface_impl.h
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/server_interface.h
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/interceptor_common.h
src/opof_activation.o: /usr/include/c++/8/array /usr/include/c++/8/functional
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/call_op_set_interface.h
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/intercepted_channel.h
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/server_interceptor.h
src/opof_activation.o: /usr/include/c++/8/atomic
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/rpc_service_method.h
src/opof_activation.o: /usr/include/c++/8/climits /usr/include/limits.h
src/opof_activation.o: /usr/include/linux/limits.h
src/opof_activation.o: /usr/include/bits/posix1_lim.h
src/opof_activation.o: /usr/include/bits/local_lim.h
src/opof_activation.o: /usr/include/linux/limits.h
src/opof_activation.o: /usr/include/bits/posix2_lim.h
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/server_context_impl.h
src/opof_activation.o: /usr/include/c++/8/cassert /usr/include/assert.h
src/opof_activation.o: /usr/include/c++/8/type_traits
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/call_op_set.h
src/opof_activation.o: /usr/include/c++/8/cstring
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/callback_common.h
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/message_allocator.h
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/server_callback_impl.h
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/impl/rpc_service_method.h
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/security/server_credentials.h
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/security/server_credentials_impl.h
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/security/auth_metadata_processor.h
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/security/auth_metadata_processor_impl.h
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/server_builder.h
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/server_builder_impl.h
src/opof_activation.o: /home/jmcdowall/local/include/grpc/support/cpu.h
src/opof_activation.o: /home/jmcdowall/local/include/grpc/support/workaround_list.h
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/impl/channel_argument_option.h
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/impl/server_builder_option.h
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/impl/server_builder_option_impl.h
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/impl/server_builder_plugin.h
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/server_context.h
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/server_context.h
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/server_posix.h
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/server_posix_impl.h
src/opof_activation.o: openoffload.grpc.pb.h openoffload.pb.h
src/opof_activation.o: /usr/include/c++/8/limits
src/opof_activation.o: /home/jmcdowall/local/include/google/protobuf/port_def.inc
src/opof_activation.o: /home/jmcdowall/local/include/google/protobuf/port_undef.inc
src/opof_activation.o: /home/jmcdowall/local/include/google/protobuf/io/coded_stream.h
src/opof_activation.o: /usr/include/c++/8/cstddef /usr/include/sys/param.h
src/opof_activation.o: /usr/include/bits/param.h /usr/include/linux/param.h
src/opof_activation.o: /usr/include/asm/param.h
src/opof_activation.o: /usr/include/asm-generic/param.h
src/opof_activation.o: /home/jmcdowall/local/include/google/protobuf/stubs/common.h
src/opof_activation.o: /usr/include/c++/8/set
src/opof_activation.o: /usr/include/c++/8/bits/stl_set.h
src/opof_activation.o: /usr/include/c++/8/bits/stl_multiset.h
src/opof_activation.o: /home/jmcdowall/local/include/google/protobuf/stubs/port.h
src/opof_activation.o: /usr/include/c++/8/cstdint
src/opof_activation.o: /home/jmcdowall/local/include/google/protobuf/stubs/platform_macros.h
src/opof_activation.o: /usr/include/byteswap.h
src/opof_activation.o: /home/jmcdowall/local/include/google/protobuf/stubs/macros.h
src/opof_activation.o: /home/jmcdowall/local/include/google/protobuf/stubs/logging.h
src/opof_activation.o: /home/jmcdowall/local/include/google/protobuf/stubs/strutil.h
src/opof_activation.o: /home/jmcdowall/local/include/google/protobuf/stubs/stringpiece.h
src/opof_activation.o: /home/jmcdowall/local/include/google/protobuf/stubs/hash.h
src/opof_activation.o: /usr/include/c++/8/unordered_map
src/opof_activation.o: /usr/include/c++/8/unordered_set
src/opof_activation.o: /home/jmcdowall/local/include/google/protobuf/port.h
src/opof_activation.o: /home/jmcdowall/local/include/google/protobuf/arena.h
src/opof_activation.o: /usr/include/c++/8/typeinfo
src/opof_activation.o: /home/jmcdowall/local/include/google/protobuf/arena_impl.h
src/opof_activation.o: /home/jmcdowall/local/include/google/protobuf/arenastring.h
src/opof_activation.o: /home/jmcdowall/local/include/google/protobuf/stubs/fastmem.h
src/opof_activation.o: /home/jmcdowall/local/include/google/protobuf/generated_message_table_driven.h
src/opof_activation.o: /home/jmcdowall/local/include/google/protobuf/map.h
src/opof_activation.o: /usr/include/c++/8/initializer_list
src/opof_activation.o: /home/jmcdowall/local/include/google/protobuf/generated_enum_util.h
src/opof_activation.o: /home/jmcdowall/local/include/google/protobuf/message_lite.h
src/opof_activation.o: /home/jmcdowall/local/include/google/protobuf/metadata_lite.h
src/opof_activation.o: /home/jmcdowall/local/include/google/protobuf/stubs/once.h
src/opof_activation.o: /home/jmcdowall/local/include/google/protobuf/map_type_handler.h
src/opof_activation.o: /home/jmcdowall/local/include/google/protobuf/parse_context.h
src/opof_activation.o: /home/jmcdowall/local/include/google/protobuf/io/zero_copy_stream.h
src/opof_activation.o: /home/jmcdowall/local/include/google/protobuf/implicit_weak_message.h
src/opof_activation.o: /home/jmcdowall/local/include/google/protobuf/repeated_field.h
src/opof_activation.o: /home/jmcdowall/local/include/google/protobuf/stubs/casts.h
src/opof_activation.o: /home/jmcdowall/local/include/google/protobuf/wire_format_lite.h
src/opof_activation.o: /home/jmcdowall/local/include/google/protobuf/map_entry_lite.h
src/opof_activation.o: /home/jmcdowall/local/include/google/protobuf/generated_message_util.h
src/opof_activation.o: /home/jmcdowall/local/include/google/protobuf/any.h
src/opof_activation.o: /home/jmcdowall/local/include/google/protobuf/has_bits.h
src/opof_activation.o: /home/jmcdowall/local/include/google/protobuf/map_field_lite.h
src/opof_activation.o: /home/jmcdowall/local/include/google/protobuf/inlined_string_field.h
src/opof_activation.o: /home/jmcdowall/local/include/google/protobuf/generated_message_reflection.h
src/opof_activation.o: /home/jmcdowall/local/include/google/protobuf/descriptor.h
src/opof_activation.o: /home/jmcdowall/local/include/google/protobuf/stubs/mutex.h
src/opof_activation.o: /home/jmcdowall/local/include/google/protobuf/generated_enum_reflection.h
src/opof_activation.o: /home/jmcdowall/local/include/google/protobuf/unknown_field_set.h
src/opof_activation.o: /home/jmcdowall/local/include/google/protobuf/io/zero_copy_stream_impl_lite.h
src/opof_activation.o: /home/jmcdowall/local/include/google/protobuf/stubs/callback.h
src/opof_activation.o: /home/jmcdowall/local/include/google/protobuf/stubs/stl_util.h
src/opof_activation.o: /home/jmcdowall/local/include/google/protobuf/message.h
src/opof_activation.o: /home/jmcdowall/local/include/google/protobuf/extension_set.h
src/opof_activation.o: /home/jmcdowall/local/include/google/protobuf/timestamp.pb.h
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/async_generic_service.h
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/async_stream_impl.h
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/service_type.h
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/server_callback_handlers.h
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/async_stream.h
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/async_unary_call.h
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/async_unary_call_impl.h
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/client_callback.h
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/client_callback_impl.h
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/method_handler.h
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/method_handler_impl.h
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/sync_stream_impl.h
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/proto_utils.h
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/config_protobuf.h
src/opof_activation.o: /home/jmcdowall/local/include/google/protobuf/descriptor.pb.h
src/opof_activation.o: /home/jmcdowall/local/include/google/protobuf/descriptor_database.h
src/opof_activation.o: /home/jmcdowall/local/include/google/protobuf/util/json_util.h
src/opof_activation.o: /home/jmcdowall/local/include/google/protobuf/util/type_resolver.h
src/opof_activation.o: /home/jmcdowall/local/include/google/protobuf/type.pb.h
src/opof_activation.o: /home/jmcdowall/local/include/google/protobuf/any.pb.h
src/opof_activation.o: /home/jmcdowall/local/include/google/protobuf/source_context.pb.h
src/opof_activation.o: /home/jmcdowall/local/include/google/protobuf/stubs/status.h
src/opof_activation.o: /home/jmcdowall/local/include/google/protobuf/stubs/bytestream.h
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/proto_buffer_reader.h
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/proto_buffer_writer.h
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/server_callback.h
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/stub_options.h
src/opof_activation.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/sync_stream.h
src/opof_session.o: ./include/opof.h /usr/include/stdio.h
src/opof_session.o: /usr/include/bits/libc-header-start.h
src/opof_session.o: /usr/include/features.h /usr/include/stdc-predef.h
src/opof_session.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
src/opof_session.o: /usr/include/bits/long-double.h /usr/include/gnu/stubs.h
src/opof_session.o: /usr/include/linux/stddef.h
src/opof_session.o: /usr/lib/gcc/x86_64-redhat-linux/8/include/stdarg.h
src/opof_session.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
src/opof_session.o: /usr/include/bits/types/__fpos_t.h
src/opof_session.o: /usr/include/bits/types/__mbstate_t.h
src/opof_session.o: /usr/include/bits/types/__fpos64_t.h
src/opof_session.o: /usr/include/bits/types/__FILE.h
src/opof_session.o: /usr/include/bits/types/FILE.h
src/opof_session.o: /usr/include/bits/types/struct_FILE.h
src/opof_session.o: /usr/include/bits/stdio_lim.h
src/opof_session.o: /usr/include/bits/sys_errlist.h ./include/opof_error.h
src/opof_session.o: /usr/include/string.h /usr/include/bits/types/locale_t.h
src/opof_session.o: /usr/include/bits/types/__locale_t.h
src/opof_session.o: /usr/include/strings.h /usr/include/unistd.h
src/opof_session.o: /usr/include/bits/posix_opt.h
src/opof_session.o: /usr/include/bits/environments.h
src/opof_session.o: /usr/include/bits/confname.h
src/opof_session.o: /usr/include/bits/getopt_posix.h
src/opof_session.o: /usr/include/bits/getopt_core.h /usr/include/stdlib.h
src/opof_session.o: /usr/include/bits/waitflags.h
src/opof_session.o: /usr/include/bits/waitstatus.h /usr/include/bits/floatn.h
src/opof_session.o: /usr/include/bits/floatn-common.h
src/opof_session.o: /usr/include/sys/types.h
src/opof_session.o: /usr/include/bits/types/clock_t.h
src/opof_session.o: /usr/include/bits/types/clockid_t.h
src/opof_session.o: /usr/include/bits/types/time_t.h
src/opof_session.o: /usr/include/bits/types/timer_t.h
src/opof_session.o: /usr/include/bits/stdint-intn.h /usr/include/endian.h
src/opof_session.o: /usr/include/bits/endian.h /usr/include/bits/byteswap.h
src/opof_session.o: /usr/include/bits/uintn-identity.h
src/opof_session.o: /usr/include/sys/select.h /usr/include/bits/select.h
src/opof_session.o: /usr/include/bits/types/sigset_t.h
src/opof_session.o: /usr/include/bits/types/__sigset_t.h
src/opof_session.o: /usr/include/bits/types/struct_timeval.h
src/opof_session.o: /usr/include/bits/types/struct_timespec.h
src/opof_session.o: /usr/include/bits/pthreadtypes.h
src/opof_session.o: /usr/include/bits/thread-shared-types.h
src/opof_session.o: /usr/include/bits/pthreadtypes-arch.h
src/opof_session.o: /usr/include/alloca.h /usr/include/bits/stdlib-float.h
src/opof_session.o: /usr/include/syslog.h /usr/include/sys/syslog.h
src/opof_session.o: /usr/include/bits/syslog-path.h /usr/include/errno.h
src/opof_session.o: /usr/include/bits/errno.h /usr/include/linux/errno.h
src/opof_session.o: /usr/include/asm/errno.h /usr/include/asm-generic/errno.h
src/opof_session.o: /usr/include/asm-generic/errno-base.h
src/opof_session.o: /usr/include/signal.h /usr/include/bits/signum.h
src/opof_session.o: /usr/include/bits/signum-generic.h
src/opof_session.o: /usr/include/bits/types/sig_atomic_t.h
src/opof_session.o: /usr/include/bits/types/siginfo_t.h
src/opof_session.o: /usr/include/bits/types/__sigval_t.h
src/opof_session.o: /usr/include/bits/siginfo-arch.h
src/opof_session.o: /usr/include/bits/siginfo-consts.h
src/opof_session.o: /usr/include/bits/types/sigval_t.h
src/opof_session.o: /usr/include/bits/types/sigevent_t.h
src/opof_session.o: /usr/include/bits/sigevent-consts.h
src/opof_session.o: /usr/include/bits/sigaction.h
src/opof_session.o: /usr/include/bits/sigcontext.h
src/opof_session.o: /usr/include/bits/types/stack_t.h
src/opof_session.o: /usr/include/sys/ucontext.h /usr/include/bits/sigstack.h
src/opof_session.o: /usr/include/bits/ss_flags.h
src/opof_session.o: /usr/include/bits/types/struct_sigstack.h
src/opof_session.o: /usr/include/bits/sigthread.h /usr/include/c++/8/iostream
src/opof_session.o: /usr/include/c++/8/x86_64-redhat-linux/bits/c++config.h
src/opof_session.o: /usr/include/c++/8/x86_64-redhat-linux/bits/os_defines.h
src/opof_session.o: /usr/include/c++/8/x86_64-redhat-linux/bits/cpu_defines.h
src/opof_session.o: /usr/include/c++/8/ostream /usr/include/c++/8/ios
src/opof_session.o: /usr/include/c++/8/iosfwd
src/opof_session.o: /usr/include/c++/8/bits/stringfwd.h
src/opof_session.o: /usr/include/c++/8/bits/memoryfwd.h
src/opof_session.o: /usr/include/c++/8/bits/postypes.h
src/opof_session.o: /usr/include/c++/8/cwchar /usr/include/wchar.h
src/opof_session.o: /usr/include/bits/wchar.h
src/opof_session.o: /usr/include/bits/types/wint_t.h
src/opof_session.o: /usr/include/bits/types/mbstate_t.h
src/opof_session.o: /usr/include/c++/8/exception
src/opof_session.o: /usr/include/c++/8/bits/exception.h
src/opof_session.o: /usr/include/c++/8/bits/char_traits.h
src/opof_session.o: /usr/include/c++/8/bits/stl_algobase.h
src/opof_session.o: /usr/include/c++/8/bits/functexcept.h
src/opof_session.o: /usr/include/c++/8/bits/exception_defines.h
src/opof_session.o: /usr/include/c++/8/bits/cpp_type_traits.h
src/opof_session.o: /usr/include/c++/8/ext/type_traits.h
src/opof_session.o: /usr/include/c++/8/ext/numeric_traits.h
src/opof_session.o: /usr/include/c++/8/bits/stl_pair.h
src/opof_session.o: /usr/include/c++/8/bits/move.h
src/opof_session.o: /usr/include/c++/8/bits/concept_check.h
src/opof_session.o: /usr/include/c++/8/bits/stl_iterator_base_types.h
src/opof_session.o: /usr/include/c++/8/bits/stl_iterator_base_funcs.h
src/opof_session.o: /usr/include/c++/8/debug/assertions.h
src/opof_session.o: /usr/include/c++/8/bits/stl_iterator.h
src/opof_session.o: /usr/include/c++/8/bits/ptr_traits.h
src/opof_session.o: /usr/include/c++/8/debug/debug.h
src/opof_session.o: /usr/include/c++/8/bits/predefined_ops.h
src/opof_session.o: /usr/include/c++/8/bits/localefwd.h
src/opof_session.o: /usr/include/c++/8/x86_64-redhat-linux/bits/c++locale.h
src/opof_session.o: /usr/include/c++/8/clocale /usr/include/locale.h
src/opof_session.o: /usr/include/bits/locale.h /usr/include/c++/8/cctype
src/opof_session.o: /usr/include/ctype.h /usr/include/c++/8/bits/ios_base.h
src/opof_session.o: /usr/include/c++/8/ext/atomicity.h
src/opof_session.o: /usr/include/c++/8/x86_64-redhat-linux/bits/gthr.h
src/opof_session.o: /usr/include/c++/8/x86_64-redhat-linux/bits/gthr-default.h
src/opof_session.o: /usr/include/pthread.h /usr/include/sched.h
src/opof_session.o: /usr/include/bits/sched.h
src/opof_session.o: /usr/include/bits/types/struct_sched_param.h
src/opof_session.o: /usr/include/bits/cpu-set.h /usr/include/time.h
src/opof_session.o: /usr/include/bits/time.h
src/opof_session.o: /usr/include/bits/types/struct_tm.h
src/opof_session.o: /usr/include/bits/types/struct_itimerspec.h
src/opof_session.o: /usr/include/bits/setjmp.h
src/opof_session.o: /usr/include/c++/8/x86_64-redhat-linux/bits/atomic_word.h
src/opof_session.o: /usr/include/c++/8/bits/locale_classes.h
src/opof_session.o: /usr/include/c++/8/string
src/opof_session.o: /usr/include/c++/8/bits/allocator.h
src/opof_session.o: /usr/include/c++/8/x86_64-redhat-linux/bits/c++allocator.h
src/opof_session.o: /usr/include/c++/8/ext/new_allocator.h
src/opof_session.o: /usr/include/c++/8/new
src/opof_session.o: /usr/include/c++/8/bits/ostream_insert.h
src/opof_session.o: /usr/include/c++/8/bits/cxxabi_forced.h
src/opof_session.o: /usr/include/c++/8/bits/stl_function.h
src/opof_session.o: /usr/include/c++/8/backward/binders.h
src/opof_session.o: /usr/include/c++/8/bits/range_access.h
src/opof_session.o: /usr/include/c++/8/bits/basic_string.h
src/opof_session.o: /usr/include/c++/8/ext/alloc_traits.h
src/opof_session.o: /usr/include/c++/8/bits/basic_string.tcc
src/opof_session.o: /usr/include/c++/8/bits/locale_classes.tcc
src/opof_session.o: /usr/include/c++/8/stdexcept /usr/include/c++/8/streambuf
src/opof_session.o: /usr/include/c++/8/bits/streambuf.tcc
src/opof_session.o: /usr/include/c++/8/bits/basic_ios.h
src/opof_session.o: /usr/include/c++/8/bits/locale_facets.h
src/opof_session.o: /usr/include/c++/8/cwctype
src/opof_session.o: /usr/include/c++/8/x86_64-redhat-linux/bits/ctype_base.h
src/opof_session.o: /usr/include/c++/8/bits/streambuf_iterator.h
src/opof_session.o: /usr/include/c++/8/x86_64-redhat-linux/bits/ctype_inline.h
src/opof_session.o: /usr/include/c++/8/bits/locale_facets.tcc
src/opof_session.o: /usr/include/c++/8/bits/basic_ios.tcc
src/opof_session.o: /usr/include/c++/8/bits/ostream.tcc
src/opof_session.o: /usr/include/c++/8/istream
src/opof_session.o: /usr/include/c++/8/bits/istream.tcc
src/opof_session.o: /usr/include/c++/8/memory
src/opof_session.o: /usr/include/c++/8/bits/stl_construct.h
src/opof_session.o: /usr/include/c++/8/bits/stl_uninitialized.h
src/opof_session.o: /usr/include/c++/8/bits/stl_tempbuf.h
src/opof_session.o: /usr/include/c++/8/bits/stl_raw_storage_iter.h
src/opof_session.o: /usr/include/c++/8/backward/auto_ptr.h
src/opof_session.o: /usr/include/c++/8/sstream
src/opof_session.o: /usr/include/c++/8/bits/sstream.tcc
src/opof_session.o: /usr/include/c++/8/fstream
src/opof_session.o: /usr/include/c++/8/bits/codecvt.h
src/opof_session.o: /usr/include/c++/8/cstdio
src/opof_session.o: /usr/include/c++/8/x86_64-redhat-linux/bits/basic_file.h
src/opof_session.o: /usr/include/c++/8/x86_64-redhat-linux/bits/c++io.h
src/opof_session.o: /usr/include/c++/8/bits/fstream.tcc
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/grpcpp.h
src/opof_session.o: /home/jmcdowall/local/include/grpc/grpc.h
src/opof_session.o: /home/jmcdowall/local/include/grpc/support/port_platform.h
src/opof_session.o: /home/jmcdowall/local/include/grpc/impl/codegen/port_platform.h
src/opof_session.o: /usr/include/stdint.h /usr/include/bits/stdint-uintn.h
src/opof_session.o: /home/jmcdowall/local/include/grpc/status.h
src/opof_session.o: /home/jmcdowall/local/include/grpc/impl/codegen/status.h
src/opof_session.o: /home/jmcdowall/local/include/grpc/byte_buffer.h
src/opof_session.o: /home/jmcdowall/local/include/grpc/impl/codegen/byte_buffer.h
src/opof_session.o: /home/jmcdowall/local/include/grpc/impl/codegen/grpc_types.h
src/opof_session.o: /home/jmcdowall/local/include/grpc/impl/codegen/compression_types.h
src/opof_session.o: /home/jmcdowall/local/include/grpc/impl/codegen/gpr_types.h
src/opof_session.o: /home/jmcdowall/local/include/grpc/impl/codegen/slice.h
src/opof_session.o: /home/jmcdowall/local/include/grpc/impl/codegen/gpr_slice.h
src/opof_session.o: /home/jmcdowall/local/include/grpc/slice_buffer.h
src/opof_session.o: /home/jmcdowall/local/include/grpc/slice.h
src/opof_session.o: /home/jmcdowall/local/include/grpc/support/sync.h
src/opof_session.o: /home/jmcdowall/local/include/grpc/impl/codegen/sync.h
src/opof_session.o: /home/jmcdowall/local/include/grpc/impl/codegen/sync_generic.h
src/opof_session.o: /home/jmcdowall/local/include/grpc/impl/codegen/atm.h
src/opof_session.o: /home/jmcdowall/local/include/grpc/impl/codegen/connectivity_state.h
src/opof_session.o: /home/jmcdowall/local/include/grpc/impl/codegen/propagation_bits.h
src/opof_session.o: /home/jmcdowall/local/include/grpc/support/time.h
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/channel.h
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/channel_impl.h
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/impl/call.h
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/call.h
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/call_hook.h
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/channel_interface.h
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/status.h
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/config.h
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/status_code_enum.h
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/time.h
src/opof_session.o: /usr/include/c++/8/chrono
src/opof_session.o: /usr/include/c++/8/bits/c++0x_warning.h
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/client_interceptor.h
src/opof_session.o: /usr/include/c++/8/vector
src/opof_session.o: /usr/include/c++/8/bits/stl_vector.h
src/opof_session.o: /usr/include/c++/8/bits/stl_bvector.h
src/opof_session.o: /usr/include/c++/8/bits/vector.tcc
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/interceptor.h
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/byte_buffer.h
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/core_codegen_interface.h
src/opof_session.o: /home/jmcdowall/local/include/grpc/impl/codegen/byte_buffer_reader.h
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/serialization_traits.h
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/slice.h
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/string_ref.h
src/opof_session.o: /usr/include/c++/8/algorithm /usr/include/c++/8/utility
src/opof_session.o: /usr/include/c++/8/bits/stl_relops.h
src/opof_session.o: /usr/include/c++/8/bits/stl_algo.h
src/opof_session.o: /usr/include/c++/8/cstdlib
src/opof_session.o: /usr/include/c++/8/bits/std_abs.h /usr/include/stdlib.h
src/opof_session.o: /usr/include/c++/8/bits/algorithmfwd.h
src/opof_session.o: /usr/include/c++/8/bits/stl_heap.h
src/opof_session.o: /usr/include/c++/8/iterator
src/opof_session.o: /usr/include/c++/8/bits/stream_iterator.h
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/metadata_map.h
src/opof_session.o: /usr/include/c++/8/map /usr/include/c++/8/bits/stl_tree.h
src/opof_session.o: /usr/include/c++/8/bits/stl_map.h
src/opof_session.o: /usr/include/c++/8/bits/stl_multimap.h
src/opof_session.o: /home/jmcdowall/local/include/grpc/impl/codegen/log.h
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/rpc_method.h
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/completion_queue_impl.h
src/opof_session.o: /usr/include/c++/8/list
src/opof_session.o: /usr/include/c++/8/bits/stl_list.h
src/opof_session.o: /usr/include/c++/8/bits/list.tcc
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/completion_queue_tag.h
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/grpc_library.h
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/sync.h
src/opof_session.o: /usr/include/c++/8/mutex
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/client_context.h
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/client_context.h
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/client_context_impl.h
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/create_auth_context.h
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/security/auth_context.h
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/completion_queue.h
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/completion_queue.h
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/create_channel.h
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/create_channel_impl.h
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/security/credentials.h
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/security/credentials_impl.h
src/opof_session.o: /home/jmcdowall/local/include/grpc/grpc_security_constants.h
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/security/auth_context.h
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/security/tls_credentials_options.h
src/opof_session.o: /home/jmcdowall/local/include/grpc/support/log.h
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/support/config.h
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/support/channel_arguments_impl.h
src/opof_session.o: /home/jmcdowall/local/include/grpc/compression.h
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/resource_quota.h
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/resource_quota_impl.h
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/support/status.h
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/support/string_ref.h
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/support/channel_arguments.h
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/create_channel_posix.h
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/create_channel_posix_impl.h
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/server.h
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/server_impl.h
src/opof_session.o: /home/jmcdowall/local/include/grpc/support/atm.h
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/completion_queue_impl.h
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/health_check_service_interface.h
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/health_check_service_interface_impl.h
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/server_interface.h
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/interceptor_common.h
src/opof_session.o: /usr/include/c++/8/array /usr/include/c++/8/functional
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/call_op_set_interface.h
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/intercepted_channel.h
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/server_interceptor.h
src/opof_session.o: /usr/include/c++/8/atomic
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/rpc_service_method.h
src/opof_session.o: /usr/include/c++/8/climits /usr/include/limits.h
src/opof_session.o: /usr/include/linux/limits.h
src/opof_session.o: /usr/include/bits/posix1_lim.h
src/opof_session.o: /usr/include/bits/local_lim.h /usr/include/linux/limits.h
src/opof_session.o: /usr/include/bits/posix2_lim.h
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/server_context_impl.h
src/opof_session.o: /usr/include/c++/8/cassert /usr/include/assert.h
src/opof_session.o: /usr/include/c++/8/type_traits
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/call_op_set.h
src/opof_session.o: /usr/include/c++/8/cstring
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/callback_common.h
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/message_allocator.h
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/server_callback_impl.h
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/impl/rpc_service_method.h
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/security/server_credentials.h
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/security/server_credentials_impl.h
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/security/auth_metadata_processor.h
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/security/auth_metadata_processor_impl.h
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/server_builder.h
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/server_builder_impl.h
src/opof_session.o: /home/jmcdowall/local/include/grpc/support/cpu.h
src/opof_session.o: /home/jmcdowall/local/include/grpc/support/workaround_list.h
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/impl/channel_argument_option.h
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/impl/server_builder_option.h
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/impl/server_builder_option_impl.h
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/impl/server_builder_plugin.h
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/server_context.h
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/server_context.h
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/server_posix.h
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/server_posix_impl.h
src/opof_session.o: openoffload.grpc.pb.h openoffload.pb.h
src/opof_session.o: /usr/include/c++/8/limits
src/opof_session.o: /home/jmcdowall/local/include/google/protobuf/port_def.inc
src/opof_session.o: /home/jmcdowall/local/include/google/protobuf/port_undef.inc
src/opof_session.o: /home/jmcdowall/local/include/google/protobuf/io/coded_stream.h
src/opof_session.o: /usr/include/c++/8/cstddef /usr/include/sys/param.h
src/opof_session.o: /usr/include/bits/param.h /usr/include/linux/param.h
src/opof_session.o: /usr/include/asm/param.h /usr/include/asm-generic/param.h
src/opof_session.o: /home/jmcdowall/local/include/google/protobuf/stubs/common.h
src/opof_session.o: /usr/include/c++/8/set /usr/include/c++/8/bits/stl_set.h
src/opof_session.o: /usr/include/c++/8/bits/stl_multiset.h
src/opof_session.o: /home/jmcdowall/local/include/google/protobuf/stubs/port.h
src/opof_session.o: /usr/include/c++/8/cstdint
src/opof_session.o: /home/jmcdowall/local/include/google/protobuf/stubs/platform_macros.h
src/opof_session.o: /usr/include/byteswap.h
src/opof_session.o: /home/jmcdowall/local/include/google/protobuf/stubs/macros.h
src/opof_session.o: /home/jmcdowall/local/include/google/protobuf/stubs/logging.h
src/opof_session.o: /home/jmcdowall/local/include/google/protobuf/stubs/strutil.h
src/opof_session.o: /home/jmcdowall/local/include/google/protobuf/stubs/stringpiece.h
src/opof_session.o: /home/jmcdowall/local/include/google/protobuf/stubs/hash.h
src/opof_session.o: /usr/include/c++/8/unordered_map
src/opof_session.o: /usr/include/c++/8/unordered_set
src/opof_session.o: /home/jmcdowall/local/include/google/protobuf/port.h
src/opof_session.o: /home/jmcdowall/local/include/google/protobuf/arena.h
src/opof_session.o: /usr/include/c++/8/typeinfo
src/opof_session.o: /home/jmcdowall/local/include/google/protobuf/arena_impl.h
src/opof_session.o: /home/jmcdowall/local/include/google/protobuf/arenastring.h
src/opof_session.o: /home/jmcdowall/local/include/google/protobuf/stubs/fastmem.h
src/opof_session.o: /home/jmcdowall/local/include/google/protobuf/generated_message_table_driven.h
src/opof_session.o: /home/jmcdowall/local/include/google/protobuf/map.h
src/opof_session.o: /usr/include/c++/8/initializer_list
src/opof_session.o: /home/jmcdowall/local/include/google/protobuf/generated_enum_util.h
src/opof_session.o: /home/jmcdowall/local/include/google/protobuf/message_lite.h
src/opof_session.o: /home/jmcdowall/local/include/google/protobuf/metadata_lite.h
src/opof_session.o: /home/jmcdowall/local/include/google/protobuf/stubs/once.h
src/opof_session.o: /home/jmcdowall/local/include/google/protobuf/map_type_handler.h
src/opof_session.o: /home/jmcdowall/local/include/google/protobuf/parse_context.h
src/opof_session.o: /home/jmcdowall/local/include/google/protobuf/io/zero_copy_stream.h
src/opof_session.o: /home/jmcdowall/local/include/google/protobuf/implicit_weak_message.h
src/opof_session.o: /home/jmcdowall/local/include/google/protobuf/repeated_field.h
src/opof_session.o: /home/jmcdowall/local/include/google/protobuf/stubs/casts.h
src/opof_session.o: /home/jmcdowall/local/include/google/protobuf/wire_format_lite.h
src/opof_session.o: /home/jmcdowall/local/include/google/protobuf/map_entry_lite.h
src/opof_session.o: /home/jmcdowall/local/include/google/protobuf/generated_message_util.h
src/opof_session.o: /home/jmcdowall/local/include/google/protobuf/any.h
src/opof_session.o: /home/jmcdowall/local/include/google/protobuf/has_bits.h
src/opof_session.o: /home/jmcdowall/local/include/google/protobuf/map_field_lite.h
src/opof_session.o: /home/jmcdowall/local/include/google/protobuf/inlined_string_field.h
src/opof_session.o: /home/jmcdowall/local/include/google/protobuf/generated_message_reflection.h
src/opof_session.o: /home/jmcdowall/local/include/google/protobuf/descriptor.h
src/opof_session.o: /home/jmcdowall/local/include/google/protobuf/stubs/mutex.h
src/opof_session.o: /home/jmcdowall/local/include/google/protobuf/generated_enum_reflection.h
src/opof_session.o: /home/jmcdowall/local/include/google/protobuf/unknown_field_set.h
src/opof_session.o: /home/jmcdowall/local/include/google/protobuf/io/zero_copy_stream_impl_lite.h
src/opof_session.o: /home/jmcdowall/local/include/google/protobuf/stubs/callback.h
src/opof_session.o: /home/jmcdowall/local/include/google/protobuf/stubs/stl_util.h
src/opof_session.o: /home/jmcdowall/local/include/google/protobuf/message.h
src/opof_session.o: /home/jmcdowall/local/include/google/protobuf/extension_set.h
src/opof_session.o: /home/jmcdowall/local/include/google/protobuf/timestamp.pb.h
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/async_generic_service.h
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/async_stream_impl.h
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/service_type.h
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/server_callback_handlers.h
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/async_stream.h
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/async_unary_call.h
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/async_unary_call_impl.h
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/client_callback.h
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/client_callback_impl.h
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/method_handler.h
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/method_handler_impl.h
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/sync_stream_impl.h
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/proto_utils.h
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/config_protobuf.h
src/opof_session.o: /home/jmcdowall/local/include/google/protobuf/descriptor.pb.h
src/opof_session.o: /home/jmcdowall/local/include/google/protobuf/descriptor_database.h
src/opof_session.o: /home/jmcdowall/local/include/google/protobuf/util/json_util.h
src/opof_session.o: /home/jmcdowall/local/include/google/protobuf/util/type_resolver.h
src/opof_session.o: /home/jmcdowall/local/include/google/protobuf/type.pb.h
src/opof_session.o: /home/jmcdowall/local/include/google/protobuf/any.pb.h
src/opof_session.o: /home/jmcdowall/local/include/google/protobuf/source_context.pb.h
src/opof_session.o: /home/jmcdowall/local/include/google/protobuf/stubs/status.h
src/opof_session.o: /home/jmcdowall/local/include/google/protobuf/stubs/bytestream.h
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/proto_buffer_reader.h
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/proto_buffer_writer.h
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/server_callback.h
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/stub_options.h
src/opof_session.o: /home/jmcdowall/local/include/grpcpp/impl/codegen/sync_stream.h
src/opof_statistics.o: ./include/opof.h /usr/include/stdio.h
src/opof_statistics.o: /usr/include/bits/libc-header-start.h
src/opof_statistics.o: /usr/include/features.h /usr/include/stdc-predef.h
src/opof_statistics.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
src/opof_statistics.o: /usr/include/bits/long-double.h
src/opof_statistics.o: /usr/include/gnu/stubs.h /usr/include/linux/stddef.h
src/opof_statistics.o: /usr/lib/gcc/x86_64-redhat-linux/8/include/stdarg.h
src/opof_statistics.o: /usr/include/bits/types.h
src/opof_statistics.o: /usr/include/bits/typesizes.h
src/opof_statistics.o: /usr/include/bits/types/__fpos_t.h
src/opof_statistics.o: /usr/include/bits/types/__mbstate_t.h
src/opof_statistics.o: /usr/include/bits/types/__fpos64_t.h
src/opof_statistics.o: /usr/include/bits/types/__FILE.h
src/opof_statistics.o: /usr/include/bits/types/FILE.h
src/opof_statistics.o: /usr/include/bits/types/struct_FILE.h
src/opof_statistics.o: /usr/include/bits/stdio_lim.h
src/opof_statistics.o: /usr/include/bits/sys_errlist.h ./include/opof_error.h
src/opof_statistics.o: /usr/include/string.h
src/opof_statistics.o: /usr/include/bits/types/locale_t.h
src/opof_statistics.o: /usr/include/bits/types/__locale_t.h
src/opof_statistics.o: /usr/include/strings.h /usr/include/unistd.h
src/opof_statistics.o: /usr/include/bits/posix_opt.h
src/opof_statistics.o: /usr/include/bits/environments.h
src/opof_statistics.o: /usr/include/bits/confname.h
src/opof_statistics.o: /usr/include/bits/getopt_posix.h
src/opof_statistics.o: /usr/include/bits/getopt_core.h /usr/include/stdlib.h
src/opof_statistics.o: /usr/include/bits/waitflags.h
src/opof_statistics.o: /usr/include/bits/waitstatus.h
src/opof_statistics.o: /usr/include/bits/floatn.h
src/opof_statistics.o: /usr/include/bits/floatn-common.h
src/opof_statistics.o: /usr/include/sys/types.h
src/opof_statistics.o: /usr/include/bits/types/clock_t.h
src/opof_statistics.o: /usr/include/bits/types/clockid_t.h
src/opof_statistics.o: /usr/include/bits/types/time_t.h
src/opof_statistics.o: /usr/include/bits/types/timer_t.h
src/opof_statistics.o: /usr/include/bits/stdint-intn.h /usr/include/endian.h
src/opof_statistics.o: /usr/include/bits/endian.h
src/opof_statistics.o: /usr/include/bits/byteswap.h
src/opof_statistics.o: /usr/include/bits/uintn-identity.h
src/opof_statistics.o: /usr/include/sys/select.h /usr/include/bits/select.h
src/opof_statistics.o: /usr/include/bits/types/sigset_t.h
src/opof_statistics.o: /usr/include/bits/types/__sigset_t.h
src/opof_statistics.o: /usr/include/bits/types/struct_timeval.h
src/opof_statistics.o: /usr/include/bits/types/struct_timespec.h
src/opof_statistics.o: /usr/include/bits/pthreadtypes.h
src/opof_statistics.o: /usr/include/bits/thread-shared-types.h
src/opof_statistics.o: /usr/include/bits/pthreadtypes-arch.h
src/opof_statistics.o: /usr/include/alloca.h /usr/include/bits/stdlib-float.h
src/opof_statistics.o: /usr/include/syslog.h /usr/include/sys/syslog.h
src/opof_statistics.o: /usr/include/bits/syslog-path.h /usr/include/errno.h
src/opof_statistics.o: /usr/include/bits/errno.h /usr/include/linux/errno.h
src/opof_statistics.o: /usr/include/asm/errno.h
src/opof_statistics.o: /usr/include/asm-generic/errno.h
src/opof_statistics.o: /usr/include/asm-generic/errno-base.h
src/opof_statistics.o: /usr/include/signal.h /usr/include/bits/signum.h
src/opof_statistics.o: /usr/include/bits/signum-generic.h
src/opof_statistics.o: /usr/include/bits/types/sig_atomic_t.h
src/opof_statistics.o: /usr/include/bits/types/siginfo_t.h
src/opof_statistics.o: /usr/include/bits/types/__sigval_t.h
src/opof_statistics.o: /usr/include/bits/siginfo-arch.h
src/opof_statistics.o: /usr/include/bits/siginfo-consts.h
src/opof_statistics.o: /usr/include/bits/types/sigval_t.h
src/opof_statistics.o: /usr/include/bits/types/sigevent_t.h
src/opof_statistics.o: /usr/include/bits/sigevent-consts.h
src/opof_statistics.o: /usr/include/bits/sigaction.h
src/opof_statistics.o: /usr/include/bits/sigcontext.h
src/opof_statistics.o: /usr/include/bits/types/stack_t.h
src/opof_statistics.o: /usr/include/sys/ucontext.h
src/opof_statistics.o: /usr/include/bits/sigstack.h
src/opof_statistics.o: /usr/include/bits/ss_flags.h
src/opof_statistics.o: /usr/include/bits/types/struct_sigstack.h
src/opof_statistics.o: /usr/include/bits/sigthread.h

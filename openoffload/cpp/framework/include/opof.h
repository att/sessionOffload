/*
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef OPOF_H
#define OPOF_H

#define SUCCESS 0
#define FAILURE -1

#define BUFFER_MAX 64

#include <sys/socket.h>
#include <netinet/in.h>

typedef enum  { 
  _HOPOPT = 0,
  _TCP = 6,
  _UDP = 17,
} PROTOCOL_ID_T;

typedef enum  {
   _IPV4 = 0,
   _IPV6 = 1,
} IP_VERSION_T;

typedef enum {
   _DROP = 0,
   _FORWARD = 1,
   _MIRROR = 2,
   _SNOOP = 3,
 } ACTION_VALUE_T;

typedef enum {
  _ESTABLISHED = 0,
  _CLOSING_1   = 1,
  _CLOSING_2   = 2,
  _CLOSED      = 3,
  _UNKNOWN_STATE     = 4,
} SESSION_STATE_T;

typedef enum {
  _NOT_CLOSED = 0,
  _FINACK     = 1,
  _RST        = 2,
  _TIMEOUT    = 3,
  _UNKNOWN_CLOSE_CODE    = 4,
} SESSION_CLOSE_T;

typedef enum  {
  /// Not an error; returned on success.
  _OK = 0,

  /// The operation was cancelled (typically by the caller).
  _CANCELLED = 1,

  /// Unknown error. An example of where this error may be returned is if a
  /// Status value received from another address space belongs to an error-space
  /// that is not known in this address space. Also errors raised by APIs that
  /// do not return enough error information may be converted to this error.
  _UNKNOWN = 2,

  /// Client specified an invalid argument. Note that this differs from
  /// FAILED_PRECONDITION. INVALID_ARGUMENT indicates arguments that are
  /// problematic regardless of the state of the system (e.g., a malformed file
  /// name).
  _INVALID_ARGUMENT = 3,

  /// Deadline expired before operation could complete. For operations that
  /// change the state of the system, this error may be returned even if the
  /// operation has completed successfully. For example, a successful response
  /// from a server could have been delayed long enough for the deadline to
  /// expire.
  _DEADLINE_EXCEEDED = 4,

  /// Some requested entity (e.g., file or directory) was not found.
  _NOT_FOUND = 5,

  /// Some entity that we attempted to create (e.g., file or directory) already
  /// exists.
  _ALREADY_EXISTS = 6,

  /// The caller does not have permission to execute the specified operation.
  /// PERMISSION_DENIED must not be used for rejections caused by exhausting
  /// some resource (use RESOURCE_EXHAUSTED instead for those errors).
  /// PERMISSION_DENIED must not be used if the caller can not be identified
  /// (use UNAUTHENTICATED instead for those errors).
  _PERMISSION_DENIED = 7,

  /// The request does not have valid authentication credentials for the
  /// operation.
  _UNAUTHENTICATED = 16,

  /// Some resource has been exhausted, perhaps a per-user quota, or perhaps the
  /// entire file system is out of space.
  _RESOURCE_EXHAUSTED = 8,

  /// Operation was rejected because the system is not in a state required for
  /// the operation's execution. For example, directory to be deleted may be
  /// non-empty, an rmdir operation is applied to a non-directory, etc.
  ///
  /// A litmus test that may help a service implementor in deciding
  /// between FAILED_PRECONDITION, ABORTED, and UNAVAILABLE:
  ///  (a) Use UNAVAILABLE if the client can retry just the failing call.
  ///  (b) Use ABORTED if the client should retry at a higher-level
  ///      (e.g., restarting a read-modify-write sequence).
  ///  (c) Use FAILED_PRECONDITION if the client should not retry until
  ///      the system state has been explicitly fixed. E.g., if an "rmdir"
  ///      fails because the directory is non-empty, FAILED_PRECONDITION
  ///      should be returned since the client should not retry unless
  ///      they have first fixed up the directory by deleting files from it.
  ///  (d) Use FAILED_PRECONDITION if the client performs conditional
  ///      REST Get/Update/Delete on a resource and the resource on the
  ///      server does not match the condition. E.g., conflicting
  ///      read-modify-write on the same resource.
  _FAILED_PRECONDITION = 9,

  /// The operation was aborted, typically due to a concurrency issue like
  /// sequencer check failures, transaction aborts, etc.
  ///
  /// See litmus test above for deciding between FAILED_PRECONDITION, ABORTED,
  /// and UNAVAILABLE.
  _ABORTED = 10,

  /// Operation was attempted past the valid range. E.g., seeking or reading
  /// past end of file.
  ///
  /// Unlike INVALID_ARGUMENT, this error indicates a problem that may be fixed
  /// if the system state changes. For example, a 32-bit file system will
  /// generate INVALID_ARGUMENT if asked to read at an offset that is not in the
  /// range [0,2^32-1], but it will generate OUT_OF_RANGE if asked to read from
  /// an offset past the current file size.
  ///
  /// There is a fair bit of overlap between FAILED_PRECONDITION and
  /// OUT_OF_RANGE. We recommend using OUT_OF_RANGE (the more specific error)
  /// when it applies so that callers who are iterating through a space can
  /// easily look for an OUT_OF_RANGE error to detect when they are done.
  _OUT_OF_RANGE = 11,

  /// Operation is not implemented or not supported/enabled in this service.
  _UNIMPLEMENTED = 12,

  /// Internal errors. Means some invariants expected by underlying System has
  /// been broken. If you see one of these errors, Something is very broken.
  _INTERNAL = 13,

  /// The service is currently unavailable. This is a most likely a transient
  /// condition and may be corrected by retrying with a backoff. Note that it is
  /// not always safe to retry non-idempotent operations.
  ///
  /// \warning Although data MIGHT not have been transmitted when this
  /// status occurs, there is NOT A GUARANTEE that the server has not seen
  /// anything. So in general it is unsafe to retry on this status code
  /// if the call is non-idempotent.
  ///
  /// See litmus test above for deciding between FAILED_PRECONDITION, ABORTED,
  /// and UNAVAILABLE.
  _UNAVAILABLE = 14,

  /// Unrecoverable data loss or corruption.
  _DATA_LOSS = 15,

  /// Force users to include a default branch:
  _DO_NOT_USE = -1
  //
} SESSION_STATUS_T;

typedef enum {
  _SESSION_ACCEPTED = 0,
  _SESSION_REJECTED = 1,
  _SESSION_TABLE_FULL = 2,
  _SESSION_TABLE_UNAVAILABLE =3,
  _SESSION_ALREADY_EXISTS=4,
} ADD_SESSION_STATUS_T;

typedef enum {
  _ACCEPTED = 0,
   _REJECTED = 1,
   _REJECTED_SESSION_NONEXISTENT = 2,
   _REJECTED_SESSION_TABLE_FULL = 3, //[deprecated=true]
   _REJECTED_SESSION_ALREADY_EXISTS = 4, // [deprecated=true]
   _NO_CLOSED_SESSIONS = 5, //[deprecated=true]
   _REJECTED_INTERNAL_ERROR =6,
} REQUEST_STATUS_T;


typedef struct {
  void *handle;
  unsigned int pageSize;
} streamArgs_t;

//typedef struct sessionRequestTuple {
//  int64_t  sessionId;
//} sessionRequest_t;

typedef struct {
  unsigned long sessionId;
  int errorStatus;
} addSessionErrors; 

typedef struct addSessionResponseTuple {
  REQUEST_STATUS_T requestStatus;
  addSessionErrors sessionErrors[BUFFER_MAX];
  int number_errors;
  unsigned long errorStatus;
  //google.protobuf.Timestamp startTime = 3;
} addSessionResponse_t;

typedef struct sessionResponseTuple {
    unsigned long sessionId;
    long  inPackets;
    long  outPackets;
    long  inBytes;
    long  outBytes;
    SESSION_STATE_T sessionState;
    SESSION_CLOSE_T sessionCloseCode;
    REQUEST_STATUS_T requestStatus;
  //google.protobuf.Timestamp startTime = 9;
  //google.protobuf.Timestamp endTime = 10;
} sessionResponse_t;

typedef struct statisticsRequestArgsTuple{
  //  pageSize = 0 will turn off paging
  //  does paging make sense for a stream ?
  //  the client should read/process each event on the stream anyway.
  unsigned int pageSize;
  unsigned int page;
  //  what other arguments make sense for retrieving or filtering streams 
}statisticsRequestArgs_t;

typedef struct sessionRequestTuple {
    unsigned long sessId;
    unsigned int inlif;
    unsigned int outlif;
    struct in_addr srcIP;
    struct in6_addr srcIPV6;
    struct in_addr dstIP;
    struct in6_addr dstIPV6;
    unsigned short srcPort;
    unsigned short dstPort;
    PROTOCOL_ID_T proto;
    IP_VERSION_T ipver;
    ACTION_VALUE_T actType;
    struct in_addr  nextHop;
    struct in6_addr nextHopV6;
    unsigned int cacheTimeout;
} sessionRequest_t;


#endif  /* OPOF_H */
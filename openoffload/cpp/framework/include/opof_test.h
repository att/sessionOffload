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
#ifndef OPOF_TEST_H
#define OPOF_TEST_H

/**
* \defgroup testlibrary
*
* \brief gRPC Test Library for C/C++
*
* The test library is a set of code written using the C Interfaces for OpenOffload gRPC. The
* intention is to provide a test framework to test the underlying client/server code and test
* performance.
*/
#define OPOF_VERSION "v1alpha5"
#define LOG_DIR "log/opof"
#define LOG_FILE "log/opof/opof.log"
#define CERT_FILE "ssl/server.crt"
#define KEY_FILE "ssl/server.key"

#endif  /* OPOF_TEST_H */
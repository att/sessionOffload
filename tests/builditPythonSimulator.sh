#============LICENSE_START=============================================================================================================
# Copyright (C) 2020 AT&T Intellectual Property. All rights reserved.
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
targetdir=buildPythonSimulator
if [ ! -d "$targetdir/ssl" ]; then
    mkdir -p $targetdir/ssl
fi
cd $targetdir
# copy the client and server python scripts
cp ../pythonSimulator/*.py .
# copy the client and server start scripts
cp ../pythonSimulator/*.sh .
# copy the ssl cert and key
cd ssl
cp ../../basic/ssl/* .
# build the libraries from proto buf files
cd ..
python3 -m grpc_tools.protoc -I../../protos --python_out=. --grpc_python_out=. ../../protos/openoffload.proto

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

#The following self-signed certificates were created for testing only.
#You can replace them with your own if appropriate using this process or official certificates using what ever process is appropriate.


# create key and certificate files
# it will prompt for Country, location and importanly CN or Common Name/ FQDN
# Enter 'localhost' to set  CN=localhost so that the client session to the server
# matches the certificate
openssl req -newkey rsa:2048 -nodes -keyout server.key -x509 -days 3650 -out server.crt



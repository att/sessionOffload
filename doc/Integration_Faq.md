# Open Offload Integration FAQ

## Introduction
This is not intended to be a comprehensive list of answers, but rather a compendium of questions and answers to help prospective developers implement a working open offload server.

## FAQ

**Question:** Is there any examples of an open offload server that can be used get started?

**Answer:** Yes, there are a few implementations that are available.

*Python:* There is a test Python implementation in this github repository under the test directory.

*C/C++:* There is a sample C++ implementation in this github repository under the openoffload directory.

*C Implementation:* Nivdia has kindly put their implementation in github under open source at: [Firewall Offload]https://github.com/BodongWang/firewall_offload

**Question:** Can multiple VMs talking to the same SmartNIC be supported?

**Answer:** Currently the implementation is focused on a single VM supporting a single SmartNIC. However more deployment models could be supported depending on customer use cases.

**Question:** What is the impact/use of inLIF/outLIF for SmartNIC deployments?

**Answer:** The inLIF/outLIF parameters are just placeholders for the future network router case. They can be just hardwired to 1 and 2 for the basic SmartNIC case.

**Question:** Are there any management functions in the gRPC API?

**Answer:** Not currently but this is something we would like to add and are actively soliciting feedback and suggestions.

**Question:** Does the current implementation support IPv6?

**Answer:** Yes the current implementation supports IPv6

**Question:** Does the current implementation support VLANs?

**Answer:** Yes the current implementation supports VLANs

**Question:** Can the current implementation support High Availability scenarios?

**Answer:** Yes the current implementation supports High Availability Scenarios.

**Question:** Where is the TCP State managed?

**Answer:** All TCP state is managed by the firewall, as there are several TCP attack vectors that the firewall needs to protect against and it is easier if there is only one entity managing state. Therefore all all TCP control packets must be sent to the firewall and the offload device should not act on them.

**Question:** Is there a preferred implementation technology for the offload server?

**Answer:** Providing the server implements the gRPC API to the offload client the implementation approach and technology is left open to the developers of the offload device.

**Question:** Can other applications beside firewall use the API?

**Answer:** Yes. Any application that can use the match/action parameters in the API can use the API for session offload.

**Question:** Are additional offload types like UPF accelleration supported?

**Answer:** Not at this time. The plan is to extend the API over time to handle more use cases through extenstion to the match/action parameters. We encourage suggestions on additional  offload use cases.

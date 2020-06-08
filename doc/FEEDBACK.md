# Feedback on offload API

## General Comments

1. Separate statistics on session and session state changess
2. Action type add Allow. There are three states for shared sessions
    **Allow:** Traffic has been inspected and all future packets for this session should be forwarded by the offload. 
    **Drop:** Traffic has been inspected and all future packets for this session should be dropped by the offload.
    **Inspect:** Continue sending packets for this session to the firewall.
3.  Need two streams one for statistics and one for session changes
4.  Agree on License type for code, think Apache is fine just want to make sure we formally agree.
5.  Align session states with firewall session states (see Reference 2)
6.  Shared key between the firewall and offload to ensure that they are talking about the same session.
7.  Authentication of APIs and general security issues?
8.  API Versioning 


# Todos

- [ ] Modify sequence diagrams to match the gRPC calls
- [ ] Performance tests on session state flows (Assume C++ to C++ for best performance?)
- [ ] Performance tests on session statistics
- [ ] Error handling 

# References
1. [PACKET FLOW SEQUENCE IN PAN-OS](https://knowledgebase.paloaltonetworks.com/KCSArticleDetail?id=kA10g000000ClVHCA0)
2. [PALO ALTO NETWORKS FIREWALL SESSION OVERVIEW](https://knowledgebase.paloaltonetworks.com/KCSArticleDetail?id=kA10g000000ClVECA0)
3. [gRPC Performance - appears to be WIP](https://grpc.io/docs/guides/benchmarking)

Draft Open Offload Geneve Format
Problem Statement
To steer traffic to a NGFW for OpenOffload without using routing it is easiest to encapsulate the original packet and deliver it to the firewall as an encapsulated packet. This preserves the original source/destination in the inner packet/flow for application of policy. In addition the ingress and egress interfaces on the router/switch can be included in the metadata. This enables the NGFW to apply zone based policy.
Requirements
Requirements for the tunneling protocol
The encapsulation protocol SHOULD be an IETF standard.
The encapsulation protocol MUST enable the inner flows to be delivered to the firewall unchanged for classification and inspection.
The encapsulation protocol SHOULD be defined as a fixed header format to make inspection efficient by software and hardware.
The encapsulation protocol SHOULD have an identifier to enable the firewall to identify that it is a specific encapsulation format.
The encapsulation protocol MUST be extensible to carry additional metadata such as zone information (ingress/egress interface).
The encapsulation protocol MUST support IPV6 and IPV4.
Discussion
Three possibilities for the encapsulation format were considered.
VXLAN 
VXLAN while a popular standard does not have any extension mechanisms, VXLAN-GPE is designed to enable other embedded protocols, the only one currently defined is VXLAN-GPE with NSH. NSH is designed for service chaining and as such is fairly complex.
Geneve 
Geneve is  another option, it is designed to be a flexible protocol with the ability to register a fixed format with ICAN. From the Palo Alto Networks POV we have Geneve already implemented for NSX-T and other upcoming deliverables, so the cost and time to implement it would be low. Its extensibility allows the definition of a new protocol while using a standard header and well known port. The extensions can be registered with ICAN to identify a specific set of extensions.
Custom Protocol
A custom protocol was considered and evaluated but after developing it, there were not a lot of differences between the custom protocol and the Geneve extensions. The custom protocol could have used fewer bits, but the required information required was the same.
Comparisons
While all three approaches would work, VXLAN-GPE was discarded as it would have required defining a new embedded protocol or trying to make NSH work. The custom protocol is more compact but had a similar disadvantage of defining a new proprietary protocol. Geneve was selected as it was sufficiently expressive and the format could be fixed to make it simpler for hardware to process
Proposed Geneve Format
This section describes the proposed Geneve header for OpenOffload. The standard Geneve header fields are not defined here as they are already well defined in Reference 1.

The diagram below shows the proposed format for the fixed length Geneve Header. There are 128 bits of option fields defined. 


The fields are as follows:

In-Port: Physical Port on Jericho??

Out-Port:

In-LIF: A 32 identifier defining the logical interface for incoming traffic. This value is set by the transmitting network element. The receiver of the encapsulated packet uses the identifier to map to a policy element to be applied to the packet.

Out-LIF: A 32 identifier defining the logical interface for outgoing traffic. This value is set by the transmitting network element. The receiver of the encapsulated packet uses the identifier to map to a policy element to be applied to the packet.

Hardware Key: This a unique 32 bit hardware key set by the transmitting network element and is not used by the receiving service. 

For this Geneve extension there will be a new Option Class requested. The current Option Classes are listed in Reference 2 and the form to request a new Option Class is in Reference 3. 
Optimizations
Both GPE and Geneve have the option of encapsulating the original packet to either include the Ethernet header or exclude it and encapsulate the original packet starting at the IP header. Palo Alto Networks can handle either implementation providing the encapsulation header is set correctly. Just encapsulating the packet at the IP header will save 14 bytes, however the choice of implementation is left to the hardware as the cost and complexity of the encapsulation is most likely the highest impact, rather than the bandwidth.

References
Geneve: Generic Network Virtualization Encapsulation
IANA Network Virtualization Overlay (NVO3) Assignments
Form to Request an Assignment from IANA for NVO3



# Implementation Guide SmartNIC

## Introduction
This is a guide to implement a generic OpenOffload solution using the gRPC code provided in this repository. 

### Assumptions
This guide assumes there is a virtual network function (VNF) connected to a SmartNIC. The VNF is responsible for maintaining its own flow table of all sessions, and depending on internal policy, which is invisible to the SmartNIC, instructing the SmartNIC to perform actions on the corresponding flows. The initial actions defined in the  gRPC module are:

- DROP: Drop all packets in this session.
- FORWARD: Forward (allow) all packets in this session to go to final destination.
- MIRROR: Create a copy of the packet and send to a traffic collector.
- SNOOP: Apply policy and send copy of packet to a collector.

## Sequence of Operations

The following is the sequence of operations for OpenOffload.

### Register
The VNF implements the server side of the Activatation service. It is assumed that in the future a VNF may have more than one OpenOffload device registered. 

The goal of the registration process is to provide the VNF with information to securely connect to the OpenOffload device and to provide the VNF administrator with some metadata about the vendor, device type and capabilities. No checking is done on the metadata as it is only used as a guide to the admin. The information about how to securely connect to the device is used in the activation phase.

### Activate
From the available OpenOffload devices registered the VNF administrator can select one to be activated. For version 1 the number of devices that can be activated by a VNF is limited to one, this may be changed in future versions.

Once the device has been activated it uses the information supplied in the registration process to securely connect to the OpenOffload device.

### Add Session
Once the device is activated the VNF can decide to offload flows to the OpenOffload device. To do this the VNF sends an addSession message to the activsated OpenOffload device. At this point the VNF should not see any payload packets from the OpenOffload device, all packets for the session should be handled by the offload device.

This message is delivered as a RPC so the VNF will get a synchronous response with a status code.

### Get Closed Sessions
When the OpenOffload device determines that the session is complete, either TCP control flag or timeout, it will send the session informationa and session statistics over the getClosedSession stream to the VNF. The VNF is listening on this rRPC channel all the time and there is no positive acknoledgement to minimize overhead.


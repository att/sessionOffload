# OpenOffload API Versioning

## Introduction
OpenOffload follows a similar versioning model for its gRPC API as Kubernetes, see Reference 1. The intent is to enable teh evolution of the OpenOffload API while enabling developers of OpenOffload devices to have a stable APi to develop and deliver against.

The naming convention will be as follows:

**Alpha** /openoffload/v1/alpha1, this would be the first alpha of API version 1.
**Beta** /openoffload/v1/beta1, this would be the first beta of API version 1
**Release** /openoffload/v1, this would be the released and supported of API version 1

## Mapping to Product Release Versions
OpenOffload device developers are free to select any released version for their product. They are not required to use the most current version just the version that has the necessary features for their product.

## Version Definitions

### Alpha Version
Alpha versions are by definition unstable and the API definitions can change between versions.
- No guarentees of API stability (minimal testing)
- No guarentees features will be supported moving forward.
- Goal of Alpha is to solicit feedback, which may result in significant changes to API.
### Beta Version
Beta verions have been tested and the API/features reviewed.
- A Beta version implies that the API/Features will be delivered.
- Only minor changes (bug fixes) to the API/Feature will be committed.
### Release Version
Released versions will be supported for a specific period or time (or number of release versions).
- Released versions will never change the interface, bugs will be fixed and the versions will be incremented.
- Release versions will always be backwards compatible. 

## References

1. [Kubernetes API and Versioning](https://kubernetes.io/docs/concepts/overview/kubernetes-api/)


## Todo
- [ ] Define the support timeframe for released APIs 
- [ ] Bug fixing strategy and versioning

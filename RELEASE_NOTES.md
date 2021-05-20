# Open Offload Release Notes

## Supported Operating Systems

* Centos 8
* Ubuntu 18.04

## Release v1

-  Promote v1beta1 to v1 as the first official release.
-  Add Integration FAQ to help hardware developers understand details of the solution


## Release v1beta1

### Bug Fixes

- For python tests, updated the ssl certificate since the original had expired. Changed expiry to 10 years.

### Feature Changes

- Removed Activation from protocol buffer defininition and reference implementations.  During implemenation a better approach was 
  identofied and will be added in a later release. Config will be used until the new Activation approach is re-implemented.

### Major Protocol Changes

- Renamed statisticsRequestArgs to sessionRequestArgs
- Renamed sessionResponseArray to sessionResponses
- Removed Activation messages


## Release v1alpha5

### Bug Fixes

- Fixed bugs in C/C++ code in opof_get_closed_sessions

### Feature Changes

- Add more test cases in C/C++ code

### Major Protocol Changes

- Added field in addSession to define the session cache timeout.
- Added message for to improve addSession error handling and depreciated prevous code.
- Changed model for getAllSessions from streaming to buffers.

### Minor Protocol Changes

- None 

## Release v1alpha4

### Bug Fixes

- None

### Feature Changes

- Added C++ libraries and bindings
- Added C libraries for server and client
- Added Doxygen for C/C++ code

### Major Protocol Changes

- Changed addSession to be a streaming interface.
- Changed IP V4 and V6 to network byte order format instead of string.

### Minor Protocol Changes

- None 

## Release v1alpha2

### Bug Fixes

- None

### Feature Changes

- Added RELEASE_NOTES.md (this file)
- Added Makefile to generate language bindings (Python, Golang and C++)
- Added Documentation on how to use interfaces

### Major Protocol Changes

- Updated Activatation API to be Camel Case
- Changed Activation Service to have simpler API

### Minor Protocol Changes

- None

## Release v1alpha1

### Bug Fixes

- Original Version

### Feature Changes

- Original Version
-
### Major Protocol Changes

- Original Version

### Minor Protocol Changes

- Original Version

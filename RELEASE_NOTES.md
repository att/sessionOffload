# Open Offload Release Notes

## Supported Operating Systems

* Centos 8
* Ubuntu 18.04

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

# Installation of Software on Bluefield 1 (Ubuntu OS)

## Introduction
This is a series of notes to help with the installation of software onto the Bluefield-1 card for OpenOffload. The documentation assumes that the
BF-1 card is installed with IP address 192.168.100.2 which is connected to the tmfifo_net0: on the host with IP Address 192.168.100.1

##Prerequsites

## Check the software version


```bash
$ ssh ubuntu@192.168.100.2
$ password: *********
$ ofed_info -s
```
which should return
```bash
$ MLNX_OFED_LINUX-5.1-2.3.7:
```

or greater. If it returns an older version then go to the host machine and download a newer version
```bash
$ wget http://www.mellanox.com/downloads/BlueField/BFBs/Ubuntu20.04/Ubuntu20.04-5.4.44-mlnx.14.gd7fb187-MLNX_OFED_LINUX-UPSTREAM-LIBS-5.1-2.3.7.1-2-aarch64.bfb
```

This package now needs to be loaded onto the BF-1 card. This step will errase all software and configuration on the BF-1.
```bash
$ cat  Ubuntu20.04-5.4.44-mlnx.14.gd7fb187-MLNX_OFED_LINUX-UPSTREAM-LIBS-5.1-2.3.7.1-2-aarch64.bfb > /dev/rshim0/boot
```
The BF-1 card will now reboot this will take several minutes.
```bash
$  minicom -D /dev/rshim0/console
```
Will take you to the BF-1 console and the system will now be accessible through ssh.
```bash
$ ssh ubuntu@192.168.100.2
$ password: ********
```
To make external servers available netplan may need to be updated with external DNS servers such as 8.8.8.8 (Google DNS).

## Install Software


#### Install gRPC Libraries
```bash
$ mkdir -p /home/ubuntu/local
$ export GRPC_INSTALL=/home/ubuntu/local
$ export PATH=$GRPC_INSTALL/bin:$PATH
$ git config --global  http.sslVerify false
$ git clone --recurse-submodules -b v1.30.0 https://github.com/grpc/grpc
$ cd grpc
$ mkdir -p cmake/build 
$ cd cmake/build
$ cmake -DgRPC_INSTALL=ON -DgRPC_BUILD_TESTS=OFF -DCMAKE_INSTALL_PREFIX=$GRPC_INSTALL ../..
$ make -j 4
$ make install
```
### Install Libconfig Libraries
```bash
$ mkdir -p /home/ubuntu/tmp
$ cd /home/grpc/tmp
$ wget http://hyperrealm.github.io/libconfig/dist/libconfig-1.7.2.tar.gz
$ tar xvzf libconfig-1.7.2.tar.gz
$ cd libconfig-1.7.2
$ ./configure --prefix=$GRPC_INSTALL
$ make
#
# Install in prefix location
#
$ make install
```
### Build the BF-1 Server
Get the code from the [github repository](https://github.com/BodongWang/firewall_offload)
```bash
$ git clone https://github.com/BodongWang/firewall_offload
$ cd firewall_offload
$ git checkout v1.0
```
The Makefile in firewall_offload/daemon needs two changes

1. Add HOME=/home/ubuntu in the build.sh file in firewall_offload to point to the installed files
2. Change line 9 of the makefile from   LIB64DIR := $(GRPC_DIR)/lib64 to LIB64DIR := $(GRPC_DIR)/lib for some reason Ubuntu does not install files in lib64

build the software packages
```bash
$ . build.sh
```
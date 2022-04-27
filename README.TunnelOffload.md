
# TunnelOffload

gRPC API to offload tunnel packet processing for security applications like IPSEC from an application to a hardware switch


# Installation and build for tests

The TunnelOffload only has Python bindings of a reference implemeentation to help demonstrate the API and to show scenarios like Rekeying for IPSEC.  To just run the tests only Python is required.

## Requisites

* Install Python
* Install gRPC for Python (see References)
* Install python pudb (debugger)

See the github workflow main.yaml for details on the require library installations (.github/workflows/main.yaml)

## Build

The following command builds the Basic Python code

```bash
$ ./builditIPTBasic.sh
```

## Testing the sample code with robot

A set of Robotframework tests are provided to quickly run the client and server tests.

The IPTunnel test results can be viewed in the Client and Server logs a sshow here.

```bash
$ cd robot
$ mvn clean install
```

### Client Log

```bash
########## Sending capabilities request ##############
########## Got capabilities response ##############
matchCapabilities {
  ingressInterfaceMatching: true
  geneveMatching: true
  spiMatching: true
}
ipsecCapabilities {
  tunnelTypeSupported: TRANSPORT
  tunnelTypeSupported: TUNNEL
  tunnelTypeSupported: TRANSPORT_NAT_TRAVERSAL
  tunnelTypeSupported: TUNNEL_NAT_TRAVERSAL
  encryptionSupported: _aes256gcm64
  encryptionSupported: _aes256gcm96
  encryptionSupported: _aes256gcm128
}
geneveCapabilities {
  numberGeneveOptionsSupported: 5
}

Sending IPSec Tunnel Request
Updating IPSec Enc Tunnel
Performing rekey - decryption

Performing rekey - decryption


Getting IP Tunnel stats

tunnelId: 10000
tunnelCounters {
  inPackets: 739
  outPackets: 3458
  inBytes: 5912
  outBytes: 27664
  inPacketsDrops: 3545
  outPacketsDrops: 3489
  inBytesDrops: 28360
  outBytesDrops: 27912
}


Getting IP Tunnel

tunnelId: 10000
ipTunnel {
  tunnelId: 10000
  operation: _CREATE
  match_criteria {
    ipv4Match {
      sourceIp: 33554443
      sourceIpPrefix: 32
    }
  }
  nextAction: RECIRCULATE
  ipsecTunnel {
    ipsecDec {
      ipsecSAs {
        spi: 780
        encryptionKey: "72169aa0898505ef03bc85349fa22b8521850e9e8b0461b3f27703f015a7d867"
      }
      ipsecSAs {
        spi: 1030
        encryptionKey: "62dbcf1f1f9bb8973f2ffb876fcc70651ab74c5e43906810db26cf946acda32b"
      }
    }
  }
}
tunnelCounters {
  inPackets: 739
  outPackets: 3458
  inBytes: 5912
  outBytes: 27664
  inPacketsDrops: 3545
  outPacketsDrops: 3489
  inBytesDrops: 28360
  outBytesDrops: 27912
}

Removing old rekey tunnel
Getting all tunnel stats
responses {
  tunnelId: 10000
  tunnelCounters {
    inPackets: 739
    outPackets: 3458
    inBytes: 5912
    outBytes: 27664
    inPacketsDrops: 3545
    outPacketsDrops: 3489
    inBytesDrops: 28360
    outBytesDrops: 27912
  }
}
responses {
  tunnelId: 10001
  tunnelCounters {
    inPackets: 4055
    outPackets: 4382
    inBytes: 32440
    outBytes: 35056
    inPacketsDrops: 1760
    outPacketsDrops: 2681
    inBytesDrops: 14080
    outBytesDrops: 21448
  }
}

responses {
  tunnelId: 10002
  tunnelCounters {
    inPackets: 1471
    outPackets: 4253
    inBytes: 11768
    outBytes: 34024
    inPacketsDrops: 1352
    outPacketsDrops: 3150
    inBytesDrops: 10816
    outBytesDrops: 25200
  }
}
responses {
  tunnelId: 10003
  tunnelCounters {
    inPackets: 2279
    outPackets: 2569
    inBytes: 18232
    outBytes: 20552
    inPacketsDrops: 304
    outPacketsDrops: 1612
    inBytesDrops: 2432
    outBytesDrops: 12896
  }
}

Sending IPSec Tunnel Request
Sending IPSec Tunnel Request
Sending IPSec Tunnel Request
```
### Server Log
```bash
############ Create Tunnel ID - 10000 ##################
tunnelId: 10000
operation: _CREATE
match_criteria {
  ipv4Match {
    sourceIp: 33554443
    sourceIpPrefix: 32
  }
}
nextAction: RECIRCULATE
ipsecTunnel {
  ipsecDec {
    tunnelType: TRANSPORT_NAT_TRAVERSAL
    ipsecSAs {
      spi: 780
      encryptionKey: "72169aa0898505ef03bc85349fa22b8521850e9e8b0461b3f27703f015a7d867"
    }
  }
}

Checking validity of the request
############ Create Tunnel ID - 10001 ##################
tunnelId: 10001
operation: _CREATE
match_criteria {
  tunnelId: 10000
}
nextAction: FORWARD
geneve {
  geneveEncap {
    outerIpv4Pair {
      sourceIp: 84215045
      destinationIp: 101058054
    }
    innerMacPair {
      destinationMac: "605040302010"
      sourceMac: "102030405060"
    }
    optionLength: 4
    vni: 500
    geneveOption {
      optionClass: 260
      type: 17
      length: 1
      data: "\020 0@"
    }
  }
}

Checking validity of the request
Validating option optionClass: 260
type: 17
length: 1
data: "\020 0@"

############ Create Tunnel ID - 10002 ##################
tunnelId: 10002
operation: _CREATE
match_criteria {
  ipv4Match {
    sourceIp: 33554443
    sourceIpPrefix: 32
  }
  geneveMatch {
    vni: 500
  }
}
nextAction: RECIRCULATE
geneve {
  geneveDecap {
  }
}

Checking validity of the request
############ Create Tunnel ID - 10003 ##################
tunnelId: 10003
operation: _CREATE
match_criteria {
  tunnelId: 10002
}
nextAction: FORWARD
ipsecTunnel {
  ipsecEnc {
    tunnelType: TUNNEL_NAT_TRAVERSAL
    ipsecSA {
      spi: 4587
      encryptionKey: "c6a2f973502f1f1be67ac553f17ff329e65261b9e3db36bfcdfcf58c6e48c31e"
    }
    ipv4_tunnel {
      sourceIp: 33554443
      destinationIp: 16777227
    }
  }
}

Checking validity of the request
Printing tunnels summary
The following tunnels are 'stand-alone' tunnels, and not chained to any other tunnel
----------------------------------------------
Following tunnels are chained to each other:
----------------------------------------------
**********
Tunnel ID: 10000
Tunnel Type: IPSec Decryption
**********
     ▼
**********
Tunnel ID: 10001
Tunnel Type: GENEVE Encap
**********


----------------------------------------------
Following tunnels are chained to each other:
----------------------------------------------
**********
Tunnel ID: 10002
Tunnel Type: GENEVE Decap
**********
     ▼
**********
Tunnel ID: 10003
Tunnel Type: IPSEC Encryption
**********


############ Update Tunnel ID - 10003 ##################
tunnelId: 10003
operation: _UPDATE
ipsecTunnel {
  ipsecEnc {
    ipsecSA {
      spi: 4590
      encryptionKey: "fc457eebfed182bb21017a9451769b386dfcbf751bb76d40d9c3cecddaa21510"
    }
  }
}

Printing tunnels summary
The following tunnels are 'stand-alone' tunnels, and not chained to any other tunnel
----------------------------------------------
Following tunnels are chained to each other:
----------------------------------------------
**********
Tunnel ID: 10000
Tunnel Type: IPSec Decryption
**********
     ▼
**********
Tunnel ID: 10001
Tunnel Type: GENEVE Encap
**********


----------------------------------------------
Following tunnels are chained to each other:
----------------------------------------------
**********
Tunnel ID: 10002
Tunnel Type: GENEVE Decap
**********
     ▼
**********
Tunnel ID: 10003
Tunnel Type: IPSEC Encryption
**********


############ Update Tunnel ID - 10000 ##################
tunnelId: 10000
operation: _UPDATE
ipsecTunnel {
  ipsecDec {
    ipsecSAs {
      spi: 780
      encryptionKey: "72169aa0898505ef03bc85349fa22b8521850e9e8b0461b3f27703f015a7d867"
    }
    ipsecSAs {
      spi: 1030
      encryptionKey: "62dbcf1f1f9bb8973f2ffb876fcc70651ab74c5e43906810db26cf946acda32b"
    }
  }
}

Printing tunnels summary
The following tunnels are 'stand-alone' tunnels, and not chained to any other tunnel
----------------------------------------------
Following tunnels are chained to each other:
----------------------------------------------
**********
Tunnel ID: 10000
Tunnel Type: IPSec Decryption
**********
     ▼
**********
Tunnel ID: 10001
Tunnel Type: GENEVE Encap
**********


----------------------------------------------
Following tunnels are chained to each other:
----------------------------------------------
**********
Tunnel ID: 10002
Tunnel Type: GENEVE Decap
**********
     ▼
**********
Tunnel ID: 10003
Tunnel Type: IPSEC Encryption
**********


Getting IP Tunnel stats 10000
tunnelId: 10000
tunnelCounters {
  inPackets: 739
  outPackets: 3458
  inBytes: 5912
  outBytes: 27664
  inPacketsDrops: 3545
  outPacketsDrops: 3489
  inBytesDrops: 28360
  outBytesDrops: 27912
}

Getting IP Tunnel 10000
tunnelId: 10000
ipTunnel {
  tunnelId: 10000
  operation: _CREATE
  match_criteria {
    ipv4Match {
      sourceIp: 33554443
      sourceIpPrefix: 32
    }
  }
  nextAction: RECIRCULATE
  ipsecTunnel {
    ipsecDec {
      ipsecSAs {
        spi: 780
        encryptionKey: "72169aa0898505ef03bc85349fa22b8521850e9e8b0461b3f27703f015a7d867"
      }
      ipsecSAs {
        spi: 1030
        encryptionKey: "62dbcf1f1f9bb8973f2ffb876fcc70651ab74c5e43906810db26cf946acda32b"
      }
    }
  }
}
tunnelCounters {
  inPackets: 739
  outPackets: 3458
  inBytes: 5912
  outBytes: 27664
  inPacketsDrops: 3545
  outPacketsDrops: 3489
  inBytesDrops: 28360
  outBytesDrops: 27912
}

############ Update Tunnel ID - 10000 ##################
tunnelId: 10000
operation: _UPDATE
ipsecTunnel {
  ipsecDec {
    ipsecSAs {
      spi: 1030
      encryptionKey: "62dbcf1f1f9bb8973f2ffb876fcc70651ab74c5e43906810db26cf946acda32b"
    }
  }
}

Printing tunnels summary
The following tunnels are 'stand-alone' tunnels, and not chained to any other tunnel
----------------------------------------------
Following tunnels are chained to each other:
----------------------------------------------
**********
Tunnel ID: 10000
Tunnel Type: IPSec Decryption
**********
     ▼
**********
Tunnel ID: 10001
Tunnel Type: GENEVE Encap
**********


----------------------------------------------
Following tunnels are chained to each other:
----------------------------------------------
**********
Tunnel ID: 10002
Tunnel Type: GENEVE Decap
**********
     ▼
**********
Tunnel ID: 10003
Tunnel Type: IPSEC Encryption
**********


Got getAllIpTunnelsStats request!
Number of stats per response: 2
Sending response in stream!
responses {
  tunnelId: 10000
  tunnelCounters {
    inPackets: 739
    outPackets: 3458
    inBytes: 5912
    outBytes: 27664
    inPacketsDrops: 3545
    outPacketsDrops: 3489
    inBytesDrops: 28360
    outBytesDrops: 27912
  }
}
responses {
  tunnelId: 10001
  tunnelCounters {
    inPackets: 4055
    outPackets: 4382
    inBytes: 32440
    outBytes: 35056
    inPacketsDrops: 1760
    outPacketsDrops: 2681
    inBytesDrops: 14080
    outBytesDrops: 21448
  }
}

Sending response in stream!
responses {
  tunnelId: 10002
  tunnelCounters {
    inPackets: 1471
    outPackets: 4253
    inBytes: 11768
    outBytes: 34024
    inPacketsDrops: 1352
    outPacketsDrops: 3150
    inBytesDrops: 10816
    outBytesDrops: 25200
  }
}
responses {
  tunnelId: 10003
  tunnelCounters {
    inPackets: 2279
    outPackets: 2569
    inBytes: 18232
    outBytes: 20552
    inPacketsDrops: 304
    outPacketsDrops: 1612
    inBytesDrops: 2432
    outBytesDrops: 12896
  }
}

############ Create Tunnel ID - 20000 ##################
tunnelId: 20000
operation: _CREATE
match_criteria {
  ipv4Match {
    sourceIp: 33554444
    sourceIpPrefix: 32
  }
}
nextAction: RECIRCULATE
ipsecTunnel {
  ipsecDec {
    tunnelType: TUNNEL
    ipsecSAs {
      spi: 980
      encryptionKey: "1db433168d58183cdf77e248de7d122810f71ec911614d1681045a6d98c11e37"
    }
  }
}

Checking validity of the request
############ Create Tunnel ID - 20001 ##################
tunnelId: 20001
operation: _CREATE
match_criteria {
  tunnelId: 20000
}
nextAction: RECIRCULATE
geneve {
  geneveDecap {
  }
}

Checking validity of the request
############ Create Tunnel ID - 20002 ##################
tunnelId: 20002
operation: _CREATE
match_criteria {
  tunnelId: 20001
}
nextAction: RECIRCULATE
geneve {
  geneveEncap {
    outerIpv4Pair {
      sourceIp: 117901063
      destinationIp: 134744072
    }
    innerMacPair {
      destinationMac: "605040302010"
      sourceMac: "102030405060"
    }
    optionLength: 4
    vni: 700
    geneveOption {
      optionClass: 260
      type: 17
      length: 1
      data: "\020 0@"
    }
  }
}

Checking validity of the request
Validating option optionClass: 260
type: 17
length: 1
data: "\020 0@"

############ Create Tunnel ID - 20003 ##################
tunnelId: 20003
operation: _CREATE
match_criteria {
  tunnelId: 20002
  ipsecMatch {
    spi: 920
  }
}
nextAction: FORWARD
ipsecTunnel {
  ipsecEnc {
    ipsecSA {
      spi: 45888
      encryptionKey: "17bf2e5e78ebd887eb49d23b04041895e37624deffbfaafeb720c4b57851947e"
    }
    ipv4_tunnel {
      sourceIp: 33554445
      destinationIp: 16777229
    }
  }
}

Checking validity of the request
Printing tunnels summary
The following tunnels are 'stand-alone' tunnels, and not chained to any other tunnel
----------------------------------------------
Following tunnels are chained to each other:
----------------------------------------------
**********
Tunnel ID: 10000
Tunnel Type: IPSec Decryption
**********
     ▼
**********
Tunnel ID: 10001
Tunnel Type: GENEVE Encap
**********


----------------------------------------------
Following tunnels are chained to each other:
----------------------------------------------
**********
Tunnel ID: 10002
Tunnel Type: GENEVE Decap
**********
     ▼
**********
Tunnel ID: 10003
Tunnel Type: IPSEC Encryption
**********


----------------------------------------------
Following tunnels are chained to each other:
----------------------------------------------
**********
Tunnel ID: 20000
Tunnel Type: IPSec Decryption
**********
     ▼
**********
Tunnel ID: 20001
Tunnel Type: GENEVE Decap
**********
     ▼
**********
Tunnel ID: 20002
Tunnel Type: GENEVE Encap
**********
     ▼
**********
Tunnel ID: 20003
Tunnel Type: IPSEC Encryption
**********


############ Create Tunnel ID - 30000 ##################
tunnelId: 30000
operation: _CREATE
match_criteria {
  ipv4Match {
    sourceIp: 50529027
    sourceIpPrefix: 32
  }
}
nextAction: RECIRCULATE
ipsecTunnel {
  ipsecEnc {
    tunnelType: TUNNEL
    ipsecSA {
      spi: 50000
      encryptionKey: "eedc22f9fcea28c527e4feaa41b99bf4673fa7570d0f2857e7cfb99f10921eb6"
    }
    ipv4_tunnel {
      sourceIp: 33554446
      destinationIp: 16777230
    }
  }
}

Checking validity of the request
############ Create Tunnel ID - 30001 ##################
tunnelId: 30001
operation: _CREATE
match_criteria {
  ipv4Match {
    sourceIp: 16777230
    sourceIpPrefix: 32
  }
}
nextAction: RECIRCULATE
geneve {
  geneveEncap {
    outerIpv4Pair {
      sourceIp: 151587081
      destinationIp: 168430090
    }
    innerMacPair {
      destinationMac: "605040302010"
      sourceMac: "102030405060"
    }
    optionLength: 4
    vni: 701
    geneveOption {
      optionClass: 260
      type: 17
      length: 1
      data: "\020 0@"
    }
  }
}

Checking validity of the request
Validating option optionClass: 260
type: 17
length: 1
data: "\020 0@"

Printing tunnels summary
The following tunnels are 'stand-alone' tunnels, and not chained to any other tunnel
**********
Tunnel ID: 30000
Tunnel Type: IPSEC Encryption
**********
**********
Tunnel ID: 30001
Tunnel Type: GENEVE Encap
**********
----------------------------------------------
Following tunnels are chained to each other:
----------------------------------------------
**********
Tunnel ID: 10000
Tunnel Type: IPSec Decryption
**********
     ▼
**********
Tunnel ID: 10001
Tunnel Type: GENEVE Encap
**********


----------------------------------------------
Following tunnels are chained to each other:
----------------------------------------------
**********
Tunnel ID: 10002
Tunnel Type: GENEVE Decap
**********
     ▼
**********
Tunnel ID: 10003
Tunnel Type: IPSEC Encryption
**********


----------------------------------------------
Following tunnels are chained to each other:
----------------------------------------------
**********
Tunnel ID: 20000
Tunnel Type: IPSec Decryption
**********
     ▼
**********
Tunnel ID: 20001
Tunnel Type: GENEVE Decap
**********
     ▼
**********
Tunnel ID: 20002
Tunnel Type: GENEVE Encap
**********
     ▼
**********
Tunnel ID: 20003
Tunnel Type: IPSEC Encryption
**********


############ Create Tunnel ID - 40000 ##################
tunnelId: 40000
operation: _CREATE
match_criteria {
  ipv4Match {
    sourceIp: 50529027
    sourceIpPrefix: 32
  }
}
nextAction: RECIRCULATE
ipsecTunnel {
  ipsecDec {
    tunnelType: TUNNEL_NAT_TRAVERSAL
    ipsecSAs {
      spi: 80000
      encryptionKey: "6f8f3228ad056f305e91be38f1dd4f01db6e6c804e9c64e242fdae0d079ec53a"
    }
  }
}

Checking validity of the request
############ Create Tunnel ID - 40001 ##################
tunnelId: 40001
operation: _CREATE
match_criteria {
  ipv4Match {
    sourceIp: 16777231
    sourceIpPrefix: 32
  }
  tunnelId: 40000
}
nextAction: RECIRCULATE
geneve {
  geneveEncap {
    outerIpv4Pair {
      sourceIp: 151587081
      destinationIp: 168430090
    }
    innerMacPair {
      destinationMac: "605040302010"
      sourceMac: "102030405060"
    }
    optionLength: 4
    vni: 701
    geneveOption {
      optionClass: 260
      type: 17
      length: 1
      data: "\020 0@"
    }
  }
}

Checking validity of the request
Validating option optionClass: 260
type: 17
length: 1
data: "\020 0@"

############ Create Tunnel ID - 40002 ##################
tunnelId: 40002
operation: _CREATE
match_criteria {
  ipv4Match {
    sourceIp: 16777232
    sourceIpPrefix: 32
  }
  tunnelId: 40000
}
nextAction: RECIRCULATE
geneve {
  geneveEncap {
    outerIpv4Pair {
      sourceIp: 185273099
      destinationIp: 202116108
    }
    innerMacPair {
      destinationMac: "605040302010"
      sourceMac: "102030405060"
    }
    optionLength: 4
    vni: 702
    geneveOption {
      optionClass: 260
      type: 17
      length: 1
      data: "\020 0@"
    }
  }
}

Checking validity of the request
Validating option optionClass: 260
type: 17
length: 1
data: "\020 0@"

Printing tunnels summary
The following tunnels are 'stand-alone' tunnels, and not chained to any other tunnel
**********
Tunnel ID: 30000
Tunnel Type: IPSEC Encryption
**********
**********
Tunnel ID: 30001
Tunnel Type: GENEVE Encap
**********
----------------------------------------------
Following tunnels are chained to each other:
----------------------------------------------
**********
Tunnel ID: 10000
Tunnel Type: IPSec Decryption
**********
     ▼
**********
Tunnel ID: 10001
Tunnel Type: GENEVE Encap
**********


----------------------------------------------
Following tunnels are chained to each other:
----------------------------------------------
**********
Tunnel ID: 10002
Tunnel Type: GENEVE Decap
**********
     ▼
**********
Tunnel ID: 10003
Tunnel Type: IPSEC Encryption
**********


----------------------------------------------
Following tunnels are chained to each other:
----------------------------------------------
**********
Tunnel ID: 20000
Tunnel Type: IPSec Decryption
**********
     ▼
**********
Tunnel ID: 20001
Tunnel Type: GENEVE Decap
**********
     ▼
**********
Tunnel ID: 20002
Tunnel Type: GENEVE Encap
**********
     ▼
**********
Tunnel ID: 20003
Tunnel Type: IPSEC Encryption
**********


----------------------------------------------
Following tunnels might be chained to each other, they match via tunnel
and shuold match via IP as well
----------------------------------------------
**********
Tunnel ID: 40000
Tunnel Type: IPSec Decryption
**********
     ▼
**********
Tunnel ID: 40001
Tunnel Type: GENEVE Encap
**********


----------------------------------------------
Following tunnels might be chained to each other, they match via tunnel
and shuold match via IP as well
----------------------------------------------
**********
Tunnel ID: 40000
Tunnel Type: IPSec Decryption
**********
     ▼
**********
Tunnel ID: 40002
Tunnel Type: GENEVE Encap
**********


```


## Testing the sample code with python directly

Create four separate terminal windows a client and three server windows

### Server Window (Offload sessions)

Start the tunnel server in one window by going to the build directory and running the script below.

```bash
$ cd buildIPTBasic
$ ./runIPTOffloadServer.sh
```

### Client window

Start the client in the second window by going to the build directory and running the script below.

```bash
$ cd buildIPTBasic
$ ./runIPTClient.sh
```
There should be results in the client and server windows similar to the robot output above.



MAINTAINERS
-----------

Developed by Omri Peri and Brian Freeman , April 2022

Current maintainers:
 * Omri Peri (drivenets)John McDowall (palo alto networks)
 * Brian Freeman (at&t)

# References




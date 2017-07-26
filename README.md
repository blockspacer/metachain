# TrustChainTechnologies - MetaChain

[![TrustChainTechnologies](https://www.trustchaintechnologies.io/logo.png)](https://www.trustchaintechnologies.io/)

These are the MetaChain sources of the TCT blockchain technology. It features block signing, PoT with PtA and an extensive MetaChain API.

### Important links
 - The [roadmap] can be found in our forums and will be updated frequently.
 - More technical info in the [whitepaper].

### Development

You want to contribute? Have the guts to tackle crypto development?
Perfect, then register in our [forum] and get in touch with us!

# Installation

Since we're currently in heavy development, we don't suggest installing the MetaChain yet. As soon as this changes we'll publish the installation details.
#### Using binaries
currently not supported
#### Dependencies
- for compiling from source you'll need a working [boost] installation with all libraries built.
- [openssl] is widely used in this project. you'll need a working installation with all libraries.
- C++ compiler needs to have C++11 compability. As soon as C++17.2 is available, changes will be made to use the C++17.2 standard as dependency!
#### Building for source on windows
- clone the git repository using github for Desktop
- open the MetaChain.sln in Microsoft Visual Studio
- build for Debug or Release
#### Building for source on linux
g++, automake and autoconf as well as regular dev tools are required!
Clone the github repository:
```sh
$ git clone https://github.com/TrustChainTechnologies/metachain.git
$ cd metachain
$ ./configure
$ ./make
```
# Configuration
#### Command line arguments
-v, --version: print version info
-?, -h, --help: print all command line arguments
-c, --conf=<file>: use this ini file for configuration (default: node.ini)
#### node.ini
```
; general configuration
[general]
daemonize = true    ; use daemonize if OS supports it

; logging configuration
[logging]
log_to_stdout = true    ; log everything to stdout
log_to_file = true      ; log to a logfile
log_file = output.log   ; name of the logfile

; network configuration
[network]
listening_ip = 127.0.0.1                    ; ip to bind listening socket to
listening_port = 5634                       ; port to bind listening socket to
peer_file = peers.dat                       ; file that stores the known peers which is automatically updated
ban_file = bans.dat                         ; file that stores banned peers which is automatically updated
connect_timeout = 5000                      ; connection timeout for a new node in ms
max_outgoing_connections = 1000             ; number of maximum outgoing connections
max_incoming_connections = 1000             ; number of maximum incoming connections
time_between_unsuccessfull_connects = 30    ; delay between two connects to the same node

; autoupdate configuration
[autoupdate]
ticks_until_update_triggered = 10           ; how many nodes need to have a newer version to trigger update
do_autoupdate = true                        ; autoupdate? if false, node exits automatically
```

# External resources used in the source code
- [brofield/simpleini] - for parsing ini files
- [bitcoin/bitcoin] - parts of the network communication, parts of the crypto sources (e.g. sha256 etc). Everything heavily modified, changed for our demands and integrated into our structure
- [boost] - used for special timings, threads and scheduling
- [openssl] - used for securing communications as well as some crypto functionalities.

License
----

GPL


**(c) the TCT-Devs**

[//]: # (These are reference links used in the body of this note and get stripped out when the markdown processor does its job. There is no need to format nicely because it shouldn't be seen. Thanks SO - http://stackoverflow.com/questions/4823468/store-comments-in-markdown-syntax)


   [roadmap]: <https://forum.trustchaintechnologies.io/showthread.php?tid=13&pid=21#pid21>
   [whitepaper]: <https://backoffice.trustchaintechnologies.io/downloads/whitepaper.pdf>
   [forum]: <https://forum.trustchaintechnologies.io>
   [brofield/simpleini]: <https://github.com/brofield/simpleini>
   [bitcoin/bitcoin]: <https://github.com/bitcoin/bitcoin>
   [boost]: <http://www.boost.org/>
   [openssl]: <https://github.com/openssl/openssl>

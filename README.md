kibitz
======

A message centric grid computing environment. 

See the wiki for details on using the library. https://github.com/murphybytes/kibitz/wiki

Building
------------

Currently I've managed to build and run kibitz on Ubuntu and Mac OS X 10.7.4

CMake is required to build kibitz. Generate a make file like so:

```
cd src
cmake .
make
```

Binaries will be found in the src directory after building.

Dependencies
------------

ZeroMQ 
```
git clone ssh://git@github.com/zeromq/zeromq-x.git
cd zeromq-x
./autogen.sh
./configure 
make 
sudo make install
```

Boost 1.49

From Source 
http://sourceforge.net/projects/boost/files/boost/1.49.0/boost_1_49_0.tar.gz/download

Mac OSX
brew install boost

glog 0.3.2

http://code.google.com/p/google-glog/

Mac OSX
brew install glog

### yaml-cpp

http://code.google.com/p/yaml-cpp/downloads/detail?name=yaml-cpp-0.3.0.tar.gz&can=2&q=

#### Linux 
yaml-cpp builds a static lib by default. If you need a shared
lib because your build fails in Linux use cmake to generate
yaml-cpp make file thus
```
cmake -DBUILD_SHARED_LIBS=ON

```

#### Mac OSX
brew install yaml-cpp

Management Scripts
------------------------
Kibitz provides a handy control script to manage jobs.  You'll need to install Ruby and RVM to use
these scripts.  

#### RVM

https://rvm.io/

Once you install RVM there is a control script in the project root that will set the rest of the Ruby 
dependencies. (.rvmrc)  This script will run the first time you enter the project root directory and download
Ruby gems and install the appropriate version of Ruby.  Once installation is complete you can run

```
# stops workers
script\kibitz stop

#starts a job
KIBITZ_LOCATOR='tcp://196.1.3.122:5556' script\kibitz initiate --worker-type=starter --worker-id=1

```

Components
------------------------

worker-locator - provides mechanism for workers to discover each other
```
worker-locator:
  -h [ --help ]                         Show help message
  -b [ --heartbeat-binding ] arg (=tcp://*:5556)
                                        Binding to listen for worker heartbeats
  -p [ --port ] arg (=5557)             Port used to distribute locator 
                                        messages
  -t [ --context-threads ] arg (=1)     zmq context thread count
```

test-worker-a - sample worker that implements the kibitz library
```
"test-worker-a" options:
  -h [ --help ]                         Show help message
  -I [ --worker-id ] arg                (Required) Integer that identifies 
                                        worker. Must be unique with worker type
  -T [ --worker-type ] arg              (Required) Name of the type of worker.
  -b [ --heartbeat-binding ] arg        zmq tcp binding for 
                                        publishing heartbeats
  -d [ --discovery-binding ] arg        zmq tcp binding for 
                                        recieving locator information
  -p [ --tcp-port ] arg (=8999)         Port for tcp based zmq messages
  -t [ --context-threads ] arg (=1)     Thread count passed to zmq_init
  -h [ --heartbeat-frequency ] arg (=1000)
                                        Heartbeat frequency in milliseconds
```
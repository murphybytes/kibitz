kibitz
======

A message centric grid computing environment

Building
------------

Currently I've managed to build and run kibitz on Ubuntu and Mac OS X 10.7.4

CMake is required to build kibitz. Generate a make file like so:

```
cd src
cmake .
```

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



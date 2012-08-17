kibitz
======

A message centric grid computing environment

Building
------------

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



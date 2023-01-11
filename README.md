# ircb
An irc bot framework / library with an example client. 

To build on Linux, 
  1.  mkdir build
  2.  cd build
  3.  cmake ..
  4.  make

This will make the binary "ircb" and any test binaries.

To build on Windows with MSVC,
  1. clone from git
  2. build

You will have to set up your project to pass arguments to the binary.

To build on Mac OS X with VSCode,
  1. clone from git
  2. build with gcc - it will not build with AppleClang

Checking test coverage can be done by running the following in the build directory:

gcovr -r .. -e _deps

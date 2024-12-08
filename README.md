# options_cpp
A C++ framework to price options and calibrate models to market data.
The whole thing is being written in the `C++17` standard. Here's a brief
list of some of the dependencies required to run the code:
    
    -`rapidcsv.h` is an amazing csv parser written by @d99kris and alter, that
      allows to read and parse csv in C++ with great speed

To run the code, you need `cmake` (versio idk?). Compile the main file using 

`cmake CMakeLists.txt`

which should generate a `Makefile` in the current folder. Now, use `make`
to generate the executable. By default, the `.exe` files will be created 
in the `cmake-build-debug\bin` folder, but you are more than welcomed to change that.

For any issues, feel free to contact me at `matte.campa at yahoo.it`

<hr>
Authors: @Dr4k3z

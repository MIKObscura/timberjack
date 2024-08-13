# Timberjack
Qt app made to view VDF format files as a sort of tree.

# How to build
First, you need CMake and the Qt framework libraries installed in order to build it.  
Then, you first setup your CMake build directory like this for example:
```
mkdir build && cd build
cmake ..
```
Then, run this to build the project (replace the dot by the name of your build directory if you're not in it):
```
cmake --build .
```

# Acknowledgements
Parsing of VDF data format is done with an external library found here https://github.com/TinyTinni/ValveFileVDF
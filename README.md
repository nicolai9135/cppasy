# cppasy

cppasy is a C++ parameter synthesis tool derived from [PaSyPy](https://github.com/awiegel/PaSyPy). For a detailed description of this project, read our [publication](link/to/master_thesis)

## Feature Overview

## CMake Build

To build this project, execute the following in the root directory of this project
```
$ mkdir build
$ cd build
$ cmake ..
$ cmake --build . [--target <target>]
```

## Usage

### GUI
To use the GUI, simply execute `$ ./gui`. To understand available options, we recommend reading the [publication](link/to/master_thesis).
 
### CLI
To use the CLI, execute `$ ./cli`. Execute `$ ./cli --help` for information on possible arguments. To understand available options, we recommend reading the [publication](link/to/master_thesis).

## Documentation

The documentation of this project is hosted [here](https://nicolai9135.github.io/cppasy/). Note, that the GUI is missing a documentation as the code it is not expected to be used by anyone else than [myself](https://github.com/nicolai9135). If you want to substantially change the GUI, I recommend [abandoning `wxWidgets`](https://en.wikipedia.org/wiki/WxWidgets#Criticism) and using a different library.
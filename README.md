# srs-daq - A data acquisition program for SRS FEC & VMM3

[![Codacy Badge](https://app.codacy.com/project/badge/Grade/7e8c956af1bc46c7836524f1ace32c11)](https://app.codacy.com/gh/YanzhaoW/srs-daq/dashboard?utm_source=gh&utm_medium=referral&utm_content=&utm_campaign=Badge_grade)

## Introduction

srs-daq is an asynchronous data IO program for SRS system.

### Included

- Control/monitoring for SRS FEC & VMM3a hybrids
- Readout of data
- Data deserialization from SRS system

## Building

### Prerequisites

- C++ compiler 
  - gcc <span>&#8805;</span> 14.2.
  - clang <span>&#8805;</span> 18

  To check the version of compiler, use
  ```bash
  gcc --version # if gcc is used
  clang --version # if clang is used
  ```
  if the version is less than 14.2, use the correct version by
  ```bash
  export CXX=[g++ command] # e.g. export CXX=g++-14.2.0
  export CC=[gcc command] # e.g. export CC=gcc-14.2.0
  ```
- Conan <span>&#8805;</span> 2.2. To install Conan
    ```bash
    python -m pip install conan
    ```
- cmake <span>&#8805;</span> 3.26. The version can be checked with
  ```bash
  cmake --version # please make sure the output is larger than 3.26
  ```
- **(Optional)** ROOT <span>&#8805;</span> 6.32

> [!CAUTION]
> DO NOT USE Conda to install ROOT. Use latest compiler to install ROOT from the source.

### Installation

```
git clone -b dev https://github.com/YanzhaoW/srs-daq.git
cd srs-daq
git submodule update --init
cmake --preset default .
cmake --build ./build -- -j[nproc]
```

The executable programs are compiled in the `build/bin` directory whereas the dynamic library in `build/lib`.

## srs_control - The control program

To run the program, first go to `build/bin` directory and run

```bash
./srs_control [-p DATA_PRINT_OPTION] [-v LOG_LEVEL] [-h]
```

### Run-time options

- `-h` or `--help`: print the help message.
- `-v` or `--verbose-level`: set the verbose level. Available options: "critical", "error", "warn", "info" (default), "debug", "trace", "off".
- `-p` or `--print-mode`: set the data printing mode. Available options:
  - speed (default): print the reading rate of received data.
  - header: print the header message of received data.
  - raw: print the received raw bytes.
  - all: print all data, including header, hit and marker data, but no raw data.
- `-o` or `--output-files`: set the file outputs (more detail below).

#### Data output to multiple sinks

srs-daq can output received data into multiple sink types at the same time. Currently, following output types are available:

- binary. File extensions: ".lmd" or ".bin"
- root. File extensions: ".root" (require ROOT library)
- json. File extensions: ".json" (not yet implemented)
- UDP socket. string format: "ip:port" (not yet implemented)
- google protobuf. (planned)

Users have to use the correct file extensions to enable the corresponding output types. Except the UDP socket, each output type should only have one file.

For example, to output data both to a binary file and a root file:

```bash
./srs_control -o "output.root" -o "output.bin"
```

### Custom configuration

To be added ...

## Acknowledgments

- A lot of information was used from the existing codebase of the VMM slow control software [vmmsc](https://gitlab.cern.ch/rd51-slow-control/vmmsc.git).

## TODO list

- calibration routines
- graphical user interface
- ROOT output file

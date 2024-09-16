# srslib - A data IO program for SRS FEC & VMM3

[![Codacy Badge](https://app.codacy.com/project/badge/Grade/f3a70c8e8a9d4909a1953142fe5ff1e9)](https://app.codacy.com/gh/YanzhaoW/srslib/dashboard?utm_source=gh&utm_medium=referral&utm_content=&utm_campaign=Badge_grade)

## Introduction

srslib is an asynchronous data IO program for SRS system.

### Included

- Control/monitoring for SRS FEC & VMM3a hybrids
- Readout of data
- Data deserialization from SRS system

## Building

### Prerequisite

- gcc >= 14.2
- conan >= 2.2. To install conan
    ```bash
    python -m pip install conan
    ```
- cmake >= 3.26

### Installation

```
git clone -b dev https://github.com/YanzhaoW/srslib.git
cd srslib
git submodule update --init
cmake --preset default .
cmake --build ./build -- -j[nproc]
```

The executable programs are compiled in the build directory:

```
./build/apps/
```

## srs_control - The control program

To run the program

```bash
./srs_control [-p DATA_PRINT_OPTION] [-v LOG_LEVEL] [-h]
```

### Run-time options

- `-h` or `--help`: print the help message
- `-v` or `--verbose-level`: set the verbose level. Available options: "critical", "error", "warn", "info" (default), "debug", "trace", "off".
- `-p` or `--print-mode`: set the data printing mode. Available options:
    - speed (default): print the reading rate of received data
    - header: print the header message of received data
    - raw: print the received raw bytes
    - all: print all data, including header, hit and marker data, but no raw data.

### Custom configuration

To be added ...

## Acknowledgments

- A lot of information was used from the existing codebase of the VMM slow control software [vmmsc](https://gitlab.cern.ch/rd51-slow-control/vmmsc.git).

## TODO list

- calibration routines
- graphical user interface
- ROOT output file

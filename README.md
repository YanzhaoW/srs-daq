# srs-daq - A data acquisition program for SRS FEC & VMM3

[![Codacy Badge](https://app.codacy.com/project/badge/Grade/7e8c956af1bc46c7836524f1ace32c11)](https://app.codacy.com/gh/YanzhaoW/srs-daq/dashboard?utm_source=gh&utm_medium=referral&utm_content=&utm_campaign=Badge_grade) [![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

## Introduction

srs-daq is an asynchronous data IO program for SRS system.

### Included

- Readout of data
- Data deserialization from SRS system

## Building

### Prerequisites

- C++ compiler (choose either one below)
  - gcc <span>&#8805;</span> 14.2.
  - clang <span>&#8805;</span> 18

- (mini) Conda. See [this instruction](doc/install_conda.md) to install miniconda quickly.
- **(Optional)** ROOT <span>&#8805;</span> 6.32

> [!CAUTION]
> DO NOT USE Conda to install ROOT. Use latest compiler to install ROOT from the source.

### Installation

#### Step 1: Clone the latest version

```bash
git clone -b latest https://github.com/YanzhaoW/srs-daq.git
cd srs-daq
git submodule update --init
```

#### Step 2: Prepare the environment

```bash
conda env create -f environment.yml
conda activate srs
```

#### Step 3: Build the project

*Make sure conda environment `srs` is activated.*

```bash
cmake --preset default . [optional settings]
cmake --build ./build -- -j[nproc]
```

The executable programs are compiled in the `build/bin` directory whereas the dynamic library in `build/lib`.

Following CMake preset optional settings are available:

- `-DUSE_ROOT=`
  - `OFF` or `FALSE` (default). The program would only compile with ROOT if ROOT exists. 
  - `ON` or `TRUE`. CMake configuration will fail if `ROOT` is not found. 
- `-DNO_ROOT=`
  - `OFF` or `FALSE` (default). Same as `-DUSE_ROOT=OFF`.
  - `ON` or `TRUE`. The program does NOT compiler with ROOT even if ROOT exists.

For example, to disable the ROOT dependency, run the `cmake --preset` with:

```bash
cmake --preset default . -DNO_ROOT=TRUE
```

### Update to the latest version

srs-daq is under a continuous development. To update to the latest version, run the following git commands:

```bash
git fetch origin
git checkout latest
git submodule update --init
```
After this, build the project again from [Step 3](#step-3-build-the-project).


## srs_control - The control program

<!-- To run the program, first make sure you have activated the conda environment `srs`, which can be checked by `conda info`. If not, run `conda activate srs` to activate `srs` environment. --> Go to `build/bin` directory and run

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

#### Data output to multiple files

srs-daq can output received data into multiple files with different types at the same time. Currently, following output types are available (or planned):

- **binary**. File extensions: `.lmd` or `.bin`
  - raw data if `.lmd` or `.bin`
  - protobuf data if `.binpb`
- **json**. File extensions: `.json` (NOTE: JSON file could be very large)
- **root**. File extensions: `.root` (require ROOT library)
- **UDP socket** (protobuf). Input format: `[ip]:[port]`

Users have to use the correct file extensions to enable the corresponding output types.

For example, to output data both to a binary file and a root file:

```bash
./srs_control -o "output.root" -o "output.bin"
```

### Custom configuration

To be added ...

## Acknowledgments

- A lot of information was used from the existing codebase of the VMM slow control software [vmmsc](https://gitlab.cern.ch/rd51-slow-control/vmmsc.git).

## TODO list

- Control/monitoring for SRS FEC & VMM3a hybrids
- Graphical user interface (typescript + react + websocket).

## Build the project from source

### Prerequisites

- C++ compiler (choose either one below)
  - gcc <span>&#8805;</span> 14.3.
  - clang <span>&#8805;</span> 18
- oneTBB
- **(Optional)** ROOT <span>&#8805;</span> 6.32

- (mini) Conda. See [this instruction](install_conda.md) to install miniconda quickly.

> [!CAUTION]
> DO NOT USE Conda to install ROOT. Use latest compiler to install ROOT from the source.

### Installation

#### Step 1: Clone the latest version

```bash
git clone -b latest https://github.com/YanzhaoW/srs-daq.git
cd srs-daq
git submodule update --init
```

#### Step 2: Activate the Conda environment

```bash
conda env create -f environment.yml
conda activate srs
```

#### Step 3: Build the project

**Make sure conda environment `srs` is activated.**

```bash
cmake --workflow --preset default
```

or

```bash
cmake --preset default -DCMAKE_INSTALL_PREFIX=[/usr/local] [optional settings]
cmake --build ./build --target install -- -j[nproc]
```

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
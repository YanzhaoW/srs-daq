## Installation of required software using Conda

Conda is a package manager for easily installing required libraries and software.

### Step 1: Install mini-conda

Go to [mini-conda quick command line install](https://docs.anaconda.com/free/miniconda/#quick-command-line-install) and choose the method for your operating system. For example, Linux users can do

```bash
mkdir -p ~/miniconda3
wget https://repo.anaconda.com/miniconda/Miniconda3-latest-Linux-x86_64.sh -O ~/miniconda3/miniconda.sh
bash ~/miniconda3/miniconda.sh -b -u -p ~/miniconda3
rm ~/miniconda3/miniconda.sh
```

After this, for bash user:

```bash
~/miniconda3/bin/conda init bash
```

and for zsh user:

```bash
~/miniconda3/bin/conda init zsh
```

Once all is done successfully, log out and restart your shell.

### Step 2: Install dependencies

Install ROOT and Conan with:

```bash
conda install -c conda-forge root
conda install -c conda-forge "conan>=2.7.0"
```

### Step 3: Define the correct environment

Put the following lines after `<<< conda initialize <<<` line:

```bash
export CC=[gcc command]
export CXX=[g++ command]
```
where `[g++/gcc command]` is the command used to run a specific version of gcc. Please make sure it exists in the system by checking

```bash
[g++ command] --version
```

for example:

```bash
export CC=gcc-14.2.0
export CXX=g++-14.2.0
```

## Troubleshooting

### libarchive error

#### Error message

```text
Error while loading conda entry point: conda-libmamba-solver (libarchive.so.20: cannot open shared object file: No such file or directory)
```

This could happen if you are in an old operating system.

#### Solution

```bash
conda install --solver=classic conda-forge::conda-libmamba-solver conda-forge::libmamba conda-forge::libmambapy conda-forge::libarchive
```

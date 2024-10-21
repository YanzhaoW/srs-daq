## Installation of required software using Conda

Conda is a package manager for easily installing required libraries and software.

### Install mini-conda

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

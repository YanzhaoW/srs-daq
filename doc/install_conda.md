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

### Step2: Install dependencies

Install ROOT and Conan with:

```bash
conda install -c conda-forge root conan
```

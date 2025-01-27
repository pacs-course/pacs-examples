# EXAMPLES AND EXERCISES FOR THE PACS COURSE

This repo contains source code and descriptions for the examples and exercises
that will be explained during the PACS (Programmazione Avanzata per il Calcolo
Scientifico - Advanced Programming for Scientific Computing) and Advanced Methods for Scientific Computing (AMSC) courses at Politecnico di Milano.
**A NOTE** For the AMSC course, only a subset of the examples will be used at the lectures and labs.
However, I encourage interested students to look at the various examples.

The repository contains git submodules, so to check it out properly, you have to type

```bash
git clone --recursive https://github.com/HPC-Courses/pacs-examples.git
```

or **(preferably)**

```bash
git clone --recursive git@github.com:HPC-Courses/pacs-examples.git
```
*In the latter case, you must register your ssh keys on a personal GitHub account*.

To keep your local repo updated with changes upstream, do frequently

```bash
git pull --recurse-submodules
```

or just from this directory,

```bash
./update.sh
```

The software requires a C++ compiler, i.e., gcc >= 10.0 or clang >= 9.
You may check the version of your compiler with the option -v. Several examples have now been ported to c++17
and some examples of c++20 are being added. Check whether your compile supports those standards. You may have a look
[here](https://en.cppreference.com/w/cpp/compiler_support)

## Module (mk) system

In the course, we use the mk module system provided in [this repository](https://github.com/pcafrica/mk_) (nothing to do with girt submodules!). However, this is not strictly required, you can do without, if you have
a recent Linux distribution.

You may want to use some IDE. Either [eclipse](https://www.eclipse.org/ide/), [CLion](https://www.jetbrains.com/clion/) or [VisualStudio](https://www.jetbrains.com/clion/). Clion and VisualStudio allow integration with Copilot. Visual studio allows also integration with dev containers

## Directory Contents

This directory contains some utilities:

**load_modules**
If you use the module (mk) system, by typing

```bash
source load_modules
```

or

```bash
. load_modules.sh
```

will load the main modules used in the examples, apart from  `hdf5`, which is used only in the example on input-output streams. If you do not use module youu do not need it.

**change_submodules_url.sh**
This script changes the protocol of the submodules to https. You may need it if you use containers (see later on)

**change_submodules_url_ssh.sh**
This script changes the protocol of the submodules to ssh. SSSh protocol is the default for the submodules. You may need this utilityit if you use containers (see later on)

## Installing git submodules

If you do not have used `--recursive` when cloning this repo and you want to use
the git submodules containing third-party software type:

```bash
./install_git_submodules.sh
```

The file submodule_commands.txt contains some reminders of useful git commands for operating with submodules

- `change_submodules_url.sh` changes the protocol of the submodules to https. You should not need it!

- `change_submodules_url_ssh.sh` changes the protocol of the submodules to ssh. You should not need it!

## WHAT DO DO NEXT?

- Go to the `Extras/` directory, which contains additional software used by some examples and follow the instructions in the local `README.md` file.
- Go to the `Examples/` directory and follow the instructions in the local `README.md` file.

## WHY MAKEFILES?

I have used standard makefiles since I find that they allow simpler handling of "disaggregated software", where you have several partially independent examples which share, however, some common features. Indeed porting to `cmake` is possible, but so far it is not planned. If you want to do it, you are welcome to contribute.

## DOCKER CONTAINERS

You can download and use the Examples (together with the exercises) in a dev container of Visual Studio Code or in an Apptainer sandbox. For more information on how to use dev containers, please visit the [Visual Studio Code documentation on dev containers](https://code.visualstudio.com/docs/remote/containers), for Apptainer sandbox look at this  [documentation](https://apptainer.org/docs).

To do that you can to download the pacs container git repo

```bash
git clone https://github.com/pacs-course/PacsContainer.git
```

or

```bash
git clone git@github.com:pacs-course/PacsContainer.git
```

if you prefer ssh protocol, and follow the instructions in the README.md file of the container repo.

Or, if you already know what to do, simply use the docker hub address of the image:

```bash
docker pull lucaformaggia/pacs-exercises:latest
```

### For people using Visual Studio Code
If you use Visual Studio Code you can use the `Remote - Containers` extension to open the Examples in a dev container. You can find the extension in the Visual Studio Code marketplace. 

After having dowloaded the PacsContainer repo, you can open the Examples in a dev container by opening the command palette (Ctrl+Shift+P) and typing `Remote-Containers: Open Folder in Container...`. Then select the folder `vsCode_precompiled` in the PacsContainer repo.

### For people using Apptainer
If you use Apptainer, you can use the sandbox to open the Examples in a dev container. it is sufficient tu run the script `make_apptainer_sandbox.sh` in the `scripts` directory of the PacsContainer repo.    

### For people using Docker
You find the full Docker file that I use to build the container in the `Docker` directory of the PacsContainer repo. You can use it to build your own container.

### For people using Clion
You can use the `Remote Development` plugin of Clion to open the Examples in a dev container. You can find the plugin in the Clion marketplace.
Unfortunatley dev containers in cLion are not as well integrated as in Visual Studio Code. You can use the `Remote Development` plugin to open the Examples in a dev container, but you have to manually set up the container. You can find the Dockerfile in the `Docker` directory of the PacsContainer repo.


## DISCLAIMER

The code in this repository is just a collection of examples. Some of them are rather complete and may form the basis of derivative work. You are free to use them at your wish (give credits please and let me know of bugs or possible improvements), but remember that *I do not make any claim on their suitability for your work, nor that they are free of bugs*. 

Luca Formaggia (github username: lformaggia)

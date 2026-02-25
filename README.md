# IMPORTANT NOTICE #
On the 16th of February 2026, this repo underwent a major update. It is no longer compatible with the previous state
If you cloned the repository before the 16th of February, you need to clone it again! Pull and push will not work

EXAMPLES AND EXERCISES FOR AMSC and APSC (PACS) COURSES
--------------------------------------
This repo contains source code and descriptions for the examples and exercises
that will be explained during the PACS (Programmazione Avanzata per il Calcolo
Scientifico - Advanced Programming for Scientific Computing) and Advanced Methods for Scientific Computing (AMSC) courses at Politecnico di Milano.
*For the AMSC course, only a subset of the examples will be used at the lectures and labs.*
I encourage interested students to look at the various examples.

**Essential note**: The examples for the AMSC course are located in the `AMSC` branch, not in the `master` branch. 
You can create other branches for your experiments, if you want, but **do not mess around with the `master` branch**
The master branch is the main branch for the APSC (PACS) course.


The repository contains git submodules, so to check it out properly, you have to type

```
git clone --recurse-submodules https://github.com/HPC-Courses/AMSC-CodeExamples.git
```

or **(preferably)**

```
git clone --recurse-submodules git@github.com:HPC-Courses/AMSC-CodeExamples.git
```
*In the latter case, you must register your ssh keys on a personal GitHub account*.


To keep your local repo updated with changes upstream, do frequently:

```bash
git pull --recurse-submodules
```

or just from this directory,

```bash
./update.sh
```

The software requires a C++ compiler, i.e., gcc >= 13.0 or clang >= 18.
You may check the version of your compiler with the option -v. Several examples have now been ported to C++20, and some C++23 constructs are also used sometimes. Check whether your compiler supports those standards. You may have a look
[here](https://en.cppreference.com/w/cpp/compiler_support)

## Module (mk) system

In the course, we may use the mk module system provided in [this repository](https://github.com/pcafrica/mk_) (nothing to do with git submodules!). However, this is not strictly required; we can do without it if you have
a recent Linux distribution.

You may want to use an IDE. Either [eclipse](https://www.eclipse.org/ide/), [CLion](https://www.jetbrains.com/clion/) or [VisualStudio](https://www.jetbrains.com/clion/) are good choices (in my opinion). Clion and Visual Studio support Copilot integration. Visual Studio also allows integration with dev containers

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

will load the main modules used in the examples, apart from  `hdf5`, which is used only in the example on input-output streams. If you do not use the mk module system, you do not need it.

**change_submodules_url.sh**
This script changes the submodules' protocols to https. You may need it if you use containers (see later on)

**change_submodules_url_ssh.sh**
This script changes the submodules' protocol to SSH. SSS protocol is the default for the submodules. You may need this utility if you use containers (see later on)

## Installing git submodules

If you have not used `--recurse-submodules` when cloning this repo, and you want to use
The git submodules containing third-party software type:

```bash
./install_git_submodules.sh
```


The file submodule_commands.txt contains some reminders of useful git commands for operating with submodules

- `change_submodules_url.sh` changes the protocol of the submodules to https. You should not need it!

- `change_submodules_url_ssh.sh` changes the protocol of the submodules to ssh. You should not need it!

## WHAT TO DO NEXT?

- Go to the `Extras/` directory, which contains additional software used by some examples, and follow the instructions in the local `README.md` file.
- Go to the `Examples/` directory and follow the instructions in the local `README.md` file. 

## What ELSE? ##

If you want the repository of the laboratories of the course, you can clone  [this repo](https://github.com/HPC-Courses/AMSC-Labs)

## WHY MAKEFILES? ##
I have used standard makefiles since I find that they allow simpler handling of "disaggregated software", where you have several partially independent examples which share, however, some common features. Indeed, porting to `cmake` is possible, but it is not planned so far. 

## DISCLAIMER ##
The code in this repository is just a collection of examples. Some of them are rather complete and may form the basis of derivative work. You are free to use them at your discretion (please give credit and let me know of any bugs or possible improvements), but remember that *I do not make any claim about their suitability for your work, nor that they are free of bugs*. 

Luca Formaggia (github username: lformaggia)

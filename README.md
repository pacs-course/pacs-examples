EXAMPLES AND EXERCISES FOR AMSC and APSC (PACS) COURSES
--------------------------------------

This repo contains source code and descriptions for the examples and exercises
that will be explained during the PACS (Programmazione Avanzata per il Calcolo
Scientifico - Advanced Programming for Scientific Computing) and Advanced Methods for Scientific Computing (AMSC) courses at Politecnico di Milano.

**A NOTE** For the AMSC course, only a subset of the examples will be used at the lectures and labs. 
However, I encourage interested students to look at the various examples.

The repository contains git submodules, so to check it out properly, you have to type

```
git clone --recursive https://github.com/HPC-Courses/AMSC-CodeExamples.git
```

or **(preferably)**

```
git clone --recursive git@github.com:HPC-Courses/AMSC-CodeExamples.git
```
*In the latter case, you must register your ssh keys on a personal GitHub account*.

To keep your local repo updated with changes upstream, do frequently

```
git pull --recurse-submodules
```

or just from this directory,

```
./update.sh
```

The software requires a C++ compiler, i.e., gcc >= 8.0 or clang >= 8.
You may check the version of your compiler with the option -v. Several examples have now been ported to c++17
and some examples of c++20 are being added. Check whether your compile supports those standards. You may have a look
on https://en.cppreference.com/w/cpp/compiler_support

You may want to use some IDE. Either [eclipse](https://www.eclipse.org/ide/), [CLion](https://www.jetbrains.com/clion/) or [VisualStudio](https://www.jetbrains.com/clion/). Clion and VisualStudio allow integration with Copilot.

## Directory Contents ##
This directory contains some utilities:

**  load_modules
If you use the module (mk) system, by typing

```
source load_modules
```
or
```
. load_modules.sh
```

will load the main modules used in the examples, a part hdf5 which is used only on the example on iostreams (binary file).

**  install_modules.sh

If you do not have used --recursive when cloning this repo and you want to use
the git submodules containing third-party software type:
```
./install_modules
```

The file submodule_commands.txt contains some reminders of useful git commands for operating with submodules


** change_submodules_url.sh
Change the protocol of the submodules to https. You should not need it!

** change_submodules_url_ssh.sh
Change the protocol of the submodules to ssh. You should not need it!

## WHAT DO DO NEXT? ##

- Go to the `Extras/` directory that contains additional software used by some examples and follow the instructions in the local `README.md` file.
- Go to the `Examples/` directory and follow the instructions in the local `README.md` file. 

## What ELSE? ##

If you want the repository of the laboratories of the course, you can clone  [this repo](https://github.com/HPC-Courses/AMSC-Labs)



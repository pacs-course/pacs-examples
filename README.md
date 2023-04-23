EXAMPLES AND EXERCISES FOR PACS COURSE
--------------------------------------

This repo contains source code and descriptions for the examples and exercises
that will be explained during the PACS (Programmazione Avanzata per il Calcolo
Scientifico - Advanced Programming for Scientific Computing) course at
Politecnico di Milano.

The repository contains submodules, so to check it out properly you have to type

```
git clone --recursive https://github.com/pacs-course/pacs-examples.git
```

or

```
git clone --recursive git@github.com:pacs-course/pacs-examples.git
```

In the latter case you have to register your ssh keys on a github account.

To keep your local repo update with changes upstream do frequently

```
git pull --recurse-submodules
```

or just, from this directory,

```
./update.sh
```


The software requires a c++ compiler, i.e. gcc >= 8.0 or clang >= 8.
You may check the version of you compiler with the option -v. Several examples have now been ported to c++17
and some examples of c++20 are being added. Check wether your compile support those standards. You may have a look
on https://en.cppreference.com/w/cpp/compiler_support

This directory contains some utilities:

**  load_modules
If you use the module system, typing

```
source load_modules
```
or
```
. load_modules.sh
```

will load the main modules used in the examples, a part hdf5 wich is used only on the example on iostreams (binary file).

**  install_modules.sh

If you do not have used --recursive when cloning this repo and you want to use
the git submodules containing third party software type:
```
./install_modules
```

The file submodule_commands.txt contains some reminder of useful git commands for operating with submodules


** change_submodules_url.sh
Change the protocol of the submodules to https. You should not need it!

** change_submodules_url_ssh.sh
Change the protocol of the submodules to ssh. You should not need it!

## WHAT DO DO NEXT? ##

- Go to the `Extras/` directory that contains additional software used by some examples and follow the instructions in the local `README.md` file.
- Go to the `Examples/` directory and follow the instructions in the local `README.md` file. 

## What ELSE? ##

  * Labs/` Directory with the laboratory sessions of the course, subdivided by year! Enjoy!
  




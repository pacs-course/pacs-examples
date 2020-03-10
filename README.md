EXAMPLES AND EXERCISES FOR PACS COURSE
--------------------------------------

This repo contains source code and descriptions for the examples and exercises
that will be explained during the PACS (Programmazione Avanzata per il Calcolo
Scientifico - Advanced Programming for Scientific Calculus) course at
Politecnico di Milano.

The software require a c++ compiler, i.e. gcc >= 5.0 or clang >= 5.
gcc 9.2 is the one you find in the module system provided. But you may
check the version of you compiler with the option -v Some examples have now been ported to c++17

This directory contains some utilities:

**  load_modules
If you use the module system, typing

source load_modules
or
. load_modules.sh

will load the main modules used in the examples, a part hdf5 wich is used only on the example on iostreams (binary file).

**  install_modules.sh

If you do not have used --recursive when cloning this repo and you want to use
the git submodules containing third party software type:

./install_modules

The file submodule_commands.txt contains some reminder of useful git commands for operating with submodules


** change_submodules_url.sh
Change the protocol of the submodules to https. You should not need it!

** change_submodules_url_ssh.sh
Change the protocol of the submodules to ssh. You should not need it!


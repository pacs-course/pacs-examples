# Course on Advanced programming for scientific computing #
## Mathematical Engineering, Politecnico di Milano ##
# Course on Advanced Modelling for scientific computing #
## HPC and Big Data, Politecnico di Milano ##
## Copyright Luca Formaggia 2012-2023 ##

## LICENCING ##
The software contained in the subfolders of this directory is free-software released under a GNU General Public Licens (GPL). The details of the licence are availabe in the `COPYRIGHT` file in the root directory.

## CONTENT ##

`bin/` -> Directory where the executables are installed. It contains some scripts to beautify your code (see below)

`lib/` -> Directory where the libraries are installed. Initially empty, but it will be filled with the libraries produced by the examples

`lib64/` -> Only for compatibility. In fact is a link to `lib`

`include/` -> General include files. Initially empty, but it will be filled with the include files of the examples

`share/` -> Directory where the documentation is installed. Initially empty, but it will be filled with the documentation of the examples

`src/` -> Directory with the example sources. Probably the most important directory. Look at the `CONTENT.md` file for a description of the examples.

`astyle_scripts_and_hooks/` Some tools to beautify your code, based on [Artistic Style](http://astyle.sourceforge.net/) (you must install astyle if you wish to use the tools in this directory).

`clang-format_scripts_and_hooks/` Some tools to beautify your code, based on [clang-format](https://www.electronjs.org/docs/latest/development/clang-format) (you must have clang-tools installed! if you want to use the utilities in this directory)

`environment.sh` A bash script that sets the environment variables needed by the examples. It can simplify life if you are not using the mk-modules adopted in the course. In that case you can copy it in your `.profile` file in your home directory (see note at the end of the file), **after having changed the path correctly**. 


`setup.sh` A bash script that compiles the main utilities in one shot so that you have everything set. It is explained in the next section.

`Makefile_description.md` A file that describes the CMake build system used in the examples. It is explained in the next section.

## HOW TO COMPILE AND INSTALL THE EXAMPLES: ##

### THE FIRST STEP ###

Open a terminal and go in the directory `Examples` of the repository (the directory that contains this README.md file, from now on denoted as **root directory**).

The build system is now CMake. Each example directory under `src/` is an independent CMake project, so you can compile any example in-place.

Typical out-of-source build (recommended):

```bash
cmake -S . -B build
cmake --build build
cmake --install build
```

In-source build (allowed, but it will create build artifacts in the example directory):

```bash
cmake -S . -B .
cmake --build .
cmake --install .
```

By default, install goes to `Examples/include` and `Examples/lib`. If needed, you can override the root with:

```bash
cmake -S . -B build -DPACS_ROOT=/path/to/Examples
```

You can also configure additional paths with CMake cache variables, for example:

- `-DEIGEN_DIR=/path/to/eigen3`

More details in the `Makefile_description.md` file (now describing the CMake system).

## WORKING WITH MODULES (but also if you do not use them...) ##

If you are using modules, some environmental variables are set by the
module system and can be passed to CMake as cache variables if needed. In particular

- `mkEigenInc`    Where the Eigen libraries (infact the header files) is stored
- `mkOpenmpiLib`  The directory with the mpi libraries
- `mkOpenmpiInc`  The directory with the mpi headers
- `mkTbbInc`  Include files for the threding building block libraries (needed for parallel algorithms)
- `mkTbbLib`  Library files for the threading building block libraries (needed for parallel algorithms)
- `mkCxxCompiler` The C++ compiler of your choice
- `mkEigenHome`      Where the Eigen files are kept (normally equal to MkEigenInc)


Other important environment variables set up by the module system that may be 
not used directly by the CMake files but may still be useful for some examples are

- `mkClangSystemBin`  The directory holding all clang compiler executables 
- `mkCCompiler`       The C compiler of your choice
- `mkSuitesparseLib`  Directory holding the libraries of the suitesparse suite (umfpack for instance)
- `mkSuitesparseInc`  Directory holding the header files of the suitesparse suite (umfpack for instance)
- `mkCgalLib`         Directory with the CGAL libraries (computational geometry libraries)
- `mkCgalInc`         Directory with the CGAL header files
- `mkBoostInc`        Directory with the boost libraries     
- `mkBoostLib`        Directory with the boost header files
- `mkHdf5Lib `       The directory with the hdf5 libraries
- `mkHdf5Inc`        The directory with the hdf5 header files


So, you have two main choices

  * You use the module system provided with the virtual machine given in the course. Then
most paths are already available and CMake will typically find them automatically. If you want to be explicit, pass them via `-D...` arguments (e.g. `-DEIGEN_DIR=$mkEigenInc`).

-------------------------------------------------------------------------------

```
env | grep mk 
```

-------------------------------------------------------------------------------

  * You do not use the module system. Then you have again two choices
   * You pass the relevant paths directly to CMake using `-D` cache variables
   * You simulate the module environment by creating the environmental variables: you have to put in the `.bash-profile` (or `.profile `) 
   file in your home directory the corresponding instructions
        for the bash (the `.bash-profile` and `~/.profile` files are sourced by the bash shell every time you do a login).  For example, in my `.profile` I have:

-------------------------------------------------------------------------------

    export mkSuitesparseInc=/usr/include/suitesparse/
    export mkSuitesparseLib=/usr/lib/x86_64-linux-gnu
    export mkCCompiler=gcc
    export mkCxxCompiler=g++
    export mkTbbInc=/usr/include
    export mkTbbLib=/usr/lib/x86_64-linux-gnu/
    export mkHdf5Inc=/usr/include/hdf5/serial/
    export mkEigenHome=/usr/local/include/eigen3
    export mkEigenInc=/usr/local/include/eigen3
    export mkBoostInc=/usr/local/boost_1_72_0/include
    export mkBoostLib=/usr/local/boost_1_72_0/lib
    export mkOpenmpiLib=/usr/lib/x86_64-linux-gnu
    export mkOpenmpiInc=/usr/include
    export mkCgalInc=/usr/include
    export mkCgalLib=/usr/lib/x86_64-linux-gnu


-------------------------------------------------------------------------------

Remember that after you have modified your .profile file you need to do a new login for the changes to be effective (see note at the end of the file)

### SECOND STEP: INSTALL UTILITIES ###

The examples require some common utilities that are contained in `src/Utilities/`. So it is simpler if we compile and install them once for all.

You must go to `src/Utilities` and do

```bash
cmake -S . -B build
cmake --build build
cmake --install build
```

This way, a library called libpacs.a (and ist dynamic equivalent
libpacs.so) is installed in `PACS_LIB_DIR` and some header files are
installed in `PACS_INC_DIR`

If you then build the test executables (they are part of the same directory) compilation should complete with no errors. You may have at look at the tests and try to execute them.

More advanced examples may use also the Utilities in `LinearAlgebraUtil`. Go in that directory and  just type

```bash
cmake -S . -B build
cmake --build build
cmake --install build
```

to install everything in the `Examples/include` directory.
Here the tests have been collected in the subfolder `test`, but the rule is the same, go there and run the CMake build commands.

Have a look at the tests because they contain information on how to use the utilities!

To simplify life, you can just do
```bash
bash ./setup.sh
```
and averything is done for you.


### THIRD STEP EXTRAS ###
Some utilities that are used in a few examples (and interesting on their own) are kept as submodules, since they refer to third party software.
To compile and install them, go in the `Extras` directory of the repository and follow the instructions in the `README.md` file stored in that directory.

**NOW YOU ARE READY TO ENJOY THE EXAMPLES**

**NOTE FOR C++ PARALLEL ALGORITHMS** 
Some test use the native parallel programming of c++ that, at
least with gnu and LLVM compiler, requires having the multithreading
building block library installed, and to link to the `libtbb.so`
library when compiling your code. Check that the library is installed in your system (it's
available on all Linux distribution, but not necessarily by default!).


## CODE DOCUMENTATION ##
I tried, as far as possible, to use [Doxygen](https://www.doxygen.nl/) to document the code. Doxygen requires to be run in the directory with the specific source files
of which you want to produce documentation. It uses a *special file*, typically called `Doxyfile`, of which you can generate a prototype with the command `doxygen -g`. The file should then be changed to satisfy your needs. However, I have produced in this folder a Doxyfile, called `DoxyfileCommon`, with the most useful options already set. You have to

- Look for the line with `INCLUDE_PATH` and change the last path with the full path of the folder where the general include files of the Examples reside in your PC. It may be found easily by adding `/include` to the string
obtaining by typing `pwd` in the folder where this README file resides. 
- If you have not [graphviz](https://graphviz.org/) installed (but I warmly suggest you to intall it, it is available in any Linux distro), you also have to set `HAS_DOT` to `NO`.
- After this preliminary steps, go in the directory where the specific example reside, copy the `DoxyfileCommon` with the name `Doxyfile` and run `doxygen` (or `doxygen Doxifile`).
- In the sub-directory `doc/html` you should have the file `index.html`, that you can open with your favourite browser and look at the documentation. More info on how to use Doxygen is availabel in the Doxigen site.

## FURTHER INFO ##

- An overview of the examples is in the `CONTENT.md` file
- The bash script `directories.sh` creates a searcheable
tree, `directory.html` of the subfolder `src` that may be parsed with
a browser. To run it you must have the unix utility `tree`
installed. Anyway, a copy of `directories.html` is present in this directory.

### The content of `src` Directory ###
A description of the examples in the `src` directory is found in `CONTENT.md`.

### A note on `.bashrc` or `.profile` (or `.bash_profile`) ### 

To control the settings of your Linux environment everytime you login
the (hidden) file `.bash_profile` (or `.profile` if you are using its
old, but still valid, name) in your home directory is sourced (its
content is run as if typed on a terminal). Everytime you open a bash
shell the `.bashrc` file is sourced. 

What's the difference? Well today people prefer to put everything in
the `.bashrc` file, but in principle definitions of environmental variables
should be put in the `.bash_profile`  (or `.profile`),
while `.bashrc` should be reserved for command alias or customization
that need to change depending whether you are in a interactive shell (i.e. a terminal)
or not (batch job). 

A practical difference is that if you change `.bash_profile`  (or `.profile`)
the changes become operative only after a new login. 
Changes in `.bashrc` becomes active just by opening a new terminal.

If this is too confusing for you, replace `.profile` with `.bashrc` in
my instructions above and everything will work the same way in
practice.

If you want to know more (and discover that you have also
`.bash_login`) go
[here](https://www.baeldung.com/linux/bashrc-vs-bash-profile-vs-profile),
or look to any good Unix reference manual.

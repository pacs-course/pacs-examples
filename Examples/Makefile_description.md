# The Makefile system of the examples.

Each examples has a makefile that allowd to compile the code and or produce libraries, depending on the case. 

All makefiles follow the same structure (a parte some exceptions). They

- Include the file `Makefile.inc` in `Examples` folder (also called root folder) that contains the configuration of the system and is produced frome `Makefile.user` by eventually changing come of th econtained variables. In particular `PACS_ROOT` that must contain the full path of the root folder of the examples.
- Optionally includes a local `Makefile.inc`, that can be used to change some of the variables defined in the root `Makefile.inc`.
- For the rest, it is based on the `Makefile.template` file contained in the root folder, or `Makefile_tempalte_lib` if the makefile is used to produce a library.

Let's descrive the content of the various components

## The makefile.inc file

The `Makefile.inc` file is a configuration file used in conjunction with `make` to build software projects. It defines various environment variables and paths required for the build process. It should be generated copying `Makefile.user` and eventually changing some of the variables.

Below is an explanation of the content, which can be modified to suit the user specific requirements for all examples.

I recall that the make expression 

```make
VAR?=value
```

assigns `value` to `VAR` only if `VAR` is not already defined. And, in particular, if `VAR` is defined in the environment (using, for instance, the bash command `export VAR=value`), that value of `VAR` is used.

I also recall that yu can change the variable value when invoking `make` by using the syntax

```bash
make target CXX=clang++
```
This indicates that the definition of `CXX` in the Mafefile will be overridden with `clang++`.

```plaintext
# The directory where the examples reside (you HAVE to change this)
PACS_ROOT?=/home/forma/Work/Didattica/HPC_Courses/pacs-examples/Examples
```

- `PACS_ROOT` is a variable that specifies the root directory where the examples reside. You should change it to your own root directory. There are two possibilities:

- You can set the variable in the environment by using the command `export PACS_ROOT=/path/to/your/root/directory`.
- You can set the variable in the `Makefile.inc` file by adding the line `PACS_ROOT=/path/to/your/root/directory`.


```plaintext
# The directory with the Eigen (leave blank if they are in a standard directory)
mkEigenInc?=/usr/local/include/eigen3/
EIGEN_DIR=$(mkEigenInc)
```
- `mkEigenInc` is a variable that specifies the directory where the Eigen library headers are located. `EIGEN_DIR` is set to the value of `mkEigenInc`.

```plaintext
# Mpi directories for library and include files
mkOpenmpiLib?=/usr/lib/x86_64-linux-gnu
mkOpenmpiInc?=/usr/include
MPI_LIBDIR=${mkOpenmpiLib}
MPI_INCDIR=${mkOpenmpiInc}
```
- `mkOpenmpiLib` and `mkOpenmpiInc` specify the directories for the MPI library and include files, respectively. `MPI_LIBDIR` and `MPI_INCDIR` are set to the values of `mkOpenmpiLib` and `mkOpenmpiInc`.

```plaintext
# C++ native parallelization support with g++ needs the TBB libraries
mkTbbInc?=/usr/include
mkTbbLib?=/usr/lib/x86_64-linux-gnu/
TBB_LIBDIR=${mkTbbLib}
TBB_INCDIR=${mkTbbIn}
```
- `mkTbbInc` and `mkTbbLib` specify the directories for the TBB (Threading Building Blocks) library and include files, respectively. `TBB_LIBDIR` and `TBB_INCDIR` are set to the values of `mkTbbLib` and `mkTbbInc`.

There are other variables that are set in the `Makefile.inc` file, but the above are the most important ones. You can modify the values of these variables to suit your requirements.

### Summary
`Makefile.inc` is needed to set up various paths and directories required for building any of the examples listed in this repository.

## The Makefile.template file
The `Makefile.template` file serves as a base template for creating individual makefiles for each example. It contains the general structure and rules that are common across all examples, ensuring consistency and reducing redundancy. Below is an overview of its content:

### Content of `Makefile.template`
The provided Makefile is designed for a course on Advanced Programming for Scientific Computing. It includes several sections and functionalities that help manage the compilation and cleaning of a project. Here's a breakdown of its content:

### Header Comments
- The initial comments describe the purpose of the Makefile.

### Variables
- `MAKEFILEH_DIR`: Specifies the directory where the main `Makefile.inc` resides.
- `SRCS`: Uses `wildcard` to get all `.cpp` files in the directory.
- `OBJS`: Converts `.cpp` files in `SRCS` to `.o` object files.
- `HEADERS`: Uses `wildcard` to get all `.hpp` header files in the directory.
- `exe_sources`: Filters `SRCS` to get main source files (`main_XX.cpp`).
- `EXEC`: Converts `exe_sources` to executable names.

### Include Statements
- Includes `Makefile.inc` from the directory specified by `MAKEFILEH_DIR`.
- Optionally includes `Makefile.inc` from the current directory.

### Targets
- `.phony`: Declares phony targets (`all`, `clean`, `distclean`, `doc`).
- `.DEFAULT_GOAL`: Sets the default target to `all`.

### Target Definitions
- `all`: Depends on `$(DEPEND)` and `$(EXEC)`.
- `clean`: Removes executables and object files.
- `distclean`: Runs `clean` and removes additional files like documentation and dependencies.
- `doc`: Generates documentation using Doxygen.
- `$(EXEC)`: Depends on `$(OBJS)`.
- `$(OBJS)`: Depends on `$(SRCS)`.
- `install`: Copies header files to `$(PACS_INC_DIR)`.
- `$(DEPEND)`: Generates dependency files for the source files.

### Dependency Management
- `-include $(DEPEND)`: Includes the dependency file if it exists.

This Makefile automates the process of compiling source files, cleaning up generated files, and managing dependencies, making it easier to handle complex projects.

### Makefile for Libraries
The `Makefile_tempalte_lib` file is similar to `Makefile.template` but is used for building libraries. It includes additional rules for creating and installing libraries, as well as generating documentation. The main difference is the absence of executable targets and the addition of library-specific rules.

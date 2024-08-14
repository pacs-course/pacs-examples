# Examples of usage of pybind11 #

`pybind11` is a lightweight header-only library that exposes C++ types in Python and vice versa, mainly to create Python bindings of existing C++ code.

It is available via
```bash
 module load pybind11
```
if you use the module system or via

```bash
pip install pybind11
```
or, by using the submodule stored in `Extras/pybind11`, which should be automatically downloaded if you do

```bash
git pull --recurse-submodules
```
or you have cloned the repository with the option `--recurse-submodules`.

Then, you have to do

```bash
cd Extras/pybind11
./install_PACS.sh
```

## Compilation ##
A compiled python module is a shared library, so its compilation follows the rule for compiling a shared library

The complex part is the naming scheme, which is very peculiar and the need of selecting the correct path for python headers. 
In the provided Makefiles this is done just by asking python to give the correct flags. A an example, assuming that my python configuration command is `python3-config` and that I want to create a module named `basicOptim`, the Makefile should contain the following lines:

```makefile
MODULENAME=basicOptim# Must be consistent with what declared in the pybind wrapper
PY_INCLUDES != python3-config --includes
PY_EXT != python3-config --extension-suffix
PY_FLAGS=-fPIC -flto #flto is not strictly needed
PY_MODULE=$(MODULENAME)$(PY_EXT) # The name of the python module
CXXFLAGS+=$(PY_FLAGS) # Add the python flags to the C++ flags
CPPFLAGS+=$(PY_INCLUDES) # Add the python headers
PY_LDFLAGS != python3-config --ldflags
LDFLAGS+=-shared -flto  # The flags to create a shared library
LDLIBS+=$(PY_LDFLAGS) # additional libraries to produce the module (often not needed)
```

The `-flto` flag is not strictly needed, but it is a good idea to use it. It activates link time optimization and it is suggested to better efficiency. It must be used buth a compilation and linking stages.
In several examples it is useless when the code is just in one translation unit. It becomes interesting when the code forming the python module is split in several translation units.
The `shared` flag is needed to create a shared library, and a python module is a shared library.

You can then simply do
```makefile
$(PY_MODULE): $(PY_OBJS)
```
where `PY_OBJS` contain the name of the object files needed to create the python module.

## Documentation ##
You find a very detailed documentation of pybind11, with several other examples, in [this web site](https://pybind11.readthedocs.io/en/stable/).






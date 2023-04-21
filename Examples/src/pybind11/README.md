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
or, by using the submodule stored in `Extras/pybind11`, which should be automatically downloaded if you do `git pull --recurse-submodules` or you have clodes the repository with `--recurse submodules`. Then =, you have to do
```bash
cd Extras/pybind11
./install_PACS.sh
```

## Documentation ##
You find a very detailed documentation in [this web site](https://pybind11.readthedocs.io/en/stable/).






# Extra Material #

This directory contains some extra material, in particular code obtained from other sources and made available (when the copyright allows it, of course).

## External libraries ##

Here you find a set of external libraries that have been forked to be adapted to the convention I use in the course.

- `muParser`: a copy of the [muparser](https://beltoforion.de/en/muparser/) library (a fork of the original git repo). The script PACS_install.sh will install the library in the local pacs directories
- `muParserx`: a copy of the [muparserX](https://beltoforion.de/en/muparserx/) library (a fork of the original git repo). The script PACS_install.sh will install the library in the local pacs directories
- `json`: A fork of [JSON for Modern C++](https://json.nlohmann.me/) by Niels Lohman. Tools for reading/writing json files and much more.
- `pybind11`. A modified copy of the repository  one of the best python wrappers for C++. The modifications consist only in adaptation to the environment used for the PACS Examples.


All directories contain a `README_PACS.md` file for specific instructions related to the PACS environment (read also the original `README.md` file, if present). 
There you find the instructions on how to install the tool. We assume that the c++ compiler is gnu g++ and that you have the `install` command installed (if not and you have Ubuntu-type Linux distributions you can do `apt-get install coreutils` to obtain it).

## A little utility to help you ##
If you do not want to get into the separate tools  but you are breve enough to try compile all of them in one go, just type
```
bash ./install_extras.sh
```
or, simply,
```
./install_extras.sh
```


## Other material ##

This directory contains also some additional material.

- `LinearAlgebraBooksandNotes/` As the name says. A few books and notes on linear algebra and not just linear algebra
- `CRLFSaga.odt` The Carriage Return/Line Feed issue: why text files on Windows are different than in Linux/Unix/MacOS?
- `CppCon14Lecture` Lectures on the C++14 standard by the CppCon community
- `Vagrant_guid.pdf` A guide on Vagrant, a tool for virtual machines.

- `tounix`. A simple shell script to transform a DOS/Windows text file to Unix, by changing the line ending convention.

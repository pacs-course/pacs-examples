#!/bin/bash
git config --file=.gitmodules submodule.Examples/src/LinearAlgebra/CppNumericalSolvers.url https://github.com/pacs-course/CppNumericalSolvers.git
git config --file=.gitmodules submodule.Examples/src/LinearAlgebra/spectra.url https://github.com/pacs-course/spectra.git
git config --file=.gitmodules submodule.Examples/src/LinearAlgebra/redsvd-h.url https://github.com/pacs-course/redsvd-h.git
git config --file=.gitmodules submodule.Extras/muparser.url https://github.com/pacs-course/muparser.git
git config --file=.gitmodules submodule.Extras/muparserx.url https://github.com/pacs-course/muparserx.git
git config --file=.gitmodules submodule.Extras/json.url https://github.com/pacs-course/json.git
git config --file=.gitmodules submodule.Extras/pybind11.url https://github.com/pacs-course/pybind11.git
git config --file=.gitmodules submodule.Examples/src/pybind11/pybind11_examples.url https://github.com/pacs-course/pybind11_examples.git

git submodule sync
git submodule update --recursive --remote


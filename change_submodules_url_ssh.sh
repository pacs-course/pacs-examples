#!/bin/bash
git config --file=.gitmodules submodule.Examples/src/LinearAlgebra/CppNumericalSolvers.url git@github.com:pacs-course/CppNumericalSolvers.git
git config --file=.gitmodules submodule.Examples/src/LinearAlgebra/spectra.url git@github.com:pacs-course/spectra.git
git config --file=.gitmodules submodule.Examples/src/LinearAlgebra/redsvd-h.url git@github.com:pacs-course/redsvd-h.git
git config --file=.gitmodules submodule.Extras/muparser.url git@github.com:pacs-course/muparser.git
git config --file=.gitmodules submodule.Extras/muparserx.url git@github.com:pacs-course/muparserx.git
git config --file=.gitmodules submodule.Extras/json.url git@github.com:pacs-course/json.git
git config --file=.gitmodules submodule.Extras/pybind11.url git@github.com:pacs-course/pybind11.git
git config --file=.gitmodules submodule.Examples/src/pybind11/pybind11_examples.url git@github.com:pacs-course/pybind11_examples.git

git submodule sync
git submodule update --recursive --remote


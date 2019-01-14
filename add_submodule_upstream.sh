#!/bin/bash
pushd Examples/src/LinearAlgebra/CppNumericalSolvers
git remote add upstream git@github.com:PatWie/CppNumericalSolvers.git
popd
pushd Examples/src/LinearAlgebra/redsvd-h
git remote add upstream git@github.com:ntessore/redsvd-h.git
popd
pushd Examples/src/LinearAlgebra/spectra
git remote add upstream https://github.com/yixuan/spectra.git 
popd
pushd Extras/muparser
git remote add upstream git@github.com:beltoforion/muparser.git
popd
pushd Extras/muparserx
git remote add upstream git@github.com:beltoforion/muparserx.git


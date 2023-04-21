# set up for modules. This file should be included using
# source load_modules.sh
# if you want to use the gcc toolchain + main utilities used in the examples
source /u/sw/etc/profile
module load gcc-glibc/11
module load superlu
module load eigen
module load openblas
module load suitesparse
module load pybind11
# for the example on file i/o
# module load hdf5 

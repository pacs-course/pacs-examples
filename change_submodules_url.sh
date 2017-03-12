git config --file=.gitmodules submodule.Examples/src/LinearAlgebra/CppNumericalSolvers.url https://github.com/lformaggia/CppNumericalSolvers.git
git config --file=.gitmodules submodule.Examples/src/LinearAlgebra/spectra.url https://github.com/lformaggia/spectra.git
git config --file=.gitmodules submodule.Examples/src/LinearAlgebra/redsvd-h.url https://github.com/lformaggia/redsvd-h.git
git submodule init
git submodule sync
git submodule update --init --recursive --remote


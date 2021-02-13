git config --file=.gitmodules submodule.Examples/src/LinearAlgebra/CppNumericalSolvers.url https://github.com/lformaggia/CppNumericalSolvers.git
git config --file=.gitmodules submodule.Examples/src/LinearAlgebra/spectra.url https://github.com/lformaggia/spectra.git
git config --file=.gitmodules submodule.Examples/src/LinearAlgebra/redsvd-h.url https://github.com/lformaggia/redsvd-h.git
git config --file=.gitmodules submodule.Extras/muparser.url https://github.com/lformaggia/muparser.git
git config --file=.gitmodules submodule.Extras/muparserx.url https://github.com/lformaggia/muparserx.git
git submodule sync
git submodule update --recursive --remote


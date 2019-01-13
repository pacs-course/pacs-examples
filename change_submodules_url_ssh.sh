git config --file=.gitmodules submodule.Examples/src/LinearAlgebra/CppNumericalSolvers.url git@github.com:lformaggia/CppNumericalSolvers.git
git config --file=.gitmodules submodule.Examples/src/LinearAlgebra/spectra.url git@github.com:lformaggia/spectra.git
git config --file=.gitmodules submodule.Examples/src/LinearAlgebra/redsvd-h.url git@github.com:lformaggia/redsvd-h.git
git config --file=.gitmodules submodule.Extras/muparser.url git@github.com:lformaggia/muparser.git
git config --file=.gitmodules submodule.Extras/muparserx.url git@github.com:lformaggia/muparserx.git
git submodule sync
git submodule update --recursive --remote


git config --file=.gitmodules submodule.Examples/src/LinearAlgebra/CppNumericalSolvers.url git@github.com:pacs-course/CppNumericalSolvers.git
git config --file=.gitmodules submodule.Examples/src/LinearAlgebra/spectra.url git@github.com:pacs-course/spectra.git
git config --file=.gitmodules submodule.Examples/src/LinearAlgebra/redsvd-h.url git@github.com:pacs-course/redsvd-h.git
git config --file=.gitmodules submodule.Extras/muparser.url git@github.com:pacs-course/muparser.git
git config --file=.gitmodules submodule.Extras/muparserx.url git@github.com:pacs-course/muparserx.git
git config --file=.gitmodules submodule.Extras/json.url git@github.com:pacs-course/json.git
git submodule sync
git submodule update --recursive --remote


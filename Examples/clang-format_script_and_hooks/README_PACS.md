Copy of  `lformaggia/clang-format-hooks.git` on github, a fork of  barisione/clang-format-hooks. It contains the original codes but also `PACS_clang-format`, that you hve to copy into `.clang-format` in the main directory of the example if you want it to have clang-config to use the proper configuration file:

``` bash
cp PACS_clang-format ../.clang-format
```

Then you can use the `apply-format` script or install the git hook following the instructions on the `README.md` file.

If you wan to use instead the `git clang-format` utility using the style file you must copy and rename `PACS_clang-format` as explained above and do in your repo:

``` bash
git config clangFormat.style file
```
This way git knows that has to use clang-format with the option `style=file` with which `clang-format` uses the first `.clang-format`  located in one of the parent directories of the source file (that's why you put it in the main directory of your project.).

In this folder you find also the script `apply_clang-format` (not to
be confused with `apply-format`, which is part of the tools provided
in the original sofware, see `README.md`). This helps to call
clang-format with the right options. Beware, it will not create any
backup file: the modifications are made in place.

You have to put it in a diractory in your `PATH`.



# Note: you must have clang, or at least clang-format, properly installed in your system! 

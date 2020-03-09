# Tests for testing the tools for iterative linear system solvers#

To run this tests you have first to ensure that the local Makefile.inc
contains the indication of the directory where umfpack.h is contained
and where libumfpack.so or libumfpack.a is contained.

To have reasonable results about timings you should compile WIT NO
DEBUGGING ACTIVATED, by typing

make DEBUG=no

Of course take out DEBUG=no if you want debugging

*NOTE* I have changed the default so `DEBUG=no` is always activated
unless you do `make DEBUG=yes`.

Then you can edit `data.pot` (or copy to another file and modify the
copy) to select solver parameters and the solver type (direct and
iterative)

Use

    ./test_all <-i datafile> 
    
to launch the program if -i datafile is missing data.pot is taken.

*Note* The Makefile compiles also other tests, which are the modification of the originals contained in the `IML++` library. To run thoese tests you should do

    ./testName MatrixFileName
    
You have already a selection of matrices in the `MatrixData` folder.

*A final remark* In the Getpot file do not leave a space between the name of the variable and the `=`. GetoPot otherwise may get confused (its a bug...).

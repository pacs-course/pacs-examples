# An example of binary vs formatted i/o. #

We write and read a big file using formatted and binary i/o. The
binary i/o is eather native or using the HDF5 library

You need hdf5 installed! If you use the modules do

    module load hdf5

If you have you own installation, you have to change `Makefile.inc` to
indicate the directory where the `hdf5` header files and libraries are
stored.

If you have both serial and parallel version of `hdf5`, here use the
serial, since the parallel version uses `mpi`, and it is not relevant
for this example.

Not the difference in time spent for i/o!!! And the different size of
the produced files!


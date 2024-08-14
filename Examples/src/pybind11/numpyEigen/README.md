# Wrapping Eigen matrices with pybind11 and NumPy #
This example shows how to wrap Eigen matrices with pybind11 and NumPy. The example is adapted from the pybind11 documentation.

The Eigen library is perfectly compatible with numpy thanks to pybind11.

Some of the Eigen functions are overloaded. This example shows how to handle overloaded functions.

## Eigen and blas and lapack ##
Eigen is a header only library and is self contained. However, if you really want the best performances it is recommended to use the blas and lapack libraries. The Eigen library is compatible with blas and lapack, you have to compile with the following flags:
```bash
-I/path/to/eigen -DEIGEN_USE_BLAS -DEIGEN_USE_LAPACK
```
and then link with the blas and lapack libraries.

Sonce in thei example I want to compare the preformances of Eigen based function with the equilvalent numpy function, I will use the blas and lapack libraries since numpy uses them as well. This way, I have a fair comparison (otherwise the Eigen version is inevitably slower).

This is done in the Makefile by setting the `EIGEN_USE_BLAS` and `EIGEN_USE_LAPACK` flags and the corresponding linker options (assuming blas and lapacke are installed on the system).
```makefile
CPPFLAGS+=-DEIGEN_USE_BLAS -DEIGEN_USE_LAPACKE
LDLIBS+=-lblas -llapacke
```
You can comment them if you want just vanilla Eigen. You must have blas and lapacke (a porting of lapack for eigen) in your system. On a Debian based system you can do
```bash
sudo apt-get install libblas-dev liblapacke-dev
```
## Comments ##
With the addition of the blas + lapack interface the performance of Eigen and numpy is comparable. This is not surprising since numpy uses blas and lapack as well. Without blas and lapack Eigen is slower than numpy. This becouse the internal Eigen implementation of the blas and lapack utilitities are less sophisticated.
With smaller matrices it seems that the solvers based on HausholderQR and LU with partial pivoting provided by the Eigen are faster than that of numpy. For larger matrices the performance is comparable (at least on my machine)


#   What do I learn here ? #
- How to wrap Eigen matrices with pybind11
- How to use Eigen matrices in python
- How to handle overloaded functions

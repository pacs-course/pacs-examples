#Iterative solvers for Linear Systems of Equations#

Here we have the routines of the `IML++` Library
(`https://math.nist.gov/iml++/`) rewitten to be used with the Eigen
library. You nedd to install the Utilities:

* go into `../Utilities` and type

    make install

In testings you find several tests. In `../MatrixData` you have several matrices to test.

To install for the use in other examples do

    make install

Solvers implemented (they are all preconditioned so you need to give a
preconditioner!):

* `bicg.hpp` Biconjugate gradient
* `bicgstab.hpp` Biconjugate gradient with stabilization
* `cg.hpp` The good old Conjugate Gradient (only sdp matrices!)
* `cgs.hpp` Conjugate gradient squared (for unsymmetric systems)
* `cheby.hpp` Chebishev Iterations. It works well for sdp matrices provided you 
have good lower and upper bounds of the eigenvalues.
* `fgmres.hpp` Flexible GMRES. To be used if the preconditioner is itself an iterative solver
* `gmres.hpp` Generalised minimum residual (with restart). A good choice for general, non sdp, matrices. This version implements left preconditioning.
* `gmresr.hpp` My modification of GMRES to accept a right preconditioner.
* `ir.hpp` Preconditioner Richardon Iteration. The simplest iteration procedure. Normally ineffective, unless you have a fantastic preconditioner.
* `minres.hpp` My implementation of Minimal Residual. Useful for non-simmetric, but positive definite, matrices.
* `qmr.hpp` Quasi Minimal Residual.
* `tminres.hpp` Another (more sophisticted) implementation of MINRES, taken from a code available on the web. Actually, the version in `minres.hpp` seems to work better!

They are just header files, since I use templates, all contained in the subfolder `include`

The file `LineraAlgebraTraits.hpp` contains some adapters. Most of the code in this folder are indeed modification of existing code where some constructs are not compatible with the Eigen library. For instance, the dot product for two vectors is implemented as a free function, while in the Eigen is a member function. Instead of changing the code
I wrote appropriate adapters.

All methods are described in the book (freely downloadable) indicate in the Bibliography.

- In the subfolder `SaddlePointSolver` you have a working example of a code that uses the library to solve a saddlepoint problem governed by a matrix of the type (I use Matlab syntax)
```
[A,B;B';-T]
```
where A, B and T are sparse Eigen matricies. We present also some specialised preconditioners used in the second article in the bibliography. It is a complex example where I use object factories to lead solver and preconditioners at run time. Indeed you may change the options by reading data from a file. A matrix to run the example is given. The matrix is in matrix marked format. I use some 



**BIBLIOGRAPHY (Freely downloadable)** 
```bibtex
    @misc{book,
    title={Templates for the Solution of Linear Systems: Building Blocks for Iterative Methods1},
    author={Various authors},
    year={1994},
    publisher={SIAM},
    howpublished={\url{http://www.netlib.org/templates/templates.pdf}},
}
```

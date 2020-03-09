#Iterative solvers for Linear Systems of Equations#

Here we have the routines of the `IML++` Library
(`https://math.nist.gov/iml++/`) converted to be used with the Eigen
library. You nedd to install the Utilities:

* go into `../Utilities` and type

    make install

In testings you find several tests. In `../MatrixData` you have several matrices to test.

To install for the use in other examples do

    make install

Solvers implemented (thay are all preconditioned so you need to give a
preconditioner!):

* `BICG` Biconjugate gradient
* `BICGSTAB` Biconjugate gradient with stabilization
* `CG` The good old Conjugate Gradient (only sdp matrices!)
* `CGS` Conjugate gradient squared (for unsymmetric systems)
* `CHEBY` Chebishev Iterations. It works well for sdp matrices provided you 
have good lower and upper bounds of the eigenvalues.
* `FGMRES` Flexible GMRES. To be used if the preconditioner is itself an iterative solver
* `GMRES` Generalised minimum residual (with restart). A good choice for general, non sdp, matrices. This version implements left preconditioning.
* `GMRESR` My modification of GMRES to accet a right preconditioner.
* `IR` Preconditioner Richerdon Iteration. The simplest iteration procedure,
unless you have a fantastic preconditioner.
* `MINRES` My implementation of Minimal Residual. Useful for non-simmetric, but positive definite, matrices.
* `QMR` Quasi Minimal Residual.

All methods are described in the book (freely downloadable) indicate in the Bibliography.

*TODO:* I need to put all utilities in the namespace LinearAlgebra and
make traits real traits. It takes some time so I am postponing this.


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

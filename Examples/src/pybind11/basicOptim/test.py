#!/usr/bin/env python
import basicOptim as opt
def fun(x) -> float:
    return (x-1.)**2 + 4*x**4 + 3*x**3 - 2*x**2 + 1

a, b, n = opt.bracketIntervalMinimum(fun,0.1,0.1,1000)
#help(opt.bracketIntervalMinimum)
print("bracketing interval ends", a, b,n)
print("minimum by golden search", opt.golden_search(f=fun,a=a,b=b,tol=1.e-6,maxIter=1000))
print("minimum by brent", opt.Brent_local_min(a,b,tol=1.e-6,f=fun))
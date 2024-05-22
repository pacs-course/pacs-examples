#!/usr/bin/env python
import basicOptim as opt
def fun(x) -> float:
    return (x-1.)**2 +2*x


a, b, n = opt.bracketIntervalMinimum(fun,0.1,0.1,100)
print("bracketing interval ends", a, b,n)
print("minimum by golden search", opt.golden_search(f=fun,a=a,b=b,tol=1.e-6,maxIter=100))
print("minimum by brent", opt.Brent_local_min(a,b,tol=1.e-6,f=fun))
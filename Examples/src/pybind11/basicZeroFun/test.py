#!/bin/python
import zeroFun as zero
def fun(x) -> float:
    return (x-1.)**3
def dfun(x) -> float:
    return 3*(x-1.)**2

a,b,n = zero.bracketInterval(fun,0.,0.01,100)
print("zero by regula falsi", zero.regulaFalsi(fun,a,b,1.e-6,1.e-8))
print("zero by bisectionld", zero.bisectionld(fun,a,b,1.e-6))
print("zero by secant", zero.secant(fun,a,b,1.e-6,1.e-8,100))
print("zero by Newton", zero.Newton(fun,dfun,0.,1.e-12,1.e-12,100))
print("A bracket of the zero", zero.bracketInterval(fun,0.,1.e-3,100))

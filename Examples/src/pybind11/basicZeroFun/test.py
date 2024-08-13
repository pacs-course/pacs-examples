import zeroFun as zero
def fun(x) -> float:
    return (x-1.)**3
def dfun(x) -> float:
    return 3*(x-1.)**2


print("zero by regula falsi", zero.regulaFalsi(fun,-0.3,2.,1.e-6,1.e-8))
print("zero by bisectionld", zero.bisectionld(fun,-0.3,2.,1.e-6))
print("zero by secant", zero.secant(fun,-0.3,2.,1.e-6,1.e-8,100))
print("zero by Newton", zero.Newton(fun,dfun,0.,1.e-8,1.e-12,100))
print("A bracket of the zero", zero.bracketInterval(fun,0.,1.e-3,100))

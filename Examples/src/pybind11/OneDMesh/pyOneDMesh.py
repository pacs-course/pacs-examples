#!/usr/bin/env python
import numpy as np
import matplotlib.pyplot as plt
import OneDMesh as odm
d=odm.Domain1D(0,10)
m1=odm.Mesh1D(d,100) # 100 elements uniformly distributed
def h(x):
    return 1.05+np.sin(x)
gen=odm.VariableSize(d,h,100)
m2=odm.Mesh1D(gen) # max 100 elements non-uniformly distributed
print(m2)
m1v=np.zeros([m1.numNodes(),1])
m2v=0.1*np.ones([m2.numNodes(),1])

plt.plot(m1.meshNodes(),m1v,'b-*',label='uniform')
plt.plot(m2.meshNodes(),m2v,'r-+',label='non-uniform')
plt.legend()
plt.show()
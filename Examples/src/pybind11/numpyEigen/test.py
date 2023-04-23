import numpy as np
import eigenwrapper as ew
A = np.array([[1,2,1],
              [2,1,0],
              [-1,1,2]])

print('A = \n'                   , A)
print('ew.det(A) = \n'      , ew.det(A))
print('numpy.linalg.det(A) = \n' , np.linalg.det(A))
print('ew.inv(A) = \n'      , ew.inv(A))
print('numpy.linalg.inv(A) = \n' , np.linalg.inv(A))
v = np.array([1,2,3])
print('v = \n'                   , v)
print('ew.solve(A,v) = \n'      , ew.solve(A,v))
print('numpy.linalg.solve(A,v) = \n' , np.linalg.solve(A,v))

B=10
print("Multiplication as a matrix of integers\n", ew.multiply(A.astype(int  ),int  (B)))
print("Multiplication as a matrix of doubles\n", ew.multiply(A.astype(float),float(B)))
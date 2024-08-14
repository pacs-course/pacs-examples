import numpy as np
import eigenwrapper as ew
# import tools for timings
import time
# create a big matrix
N = 1000
A = np.random.rand(N,N)
# convert to double
A = A.astype(float)
# contruct the vector of ones
v = np.ones(N,dtype=float)
# time the multiplication
start = time.time()
b=ew.multiply(A,v)
end = time.time()
print("Time for multiplication of a matrix by a vector of ones using Eigen", end-start)
# time multiplication using numpy
start=time.time()
b=np.dot(A,v)
end = time.time()
print("Time for multiplication of a matrix by a vector of ones using numpy", end-start)
# Now using a standard loop
start = time.time()
b=np.zeros(N)
for i in range(N):
    for j in range(N):
        b[i]+=A[i,j]
end = time.time()
print("Time for multiplication of a matrix by a vector of ones using a standard loop", end-start)
print('Determinant computed with Eigen ew.det(A) = '      , ew.det(A))
print('Determinat computed with numpy.linalg.det(A) = ' , np.linalg.det(A))
#time inversion with eigen
start = time.time()
z=ew.inv(A)
end = time.time()
print("Time for inversion of a matrix using Eigen", end-start)
#time inversion with numpy
start = time.time()
z=np.linalg.inv(A)
end = time.time()
print("Time for inversion of a matrix using numpy", end-start)
# solve the problea Ax=b with Eigen
start = time.time()
x=ew.solve(A,b);
end = time.time()
print("Time for solving a linear system using Eigen", end-start)
# print the infinity norm of the error
print("Error ",np.linalg.norm(v-x,np.inf))
# solve the problem Ax=b with numpy
start = time.time()
x=np.linalg.solve(A,b)
end = time.time()
print("Time for solving a linear system using numpy", end-start)
print("Error ",np.linalg.norm(v-x,np.inf))

#print('ew.solve(A,v) = \n'      , ew.solve(A,v))
#print('numpy.linalg.solve(A,v) = \n' , np.linalg.solve(A,v))

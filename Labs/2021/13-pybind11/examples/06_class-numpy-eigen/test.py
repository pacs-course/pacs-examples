import numpy as np
import example_06_class_numpy_eigen as example

A = np.arange(10)

print('A = \n',A)

array = example.CustomVectorXd(A)

print('print(array) = \n', array)

print('array.multiply(default) = \n'   ,array.multiply()          )
print('array.multiply(factor=100) = \n',array.multiply(factor=100))

print('trans(A) = \n',example.trans(A))

import PointInSimplex as pis 
import numpy as np
triangle = np.array([[0,0],[1,0],[0,1]])
print("give me a 2d point")
x = float(input("x: "))
y = float(input("y: "))
point = np.array([x,y])
inside = pis.pointInTriangle(point,triangle)
print("the point is in the triangle: ", inside[0], " and the barycentric coordinates are: ", inside[1])
tetrahedron = np.array([[0,0,0],[1,0,0],[0,1,0],[0,0,1]])
print("give me a 3d point")
x = float(input("x: "))
y = float(input("y: "))
z = float(input("z: "))
point = np.array([x,y,z])
inside = pis.pointInTetrahedron(point,tetrahedron)
print("the point is in the tetrahedron: ", inside[0], " and the barycentric coordinates are: ", inside[1])

triangle3d=np.array([[0,0,0],[1,0,0],[0,1,0]],np.float64)
print("give me a 3d point (to test if inside a triangle in 3D)")
x = float(input("x: "))
y = float(input("y: "))
z = float(input("z: "))
point3d = np.array([x,y,z],np.float64)
inside = pis.pointInTriangle3(p=point3d,t=triangle3d,eps=1.e-03)
print("the point is in the triangle: ", inside[0], " and the barycentric coordinates are: ", inside[1])




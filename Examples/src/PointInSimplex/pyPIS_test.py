import PointInSimplex as pis 
import numpy as np

# Build simplices through the bound constructors.
triangle = pis.make_simplex(np.array([0.0, 0.0], np.float64),
                            np.array([1.0, 0.0], np.float64),
                            np.array([0.0, 1.0], np.float64))
print("give me a 2d point")
x = float(input("x: "))
y = float(input("y: "))
point = np.array([x, y], np.float64)
inside = pis.pointInTriangle(point,triangle)
print("the point is in the triangle: ", inside[0], " and the barycentric coordinates are: ", inside[1])
tetrahedron = pis.make_simplex(np.array([0.0, 0.0, 0.0], np.float64),
                               np.array([1.0, 0.0, 0.0], np.float64),
                               np.array([0.0, 1.0, 0.0], np.float64),
                               np.array([0.0, 0.0, 1.0], np.float64))
print("give me a 3d point")
x = float(input("x: "))
y = float(input("y: "))
z = float(input("z: "))
point = np.array([x, y, z], np.float64)
inside = pis.pointInTetrahedron(point,tetrahedron)
print("the point is in the tetrahedron: ", inside[0], " and the barycentric coordinates are: ", inside[1])

# Keep one backward-compatible alias usage in the test.
triangle3d = pis.make_simplex_triangle3D(np.array([0.0, 0.0, 0.0], np.float64),
                                         np.array([1.0, 0.0, 0.0], np.float64),
                                         np.array([0.0, 1.0, 0.0], np.float64))
print("give me a 3d point (to test if inside a triangle in 3D)")
x = float(input("x: "))
y = float(input("y: "))
z = float(input("z: "))
point3d = np.array([x, y, z], np.float64)
# retrieve the tuple returned by pis.pointInTriangle3
inside, inplane, distance, barycentric = pis.pointInTriangle3(p=point3d,t=triangle3d,eps=1.e-03)
print("Point is inside:", inside)
print("Point is in triangle plane: ", inplane)
print("Distance to triangle plane: ", distance)
print("Barycentric coordinates: ", barycentric)

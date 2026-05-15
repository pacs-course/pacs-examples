import numpy as np
import pacs_py

print("add:", pacs_py.add(2, 3))
print("scale:", pacs_py.scale([1.0, 2.0, 3.0], 10.0))
print("moments:", pacs_py.moments([1.0, 2.0, 3.0]))

x = np.array([1.0, 2.0, 3.0])
A = np.eye(3)
y = np.ones(3)
print("rank_one:\n", pacs_py.rank_one(x))
print("energy:", pacs_py.energy(x, A))
pacs_py.axpy(2.0, x, y)
print("axpy y:", y)

p = pacs_py.Polynomial([1.0, 2.0, 3.0])
print("polynomial:", p, "p(10)=", p(10.0), "degree=", p.degree())

q = pacs_py.Point(1.0, 2.0) + pacs_py.Point(3.0, 4.0)
print("point:", q, "norm=", q.norm)

s = pacs_py.State(3)
u = s.values()
u[:] = [1.0, 2.0, 3.0]
print("state values:", s.values())

print("square_double:", pacs_py.square_double(0.5))
f = pacs_py.AffineD(2.0, 1.0)
print("affine:", f(3.0))

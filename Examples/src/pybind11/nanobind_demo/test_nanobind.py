import numpy as np
import pacs_nb

print("add:", pacs_nb.add(2, 3))
print("scale:", pacs_nb.scale([1.0, 2.0, 3.0], 10.0))
print("moments:", pacs_nb.moments([1.0, 2.0, 3.0]))

x = np.array([1.0, 2.0, 3.0])
A = np.eye(3)
y = np.ones(3)
print("rank_one:\n", pacs_nb.rank_one(x))
print("energy:", pacs_nb.energy(x, A))
pacs_nb.axpy(2.0, x, y)
print("axpy y:", y)
print("sum_array:", pacs_nb.sum_array(x))

p = pacs_nb.Polynomial([1.0, 2.0, 3.0])
print("polynomial:", p, "p(10)=", p(10.0), "degree=", p.degree())

q = pacs_nb.Point(1.0, 2.0) + pacs_nb.Point(3.0, 4.0)
print("point:", q, "norm=", q.norm)

s = pacs_nb.State(3)
u = s.values()
u[:] = [1.0, 2.0, 3.0]
print("state values:", s.values())

print("square_double:", pacs_nb.square_double(0.5))
f = pacs_nb.AffineD(2.0, 1.0)
print("affine:", f(3.0))

#An example of builder design pattern.#

Two different builders for graphs have been set depending whether we
want directed or undirected graphs.

In the case of undirected graphs the edges are repeated with ends
exchanged, but with the same edge number.

The builder pattern is indeed a way of specifying policise via
polymorphism, instead of using templates. The advantage compared to
templates is that the selection of the policy can be made tun-time.

The disadvantage is the slightly loss of efficiency and a more rigid
design.


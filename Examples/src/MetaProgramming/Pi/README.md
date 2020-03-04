#computing pi as a partial sum of a series#

There are may series that produce pi. Here I am using one of them to approximate pi. Template recursion allow me to compute sum the various terms of the series, stopping at a given term, and let the compiler do the job (You may need optimization on: compile with `make DEBUG=yes` and see that `nm --demangle` does not conteain `computePi`.

Note that I have used template recursion also to define my own pow for integer exponents.
I have also put in a namespace some constexpr that contain the value of pi at different level of precision.

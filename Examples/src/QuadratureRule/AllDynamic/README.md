#Dynamic loading of numerical rules and integrands#

In this version I use an Object Factory to load als the quadrature
rule dynamically. We use a file in GetPot syntaz to define parameters,
integrand and quadrature rule.

To make it work:

*go in `../baseversion`, follow the instructions in the README file, and do

    make alllibs (maybe with DEBUG=no)
    make install

*go in `../../Utilities` and do

    make install


Data read from `quadratura.getpot` (a getpot file)

To compile the libraries:

    make dynamic [DEBUG=no]

to compile the executable

    make exec [DEBUG=no]



# A little finite element code #

An example of a program that computes the temperature in a bar solving
the 1D heat equation with either a direct solver (Thomas algorithm for tridiagonal systems) or Gauss Siedel iterations,
depending on a switch set in the parameter file, by default called `parameter.pot`. 

Taken and heavily adapted from *"Simulation numerique an C++" of I. Danaila,
   F. Hecht e O. Pironneau, 2005*
   
**This example illustrates**
 - The use of an *aggregate* to store the main parameter of the code, with default values;
 - The use of `GetPot` to pass options in the command line;
 - The use of `GetPot` to read the parameters from a file, so you can change them with no need of recompiling;
 - A use of *structured bindings*;
 - The Thomas algorithm for the solution of tridiagonal systems of equations;
 - The use of `gnuplot-iostream` to visualize results directly from the program;
 - The use of `gnuplot`.
 
Calling ``./plot.sh`` you are able to visualize the results, which are stored in result.dat.  

Or you can launch gnuplot and then at the gnuplot prompt type

``
gnuplot> plot "result.dat" u 1:2 w lp lw 2 title "uh", "result.dat" u 1:3 w l lw 2title "uex"
``

``make heat_exchange.pdf`` produces a file with the description of the example.

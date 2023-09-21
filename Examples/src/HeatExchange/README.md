# A little finite element code #

An example of a program that computes the temperature in a bar solving
the 1D heat equation with either a direct solver (Thomas algorithm for tridiagonal systems) or Gauss Siedel iterations,
depending on a switch set in the parameter file, by default called `parameter.pot`. 

Taken and heavily adapted from *"Simulation numerique an C++" of I. Danaila,
   F. Hecht e O. Pironneau, 2005* 

**Note** This program makes use of the Thomas algorithm for the solution of a tridiagonal system of linear equations. To have it at disposal for the example you must first
- go into the folder `Utility`
- do `make install`

Otherwise the example will not compile.

**ANOTHER important note**


Then, as usual, in this directory `make` (`make DEBUG=no` if you want optimization) produces the executable, which is just called `main`.
 
Calling ``./plot.sh`` you are able to visualize the result stored in `result.dat` offline.  

Or you can launch `gnuplot` from the terminal and then at the gnuplot prompt type

```
gnuplot> plot "result.dat" u 1:2 w lp lw 2 title "uh", "result.dat" u 1:3 w l lw 2title "uex"
```


``make heat_exchange.pdf`` produces a file with the description of the example.


## Note ##
The code uses `gnuplot-iostream` to plot the result on the screen directly. However, if you use a virtual machine you may have problems if the graphic card is not well supported. Moreover `gnuplot-iostream` requires the `boost` libraries, which should be installed on your system. If you waant to disable it just delete the **local** `Makefile.inc` file. If you do not use `gnuplot-iostream` you can still plot the results with gnuplot as a post processing, using the script `plot.sh`:
	
	bash ./plot.sh

of simply `,/plot.sh` since the file should be executable.

`gnoplot-iostream` uses the boost libraries. So if you want the example to be working you must have the boost library (development
version) installed. Not all of them infact, in Makefile.inc you see the one that are required: `boos-iostream`, `boost-system` and `boost-filesystem`.

On a Debian based Linux distribution normally the command

	sudo apt-get install libboost-dev
should install all boost libraries. Look at the documentation of your distribution.

If you are using the modules you have all of them with

```
module load boost
```

### Deactivating gnuplot-iostream ###
If you do not want to use gnuplot-iostream, for instance becouse your virtual machine does not support graphics, you should modify the local  `Makefile.inc` file,
commenting out (or deleting) the lines

```
    CPPFLAGS+=-I$(mkBoostInc) -DGNUPLOT
    LDLIBS+=-L$(mkBoostLib) -lboost_iostreams -lboost_system -lboost_filesystem

```

# What do you learn from this Example? #
- A very simple, but complete, finite element code;
- The use of an *aggregate* (here a `struct` with only public attributes)  to store the main parameter of the code, with default values;
- The use of `GetPot` to pass options in the command line;
- The use of `GetPot` to read the parameters from a file, so you can change them with no need of recompiling;
- The use of a json file reader to do the same with a json file;
- A use of *structured bindings*;
- The Thomas algorithm for the solution of tridiagonal systems of equations;
- The use of `gnuplot-iostream` to visualize results directly from the program;
- The use of `gnuplot`.



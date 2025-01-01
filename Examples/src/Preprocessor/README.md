# The preprocessor #

These tests are just examples on the use of the preprocessor.

I recall that executing the preprocessor is the first compilation step, you normally don't see it. The file produced by the preprocessor is then digested by the proper compiler.

 
You should compile the files with the option -E and look at the output. For example

```
    g++ -E test2.cpp >output.txt
```
or 

```
    g++ -DPLUTO -E test2.cpp >output.txt
```

Here `PLUTO` is a cpp macro. Look at `output.txt` and you will see the post-processed line. 

# What do I get here? #
- That the prepocessor exists;
- That `#include` does exactly what it says.
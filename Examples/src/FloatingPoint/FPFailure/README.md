# Some degenerate case of floating point failures #
See the file `explanation.pdf` for a full description.

To compile `explanation.tex` type either `pdflatex explanation.tex` or
``make explanation.pdf`` (the magic of make!).

To compile the code, type just `make`.

# What do I learn here #
- It is quite a contrived example. However, it shows that floating point error may sometimes 
  - drive an iterative process to the wrong direction;
  - produce utterly wrong results when you are summing quantities of very different orders of magnitute.  So, if possible, try to scale the quantities in your problem so that their orders of magniture do not differ too much!

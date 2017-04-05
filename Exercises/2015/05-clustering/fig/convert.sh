#! /bin/bash

# convert svg images to pdf for latex.
# -C means that the whole area of the figure is keeped.
# it is used here since all the figure must be the same size.
# use -D to crop the white borders

inkscape -C --file=K_Means_Example_Step_1.svg --export-pdf=wiki1.pdf
inkscape -C --file=K_Means_Example_Step_2.svg --export-pdf=wiki2.pdf
inkscape -C --file=K_Means_Example_Step_3.svg --export-pdf=wiki3.pdf
inkscape -C --file=K_Means_Example_Step_4.svg --export-pdf=wiki4.pdf


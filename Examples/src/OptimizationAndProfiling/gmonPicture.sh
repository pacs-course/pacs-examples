# !/ bin / bash
# Produce a nice diagran form the gprof profiler
#
# You should have the gmon.out file produced for the program
# given in input
#
#  SYNOPSYS
# gmonPicure.sh programName
#
# Requires to have dot installed (from imageMagick)
# and gprof2dot (pip install gprof2dot)
# Use gprof2dot to convert gmon . out to a dot file
gprof $* gmon.out | gprof2dot -w -s > profile.dot
# Use dot to generate a PNG image from the dot file
dot -Tpng -o profiling.png profile.dot

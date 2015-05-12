#!/bin/bash
export LD_LIBRARY_PATH+=:../../../lib:
./main_integration $*

# run_quadrule a=xx b=yy nint=NN

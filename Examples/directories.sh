#!/bin/bash
tree src/ -T "Directory structure of the examples" -C -d -L 2 -H ./src $*  >directories.html

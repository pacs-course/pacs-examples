############################################################
# Makefile for 
# C++ and Object Oriented Numeric Computing
# for Scientists and Engineers.
#               Daoqi Yang  August 17, 2001 
############################################################
SHELL=bash
BASE = $(PWD)
CXX=g++
CC=g++
OPTFLAGS=-std=c++11 -g -Wall 
DIRS= chapter1 chapter2 chapter3 chapter5 chapter5 \
chapter6 chapter7 chapter8 chapter9 chapter10 chapter11/gcc3.4
.PHONY=all clean

all:
	-(for f in $(DIRS); do pushd $$f; $(MAKE) CC='$(CC)' OPTFLAGS='$(OPTFLAGS)'; popd;done;)
clean:
	-(for f in $(DIRS); do pushd $$f; make clean; popd;done;)

 



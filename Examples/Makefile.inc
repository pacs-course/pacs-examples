# This is the user defined part of the Makefile
PACS_ROOT=/home/forma/workspace2/pacs/PACSCourse/Material/Examples
EIGEN_DIR=/usr/local/include/eigen3/


##
#  END USER MODIFIABLE PART
##
ifeq ($(PACS_ROOT),)
export ROOT=/usr/local
else
export ROOT=$(PACS_ROOT)
endif
#
# Libraries are stored here
#
export PACS_LIB_DIR=$(PACS_ROOT)/lib
#
# Main include directory
#
export PACS_INC_DIR=$(PACS_ROOT)/include
#
# Other directories with libraries (if needed)
#
export OTHER_LIB_DIR
#
# Other include directories (if needed)
#
export OTHER_INC_DIR
#
# The C++ compiler
#
CXX = g++ 
#CXX = icc
CXX = /opt/clang/bin/clang++
#
# Se CXX=icc for intel compiler
#
ifeq ($(findstring icc, $(CXX)),icc)
inteldir=$(dir $(CXX))
CC=CXX
OTHER_LIB_DIR+=/usr/local/opt/intel/cc/9.0/lib
endif
#
# Optimization flags
#
OPTFLAGS = -g -Wall
# -c       compile only, generate corresponding .o files
# -g       generate code suitable for the debugger
# -p       generate code suitable for the profiler
# -O       attemp code optimizations
#
# Flags for standard
#
STDFLAGS=-std=c++0x
#
#  DEFINES: Possible -DXX statements
# Example
# -DNDEBUG disactivates asserts
DEFINES=
#
#  SPECIAL SETTINGS FOR NON GCC COMPILERS
#
# Se CXX=icc for intel compiler
#
ifeq ($(findstring clang, $(CXX)),clang)
CC=CXX
OTHER_LIB_DIR+=/opt/clang/lib
STDFLAGS=-std=c++11
endif

#
# Name of additional libraries
#
LIBRARIES=
#
# MAke uses CC also for the loader
# 
CC =$(CXX)

export CC
export CXX
# 
# Location of
# Doxygen configuration file
#
export DOXYFILE=$(PACS_ROOT)/DoxyfileCommon
#
# Compose options
#
# collect all includes
ALL_INC_DIR= $(PACS_INC_DIR) $(OTHER_INC_DIR) $(EIGEN_DIR)
includes := $(foreach inc,$(ALL_INC_DIR),-I$(inc) )
# where to look for include files:
# first the current directory, then the directory ./include
# and then the possible other specifyed in OTHER_INC_DIR
INCLS  = -I. -I./include $(includes) 
# collect all library dirs
ALL_LIB_DIR=$(PACS_LIB_DIR) $(OTHER_LIB_DIR)
LIBD := $(foreach lib,$(ALL_LIB_DIR),-L$(lib) )

#
# Set make macros
#
export CPPFLAGS:=$(INCLS) $(DEFINES)
export CXXFLAGS:=$(OPTFLAGS) $(STDFLAGS)
#
# For C Programs 
#
export CCFLAGS:=$(CXXFLAGS)
#
# name of file with dependencies
#
DEPEND=make.dep
#header files
export LDFLAGS += $(OPTFLAGS) $(LIBD) $(LIBRARIES)


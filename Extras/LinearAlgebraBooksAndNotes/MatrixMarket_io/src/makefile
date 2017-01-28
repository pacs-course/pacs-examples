PREFIX=/home/forma/Work/ScientificComputing/CourseMirror/PACS
PACKAGE_NAME=MM
CC = gcc
CFLAGS = -O0 -pedantic -Wall -Wunused -Wformat -Wimplicit
IOLIB = iohb.o mmio.o
INCLUDES=iohb.h mmio.h
LIBNAME=libMMio_base.a

LDLIBS=-L. -lMMio_base

FINAL_LIB_DIR=/usr/local/lib/MM
FINAL_INC_DIR=/usr/local/include/MM
FINAL_BIN_DIR=/usr/local/bin

EXE_FILES=hbrhs2mtx hbmat2mtx hb2mtxstrm read_MM write_MM read_HB

all:  library $(EXE_FILES)



install:
	install -d  $(PREFIX)/include/$(PACKAGE_NAME)
	install -d  $(PREFIX)/lib/$(PACKAGE_NAME)
	install -d  $(PREFIX)/bin
	install   $(INCLUDES) $(PREFIX)/include/$(PACKAGE_NAME)
	install  $(LIBNAME) $(PREFIX)/lib/$(PACKAGE_NAME)
	install  $(EXE_FILES) $(PREFIX)/bin

.c.o:
	$(CC) $(CFLAGS) -c $*.c

#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([EPOCH], [0.1.0], [dont.write@me.please])
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([src/epoch.c])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([sys/time.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_CHECK_FUNCS([gettimeofday])

AC_CONFIG_FILES([Makefile
                 src/Makefile])
AC_OUTPUT

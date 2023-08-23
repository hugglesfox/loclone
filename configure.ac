#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.71])
AC_INIT([loclone], [0.1.0], [hayley@foxes.systems])
AC_CONFIG_SRCDIR([loclone.c])
AM_INIT_AUTOMAKE([foreign])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h mntent.h sys/ioctl.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_RESTRICT
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_GETMNTENT
AC_CHECK_FUNCS([getmntent realpath strerror])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT

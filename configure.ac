#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([cadencii_sinsy_plugin], [0.0])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_AUX_DIR([config])
AM_INIT_AUTOMAKE(cadencii_sinsy_plugin, 0.0, nostdinc no-installinfo no-installman)

# Checks for programs.
AC_PROG_CXX
AC_PROG_LN_S
AC_PROG_CC
AC_PROG_RANLIB
AC_PROG_LIBTOOL

# Checks for libraries.
# FIXME: Replace `main' with a function in `-lHTSEngine':
AC_CHECK_LIB([HTSEngine], [main])

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T
AC_CONFIG_MACRO_DIR([./])

# Checks for library functions.

AC_CONFIG_FILES([Makefile
                 plugin/Makefile
                 src/lib/Makefile
                 src/dic/Makefile])
AC_CONFIG_SUBDIRS([plugin
                   src/lib
                   src/dic])
AC_OUTPUT

AC_PREREQ([2.69])
AC_INIT([qix_line], [1.0], [jedi.master@augmentedjs.com])
AM_INIT_AUTOMAKE([foreign -Wall -Werror subdir-objects])
AC_PROG_CC
AC_PROG_RANLIB

# Checks for libraries.
PKG_CHECK_MODULES([GTK], [gtk4])

AC_SUBST([GTK_CFLAGS])
AC_SUBST([GTK_LIBS])

# Add this if not already present
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile])
AC_OUTPUT

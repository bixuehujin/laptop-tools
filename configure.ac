dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(tools, 1.0)
dnl AC_CONFIG_HEADER([config.h])

AC_CANONICAL_SYSTEM
AM_INIT_AUTOMAKE()

AC_PROG_CC

PKG_CHECK_MODULES([libudev], [libudev])
AC_SUBST([libudev_CFLAGS])
AC_SUBST([libudev_LIBS])

PKG_CHECK_MODULES([glib], [glib-2.0])
AC_SUBST([glib_CFLAGS])
AC_SUBST([glib_LIBS])

AC_CONFIG_FILES(Makefile src/Makefile test/Makefile scripts/Makefile)
AC_OUTPUT


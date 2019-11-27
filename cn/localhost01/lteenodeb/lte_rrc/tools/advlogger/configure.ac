#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT([advlogger], [2.9.1])

AC_CONFIG_SRCDIR([examples/console_log.c])
AC_CONFIG_HEADER([config.h])

AM_INIT_AUTOMAKE

# Checks for programs.
AC_PROG_CC
AC_PROG_LIBTOOL

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([syslog.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_STRUCT_TM

# Checks for library functions.
AC_FUNC_VPRINTF
AC_CHECK_FUNCS([atexit localtime_r])


# ############### ############### ############### ############### ##############
# Options

# Debugging support
CFLAGS="-W -Wall"
AC_ARG_ENABLE([debug], [AS_HELP_STRING([--enable-debug], [enable debugging support [[no]]])], [], [enable_debug=xno])
if test "x$enable_debug" == xyes; then
	CFLAGS="-g3 $CFLAGS"
	AC_DEFINE(_Ev3G_DEBUG, 1, [Debugging support enabled])
else
	CFLAGS="-O3 $CFLAGS"
fi

AC_ARG_ENABLE([date], [AS_HELP_STRING([--enable-date], [output date in logs [[yes]]])], [], [enable_advlogger=xno])
if test "x$enable_date" != xno; then
	AC_DEFINE(_LOG_DATE, 1, [Date output enabled])
fi

AC_ARG_ENABLE([microseconds], [AS_HELP_STRING([--enable-microseconds], [output microseconds in logs [[yes]]])], [], [enable_advlogger=xno])
if test "x$enable_microseconds" == xyes; then
	AC_DEFINE(_LOG_MICROSECONDS, 1, [Microseconds output enabled])
fi


AC_CONFIG_FILES([
Makefile
src/Makefile
examples/Makefile
netlogexd/Makefile
])

AC_OUTPUT

echo ""
echo "Configuration options: "

echo -n " -- build configuration: "
if test "x$enable_debug" == xyes; then
	echo "debug"
else
	echo "release"
fi

echo -n " -- log date: "
if test "x$enable_date" != xno; then
	echo "enabled"
else
	echo "disabled"
fi

echo -n " -- log microseconds: "
if test "x$enable_microseconds" == xyes; then
	echo "enabled"
else
	echo "disabled"
fi

echo ""
echo "Good - your configure finished. Start make now"
echo ""


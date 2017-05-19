#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT([brooksiyengar], [1.0])
AM_INIT_AUTOMAKE([-Wall foreign])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

# Checks for programs.

AC_PROG_CC([mpicc])
AC_PROG_CC_C99
if test -z "$CC" || test "$CC" = "no"; then
  AC_MSG_ERROR([No suitable C99 compiler found])
fi
AM_PROG_CC_C_O

# for library support
AC_PROG_RANLIB
m4_ifdef([AM_PROG_AR], [AM_PROG_AR])

# link with the math library if needed
AC_CHECK_LIB([m], [sin])

# Check for debugging 
AC_ARG_ENABLE(debug, 
  AS_HELP_STRING([--disable-debug], [Turn off debugging]), 
  [case "${enableval}" in 
     yes) debug=true ;; 
     no)  debug=false ;; 
     *) AC_MSG_ERROR(bad value ${enableval} for --enable-debug) ;; 
   esac], 
  [debug=true]) 
 
AS_IF([test "$debug" = "false"], [CPPFLAGS="${CPPFLAGS} -DNDEBUG"]) 

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile])
AC_OUTPUT

echo ""
echo "                       Configure Summary"
echo "                       ================="
echo ""
echo "Compiling Options:"
echo "------------------"
echo "                     C Compiler: $CC"
echo "                       CPPFLAGS: $CPPFLAGS"
echo "                         CFLAGS: $CFLAGS"
echo "                        LDFLAGS: $LDFLAGS"
echo "                           LIBS: $LIBS"

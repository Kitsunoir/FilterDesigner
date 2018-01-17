#                      -*- Autoconf -*-
#
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([FilterDesigner], [1.0.2], [simo@furrydolphin.net], [FilterDesigner])
AC_CONFIG_SRCDIR([src/appwin.cpp])

AC_MSG_NOTICE([FilterDesigner Version:])
AC_MSG_RESULT([$VERSION])

AM_INIT_AUTOMAKE

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

AC_ARG_ENABLE([optimum],
              [AS_HELP_STRING([--enable-optimum], [Enable optimum compile?])],
              [CXXFLAGS='-O3 -Wuninitialized -finline-functions -fomit-frame-pointer -fexpensive-optimizations'])

AC_ARG_ENABLE([instr],
              [AS_HELP_STRING([--enable-instr], [Enable debug instrumentation?])],
              [AC_DEFINE([INSTR],[1])])

# Checks for libraries.

AC_MSG_CHECKING([Looking for working FOX libs:])

SIMO_FOX_COMPILE

SIMO_FOX_RUN

AC_MSG_CHECKING([Looking for MPSolve:])

SIMO_MPSOLVE_FIND

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([fcntl.h string.h math.h stdio.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE

AC_CONFIG_FILES([Makefile
                 Documents/Makefile
		 Documents/Images/Makefile
                 Schemos/Makefile
		 src/Makefile
		 src/locations.h
                 etc/Makefile])
AC_OUTPUT

cat << EOF

>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Make Application <<<<<<<<<<<<<<<<<<<<<<<<<<<<<
>>                                                                        <<
>>  Configure complete :-)                                                <<
>>                                                                        <<
>>  Next, enter: "make" (no quotes) to compile the program.               <<
>>  Note: You may have to su to root to complete the install.             <<
>>  Then enter "make install" to install the program on your system.      <<
>>                                                                        <<
>>  If you haven't done so already, be sure to see INSTALL   for          <<
>>  some important information on the running of Filter Designer          <<
>>                                                                        <<
>>  Thanks for choosing Filter Designer                                   <<
>>  Enjoy your new program :-)                                            <<
>>                                                                        <<
>>            IF SOFTWARE CAN BE FREE, WHY CAN'T DOLPHINS?                <<
>>                                                                        <<
>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Make Application <<<<<<<<<<<<<<<<<<<<<<<<<<<<<

EOF

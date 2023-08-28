#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

#	This file is part of Sonar and is licensed under the terms contained in the COPYING and COPYING.LESSER files.
#
#	Copyright (C) 2023 Barcelona Supercomputing Center (BSC)

AC_PREREQ([2.69])
AC_INIT([sonar], [0.1], [pm-tools@bsc.es])

AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([src])
AC_CONFIG_HEADERS([config.h])

AC_LANG([C++])

# Prepare compilation flags before AC_PROG_CXX
AX_COMPILE_FLAGS

# Look for a valid compiler
AC_PROG_CXX

# Retrieve compiler version
AX_CHECK_CXX_VERSION

# AM_PROG_AR must be called before LT_INIT or a warning ensues
AM_PROG_AR

# Automake initialization
AM_INIT_AUTOMAKE([foreign -Wall dist-bzip2 -Wno-portability subdir-objects silent-rules])
AM_SILENT_RULES([yes])

# Checks for header files.
AC_CHECK_HEADERS([unistd.h])

# Find and check the MPI library
LX_FIND_MPI
AX_CHECK_MPI

# Check ovni instrumentation library
AC_CHECK_OVNI

# Use C++17
AX_CXX_COMPILE_STDCXX_17([noext], [mandatory])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_SIZE_T

LT_INIT([shared static pic-only])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT

echo ""
echo ""
echo "Configuration summary:"
echo "    Compiler version... ${CXX_VERSION}"
echo ""
echo "    Installation prefix... ${prefix}"
echo ""
echo "    CXXFLAGS... ${sonar_CXXFLAGS} ${asan_CXXFLAGS} ${CXXFLAGS}"
echo "    CPPFLAGS... ${sonar_CPPFLAGS} ${asan_CPPFLAGS} ${CPPFLAGS}"
echo "    LDFLAGS... ${asan_LDFLAGS} ${LDFLAGS}"
echo ""
echo "    MPI CXXFLAGS... ${MPI_CXXFLAGS}"
echo "    MPI LDFLAGS... ${MPI_CXXLDFLAGS}"
echo ""
echo "    Ovni CPPFLAGS... ${ovni_CPPFLAGS}"
echo "    Ovni LIBS... ${ovni_LIBS}"
echo ""

#	This file is part of Sonar and is licensed under the terms contained in the COPYING file.
#
#	Copyright (C) 2023 Barcelona Supercomputing Center (BSC)

ACLOCAL_AMFLAGS=-I m4

AM_CPPFLAGS=\
 -I$(top_srcdir)/src \
 -I$(top_srcdir)/src/common \
 -include "config.h" \
 $(ovni_CPPFLAGS) \
 $(sonar_CPPFLAGS) \
 $(asan_CPPFLAGS)

AM_CXXFLAGS=$(MPI_CXXFLAGS) $(sonar_CXXFLAGS) $(asan_CXXFLAGS)

AM_LDFLAGS=$(ovni_LIBS) $(asan_LDFLAGS) -ldl $(MPI_CXXLDFLAGS)
LIBS=

include_HEADERS =
pkginclude_HEADERS =

c_api_sources = \
 src/c/Operations.cpp

fortran_api_sources =

common_sources = \
 src/common/Instrument.cpp \
 src/common/IOHandler.cpp

noinst_HEADERS = \
 src/common/Compat.hpp \
 src/common/Definitions.hpp \
 src/common/Envar.hpp \
 src/common/Instrument.hpp \
 src/common/IOHandler.hpp \
 src/common/Manager.hpp \
 src/common/Operation.hpp \
 src/common/StringSupport.hpp \
 src/common/Symbol.hpp \
 src/common/Utils.hpp

lib_LTLIBRARIES = libsonar-mpi.la libsonar-mpi-c.la libsonar-mpi-fortran.la

libsonar_mpi_la_CPPFLAGS = $(AM_CPPFLAGS)
libsonar_mpi_la_SOURCES = $(common_sources) $(c_api_sources) $(fortran_api_sources)

libsonar_mpi_c_la_CPPFLAGS = $(AM_CPPFLAGS)
libsonar_mpi_c_la_SOURCES = $(common_sources) $(c_api_sources)

libsonar_mpi_fortran_la_CPPFLAGS = $(AM_CPPFLAGS)
libsonar_mpi_fortran_la_SOURCES = $(common_sources) $(fortran_api_sources)

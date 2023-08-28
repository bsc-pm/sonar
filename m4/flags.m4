#	This file is part of Sonar and is licensed under the terms contained in the COPYING file.
#
#	Copyright (C) 2023 Barcelona Supercomputing Center (BSC)

AC_DEFUN([AX_CHECK_CXX_VERSION], [
	AC_MSG_CHECKING([the ${CXX} version])
	if test x"$CXX" != x"" ; then
		CXX_VERSION=$(${CXX} --version | head -1)
	fi
	AC_MSG_RESULT([$CXX_VERSION])
	AC_SUBST([CXX_VERSION])
])

AC_DEFUN([AX_COMPILE_FLAGS], [
	# Disable debug and address sanitizer by default
	ac_enable_debug=no
	ac_enable_asan=no

	AC_ARG_ENABLE([debug], [AS_HELP_STRING([--enable-debug],
		[Adds compiler debug flags and enables additional internal debugging mechanisms @<:@default=disabled@:>@])],
		[ ac_enable_debug=$enableval ])

	AC_ARG_ENABLE([asan], [AS_HELP_STRING([--enable-asan],
		[Enables address sanitizing @<:@default=disabled@:>@])],
		[ ac_enable_asan=$enableval ])

	if test x"${ac_enable_debug}" = x"yes" ; then
		# Use debug flags
		sonar_CPPFLAGS=""
		sonar_CXXFLAGS="-Wall -Wextra -Wshadow -fvisibility=hidden -O0 -g3"
	else
		# Use optimized flags
		sonar_CPPFLAGS="-DNDEBUG"
		sonar_CXXFLAGS="-Wall -Wextra -Wshadow -fvisibility=hidden -O3"
	fi

	if test x"${ac_enable_asan}" = x"yes" ; then
		if test x"${ac_enable_debug}" = x"no" ; then
			AC_MSG_WARN([Enabling address sanitizer without enabling debug is not recommended])
		fi

		# Enable address sanitizer flags
		ac_asan_flags="-fsanitize=address -fno-omit-frame-pointer"
		asan_CPPFLAGS="${ac_asan_flags}"
		asan_CXXFLAGS="${ac_asan_flags}"
		asan_LDFLAGS="${ac_asan_flags}"
	else
		asan_CPPFLAGS=""
		asan_CXXFLAGS=""
		asan_LDFLAGS=""
	fi

	AC_SUBST(sonar_CPPFLAGS)
	AC_SUBST(sonar_CXXFLAGS)

	AC_SUBST(asan_CPPFLAGS)
	AC_SUBST(asan_CXXFLAGS)
	AC_SUBST(asan_LDFLAGS)

	# Disable autoconf default compilation flags
	: ${CPPFLAGS=""}
	: ${CFLAGS=""}
	: ${CXXFLAGS=""}
	: ${LDFLAGS=""}
])

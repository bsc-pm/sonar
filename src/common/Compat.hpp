/*
    This file is part of Sonar and is licensed under the terms contained in the COPYING file.

    Copyright (C) 2023 Barcelona Supercomputing Center (BSC)
*/

#ifndef COMPAT_HPP
#define COMPAT_HPP

#include <sys/syscall.h>
#include <unistd.h>

// Define gettid for older glibc versions (below 2.30)
#if !__GLIBC_PREREQ(2, 30)
static inline pid_t gettid(void)
{
    return (pid_t)syscall(SYS_gettid);
}
#endif

#endif // COMPAT_HPP

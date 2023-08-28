/*
    This file is part of Sonar and is licensed under the terms contained in the COPYING file.

    Copyright (C) 2023 Barcelona Supercomputing Center (BSC)
*/

#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <unistd.h>

#include "IOHandler.hpp"

namespace sonar {

class Utils {
public:
    static std::string getHostName()
    {
        char hostname[HOST_NAME_MAX+1];
        if (gethostname(hostname, HOST_NAME_MAX))
            IOHandler::fail("Could not retrieve the hostname");

        // Make sure the hostname is null-terminated
        hostname[HOST_NAME_MAX] = '\0';

        return hostname;
    }
};

} // namespace sonar

#endif // UTILS_HPP

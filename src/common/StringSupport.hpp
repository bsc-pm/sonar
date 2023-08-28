/*
    This file is part of Sonar and is licensed under the terms contained in the COPYING file.

    Copyright (C) 2023 Barcelona Supercomputing Center (BSC)
*/

#ifndef STRING_SUPPORT_HPP
#define STRING_SUPPORT_HPP

#include <algorithm>
#include <sstream>
#include <string>
#include <vector>

namespace sonar {

//! Class providing string parsing functionalities
class StringSupport {
public:
    //! \brief Parse a string to another type
    //!
    //! \param str The string to parse
    //! \param value The resulting value after parsing
    //!
    //! \returns Whether the parsing succeeded
    template <typename T>
    static bool parse(const std::string &str, T &value)
    {
        std::istringstream stream(str);
        T tmp;
        stream >> tmp;

        if (stream.fail())
            return false;

        value = tmp;
        return true;
    }
};

} // namespace sonar

#endif // STRING_SUPPORT_HPP

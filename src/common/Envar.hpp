/*
    This file is part of Sonar and is licensed under the terms contained in the COPYING file.

    Copyright (C) 2023 Barcelona Supercomputing Center (BSC)
*/

#ifndef ENVAR_HPP
#define ENVAR_HPP

#include <cstdlib>
#include <string>

#include "IOHandler.hpp"
#include "StringSupport.hpp"

namespace sonar {

//! Class that represents an environment variable
template <typename T>
class Envar {
private:
    //! The environment variable value
    T _value;

    //! Indicate whether the environment variable is defined
    bool _isPresent;

    //! The environment variable name
    std::string _name;

public:
    //! \brief Construct an environment variable
    //!
    //! \param name The name of the environment variable
    //! \param defaultValue An optional value to assign if the environment variable has not been defined
    Envar(std::string const &name, T defaultValue = T()) :
        _value(defaultValue),
        _isPresent(false),
        _name(name)
    {
        char const *valueString = getenv(name.c_str());
        if (valueString != nullptr) {
            _isPresent = StringSupport::parse(valueString, _value);
            if (!_isPresent)
                IOHandler::warn("Invalid value for ", name, "; defaulting to ", defaultValue);
        }
    }

    //! \brief Indicate if the enviornment variable has actually been defined
    bool isPresent() const
    {
        return _isPresent;
    }

    //! \brief Retrieve the environment variable name
    const std::string &getName() const
    {
        return _name;
    }

    //! \brief Retrieve the current value
    T get() const
    {
        return _value;
    }

    //! \brief Retrieve the current value
    operator T() const
    {
        return _value;
    }
};

} // namespace sonar

#endif // ENVAR_HPP

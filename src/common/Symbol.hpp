/*
    This file is part of Sonar and is licensed under the terms contained in the COPYING file.

    Copyright (C) 2023 Barcelona Supercomputing Center (BSC)
*/

#ifndef SYMBOL_HPP
#define SYMBOL_HPP

#include <dlfcn.h>

#include "IOHandler.hpp"

namespace sonar {

//! Class that allows the dynamic loading of symbols at run-time
class Symbol {
public:
    //! \brief Load a symbol from the subsequent libraries
    //!
    //! \param name The name of the symbol to load
    template <typename Func>
    static Func *load(const char *name)
    {
        Func *symbol = (Func *) dlsym(RTLD_NEXT, name);
        if (symbol == nullptr)
            IOHandler::fail("Could not find symbol ", name);

        return symbol;
    }
};

} // namesapce sonar

#endif // SYMBOL_HPP

/*
    This file is part of Sonar and is licensed under the terms contained in the COPYING file.

    Copyright (C) 2023 Barcelona Supercomputing Center (BSC)
*/

#ifndef MANAGER_HPP
#define MANAGER_HPP

#include "Operation.hpp"
#include "Instrument.hpp"
#include "Symbol.hpp"

namespace sonar {

class Manager {
public:
    template <Operation::Lang Lang, Operation::Code Code, Operation::Count Count,
              typename ReturnTy, typename... Params>
    static ReturnTy process(const char *name, Params ...params)
    {
        typedef ReturnTy FuncTy(Params...);

        static FuncTy *symbol = Symbol::load<FuncTy>(name);

        // Instrument the operation at guard construction and destruction
        Instrument::Guard<Code> guard;

        // Execute the operation
        return (*symbol)(params...);
    }
};

} // namesapce sonar

#endif // MANAGER_HPP

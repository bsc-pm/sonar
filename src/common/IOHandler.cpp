/*
    This file is part of Sonar and is licensed under the terms contained in the COPYING file.

    Copyright (C) 2023 Barcelona Supercomputing Center (BSC)
*/

#include "IOHandler.hpp"

namespace sonar {

std::mutex IOHandler::_lock;

} // namespace sonar

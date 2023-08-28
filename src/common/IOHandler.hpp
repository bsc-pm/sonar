/*
    This file is part of Sonar and is licensed under the terms contained in the COPYING file.

    Copyright (C) 2023 Barcelona Supercomputing Center (BSC)
*/

#ifndef IO_HANDLER_HPP
#define IO_HANDLER_HPP

#include <cstdlib>
#include <iostream>
#include <mutex>
#include <sstream>
#include <string.h>
#include <unistd.h>

namespace sonar {

//! Class providing the functionality of error handling
class IOHandler {
private:
    static std::mutex _lock;

    template <typename T, typename... TS>
    static void emitReasonParts(std::ostringstream &oss, T const &firstReasonPart, TS... reasonParts)
    {
        oss << firstReasonPart;
        emitReasonParts(oss, reasonParts...);
    }

    static void emitReasonParts(std::ostringstream &)
    {
    }

    static void safeEmitPart(char *, size_t, char part)
    {
        write(2, &part, 1);
    }

    static void safeEmitPart(char *buffer, size_t size, int part)
    {
        int length = snprintf(buffer, size, "%i", part);
        write(2, buffer, length);
    }

    static void safeEmitPart(char *buffer, size_t size, long part)
    {
        int length = snprintf(buffer, size, "%li", part);
        write(2, buffer, length);
    }

    static void safeEmitPart(char *, size_t, char const *part)
    {
        write(2, part, strlen(part));
    }

    static void safeEmitPart(char *buffer, size_t size, float part)
    {
        int length = snprintf(buffer, size, "%f", part);
        write(2, buffer, length);
    }

    static void safeEmitPart(char *buffer, size_t size, double part)
    {
        int length = snprintf(buffer, size, "%f", part);
        write(2, buffer, length);
    }

    template <typename T, typename... TS>
    static void safeEmitReasonParts(char *buffer, size_t size, T const &firstReasonPart, TS... reasonParts)
    {
        safeEmitPart(buffer, size, firstReasonPart);
        safeEmitReasonParts(buffer, size, reasonParts...);
    }

    static void safeEmitReasonParts(char *, size_t)
    {
    }

public:
    //! \brief Print an error message and abort the execution
    //!
    //! \param reasonParts The reason of the failure
    template <typename... TS>
    static void fail(TS... reasonParts)
    {
        std::ostringstream oss;
        oss << "Error: ";
        emitReasonParts(oss, reasonParts...);
        oss << std::endl;

        {
            std::lock_guard<std::mutex> guard(_lock);
            std::cerr << oss.str();
        }

#ifndef NDEBUG
        abort();
#else
        exit(1);
#endif
    }

    //! \brief Print an error message and abort the execution if failed
    //!
    //! \param failure Whether the condition failed
    //! \param reasonParts The reason of the failure
    template <typename... TS>
    static void failIf(bool failure, TS... reasonParts)
    {
        if (__builtin_expect(!failure, 1)) {
            return;
        }

        fail(reasonParts...);
    }

    //! \brief Print a warning message
    //!
    //! \param reasonParts The reason of the warning
    template <typename... TS>
    static void warn(TS... reasonParts)
    {
        std::ostringstream oss;
        oss << "Warning: ";
        emitReasonParts(oss, reasonParts...);
        oss << std::endl;

        {
            std::lock_guard<std::mutex> guard(_lock);
            std::cerr << oss.str();
        }
    }

    //! \brief Print a warning message if failed
    //!
    //! \param failure Whether the condition failed
    //! \param reasonParts The reason of the warning
    template <typename... TS>
    static void warnIf(bool failure, TS... reasonParts)
    {
        if (__builtin_expect(!failure, 1)) {
            return;
        }

        warn(reasonParts...);
    }
};

} // namespace sonar

#endif // IO_HANDLER_HPP

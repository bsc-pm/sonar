/*
    This file is part of Sonar and is licensed under the terms contained in the COPYING file.

    Copyright (C) 2023 Barcelona Supercomputing Center (BSC)
*/

#ifndef INSTRUMENT_HPP
#define INSTRUMENT_HPP

#include <cstdint>
#include <ovni.h>
#include <unistd.h>
#include <unordered_set>

#include "Compat.hpp"
#include "Envar.hpp"
#include "IOHandler.hpp"
#include "Operation.hpp"
#include "Utils.hpp"

namespace sonar {

class Instrument {
private:
    //! The state is composed by the enter and exit ovni MCV, which are three
    //! characters that specify the event model, category and value. There is
    //! an additional boolean specifying whether the state is an alias of
    //! another state
    struct StateInfo {
        const char *_enterMCV;
        const char *_exitMCV;
        const bool _isAlias;
    };

    //! The table storing interface states information with the enter and
    //! exit event model-category-value. The model for MPI events is 'M'
    static constexpr StateInfo Interfaces[Operation::NumCodes] = {
        //! Initializing
        [Operation::Init]                = { "MUi", "MUI", false },
        [Operation::InitThread]          = { "MUt", "MUT", false },
        [Operation::Finalize]            = { "MUf", "MUF", false },
        //! Waiting requests
        [Operation::Wait]                = { "MW[", "MW]", false },
        [Operation::Waitall]             = { "MWa", "MWA", false },
        [Operation::Waitany]             = { "MWy", "MWY", false },
        [Operation::Waitsome]            = { "MWs", "MWS", false },
        //! Testing requests
        [Operation::Test]                = { "MT[", "MT]", false },
        [Operation::Testall]             = { "MTa", "MTA", false },
        [Operation::Testany]             = { "MTy", "MTY", false },
        [Operation::Testsome]            = { "MTs", "MTS", false },
        //! Blocking primitives
        [Operation::Recv]                = { "MR[", "MR]", false },
        [Operation::Send]                = { "MS[", "MS]", false },
        [Operation::Bsend]               = { "MSb", "MSB", false },
        [Operation::Rsend]               = { "MSr", "MSR", false },
        [Operation::Ssend]               = { "MSs", "MSS", false },
        [Operation::Sendrecv]            = { "MRs", "MRS", false },
        [Operation::SendrecvReplace]     = { "MRo", "MRO", false },
        //! Blocking collectives
        [Operation::Allgather]           = { "MAg", "MAG", false },
        [Operation::Allgatherv]          = { "MAg", "MAG", true  },
        [Operation::Allreduce]           = { "MAr", "MAR", false },
        [Operation::Alltoall]            = { "MAa", "MAA", false },
        [Operation::Alltoallv]           = { "MAa", "MAA", true  },
        [Operation::Alltoallw]           = { "MAa", "MAA", true  },
        [Operation::Barrier]             = { "MCb", "MCB", false },
        [Operation::Bcast]               = { "MDb", "MDB", false },
        [Operation::Gather]              = { "MDg", "MDG", false },
        [Operation::Gatherv]             = { "MDg", "MDG", true  },
        [Operation::Reduce]              = { "ME[", "ME]", false },
        [Operation::ReduceScatter]       = { "MEs", "MES", false },
        [Operation::ReduceScatterBlock]  = { "MEb", "MEB", false },
        [Operation::Scatter]             = { "MDs", "MDS", false },
        [Operation::Scatterv]            = { "MDs", "MDS", true  },
        [Operation::Scan]                = { "MCs", "MCS", false },
        [Operation::Exscan]              = { "MCe", "MCE", false },
        //! Non-blocking primitives
        [Operation::Irecv]               = { "Mr[", "Mr]", false },
        [Operation::Isend]               = { "Ms[", "Ms]", false },
        [Operation::Ibsend]              = { "Msb", "MsB", false },
        [Operation::Irsend]              = { "Msr", "MsR", false },
        [Operation::Issend]              = { "Mss", "MsS", false },
        [Operation::Isendrecv]           = { "Mrs", "MrS", false },
        [Operation::IsendrecvReplace]    = { "Mro", "MrO", false },
        //! Non-blocking collectives
        [Operation::Iallgather]          = { "Mag", "MaG", false },
        [Operation::Iallgatherv]         = { "Mag", "MaG", true  },
        [Operation::Iallreduce]          = { "Mar", "MaR", false },
        [Operation::Ialltoall]           = { "Maa", "MaA", false },
        [Operation::Ialltoallv]          = { "Maa", "MaA", true  },
        [Operation::Ialltoallw]          = { "Maa", "MaA", true  },
        [Operation::Ibarrier]            = { "Mcb", "McB", false },
        [Operation::Ibcast]              = { "Mdb", "MdB", false },
        [Operation::Igather]             = { "Mdg", "MdG", false },
        [Operation::Igatherv]            = { "Mdg", "MdG", true  },
        [Operation::Ireduce]             = { "Me[", "Me]", false },
        [Operation::IreduceScatter]      = { "Mes", "MeS", false },
        [Operation::IreduceScatterBlock] = { "Meb", "MeB", false },
        [Operation::Iscatter]            = { "Mds", "MdS", false },
        [Operation::Iscatterv]           = { "Mds", "MdS", true  },
        [Operation::Iscan]               = { "Mcs", "McS", false },
        [Operation::Iexscan]             = { "Mce", "McE", false },
    };

    //! Whether the ovni instrumentation is enabled
    static bool _ovniEnabled;

    //! Whether the process and thread should be finalized
    static bool _ovniFinalize;

    //! \brief Emit an ovni event given the event model-category-value
    static void emit(const char *mcv)
    {
        struct ovni_ev ev = {};
        ovni_ev_set_clock(&ev, ovni_clock_now());
        ovni_ev_set_mcv(&ev, mcv);
        ovni_ev_emit(&ev);
    }

    //! \brief Emit an ovni event with three payload values
    template <typename A, typename B, typename C>
    static void emit(const char *mcv, A a, B b, C c)
    {
        struct ovni_ev ev = {};
        ovni_ev_set_clock(&ev, ovni_clock_now());
        ovni_ev_set_mcv(&ev, mcv);
        ovni_payload_add(&ev, (uint8_t *) &a, sizeof(a));
        ovni_payload_add(&ev, (uint8_t *) &b, sizeof(b));
        ovni_payload_add(&ev, (uint8_t *) &c, sizeof(c));
        ovni_ev_emit(&ev);
    }

    //! \brief Initialize the ovni instrumentation
    static void ovniInitialize()
    {
        // When the ovni thread is not ready means that nobody initialized the
        // thread before. Usually this implies the process was not initialized.
        // If this is the case, initialize the ovni process, thread and add an
        // artificial cpu
        if (!ovni_thread_isready()) {
            // Use a different loom per process. Each process will report its
            // own artificial CPU
            std::string loom = Utils::getHostName() + "." +
                               std::to_string(getpid());

            // Initialize the process and thread
            ovni_proc_init(1, loom.data(), getpid());
            ovni_thread_init(gettid());

            // Report an artificial ovni CPU
            ovni_add_cpu(0, 0);

            // Emit the ovni thread executing event on any CPU
            emit<int32_t, int32_t, uint64_t>("OHx", -1, -1, 0);

            _ovniFinalize = true;
        }
    }

    //! \brief Set the ovni process information
    static void ovniSetProcessInformation(int rank, int nranks)
    {
        ovni_proc_set_rank(rank, nranks);
    }

    //! \brief Finalize the ovni instrumentation
    static void ovniFinalize()
    {
        if (_ovniFinalize) {
            // Emit the ovni thread end event and flush
            emit("OHe");
            ovni_flush();

            // Finalize the ovni process
            ovni_proc_fini();
        }
    }

    //! \brief Check that the table of states has no duplicates
    static void checkStateTableCorrectness()
    {
        std::unordered_set<std::string> existing;

        for (StateInfo state : Interfaces) {
            if (state._isAlias) {
                if (existing.find(state._enterMCV) == existing.end())
                    IOHandler::fail("ovni mcv ", state._enterMCV,
                                    " is alias but not present");
                if (existing.find(state._exitMCV) == existing.end())
                    IOHandler::fail("ovni mcv ", state._exitMCV,
                                    " is alias but not present");
            } else {
                auto res1 = existing.insert(state._enterMCV);
                if (!res1.second)
                    IOHandler::fail("ovni mcv ", state._enterMCV,
                                    " is repeated");

                auto res2 = existing.insert(state._exitMCV);
                if (!res2.second)
                    IOHandler::fail("ovni mcv ", state._exitMCV, " is repated");
            }
        }
    }

public:
    //! \brif Preinitialize the ovni instrumentation
    //!
    //! This function may initialize the ovni process and thread if they were
    //! not initialized yet
    static void preinitialize()
    {
        // Perform a safety check
        checkStateTableCorrectness();

        Envar<std::string> instrument("SONAR_MPI_INSTRUMENT", "none");
        if (instrument.get() == "ovni")
            _ovniEnabled = true;
        else if (instrument.get() == "none")
            _ovniEnabled = false;
        else
            IOHandler::fail("Invalid value ", instrument.get(), " for ",
                            instrument.getName());

        // Initialize ovni if enabled
        if (_ovniEnabled)
            ovniInitialize();
    }

    //! \brief Finish the initialization of the ovni instrumentation
    //!
    //! \param rank The rank of the process
    //! \param nranks The number of processes
    static void initialize(int rank, int nranks)
    {
        if (_ovniEnabled)
            ovniSetProcessInformation(rank, nranks);
    }

    //! \brief Finalize the ovni instrumentation
    //!
    //! This function may finalize the ovni process and thread if they were
    //! initialized by the function above
    static void finalize()
    {
        // Finalize ovni if enabled
        if (_ovniEnabled)
            ovniFinalize();
    }

    //! \brief Enter into a interface state at an operation
    template <Operation::Code Operation>
    static void enter()
    {
        if (_ovniEnabled)
            emit(Interfaces[Operation]._enterMCV);
    }

    //! \brief Exit from a interface state at an operation
    template <Operation::Code Operation>
    static void exit()
    {
        if (_ovniEnabled)
            emit(Interfaces[Operation]._exitMCV);
    }

    //! \brief Guard class to perform automatic scope instrumentation
    //!
    //! Guard objects instrument the enter and exit points of a specific
    //! operation at construction and destruction, respectively
    template <Operation::Code Operation>
    struct Guard {
        //! \brief Enter the instrumented operation at construction
        Guard()
        {
            Instrument::enter<Operation>();
        }

        //! \brief Exit the instrumented operation at destruction
        ~Guard()
        {
            Instrument::exit<Operation>();
        }
    };
};

} // namespace sonar

#endif // OVNI_INSTRUMENT_HPP

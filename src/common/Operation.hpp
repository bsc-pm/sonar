/*
    This file is part of Sonar and is licensed under the terms contained in the COPYING file.

    Copyright (C) 2023 Barcelona Supercomputing Center (BSC)
*/

#ifndef OPERATION_HPP
#define OPERATION_HPP

namespace sonar {

class Operation {
public:
    //! The operation code
    enum Code {
        Init = 0, InitThread, Finalize,
        //! Waiting requests
        Wait, Waitall, Waitany, Waitsome,
        //! Testing requests
        Test, Testall, Testany, Testsome,
        //! Blocking primitives
        Recv, Send, Bsend, Rsend, Ssend,
        Sendrecv, SendrecvReplace,
        //! Blocking collectives
        Allgather, Allgatherv, Allreduce,
        Alltoall, Alltoallv, Alltoallw,
        Barrier, Bcast, Gather, Gatherv,
        Reduce, ReduceScatter, ReduceScatterBlock,
        Scatter, Scatterv, Scan, Exscan,
        //! Non-blocking primitives
        Irecv, Isend, Ibsend, Irsend, Issend,
        Isendrecv, IsendrecvReplace,
        //! Non-blocking collectives
        Iallgather, Iallgatherv, Iallreduce,
        Ialltoall, Ialltoallv, Ialltoallw,
        Ibarrier, Ibcast, Igather, Igatherv,
        Ireduce, IreduceScatter, IreduceScatterBlock,
        Iscatter, Iscatterv, Iscan, Iexscan,
        //! Invalid value
        NumCodes,
    };

    //! The version of the operation regarding counts and displacement fields
    enum Count {
        //! The operation uses the original count types
        Regular = 0,
        //! The operation uses large count types
        Large,
        //! Invalid value
        NumVersions,
    };

    //! The programming language of the operation
    enum Lang {
        C = 0,
        Fortran,
    };
};

} // namespace sonar

#endif // OPERATION_HPP

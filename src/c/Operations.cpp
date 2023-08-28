/*
    This file is part of Sonar and is licensed under the terms contained in the COPYING file.

    Copyright (C) 2023 Barcelona Supercomputing Center (BSC)
*/

#include <dlfcn.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <type_traits>

#include "Definitions.hpp"
#include "Instrument.hpp"
#include "IOHandler.hpp"
#include "Manager.hpp"
#include "Operation.hpp"

//! The version MPI 3.0 changes the communication functions to leverage const
//! send buffers and for arrays of counts and displacements
#if MPI_VERSION >= 3
#define MPI3CONST const
#else
#define MPI3CONST
#endif

using recvbuf_t = void *;
using sendbuf_t = MPI3CONST void *;
using request_t = MPI_Request *;
using requests_t = MPI_Request *;
using status_t = MPI_Status *;
using statuses_t = MPI_Status *;
using data_t = MPI_Datatype;
using comm_t = MPI_Comm;
using flag_t = int *;
using tag_t = int;
using rank_t = int;
using op_t = MPI_Op;
using indices_t = int *;
using count_t = int;
using count_c_t = MPI_Count;

using datas_t = MPI3CONST data_t *;
using counts_t = MPI3CONST count_t *;
using counts_c_t = MPI3CONST count_c_t *;
using displs_t = MPI3CONST int *;
using displs_c_t = MPI3CONST MPI_Aint *;

using namespace sonar;

static inline std::pair<int, int> getInformation()
{
    int rank, nranks, err;
    err = MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (err != MPI_SUCCESS)
        IOHandler::fail("Could not retrieve the MPI rank");

    err = MPI_Comm_size(MPI_COMM_WORLD, &nranks);
    if (err != MPI_SUCCESS)
        IOHandler::fail("Could not retrieve the MPI rank size");

    return { rank, nranks };
}

#pragma GCC visibility push(default)

extern "C" {

//! Initializing and finalizing
int MPI_Init(int *argc, char ***argv)
{
    Instrument::preinitialize();

    int err = Manager::process<Operation::C, Operation::Init,
                               Operation::Regular, int>("MPI_Init", argc, argv);

    auto [rank, nranks] = getInformation();
    Instrument::initialize(rank, nranks);

    return err;
}

int MPI_Init_thread(int *argc, char ***argv, int required, int *provided)
{
    Instrument::preinitialize();

    int err = Manager::process<Operation::C, Operation::InitThread,
                               Operation::Regular, int>("MPI_Init_thread", argc,
                                                        argv, required,
                                                        provided);

    auto [rank, nranks] = getInformation();
    Instrument::initialize(rank, nranks);

    return err;
}

int MPI_Finalize(void)
{
    int err = Manager::process<Operation::C, Operation::Finalize,
                               Operation::Regular, int>("MPI_Finalize");

    Instrument::finalize();

    return err;
}

//! Waiting requests
DEFINE_FUNC2(
        Operation::C, Operation::Wait, Operation::Regular,
        int, MPI_Wait,
        request_t, status_t
);
DEFINE_FUNC3(
        Operation::C, Operation::Waitall, Operation::Regular,
        int, MPI_Waitall,
        count_t, requests_t, statuses_t
);
DEFINE_FUNC4(
        Operation::C, Operation::Waitany, Operation::Regular,
        int, MPI_Waitany,
        count_t, requests_t, indices_t, statuses_t
);
DEFINE_FUNC5(
        Operation::C, Operation::Waitsome, Operation::Regular,
        int, MPI_Waitsome,
        count_t, requests_t, flag_t, indices_t, statuses_t
);

//! Testing requests
DEFINE_FUNC3(
        Operation::C, Operation::Test, Operation::Regular,
        int, MPI_Test,
        request_t, flag_t, status_t
);
DEFINE_FUNC4(
        Operation::C, Operation::Testall, Operation::Regular,
        int, MPI_Testall,
        count_t, requests_t, flag_t, statuses_t
);
DEFINE_FUNC5(
        Operation::C, Operation::Testany, Operation::Regular,
        int, MPI_Testany,
        count_t, requests_t, indices_t, flag_t, statuses_t
);
DEFINE_FUNC5(
        Operation::C, Operation::Testsome, Operation::Regular,
        int, MPI_Testsome,
        count_t, requests_t, flag_t, indices_t, statuses_t
);

//! Blocking primitives
DEFINE_FUNC6(
        Operation::C, Operation::Bsend, Operation::Regular,
        int, MPI_Bsend,
        sendbuf_t, count_t, data_t, rank_t, tag_t, comm_t
);
DEFINE_FUNC7(
        Operation::C, Operation::Recv, Operation::Regular,
        int, MPI_Recv,
        recvbuf_t, count_t, data_t, rank_t, tag_t, comm_t, status_t
);
DEFINE_FUNC6(
        Operation::C, Operation::Rsend, Operation::Regular,
        int, MPI_Rsend,
        sendbuf_t, count_t, data_t, rank_t, tag_t, comm_t
);
DEFINE_FUNC6(
        Operation::C, Operation::Send, Operation::Regular,
        int, MPI_Send,
        sendbuf_t, count_t, data_t, rank_t, tag_t, comm_t
);
DEFINE_FUNC12(
        Operation::C, Operation::Sendrecv, Operation::Regular,
        int, MPI_Sendrecv,
        sendbuf_t, count_t, data_t, rank_t, tag_t, recvbuf_t, count_t, data_t,
        rank_t, tag_t, comm_t, status_t
);
DEFINE_FUNC9(
        Operation::C, Operation::SendrecvReplace, Operation::Regular,
        int, MPI_Sendrecv_replace,
        recvbuf_t, count_t, data_t, rank_t, tag_t, rank_t, tag_t, comm_t,
        status_t
);
DEFINE_FUNC6(
        Operation::C, Operation::Ssend, Operation::Regular,
        int, MPI_Ssend,
        sendbuf_t, count_t, data_t, rank_t, tag_t, comm_t
);

//! Blocking collectives
DEFINE_FUNC7(
        Operation::C, Operation::Allgather, Operation::Regular,
        int, MPI_Allgather,
        sendbuf_t, count_t, data_t, recvbuf_t, count_t, data_t, comm_t
);
DEFINE_FUNC8(
        Operation::C, Operation::Allgatherv, Operation::Regular,
        int, MPI_Allgatherv,
        sendbuf_t, count_t, data_t, recvbuf_t, counts_t, displs_t, data_t, comm_t
);
DEFINE_FUNC6(
        Operation::C, Operation::Allreduce, Operation::Regular,
        int, MPI_Allreduce,
        sendbuf_t, recvbuf_t, count_t, data_t, op_t, comm_t
);
DEFINE_FUNC7(
        Operation::C, Operation::Alltoall, Operation::Regular,
        int, MPI_Alltoall,
        sendbuf_t, count_t, data_t, recvbuf_t, count_t, data_t, comm_t
);
DEFINE_FUNC9(
        Operation::C, Operation::Alltoallv, Operation::Regular,
        int, MPI_Alltoallv,
        sendbuf_t, counts_t, displs_t, data_t, recvbuf_t, counts_t, displs_t,
        data_t, comm_t
);
DEFINE_FUNC9(
        Operation::C, Operation::Alltoallw, Operation::Regular,
        int, MPI_Alltoallw,
        sendbuf_t, counts_t, displs_t, datas_t, recvbuf_t, counts_t, displs_t,
        datas_t, comm_t
);
DEFINE_FUNC1(
        Operation::C, Operation::Barrier, Operation::Regular,
        int, MPI_Barrier,
        comm_t
);
DEFINE_FUNC5(
        Operation::C, Operation::Bcast, Operation::Regular,
        int, MPI_Bcast,
        recvbuf_t, count_t, data_t, rank_t, comm_t
);
DEFINE_FUNC8(
        Operation::C, Operation::Gather, Operation::Regular,
        int, MPI_Gather,
        sendbuf_t, count_t, data_t, recvbuf_t, count_t, data_t, rank_t, comm_t
);
DEFINE_FUNC9(
        Operation::C, Operation::Gatherv, Operation::Regular,
        int, MPI_Gatherv,
        sendbuf_t, count_t, data_t, recvbuf_t, counts_t, displs_t, data_t,
        rank_t, comm_t
);
DEFINE_FUNC7(
        Operation::C, Operation::Reduce, Operation::Regular,
        int, MPI_Reduce,
        sendbuf_t, recvbuf_t, count_t, data_t, op_t, rank_t, comm_t
);
DEFINE_FUNC6(
        Operation::C, Operation::ReduceScatter, Operation::Regular,
        int, MPI_Reduce_scatter,
        sendbuf_t, recvbuf_t, counts_t, data_t, op_t, comm_t
);
DEFINE_FUNC6(
        Operation::C, Operation::ReduceScatterBlock, Operation::Regular,
        int, MPI_Reduce_scatter_block,
        sendbuf_t, recvbuf_t, count_t, data_t, op_t, comm_t
);
DEFINE_FUNC8(
        Operation::C, Operation::Scatter, Operation::Regular,
        int, MPI_Scatter,
        sendbuf_t, count_t, data_t, recvbuf_t, count_t, data_t, rank_t, comm_t
);
DEFINE_FUNC9(
        Operation::C, Operation::Scatterv, Operation::Regular,
        int, MPI_Scatterv,
        sendbuf_t, counts_t, displs_t, data_t, recvbuf_t, count_t, data_t,
        rank_t, comm_t
);
DEFINE_FUNC6(
        Operation::C, Operation::Scan, Operation::Regular,
        int, MPI_Scan,
        sendbuf_t, recvbuf_t, count_t, data_t, op_t, comm_t
);
DEFINE_FUNC6(
        Operation::C, Operation::Exscan, Operation::Regular,
        int, MPI_Exscan,
        sendbuf_t, recvbuf_t, count_t, data_t, op_t, comm_t
);

//! Non-blocking primitives
DEFINE_FUNC7(
        Operation::C, Operation::Ibsend, Operation::Regular,
        int, MPI_Ibsend,
        sendbuf_t, count_t, data_t, rank_t, tag_t, comm_t, request_t
);
DEFINE_FUNC7(
        Operation::C, Operation::Irecv, Operation::Regular,
        int, MPI_Irecv,
        recvbuf_t, count_t, data_t, rank_t, tag_t, comm_t, request_t
);
DEFINE_FUNC7(
        Operation::C, Operation::Irsend, Operation::Regular,
        int, MPI_Irsend,
        sendbuf_t, count_t, data_t, rank_t, tag_t, comm_t, request_t
);
DEFINE_FUNC7(
        Operation::C, Operation::Isend, Operation::Regular,
        int, MPI_Isend,
        sendbuf_t, count_t, data_t, rank_t, tag_t, comm_t, request_t
);
DEFINE_FUNC7(
        Operation::C, Operation::Issend, Operation::Regular,
        int, MPI_Issend,
        sendbuf_t, count_t, data_t, rank_t, tag_t, comm_t, request_t
);

//! Non-blocking collectives
DEFINE_FUNC8(
        Operation::C, Operation::Iallgather, Operation::Regular,
        int, MPI_Iallgather,
        sendbuf_t, count_t, data_t, recvbuf_t, count_t, data_t, comm_t,
        request_t
);
DEFINE_FUNC9(
        Operation::C, Operation::Iallgatherv, Operation::Regular,
        int, MPI_Iallgatherv,
        sendbuf_t, count_t, data_t, recvbuf_t, counts_t, displs_t, data_t,
        comm_t, request_t
);
DEFINE_FUNC7(
        Operation::C, Operation::Iallreduce, Operation::Regular,
        int, MPI_Iallreduce,
        sendbuf_t, recvbuf_t, count_t, data_t, op_t, comm_t, request_t
);
DEFINE_FUNC8(
        Operation::C, Operation::Ialltoall, Operation::Regular,
        int, MPI_Ialltoall,
        sendbuf_t, count_t, data_t, recvbuf_t, count_t, data_t, comm_t,
        request_t
);
DEFINE_FUNC10(
        Operation::C, Operation::Ialltoallv, Operation::Regular,
        int, MPI_Ialltoallv,
        sendbuf_t, counts_t, displs_t, data_t, recvbuf_t, counts_t, displs_t,
        data_t, comm_t, request_t
);
DEFINE_FUNC10(
        Operation::C, Operation::Ialltoallw, Operation::Regular,
        int, MPI_Ialltoallw,
        sendbuf_t, counts_t, displs_t, datas_t, recvbuf_t, counts_t, displs_t,
        datas_t, comm_t, request_t
);
DEFINE_FUNC2(
        Operation::C, Operation::Ibarrier, Operation::Regular,
        int, MPI_Ibarrier,
        comm_t, request_t
);
DEFINE_FUNC6(
        Operation::C, Operation::Ibcast, Operation::Regular,
        int, MPI_Ibcast,
        recvbuf_t, count_t, data_t, rank_t, comm_t, request_t
);
DEFINE_FUNC9(
        Operation::C, Operation::Igather, Operation::Regular,
        int, MPI_Igather,
        sendbuf_t, count_t, data_t, recvbuf_t, count_t, data_t, rank_t, comm_t,
        request_t
);
DEFINE_FUNC10(
        Operation::C, Operation::Igatherv, Operation::Regular,
        int, MPI_Igatherv,
        sendbuf_t, count_t, data_t, recvbuf_t, counts_t, displs_t, data_t,
        rank_t, comm_t, request_t
);
DEFINE_FUNC8(
        Operation::C, Operation::Ireduce, Operation::Regular,
        int, MPI_Ireduce,
        sendbuf_t, recvbuf_t, count_t, data_t, op_t, rank_t, comm_t, request_t
);
DEFINE_FUNC7(
        Operation::C, Operation::IreduceScatter, Operation::Regular,
        int, MPI_Ireduce_scatter,
        sendbuf_t, recvbuf_t, counts_t, data_t, op_t, comm_t, request_t
);
DEFINE_FUNC7(
        Operation::C, Operation::IreduceScatterBlock, Operation::Regular,
        int, MPI_Ireduce_scatter_block,
        sendbuf_t, recvbuf_t, count_t, data_t, op_t, comm_t, request_t
);
DEFINE_FUNC9(
        Operation::C, Operation::Iscatter, Operation::Regular,
        int, MPI_Iscatter,
        sendbuf_t, count_t, data_t, recvbuf_t, count_t, data_t, rank_t, comm_t,
        request_t
);
DEFINE_FUNC10(
        Operation::C, Operation::Iscatterv, Operation::Regular,
        int, MPI_Iscatterv,
        sendbuf_t, counts_t, displs_t, data_t, recvbuf_t, count_t, data_t,
        rank_t, comm_t, request_t
);
DEFINE_FUNC7(
        Operation::C, Operation::Iscan, Operation::Regular,
        int, MPI_Iscan,
        sendbuf_t, recvbuf_t, count_t, data_t, op_t, comm_t, request_t
);
DEFINE_FUNC7(
        Operation::C, Operation::Iexscan, Operation::Regular,
        int, MPI_Iexscan,
        sendbuf_t, recvbuf_t, count_t, data_t, op_t, comm_t, request_t
);

//! The version MPI 4.0 introduces several new functions: large count flavors
//! for all primitives and collectives, and the non-blocking sendrecv primitives
#if MPI_VERSION >= 4

//! Non-blocking sendrecv primitives
DEFINE_FUNC12(
        Operation::C, Operation::Isendrecv, Operation::Regular,
        int, MPI_Isendrecv,
        sendbuf_t, count_t, data_t, rank_t, tag_t, recvbuf_t, count_t, data_t,
        rank_t, tag_t, comm_t, request_t
);
DEFINE_FUNC9(
        Operation::C, Operation::IsendrecvReplace, Operation::Regular,
        int, MPI_Isendrecv_replace,
        recvbuf_t, count_t, data_t, rank_t, tag_t, rank_t, tag_t, comm_t,
        request_t
);

//! Blocking large primitives
DEFINE_FUNC6(
        Operation::C, Operation::Bsend, Operation::Large,
        int, MPI_Bsend_c,
        sendbuf_t, count_c_t, data_t, rank_t, tag_t, comm_t
);
DEFINE_FUNC7(
        Operation::C, Operation::Recv, Operation::Large,
        int, MPI_Recv_c,
        recvbuf_t, count_c_t, data_t, rank_t, tag_t, comm_t, status_t
);
DEFINE_FUNC6(
        Operation::C, Operation::Rsend, Operation::Large,
        int, MPI_Rsend_c,
        sendbuf_t, count_c_t, data_t, rank_t, tag_t, comm_t
);
DEFINE_FUNC6(
        Operation::C, Operation::Send, Operation::Large,
        int, MPI_Send_c,
        sendbuf_t, count_c_t, data_t, rank_t, tag_t, comm_t
);
DEFINE_FUNC12(
        Operation::C, Operation::Sendrecv, Operation::Large,
        int, MPI_Sendrecv_c,
        sendbuf_t, count_c_t, data_t, rank_t, tag_t, recvbuf_t, count_c_t,
        data_t, rank_t, tag_t, comm_t, status_t
);
DEFINE_FUNC9(
        Operation::C, Operation::SendrecvReplace, Operation::Large,
        int, MPI_Sendrecv_replace_c,
        recvbuf_t, count_c_t, data_t, rank_t, tag_t, rank_t, tag_t, comm_t,
        status_t
);
DEFINE_FUNC6(
        Operation::C, Operation::Ssend, Operation::Large,
        int, MPI_Ssend_c,
        sendbuf_t, count_c_t, data_t, rank_t, tag_t, comm_t
);

//! Blocking large collectives
DEFINE_FUNC7(
        Operation::C, Operation::Allgather, Operation::Large,
        int, MPI_Allgather_c,
        sendbuf_t, count_c_t, data_t, recvbuf_t, count_c_t, data_t, comm_t
);
DEFINE_FUNC8(
        Operation::C, Operation::Allgatherv, Operation::Large,
        int, MPI_Allgatherv_c,
        sendbuf_t, count_c_t, data_t, recvbuf_t, counts_c_t, displs_c_t, data_t,
        comm_t
);
DEFINE_FUNC6(
        Operation::C, Operation::Allreduce, Operation::Large,
        int, MPI_Allreduce_c,
        sendbuf_t, recvbuf_t, count_c_t, data_t, op_t, comm_t
);
DEFINE_FUNC7(
        Operation::C, Operation::Alltoall, Operation::Large,
        int, MPI_Alltoall_c,
        sendbuf_t, count_c_t, data_t, recvbuf_t, count_c_t, data_t, comm_t
);
DEFINE_FUNC9(
        Operation::C, Operation::Alltoallv, Operation::Large,
        int, MPI_Alltoallv_c,
        sendbuf_t, counts_c_t, displs_c_t, data_t, recvbuf_t, counts_c_t,
        displs_c_t, data_t, comm_t
);
DEFINE_FUNC9(
        Operation::C, Operation::Alltoallw, Operation::Large,
        int, MPI_Alltoallw_c,
        sendbuf_t, counts_c_t, displs_c_t, datas_t, recvbuf_t, counts_c_t,
        displs_c_t, datas_t, comm_t
);
DEFINE_FUNC1(
        Operation::C, Operation::Barrier, Operation::Large,
        int, MPI_Barrier_c,
        comm_t
);
DEFINE_FUNC5(
        Operation::C, Operation::Bcast, Operation::Large,
        int, MPI_Bcast_c,
        recvbuf_t, count_c_t, data_t, rank_t, comm_t
);
DEFINE_FUNC8(
        Operation::C, Operation::Gather, Operation::Large,
        int, MPI_Gather_c,
        sendbuf_t, count_c_t, data_t, recvbuf_t, count_c_t, data_t, rank_t,
        comm_t
);
DEFINE_FUNC9(
        Operation::C, Operation::Gatherv, Operation::Large,
        int, MPI_Gatherv_c,
        sendbuf_t, count_c_t, data_t, recvbuf_t, counts_c_t, displs_c_t, data_t,
        rank_t, comm_t
);
DEFINE_FUNC7(
        Operation::C, Operation::Reduce, Operation::Large,
        int, MPI_Reduce_c,
        sendbuf_t, recvbuf_t, count_c_t, data_t, op_t, rank_t, comm_t
);
DEFINE_FUNC6(
        Operation::C, Operation::ReduceScatter, Operation::Large,
        int, MPI_Reduce_scatter_c,
        sendbuf_t, recvbuf_t, counts_c_t, data_t, op_t, comm_t
);
DEFINE_FUNC6(
        Operation::C, Operation::ReduceScatterBlock, Operation::Large,
        int, MPI_Reduce_scatter_block_c,
        sendbuf_t, recvbuf_t, count_c_t, data_t, op_t, comm_t
);
DEFINE_FUNC8(
        Operation::C, Operation::Scatter, Operation::Large,
        int, MPI_Scatter_c,
        sendbuf_t, count_c_t, data_t, recvbuf_t, count_c_t, data_t, rank_t,
        comm_t
);
DEFINE_FUNC9(
        Operation::C, Operation::Scatterv, Operation::Large,
        int, MPI_Scatterv_c,
        sendbuf_t, counts_c_t, displs_c_t, data_t, recvbuf_t, count_c_t, data_t,
        rank_t, comm_t
);
DEFINE_FUNC6(
        Operation::C, Operation::Scan, Operation::Large,
        int, MPI_Scan_c,
        sendbuf_t, recvbuf_t, count_c_t, data_t, op_t, comm_t
);
DEFINE_FUNC6(
        Operation::C, Operation::Exscan, Operation::Large,
        int, MPI_Exscan_c,
        sendbuf_t, recvbuf_t, count_c_t, data_t, op_t, comm_t
);

//! Non-blocking large primitives
DEFINE_FUNC7(
        Operation::C, Operation::Ibsend, Operation::Large,
        int, MPI_Ibsend_c,
        sendbuf_t, count_c_t, data_t, rank_t, tag_t, comm_t, request_t
);
DEFINE_FUNC7(
        Operation::C, Operation::Irecv, Operation::Large,
        int, MPI_Irecv_c,
        recvbuf_t, count_c_t, data_t, rank_t, tag_t, comm_t, request_t
);
DEFINE_FUNC7(
        Operation::C, Operation::Irsend, Operation::Large,
        int, MPI_Irsend_c,
        sendbuf_t, count_c_t, data_t, rank_t, tag_t, comm_t, request_t
);
DEFINE_FUNC7(
        Operation::C, Operation::Isend, Operation::Large,
        int, MPI_Isend_c,
        sendbuf_t, count_c_t, data_t, rank_t, tag_t, comm_t, request_t
);
DEFINE_FUNC12(
        Operation::C, Operation::Isendrecv, Operation::Large,
        int, MPI_Isendrecv_c,
        sendbuf_t, count_c_t, data_t, rank_t, tag_t, recvbuf_t, count_c_t,
        data_t, rank_t, tag_t, comm_t, request_t
);
DEFINE_FUNC9(
        Operation::C, Operation::IsendrecvReplace, Operation::Large,
        int, MPI_Isendrecv_replace_c,
        recvbuf_t, count_c_t, data_t, rank_t, tag_t, rank_t, tag_t, comm_t,
        request_t
);
DEFINE_FUNC7(
        Operation::C, Operation::Issend, Operation::Large,
        int, MPI_Issend_c,
        sendbuf_t, count_c_t, data_t, rank_t, tag_t, comm_t, request_t
);

//! Non-blocking large collectives
DEFINE_FUNC8(
        Operation::C, Operation::Iallgather, Operation::Large,
        int, MPI_Iallgather_c,
        sendbuf_t, count_c_t, data_t, recvbuf_t, count_c_t, data_t, comm_t,
        request_t
);
DEFINE_FUNC9(
        Operation::C, Operation::Iallgatherv, Operation::Large,
        int, MPI_Iallgatherv_c,
        sendbuf_t, count_c_t, data_t, recvbuf_t, counts_c_t, displs_c_t, data_t,
        comm_t, request_t
);
DEFINE_FUNC7(
        Operation::C, Operation::Iallreduce, Operation::Large,
        int, MPI_Iallreduce_c,
        sendbuf_t, recvbuf_t, count_c_t, data_t, op_t, comm_t, request_t
);
DEFINE_FUNC8(
        Operation::C, Operation::Ialltoall, Operation::Large,
        int, MPI_Ialltoall_c,
        sendbuf_t, count_c_t, data_t, recvbuf_t, count_c_t, data_t, comm_t,
        request_t
);
DEFINE_FUNC10(
        Operation::C, Operation::Ialltoallv, Operation::Large,
        int, MPI_Ialltoallv_c,
        sendbuf_t, counts_c_t, displs_c_t, data_t, recvbuf_t, counts_c_t,
        displs_c_t, data_t, comm_t, request_t
);
DEFINE_FUNC10(
        Operation::C, Operation::Ialltoallw, Operation::Large,
        int, MPI_Ialltoallw_c,
        sendbuf_t, counts_c_t, displs_c_t, datas_t, recvbuf_t, counts_c_t,
        displs_c_t, datas_t, comm_t, request_t
);
DEFINE_FUNC2(
        Operation::C, Operation::Ibarrier, Operation::Large,
        int, MPI_Ibarrier_c,
        comm_t, request_t
);
DEFINE_FUNC6(
        Operation::C, Operation::Ibcast, Operation::Large,
        int, MPI_Ibcast_c,
        recvbuf_t, count_c_t, data_t, rank_t, comm_t, request_t
);
DEFINE_FUNC9(
        Operation::C, Operation::Igather, Operation::Large,
        int, MPI_Igather_c,
        sendbuf_t, count_c_t, data_t, recvbuf_t, count_c_t, data_t, rank_t,
        comm_t, request_t
);
DEFINE_FUNC10(
        Operation::C, Operation::Igatherv, Operation::Large,
        int, MPI_Igatherv_c,
        sendbuf_t, count_c_t, data_t, recvbuf_t, counts_c_t, displs_c_t, data_t,
        rank_t, comm_t, request_t
);
DEFINE_FUNC8(
        Operation::C, Operation::Ireduce, Operation::Large,
        int, MPI_Ireduce_c,
        sendbuf_t, recvbuf_t, count_c_t, data_t, op_t, rank_t, comm_t, request_t
);
DEFINE_FUNC7(
        Operation::C, Operation::IreduceScatter, Operation::Large,
        int, MPI_Ireduce_scatter_c,
        sendbuf_t, recvbuf_t, counts_c_t, data_t, op_t, comm_t, request_t
);
DEFINE_FUNC7(
        Operation::C, Operation::IreduceScatterBlock, Operation::Large,
        int, MPI_Ireduce_scatter_block_c,
        sendbuf_t, recvbuf_t, count_c_t, data_t, op_t, comm_t, request_t
);
DEFINE_FUNC9(
        Operation::C, Operation::Iscatter, Operation::Large,
        int, MPI_Iscatter_c,
        sendbuf_t, count_c_t, data_t, recvbuf_t, count_c_t, data_t, rank_t,
        comm_t, request_t
);
DEFINE_FUNC10(
        Operation::C, Operation::Iscatterv, Operation::Large,
        int, MPI_Iscatterv_c,
        sendbuf_t, counts_c_t, displs_c_t, data_t, recvbuf_t, count_c_t, data_t,
        rank_t, comm_t, request_t
);
DEFINE_FUNC7(
        Operation::C, Operation::Iscan, Operation::Large,
        int, MPI_Iscan_c,
        sendbuf_t, recvbuf_t, count_c_t, data_t, op_t, comm_t, request_t
);
DEFINE_FUNC7(
        Operation::C, Operation::Iexscan, Operation::Large,
        int, MPI_Iexscan_c,
        sendbuf_t, recvbuf_t, count_c_t, data_t, op_t, comm_t, request_t
);

#endif

} // extern C

#pragma GCC visibility pop

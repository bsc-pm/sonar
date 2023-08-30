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
#include "IOHandler.hpp"
#include "Manager.hpp"
#include "Operation.hpp"

using int_ptr_t = MPI_Fint *;
using recvbuf_t = void *;
using sendbuf_t = void *;
using request_t = int_ptr_t;
using requests_t = int_ptr_t;
using status_t = int_ptr_t;
using statuses_t = int_ptr_t;
using data_t = int_ptr_t;
using comm_t = int_ptr_t;
using flag_t = int_ptr_t;
using tag_t = int_ptr_t;
using rank_t = int_ptr_t;
using op_t = int_ptr_t;
using indices_t = int_ptr_t;
using count_t = int_ptr_t;
using displ_t = int_ptr_t;
using err_t = int_ptr_t;

using datas_t = data_t;
using counts_t = count_t;
using displs_t = displ_t;

using namespace sonar;

extern "C" void mpi_comm_rank_(MPI_Fint *, MPI_Fint *, MPI_Fint *);
extern "C" void mpi_comm_size_(MPI_Fint *, MPI_Fint *, MPI_Fint *);

static inline std::pair<MPI_Fint, MPI_Fint> getInformation()
{
    MPI_Fint world = MPI_Comm_c2f(MPI_COMM_WORLD);

    MPI_Fint rank, nranks, err;
    mpi_comm_rank_(&world, &rank, &err);
    if (err != MPI_SUCCESS)
        IOHandler::fail("Could not retrieve the MPI rank");

    mpi_comm_size_(&world, &nranks, &err);
    if (err != MPI_SUCCESS)
        IOHandler::fail("Could not retrieve the MPI rank size");

    return { rank, nranks };
}

#pragma GCC visibility push(default)

extern "C" {

//! Initializing and finalizing
void mpi_init_(MPI_Fint *err)
{
    Instrument::preinitialize();

    Manager::process<Operation::Fortran, Operation::Init,
                     Operation::Regular, void>("mpi_init_", err);

    auto [rank, nranks] = getInformation();
    Instrument::initialize(rank, nranks);
}

void mpi_init_thread_(MPI_Fint required, MPI_Fint *provided, MPI_Fint *err)
{
    Instrument::preinitialize();

    Manager::process<Operation::Fortran, Operation::InitThread,
                     Operation::Regular, void>("mpi_init_thread_", required,
                                               provided, err);

    auto [rank, nranks] = getInformation();
    Instrument::initialize(rank, nranks);
}

void mpi_finalize_(MPI_Fint *err)
{
    Manager::process<Operation::Fortran, Operation::Finalize,
                     Operation::Regular, void>("mpi_finalize_", err);

    Instrument::finalize();
}

//! Waiting requests
DEFINE_FUNC3(
        Operation::Fortran, Operation::Wait, Operation::Regular,
        void, mpi_wait_,
        request_t, status_t, err_t
);
DEFINE_FUNC4(
        Operation::Fortran, Operation::Waitall, Operation::Regular,
        void, mpi_waitall_,
        count_t, requests_t, statuses_t, err_t
);
DEFINE_FUNC5(
        Operation::Fortran, Operation::Waitany, Operation::Regular,
        void, mpi_waitany_,
        count_t, requests_t, indices_t, statuses_t, err_t
);
DEFINE_FUNC6(
        Operation::Fortran, Operation::Waitsome, Operation::Regular,
        void, mpi_waitsome_,
        count_t, requests_t, flag_t, indices_t, statuses_t, err_t
);

//! Testing requests
DEFINE_FUNC4(
        Operation::Fortran, Operation::Test, Operation::Regular,
        void, mpi_test_,
        request_t, flag_t, status_t, err_t
);
DEFINE_FUNC5(
        Operation::Fortran, Operation::Testall, Operation::Regular,
        void, mpi_testall_,
        count_t, requests_t, flag_t, statuses_t, err_t
);
DEFINE_FUNC6(
        Operation::Fortran, Operation::Testany, Operation::Regular,
        void, mpi_testany_,
        count_t, requests_t, indices_t, flag_t, statuses_t, err_t
);
DEFINE_FUNC6(
        Operation::Fortran, Operation::Testsome, Operation::Regular,
        void, mpi_testsome_,
        count_t, requests_t, flag_t, indices_t, statuses_t, err_t
);

//! Blocking primitives
DEFINE_FUNC7(
        Operation::Fortran, Operation::Bsend, Operation::Regular,
        void, mpi_bsend_,
        sendbuf_t, count_t, data_t, rank_t, tag_t, comm_t, err_t
);
DEFINE_FUNC8(
        Operation::Fortran, Operation::Recv, Operation::Regular,
        void, mpi_recv_,
        recvbuf_t, count_t, data_t, rank_t, tag_t, comm_t, status_t, err_t
);
DEFINE_FUNC7(
        Operation::Fortran, Operation::Rsend, Operation::Regular,
        void, mpi_rsend_,
        sendbuf_t, count_t, data_t, rank_t, tag_t, comm_t, err_t
);
DEFINE_FUNC7(
        Operation::Fortran, Operation::Send, Operation::Regular,
        void, mpi_send_,
        sendbuf_t, count_t, data_t, rank_t, tag_t, comm_t, err_t
);
DEFINE_FUNC13(
        Operation::Fortran, Operation::Sendrecv, Operation::Regular,
        void, mpi_sendrecv_,
        sendbuf_t, count_t, data_t, rank_t, tag_t, recvbuf_t, count_t, data_t,
        rank_t, tag_t, comm_t, status_t, err_t
);
DEFINE_FUNC10(
        Operation::Fortran, Operation::SendrecvReplace, Operation::Regular,
        void, mpi_sendrecv_replace_,
        recvbuf_t, count_t, data_t, rank_t, tag_t, rank_t, tag_t, comm_t,
        status_t, err_t
);
DEFINE_FUNC7(
        Operation::Fortran, Operation::Ssend, Operation::Regular,
        void, mpi_ssend_,
        sendbuf_t, count_t, data_t, rank_t, tag_t, comm_t, err_t
);

//! Blocking collectives
DEFINE_FUNC8(
        Operation::Fortran, Operation::Allgather, Operation::Regular,
        void, mpi_allgather_,
        sendbuf_t, count_t, data_t, recvbuf_t, count_t, data_t, comm_t, err_t
);
DEFINE_FUNC9(
        Operation::Fortran, Operation::Allgatherv, Operation::Regular,
        void, mpi_allgatherv_,
        sendbuf_t, count_t, data_t, recvbuf_t, counts_t, displs_t, data_t,
        comm_t, err_t
);
DEFINE_FUNC7(
        Operation::Fortran, Operation::Allreduce, Operation::Regular,
        void, mpi_allreduce_,
        sendbuf_t, recvbuf_t, count_t, data_t, op_t, comm_t, err_t
);
DEFINE_FUNC8(
        Operation::Fortran, Operation::Alltoall, Operation::Regular,
        void, mpi_alltoall_,
        sendbuf_t, count_t, data_t, recvbuf_t, count_t, data_t, comm_t, err_t
);
DEFINE_FUNC10(
        Operation::Fortran, Operation::Alltoallv, Operation::Regular,
        void, mpi_alltoallv_,
        sendbuf_t, counts_t, displs_t, data_t, recvbuf_t, counts_t, displs_t,
        data_t, comm_t, err_t
);
DEFINE_FUNC10(
        Operation::Fortran, Operation::Alltoallw, Operation::Regular,
        void, mpi_alltoallw_,
        sendbuf_t, counts_t, displs_t, datas_t, recvbuf_t, counts_t, displs_t,
        datas_t, comm_t, err_t
);
DEFINE_FUNC2(
        Operation::Fortran, Operation::Barrier, Operation::Regular,
        void, mpi_barrier_,
        comm_t, err_t
);
DEFINE_FUNC6(
        Operation::Fortran, Operation::Bcast, Operation::Regular,
        void, mpi_bcast_,
        recvbuf_t, count_t, data_t, rank_t, comm_t, err_t
);
DEFINE_FUNC9(
        Operation::Fortran, Operation::Gather, Operation::Regular,
        void, mpi_gather_,
        sendbuf_t, count_t, data_t, recvbuf_t, count_t, data_t, rank_t, comm_t,
        err_t
);
DEFINE_FUNC10(
        Operation::Fortran, Operation::Gatherv, Operation::Regular,
        void, mpi_gatherv_,
        sendbuf_t, count_t, data_t, recvbuf_t, counts_t, displs_t, data_t,
        rank_t, comm_t, err_t
);
DEFINE_FUNC8(
        Operation::Fortran, Operation::Reduce, Operation::Regular,
        void, mpi_reduce_,
        sendbuf_t, recvbuf_t, count_t, data_t, op_t, rank_t, comm_t, err_t
);
DEFINE_FUNC7(
        Operation::Fortran, Operation::ReduceScatter, Operation::Regular,
        void, mpi_reduce_scatter_,
        sendbuf_t, recvbuf_t, counts_t, data_t, op_t, comm_t, err_t
);
DEFINE_FUNC7(
        Operation::Fortran, Operation::ReduceScatterBlock, Operation::Regular,
        void, mpi_reduce_scatter_block_,
        sendbuf_t, recvbuf_t, count_t, data_t, op_t, comm_t, err_t
);
DEFINE_FUNC9(
        Operation::Fortran, Operation::Scatter, Operation::Regular,
        void, mpi_scatter_,
        sendbuf_t, count_t, data_t, recvbuf_t, count_t, data_t, rank_t, comm_t,
        err_t
);
DEFINE_FUNC10(
        Operation::Fortran, Operation::Scatterv, Operation::Regular,
        void, mpi_scatterv_,
        sendbuf_t, counts_t, displs_t, data_t, recvbuf_t, count_t, data_t,
        rank_t, comm_t, err_t
);
DEFINE_FUNC7(
        Operation::Fortran, Operation::Scan, Operation::Regular,
        void, mpi_scan_,
        sendbuf_t, recvbuf_t, count_t, data_t, op_t, comm_t, err_t
);
DEFINE_FUNC7(
        Operation::Fortran, Operation::Exscan, Operation::Regular,
        void, mpi_exscan_,
        sendbuf_t, recvbuf_t, count_t, data_t, op_t, comm_t, err_t
);

//! Non-blocking primitives
DEFINE_FUNC8(
        Operation::Fortran, Operation::Ibsend, Operation::Regular,
        void, mpi_ibsend_,
        sendbuf_t, count_t, data_t, rank_t, tag_t, comm_t, request_t, err_t
);
DEFINE_FUNC8(
        Operation::Fortran, Operation::Irecv, Operation::Regular,
        void, mpi_irecv_,
        recvbuf_t, count_t, data_t, rank_t, tag_t, comm_t, request_t, err_t
);
DEFINE_FUNC8(
        Operation::Fortran, Operation::Irsend, Operation::Regular,
        void, mpi_irsend_,
        sendbuf_t, count_t, data_t, rank_t, tag_t, comm_t, request_t, err_t
);
DEFINE_FUNC8(
        Operation::Fortran, Operation::Isend, Operation::Regular,
        void, mpi_isend_,
        sendbuf_t, count_t, data_t, rank_t, tag_t, comm_t, request_t, err_t
);
DEFINE_FUNC8(
        Operation::Fortran, Operation::Issend, Operation::Regular,
        void, mpi_issend_,
        sendbuf_t, count_t, data_t, rank_t, tag_t, comm_t, request_t, err_t
);

//! Non-blocking collectives
DEFINE_FUNC9(
        Operation::Fortran, Operation::Iallgather, Operation::Regular,
        void, mpi_iallgather_,
        sendbuf_t, count_t, data_t, recvbuf_t, count_t, data_t, comm_t,
        request_t, err_t
);
DEFINE_FUNC10(
        Operation::Fortran, Operation::Iallgatherv, Operation::Regular,
        void, mpi_iallgatherv_,
        sendbuf_t, count_t, data_t, recvbuf_t, counts_t, displs_t, data_t,
        comm_t, request_t, err_t
);
DEFINE_FUNC8(
        Operation::Fortran, Operation::Iallreduce, Operation::Regular,
        void, mpi_iallreduce_,
        sendbuf_t, recvbuf_t, count_t, data_t, op_t, comm_t, request_t, err_t
);
DEFINE_FUNC9(
        Operation::Fortran, Operation::Ialltoall, Operation::Regular,
        void, mpi_ialltoall_,
        sendbuf_t, count_t, data_t, recvbuf_t, count_t, data_t, comm_t,
        request_t, err_t
);
DEFINE_FUNC11(
        Operation::Fortran, Operation::Ialltoallv, Operation::Regular,
        void, mpi_ialltoallv_,
        sendbuf_t, counts_t, displs_t, data_t, recvbuf_t, counts_t, displs_t,
        data_t, comm_t, request_t, err_t
);
DEFINE_FUNC11(
        Operation::Fortran, Operation::Ialltoallw, Operation::Regular,
        void, mpi_ialltoallw_,
        sendbuf_t, counts_t, displs_t, datas_t, recvbuf_t, counts_t, displs_t,
        datas_t, comm_t, request_t, err_t
);
DEFINE_FUNC3(
        Operation::Fortran, Operation::Ibarrier, Operation::Regular,
        void, mpi_ibarrier_,
        comm_t, request_t, err_t
);
DEFINE_FUNC7(
        Operation::Fortran, Operation::Ibcast, Operation::Regular,
        void, mpi_ibcast_,
        recvbuf_t, count_t, data_t, rank_t, comm_t, request_t, err_t
);
DEFINE_FUNC10(
        Operation::Fortran, Operation::Igather, Operation::Regular,
        void, mpi_igather_,
        sendbuf_t, count_t, data_t, recvbuf_t, count_t, data_t, rank_t, comm_t,
        request_t, err_t
);
DEFINE_FUNC11(
        Operation::Fortran, Operation::Igatherv, Operation::Regular,
        void, mpi_igatherv_,
        sendbuf_t, count_t, data_t, recvbuf_t, counts_t, displs_t, data_t,
        rank_t, comm_t, request_t, err_t
);
DEFINE_FUNC9(
        Operation::Fortran, Operation::Ireduce, Operation::Regular,
        void, mpi_ireduce_,
        sendbuf_t, recvbuf_t, count_t, data_t, op_t, rank_t, comm_t, request_t,
        err_t
);
DEFINE_FUNC8(
        Operation::Fortran, Operation::IreduceScatter, Operation::Regular,
        void, mpi_ireduce_scatter_,
        sendbuf_t, recvbuf_t, counts_t, data_t, op_t, comm_t, request_t, err_t
);
DEFINE_FUNC8(
        Operation::Fortran, Operation::IreduceScatterBlock, Operation::Regular,
        void, mpi_ireduce_scatter_block_,
        sendbuf_t, recvbuf_t, count_t, data_t, op_t, comm_t, request_t, err_t
);
DEFINE_FUNC10(
        Operation::Fortran, Operation::Iscatter, Operation::Regular,
        void, mpi_iscatter_,
        sendbuf_t, count_t, data_t, recvbuf_t, count_t, data_t, rank_t, comm_t,
        request_t, err_t
);
DEFINE_FUNC11(
        Operation::Fortran, Operation::Iscatterv, Operation::Regular,
        void, mpi_iscatterv_,
        sendbuf_t, counts_t, displs_t, data_t, recvbuf_t, count_t, data_t,
        rank_t, comm_t, request_t, err_t
);
DEFINE_FUNC8(
        Operation::Fortran, Operation::Iscan, Operation::Regular,
        void, mpi_iscan_,
        sendbuf_t, recvbuf_t, count_t, data_t, op_t, comm_t, request_t, err_t
);
DEFINE_FUNC8(
        Operation::Fortran, Operation::Iexscan, Operation::Regular,
        void, mpi_iexscan_,
        sendbuf_t, recvbuf_t, count_t, data_t, op_t, comm_t, request_t, err_t
);

//! The version MPI 4.0 introduces the non-blocking sendrecv primitives
#if MPI_VERSION >= 4

//! Non-blocking sendrecv primitives
DEFINE_FUNC13(
        Operation::Fortran, Operation::Isendrecv, Operation::Regular,
        void, mpi_isendrecv_,
        sendbuf_t, count_t, data_t, rank_t, tag_t, recvbuf_t, count_t, data_t,
        rank_t, tag_t, comm_t, request_t, err_t
);
DEFINE_FUNC10(
        Operation::Fortran, Operation::IsendrecvReplace, Operation::Regular,
        void, mpi_isendrecv_replace_,
        recvbuf_t, count_t, data_t, rank_t, tag_t, rank_t, tag_t, comm_t,
        request_t, err_t
);

#endif

} // extern C

#pragma GCC visibility pop

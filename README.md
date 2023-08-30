# Sonar

The Sonar project defines a set of runtime libraries (`libsonar-*.so`) which
instrument parallel programming models through the [ovni][ovni] instrumentation
library.

[ovni]: https://github.com/bsc-pm/ovni
[ovni docs]: https://ovni.readthedocs.io/
[mpi]: https://www.mpi-forum.org

## Licensing

The code of Sonar libraries is lincensed under the clauses of the MIT license
included in the [COPYING](COPYING) file. The copyright of the files of this
project belongs to the Barcelona Supercomputing Center (BSC), unless otherwise
explicitly stated.

## Supported models

Currently the Sonar project only supports the instrumentation of the [MPI][mpi]
programming model through the `libsonar-mpi.so` library. The following MPI
operations are instrumented:

* *Init*, *Init Thread* and *Finalize*
* *Recv*, *Send*, *Bsend*, *Rsend*, *Ssend*, *Sendrecv*
  and *Sendrecv Replace*
* *Allgather*, *Allgatherv*, *Allreduce*, *Alltoall*,
  *Alltoallv*, *Alltoallw*, *Barrier*, *Bcast*, *Gather*,
  *Gatherv*, *Reduce*, *Reduce Scatter*, *Reduce Scatter Block*,
  *Scatter*, *Scatterv*, *Scan* and *Exscan*
* *Wait*, *Waitall*, *Waitany* and *Waitsome*
* *Test*, *Testall*, *Testany* and *Testsome*

All the existing **blocking**, **non-blocking** and **large count** variants of
the previous operations are instrumented. For instance, in the case of the send
primitive, all the `MPI_Send`, `MPI_Isend`, `MPI_Send_c` and `MPI_Isend_c` are
instrumented.

:warning: The support for Fortan interfaces is limited. All the operations above
are instrumented except the large count variants.

## Building

To build Sonar, the following software must be available:

1. A C++ compiler supporting C++17 standard
1. The automake, autoconf, libtool, and make commands
1. An MPI implementation
1. The [ovni][ovni] instrumentation library

When cloning from the repository, the building environment must be prepared
through the command below. When the code is distributed through a tarball,
it usually does not need this command.

```sh
$ autoreconf -fiv
```

Then execute the following commands:

```sh
$ ./configure --prefix=SONAR_PREFIX ...other options...
$ make
$ make install
```

where `SONAR_PREFIX` is the directory where Sonar will be installed.

The configure script accepts the following options:

1. `--with-ovni=prefix`: Specify the prefix of the ovni installation. If no
   prefix is provided, the building process will try to find the ovni library
   in the system directories. Notice that ovni is a mandatory requirement.
1. `--enable-debug`: Adds compiler debug flags and enables additional internal
   debugging mechanisms. Debug flags are **disabled** by default.
1. `--enable-asan`: Adds compiler and linker flags to enable address sanitizer
   support. Enabling debug is also recommended in this case. Address sanitizer
   support is **disabled** by default.

## Environment variables

The Sonar libraries have a default behavior which can be changed at run-time
through environment variables. By default, the libraries do not instrument the
calls to the corresponding models' interfaces. The library reads the following
envars to decide whether it should enable any instrumentation:

* `SONAR_MPI_INSTRUMENT` (default `none`): The instrumentation that should
  perform the Sonar MPI library. Valid values are `none` and `ovni`. By default,
  the value is `none` and does not enable any instrumentation. The `ovni` value
  enables the ovni instrumentation.

## Usage

To instrument the execution of an application that uses the parallel programming
model named *model*, the application must be linked to the corresponding Sonar
library: `libsonar-<model>.so`.

Then, at run-time, the user must define the envar `SONAR_<MODEL>_INSTRUMENT=ovni`
to enable the ovni instrumentation for the model's interface function calls.

For instance, in the case of MPI, the library is `libsonar-mpi.so` and the envar
is `SONAR_MPI_INSTRUMENT`. Compiling, linking and running an MPI application
`app.c` enabling the ovni instrumentation should be similar to:

```sh
$ mpicc -c app.c -o app.o
$ mpicc app.o -o app -L${SONAR_PREFIX}/lib -lsonar-mpi
$ export SONAR_MPI_INSTRUMENT=ovni
$ mpirun -n 2 ./app
```

Notice that this example assumes that the Sonar libraries can be located at
run-time (e.g., `LD_LIBRARY_PATH` was set), and the MPI implementation uses
`mpicc` and `mpirun` to compile and run, respectively.

See the [ovni documentation][ovni docs] for more information about how to
extract and emulate execution traces.

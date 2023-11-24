# Sonar Release Notes
All notable changes to this project will be documented in this file.


## Version 1.0.0, Fri Nov 24, 2023
The 1.0.0 version is the first release of the Sonar project. The Sonar library instruments the MPI function calls through the [ovni](https://github.com/bsc-pm/ovni) instrumentation library. See the [ovni documentation](https://ovni.readthedocs.io) for more information regarding the MPI instrumentation events and the configurations for Paraver.

### General
- Build the `libsonar-mpi.so` library to instrument the MPI programming model
- Instrument most common MPI communication functions in C
- Instrument most common MPI communication functions in Fortran (except large count variants)
- Add `SONAR_MPI_INSTRUMENT` environment variable to enabled instrumentation (disabled by default)

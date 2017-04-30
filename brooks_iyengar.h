#ifndef BROOKSIYENGAR_H
#define BROOKSIYENGAR_H

#include <mpi.h>
#include <stdio.h>

/* index of MPI data in info array*/
#define i_RANK 0x0
#define i_SIZE 0x1
#define MPI_INFO 0x2

/* set up any MPI related info */
#define MPI_SETUP(argc, argv, rank, size)\
  MPI_Init(&argc, &argv);\
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);\
  MPI_Comm_size(MPI_COMM_WORLD, &size);

/* clean up any MPI related info */
#define MPI_FINISH()\
  MPI_Finalize();

/* DEBUG DEBUG DEBUG */
#define DEBUG 1

#define DBG(fmt, ...) \
    if (DEBUG) fprintf(stderr, fmt, __VA_ARGS__);

#endif /* BROOKSIYENGAR_H */

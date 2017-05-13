#ifndef BROOKSIYENGAR_H
#define BROOKSIYENGAR_H

#include <mpi.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>

struct sensor_message {
  int num;
  float data;
  time_t time_of_measurement;
};

#define MESSAGE_SIZE\
  sizeof( struct sensor_message )

/* index of MPI data in info array*/
#define i_RANK 0x0
#define i_SIZE 0x1
#define MPI_INFO 0x2

/* some program specific macros */
#define SENSING 0x1

#define MPI_LOGIC( index, rank )\
  index < rank ? index : index - 1;

#define PRINT_MESSAGE(message)\
  char sbuf[30];\
  struct tm lt = *localtime( &message.time_of_measurement );\
  strftime(sbuf, 30,"%x at %I:%M%p", &lt );\
  fprintf(stderr, "Message is from %d :: data is %f on %s\n",\
      message.num, message.data, sbuf);

/* set up any MPI related info */
#define MPI_SETUP(argc, argv, rank, size)\
  MPI_Init(&argc, &argv);\
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);\
  MPI_Comm_size(MPI_COMM_WORLD, &size);

/* clean up any MPI related info */
#define MPI_FINISH()\
  EXIT:\
  MPI_Finalize();

/* DEBUG DEBUG DEBUG */
#define DEBUG 1

#define DBG(fmt, ...) \
    if (DEBUG) fprintf(stderr, fmt, __VA_ARGS__);

#endif /* BROOKSIYENGAR_H */

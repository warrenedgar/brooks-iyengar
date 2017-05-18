#ifndef BROOKSIYENGAR_H
#define BROOKSIYENGAR_H

#include <mpi.h>
#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* numeric constants */
#define SENSING 0x1
#define INTERVAL_SIZE 0xA
#define PRINT_BUFFER_SIZE 50

struct sensor_message {
  float data [INTERVAL_SIZE];
  int from_process;
  int to_process;
  time_t time_of_measurement;
  time_t time_received;
};

void measure(struct sensor_message * sm);
void fuse(struct sensor_message * buffer);
void parse_out_command_args( char ** argv );
void init_struct( struct sensor_message * init, float * data, int to_process);

/* some program specific macros */
#define MESSAGE_SIZE\
  sizeof( struct sensor_message )

#define RECV_INDEX( index, rank )\
  index < rank ? index : index - 1

/* set up any MPI related info */
#define MPI_SETUP(argc, argv, rank, size)\
  MPI_Init(&argc, &argv);\
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);\
  MPI_Comm_size(MPI_COMM_WORLD, &size);

/* see if enough time has elapsed to exit */
#define CHECK_RUNTIME(start_time, current_time, run_time, _rank)\
  int flag = 0;\
  if( _rank == 0 ){\
    gettimeofday(&current_time, NULL);\
    unsigned long elapsed_time = current_time.tv_sec - start_time.tv_sec;\
    if( (elapsed_time / 60) >= run_time ) flag = 1;\
  }\
  MPI_Bcast(&flag, 1, MPI_INT, 0, MPI_COMM_WORLD);\
  if( flag )goto EXIT;\

/* clean up any MPI related info */
#define MPI_FINISH()\
  EXIT:\
  MPI_Finalize();

/* DEBUG DEBUG DEBUG */
#define DEBUG 1

#define DBG(fmt, ...) \
  if ( DEBUG ) fprintf(stderr, fmt, __VA_ARGS__);

#define PRINT_MESSAGE(message)\
  if( DEBUG ){\
    char sbuf[PRINT_BUFFER_SIZE];\
    struct tm mt = *localtime( &message.time_of_measurement );\
    strftime(sbuf, PRINT_BUFFER_SIZE, "%x at %I:%M%p", &mt );\
    char sbuf2[PRINT_BUFFER_SIZE];\
    struct tm rt = *localtime( &message.time_received );\
    strftime(sbuf2, PRINT_BUFFER_SIZE, "%x at %I:%M%p", &rt );\
    fprintf(stderr, "Rank %d:: message is from %d :: data is %f measured at %s received at %s\n",\
      message.to_process, message.from_process, message.data[0], sbuf, sbuf2);\
  }

#endif /* BROOKSIYENGAR_H */

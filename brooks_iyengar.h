#ifndef BROOKSIYENGAR_H
#define BROOKSIYENGAR_H

#include <mpi.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* numeric constants */
#define SENSING 0x1
#define INTERVAL_SIZE 0x5
#define PRINT_BUFFER_SIZE 50

/* an example of a more complex message */
struct sensor_message {
  float data [INTERVAL_SIZE];
  int from_process;
  int to_process;
  time_t time_of_measurement;
  time_t time_received;
};

/* function prototypes  */
void measure(struct sensor_message * sm);
void fuse(struct sensor_message * buffer);
void parse_out_command_args( char ** argv );
void bubble_sort(float * data);
void init_struct( struct sensor_message * init, float * data, int to_process);

/* some program specific macros */
#define MESSAGE_SIZE\
  sizeof( struct sensor_message )

/* a little logic for MPI requests indexes */
#define RECV_INDEX( index, rank )\
  index < rank ? index : index - 1

/* Get a random float in range 0 - max */
#define GET_RAND( max )\
  ((((float)rand()/(float)(RAND_MAX)) * 2 * max) - (max))

/* set up any MPI related info */
#define MPI_SETUP(argc, argv, rank, size)\
  MPI_Init(&argc, &argv);\
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);\
  MPI_Comm_size(MPI_COMM_WORLD, &size);

/* open report file */
#define OPEN_FILE( _rank, fp )\
  char sbuf[PRINT_BUFFER_SIZE];\
  sprintf(sbuf, "processOutput%d", _rank);\
  fp = fopen(sbuf, "w");

/* write to file as newline delimited data*/
#define WRITE_TO_FILE( fused, average, fp )\
  fprintf( fp, "%04.04f,%04.04f\n", fused, average)

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
#define MPI_FINISH( fp )\
  EXIT:\
  fclose(fp);\
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
    fprintf(stderr, "Process %d:: message is from %d :: data is [ %03.03f, %03.03f ,%03.03f, %03.03f, %03.03f] measured at [%s] received at [%s]\n",\
      message.to_process, message.from_process,\
      message.data[0], message.data[1], message.data[2], message.data[3], message.data[4],\
      sbuf, sbuf2);\
  }

#endif /* BROOKSIYENGAR_H */

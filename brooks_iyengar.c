/* see brooks_iyengar.h for includes */
#include "brooks_iyengar.h"

struct timeval start_time, current_time;
int _rank = -1;
int _size = -1;
unsigned long run_time = 1;

int main( int argc, char ** argv ){

  MPI_SETUP(argc, argv, _rank, _size);
  MPI_Status status[ (_size - 1)*2 ];
  MPI_Request request[ (_size - 1)*2 ];
  struct sensor_message buffer[_size-1];
  gettimeofday(&start_time, NULL);
  run_time = atoi(argv[1]);

  while( SENSING ){
    MPI_Barrier(MPI_COMM_WORLD);

    for( int i = 0; i < _size; ++i ){
      if( i == _rank ) continue;
      struct sensor_message sender;
      sender.from_process = _rank;
      sender.data = 1.0 * i;
      sender.to_process = i;
      time( &sender.time_of_measurement );
      int recv_index = RECV_INDEX(i, _rank);
      MPI_Isend( (void*)&sender, MESSAGE_SIZE, MPI_BYTE, i, 111, MPI_COMM_WORLD, &request[recv_index]);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    for( int i = 0; i < _size; ++i ){
      if( i == _rank )continue;
      int recv_index = RECV_INDEX(i, _rank);
      MPI_Irecv(&buffer[recv_index], MESSAGE_SIZE, MPI_BYTE, i, 111, MPI_COMM_WORLD,
          &request[recv_index + _size - 1]);
    }

    MPI_Waitall((_size-1)*2, request, status);

    for( int i = 0; i < _size-1; ++i){
      struct sensor_message received = *(struct sensor_message *)&buffer[i];
      time( &received.time_received );
      PRINT_MESSAGE(received);
    }

    sleep(2);
    CHECK_RUNTIME(start_time, current_time, run_time, _rank);

  }
  MPI_FINISH();
  return 0;
}

/* see brooks_iyengar.h for includes */
#include "brooks_iyengar.h"

int main( int argc, char ** argv ){


  int _rank = -1;
  int _size = -1;
  MPI_SETUP(argc, argv, _rank, _size);
  DBG("Hello, I am %d of %d\n",_rank,_size);

  while( SENSING ){
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Status status[ (_size - 1)*2 ];
    MPI_Request request[ (_size - 1)*2 ];

    for( int i = 0; i < _size; ++i ){
      if( i == _rank ) continue;
      struct sensor_message sender;
      sender.num = _rank;
      sender.data = 1.0 * i;
      time( &sender.time_of_measurement );
      int recv_index = MPI_LOGIC(i, _rank);
      DBG("Sending %d to %d :: request_index is %d\n", _rank, sender.num, recv_index);
      MPI_Isend( (void*)&sender, MESSAGE_SIZE, MPI_BYTE, i, 111, MPI_COMM_WORLD, &request[recv_index]);
    }

    struct sensor_message buffer[_size-1];
    MPI_Barrier(MPI_COMM_WORLD);
    for( int i = 0; i < _size; ++i ){
      if( i == _rank )continue;
      int recv_index = MPI_LOGIC(i, _rank);
      DBG("Setting up reception rank is -> %d  recv index is -> %d\n", _rank, (recv_index + _size -1));
      MPI_Irecv(&buffer[recv_index], MESSAGE_SIZE, MPI_BYTE, i, 111, MPI_COMM_WORLD,
          &request[recv_index + _size - 1]);
    }

    MPI_Waitall((_size-1)*2, request, status);
    MPI_Barrier(MPI_COMM_WORLD);

    for( int i = 0; i < _size-1; ++i){
      struct sensor_message received = *(struct sensor_message *)&buffer[i];
      time( &received.time_received );
      PRINT_MESSAGE(received);
    }

    sleep(2);
    CHECK_RUNTIME();

  }
  MPI_FINISH();
  return 0;
}

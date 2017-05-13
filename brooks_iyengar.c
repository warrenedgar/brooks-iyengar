/* see brooks_iyengar.h for includes */
#include "brooks_iyengar.h"

int main( int argc, char ** argv ){


  int info [MPI_INFO]; 
  MPI_SETUP(argc, argv, info[i_RANK], info[i_SIZE]);
  DBG("Hello I am #%d out of %d.\n", info[i_RANK], info[i_SIZE]);

  while( SENSING ){

    MPI_Status status[ info[i_SIZE] - 1 ];
    MPI_Request request[ info[i_SIZE] - 1 ];

    for( int i = 0; i < info[i_SIZE]; ++i ){
      if( i != info[i_RANK] ){
        //struct sensor_message sender;
        //sender.num = info[i_RANK];
        //sender.data = 1.0 * info[i_RANK];
        //time( &sender.time_of_measurement );
        //MPI_Isend( (void*)&sender, MESSAGE_SIZE, MPI_BYTE, i, 111, MPI_COMM_WORLD, &request[i]);
        int recv_index = MPI_LOGIC(i, info[i_RANK]);
        MPI_Isend( &info[i_RANK], 1, MPI_INT, i, 111, MPI_COMM_WORLD, &request[recv_index]);
      }
    }

    //char buffer[i_SIZE][MESSAGE_SIZE];
    int buffer[i_SIZE-1];

    for( int i = 0; i < info[i_SIZE]; ++i ){
      if( i != info[i_RANK] ){
        //MPI_Irecv(buffer[i], MESSAGE_SIZE, MPI_BYTE, i, 111, MPI_COMM_WORLD,
        //    &request[i]);
        int recv_index = MPI_LOGIC(i, info[i_RANK]);
        MPI_Irecv(&buffer[recv_index], 1, MPI_INT, i, 111, MPI_COMM_WORLD,
            &request[recv_index]);
      }
    }

    MPI_Waitall(info[i_SIZE]-1, request, status);

    for( int i = 0; i < info[i_SIZE]; ++i){
      if( i != info[i_RANK] ){
        //struct sensor_message received = *(struct sensor_message *)&buffer[i];
        //time( &received.time_of_measurement );
        fprintf(stderr, "rank %d message from %d recived.\n", info[i_RANK], buffer[i] );
        //PRINT_MESSAGE(received);
      }
    }
    sleep(1);
  }

  MPI_FINISH();
  return 0;
}

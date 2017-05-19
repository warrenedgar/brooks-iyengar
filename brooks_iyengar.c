/* see brooks_iyengar.h for includes */
#include "brooks_iyengar.h"

struct timeval start_time, current_time;
int _rank = -1;
int _size = -1;
unsigned long run_time = 1;
int number_faulty_sensors = 1;
float counter = 0;
FILE * fp;

int main( int argc, char ** argv ){

  /* MPI related boilerplate */
  MPI_SETUP(argc, argv, _rank, _size);
  MPI_Status status[ (_size - 1)*2 ];
  MPI_Request request[ (_size - 1)*2 ];

  /* pre-declare space for message(s) from every sensor */
  struct sensor_message buffer[_size];
  memset(buffer, 0, _size * MESSAGE_SIZE);
  srand((unsigned int)(time(NULL)*(_rank + _size)));

  /* set up running parameters */
  parse_out_command_args( argv );
  OPEN_FILE( _rank, fp );

  /* grab a pointer to the local interval, will be used to hold measurements */
  struct sensor_message * local = &buffer[_rank];

  /* run the simulated sensor network */
  while( SENSING ){

    MPI_Barrier(MPI_COMM_WORLD);
    measure( local );

    for( int i = 0; i < _size; ++i ){
      if( i == _rank ) continue;
      struct sensor_message sender; init_struct( &sender, local->data, i);
      MPI_Isend( (void*)&sender, MESSAGE_SIZE, MPI_BYTE, i, 111, MPI_COMM_WORLD, &request[RECV_INDEX(i, _rank)]);
    }

    for( int i = 0; i < _size; ++i ){
      if( i == _rank )continue;
      int recv_index = RECV_INDEX(i, _rank);
      MPI_Irecv(&buffer[i], MESSAGE_SIZE, MPI_BYTE, i, 111, MPI_COMM_WORLD,
          &request[recv_index + _size - 1]);
    }

    MPI_Waitall((_size-1)*2, request, status);

    for( int i = 0; i < _size; ++i){
      struct sensor_message received = *(struct sensor_message *)&buffer[i];
      time( &received.time_received );
      PRINT_MESSAGE(received);
    }

    fuse(buffer);
    WRITE_TO_FILE( buffer[_rank].data[0], fp );

    sleep(2);
    CHECK_RUNTIME(start_time, current_time, run_time, _rank);

  }

  /* done */
  MPI_FINISH( fp );
  return 0;
}

/* run the code to simulate an individual sensor */
void measure(struct sensor_message * sm){
  for( int n = 0; n < INTERVAL_SIZE; ++n)
    sm->data[n] = 2.0 * sin( counter*M_PI/2 ) + GET_RAND(.1);
  bubble_sort(sm->data);
  counter += .2;
}

/* run the code to fuse all of the sensors data */
void fuse(struct sensor_message * buffer){

}

/* the much maligned bubble sort */
void bubble_sort(float * data){
  bool flag = true;
  for( int n = 0; n < INTERVAL_SIZE; ++n){
    flag = true;
    for( int i = 0; i < INTERVAL_SIZE-n-1; ++i){
      if( data[i] >  data[i+1] ){
        flag = false;
        float temp = data[i+1];
        data[i+1] = data[i];
        data[i] = temp;
      }
    }
    if( flag == true) break;
  }
}

/* any parsing from the run script it handled here */
void parse_out_command_args( char ** argv ){
  gettimeofday(&start_time, NULL);
  run_time = atoi(argv[1]);
  number_faulty_sensors = atoi(argv[2]);
}

/* init a message */
void init_struct( struct sensor_message * init, float * data, int to_process  ){
  init->from_process = _rank;
  for( int n = 0; n < INTERVAL_SIZE; ++n)
    init->data[n] = data[n];
  init->to_process = to_process;
  time( &init->time_of_measurement );
}

/* see brooks_iyengar.h for includes */
#include "brooks_iyengar.h"

int main( int argc, char ** argv ){


  int info [MPI_INFO]; 
  MPI_SETUP(argc, argv, info[i_RANK], info[i_SIZE]);
  DBG("Hello I am #%d out of %d.\n", info[i_RANK], info[i_SIZE]);


  while( SENSING ){

  }

  MPI_FINISH();
  return 0;
}

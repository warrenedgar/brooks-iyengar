#!/bin/bash

# clean and build the program
make clean 
make

# get todays date + time
today=`date +%Y-%m-%d.%H:%M:%S` 

# init variables
nprocess=1

# get the number of threads to run program with
for i in "$@";do
  case $i in
    -p=*)
      nprocess="${i#*=}"
      shift
      ;;
    -c=*)
      func=0
      shift
      ;;
    *)
      ;;
  esac
done

if [[ $OSTYPE == darwin* ]]; then
  export TMPDIR="/tmp"
fi

# create log file
mkdir -p results$today/
touch results$today/nprocess$nprocess:brooksIyengar$today.txt

mpirun -np $nprocess ./brooks_iyengar >> results$today/nprocess$nprocess:brooksIyengar$today.txt

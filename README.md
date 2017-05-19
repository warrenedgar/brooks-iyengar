# Brooks Iyengar Algorithm

## Introduction
A classical problem in distributed computing is Byzantine Generals' Problem, introduced in the 1982 white paper of the same name. It attempted to formalize a definition for faulty (traitorous) nodes in a cluster and how for the system to mitigate it. Solutions such as majority voting or signed messages were suggested. Majority voting requires that all generals have the same information, a suggestion that isn't always possible. Signed messages are a good to verify that it was the correct node in communication, even if it doesn't verify that the content itself is correct. Both are good suggestions, but it would be more interesting to have an algorithm that can survive a traitorous order every now and then.

Enter the Brooks-Iyengar algorithm as an attempt to solve this problem. This algorithm uses sensor fusion to mathematically eliminate the faulty sensors. In short, this is achieved by taking measurements over an interval, the measured interval is then shared between all sensors on the network. The fusion step then happens, by creating a weighted average of the midpoints of all the intervals. At this point you can eliminate any sensors with high variance or use heuristics to choose the reliable nodes. It runs in O(NlogN) time and can handle up to N/3 faulty sensors.

## Algorithm in pseudocode
```
while sensing

&nbsp;&nbsp;get measurement over time interval
  
&nbsp;&nbsp;send all measurements to every other sensor
  
&nbsp;&nbsp;isolate endpoints
  
&nbsp;&nbsp;make a weighted region diagram
  
&nbsp;&nbsp;discard lowly weighted sections
  
&nbsp;&nbsp;report new midpoint
```

# OpenMPI Methods Used

## Isend and Ireceive
Non-blocking sends and receives were used to communicate from sensor to sensor. In order to process the data each sensor needs the data from every other sensor in the network. This means there are a worst case of N^2 messages being passed at any given point. Due to this large number, it is best to use non-blocking communications.

## Barrier
This acts as a sync step for all sensors. Barrier merely acts as a join for processes in the context of OpenMPI. It is very useful for a simulation as this to stop one process from being a front runner.

## Broadcast
Seeing as this is a timed excution program, it is necessary for each sensor to kill itself after a fixed period of time. However, it is possible for one process to keep running if it gets to the check before all the others. To get around this one thread was designated with the resposibility to check the runtime, and then broadcasted the result to all others.

# Technical Details

## Running
As with other submissions simply run the run_script

```sh
bash run_script.sh
```

With parameters

```sh
bash run_script.sh -t=2 -p=8 -e=2
```
Where -t=# is the time in minutes, -p=# is number of sensors in the network, -e=# is the number of faulty sensors.

## What are we simulating

The function f(x) = 2 * sin((pi * x )/2) will be calculated. In addition to this every measurement will have noise added to it within the range of (-.05 , .05). For every new step .2 is added to x, the simulation starts at 0 and goes till a timeout occurs.

If a sensor is a faulty unit, the data it sends to each other sensor will be different. This will be each measurement with random noise added (-.5, .5). 

# Results
![graph of results](https://github.com/unh-hpc/project-2-brooks-iyengar/blob/master/results.png)

The green dashed line is result from running the Brooks-Iyengar algorithm, red dashed line is the Brooks-Iyengar algorithm without the last step of discarding the lower weight intervals. In other words, an average of all measurements. The small blue dots are the analytical result.

# Discussion

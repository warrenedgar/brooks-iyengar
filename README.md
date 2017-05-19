# Brooks Iyengar Algorithm

A classical problem in distributed computing is Byzantine Generals' Problem, introduced in the 1982 white paper of the same name. It attempted to formalize a definition for faulty (traitorous) nodes in a cluster and how for the system to mitigate it. Solutions such as majority voting or signed messages were suggested. Majority voting requires that all generals have the same information, a suggestion that isn't always possible. Signed messages are a good to verify that it was the correct node in communication, even if it doesn't verify that the content itself is correct. Both are good suggestions, but it would be more interesting to have an algorithm that can survive a traitorous order every now and then.

Enter the Brooks-Iyengar algorithm as an attempt to solve this problem. This algorithm uses sensor fusion to mathematically eliminate the faulty sensors. In short, this is achieved by taking measurements over an interval, the measured interval is then shared between all sensors on the network. The fusion step then happens, by creating a weighted average of the midpoints of all the intervals. At this point you can eliminate any sensors with high variance or use heuristics to choose the reliable nodes. It runs in O(NlogN) time and can handle up to N/3 faulty sensors.

# OpenMPI Methods Used

## Isend and Ireceive

## Barrier

## Broadcast

# Results

# Discussion

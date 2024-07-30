# You Spin Me Round Robin

 Implementation for round robin scheduling for a given workload and quantum length.

## Building

```shell
I use the TAILQ structure to implement the queue in round robin, sort the process list by arrival time first then add them to the queue.
```

## Running

cmd for running 
```shell
./rr processes.txt 3
```

results 
```shell
Average waiting time: 7.00
Average response time: 2.75
```

## Cleaning up

```shell
make clean
```

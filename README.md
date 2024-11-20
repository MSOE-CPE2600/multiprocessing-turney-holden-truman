# System Programming Lab 11 Multiprocessing

## Runtime Results
![Number of processes vs Time](https://github.com/MSOE-CPE2600/multiprocessing-turney-holden-truman/blob/labWeek11dev/runtimes.png)

This graph shows that the runtime initially decreases when more processors are added as expected. However, the jump from 5 to 10 processors actually makes the program run slower.
This could be because there is more overhead for creating this many processes. Perhaps creating a new process requires one of the cores to switch back and forth between two different processes,
which would drastically slow things down.


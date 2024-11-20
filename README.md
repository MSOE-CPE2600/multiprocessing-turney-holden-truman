# System Programming Lab 11 Multiprocessing
## Overview of Implementation
My implementation works by creating the specified number of processes in a for-loop. The parent process loops through the for loop and creates a new child process in each iteration
the child processes all exit after they create their image(s), so they do not ever fork. The parent process then waits for the children to finish creating their images, then the program ends.

## Runtime Results
![Number of processes vs Time](https://github.com/MSOE-CPE2600/multiprocessing-turney-holden-truman/blob/labWeek11dev/runtimes.png)

This graph shows that the runtime initially decreases when more processors are added as expected. However, the jump from 5 to 10 processors actually makes the program run slower.
This could be because there is more overhead for creating this many processes, perhaps the machines memory is being overloaded from copying over variables for each process.

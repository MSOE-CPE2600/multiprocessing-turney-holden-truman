# System Programming Lab 11 Multiprocessing
## Overview of Multi-Processing Implementation
My implementation works by creating the specified number of processes in a for-loop. The parent process loops through the for loop and creates a new child process in each iteration
the child processes all exit after they create their image(s), so they do not ever fork. The parent process then waits for the children to finish creating their images, then the program ends.

## Runtime Results
![Number of processes vs Time](https://github.com/MSOE-CPE2600/multiprocessing-turney-holden-truman/blob/labWeek11dev/runtimes.png)

This graph shows that the runtime initially decreases when more processors are added as expected. However, the jump from 5 to 10 processors actually makes the program run slower.
This could be because there is more overhead for creating this many processes, perhaps the machines memory is being overloaded from copying over variables for each process.

# System Programming Lab 12 Multithreading
## Overview of Multi-Threading Implementation
My implementation works by creating the specified number of threads in a for loop for each of the 50 images calculated for the movie. If there are 3 threads specified, for example, then the first thread calculates the top third of each image, the second calculates the middle third, and the final calculates the bottom third.

## Multi-Threading & Multi-Processing Time Data

| time [s] | # threads | 1     | 2      | 5      | 10     | 15     | 20     |
|----------|-----------|-------|--------|--------|--------|--------|--------|
| # children |           |       |        |        |        |        |        |
| 1        |           | 29.76 | 22.096 | 16.878 | 11.851 | 10.068 | 9.02   |
| 2        |           | 29.568| 22.206 | 15.761 | 11.15  | 11.233 | 9.525  |
| 5        |           | 29.526| 22.525 | 17.755 | 15.373 | 15.644 | 15.515 |
| 10       |           | 30.846| 25.337 | 24.005 | 23.756 | 23.611 | 23.49  |
| 15       |           | 31.044| 32.635 | 31.136 | 31.347 | 31.206 | 30.45  |
| 20       |           | 38.873| 37.205 | 37.127 | 38.71  | 37.595 | 38.302 |

From this table it would seem that continuing to add more threads will almost always reduce runtime, however, you reach a plateau after a certain threshold. This was not the case for multi-processing, adding too many processes would actually make the runtime longer because more memory was being copied and utilized. It would seem that 10 threads gets you very close to peak performance for my machine (regardless of # of children), adding more could get you marginally better performance though. Overall, the best performance was seen when using 10-20 threads for 1-2 children, with the absolute lowest runtime being for 20 threads and 1 child. This testing thus suggests that multi-threading is far better for obtaining optimal performance compared to multi-processing. However, it can be challenging to implement multi-threading if your program has to make calculations that are not independent of each other such as the calculations in this lab.


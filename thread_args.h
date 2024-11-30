/**********************************
* CPE 2600 121 Lab 12: Multithreading
* Filename: thread_args.h
* Description: header file that contains the thread_args_t struct
*
* Author: Holden Truman
* Date 11/29/2024
***********************************/

typedef struct {
    imgRawImage* img;
    double xmin; 
    double xmax; 
    double ymin; 
    double ymax; 
    int max;
    int start_height;
    int end_height;
} thread_args_t;
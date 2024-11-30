/**********************************
* CPE 2600 121 Lab 11: Multiprocessing
* Filename: movie.c
* Description: Program creates 50 images for a Mandelbot movie with a specified number of processes from the user
* as well as a specified number of threads
*
* Author: Holden Truman
* Date 11/29/2024
***********************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "jpegrw.h"
#include <sys/wait.h>
#include <sys/stat.h>
#include "thread_args.h"
#include <pthread.h>

// local routines
static int iteration_to_color( int i, int max );
static int iterations_at_point( double x, double y, int max );
static void compute_image( imgRawImage *img, double xmin, double xmax,
									double ymin, double ymax, int max, int num_threads );
static void show_help();


int main( int argc, char *argv[] )
{
	char c;

	// These are the default configuration values used
	// if no command line arguments are given.
	char outputprefix[] = "movie/mandel";
	double xcenter = 0;
	double ycenter = 0;
	double xscale = 4;
	double yscale = 0; // calc later
	int    image_width = 1000;
	int    image_height = 1000;
	int    max = 1000;

    int num_children = 5;
	int num_threads = 1;

	// For each command line argument given,
	// override the appropriate configuration value.
	while((c = getopt(argc,argv,"c:t:h"))!=-1) {
		switch(c) 
		{
			case 'c':
				num_children = atof(optarg);
				break;
			case 't':
				num_threads = atof(optarg);
				break;
            case 'h':
				show_help();
				exit(1);
				break;
		}
	}
    printf("Processing 50 images with %d children and %d threads\n", num_children, num_threads);

	for (int i = 0; i < num_children; i++) {
        pid_t pid = fork();
        if (pid < 0) {
            // Fork failed
            perror("Fork failed");
            exit(1);
        } else if (pid == 0) { // child process will not fork, so proper # of children made
            // Child process
        
            int image_num = i;
            while ((num_children < 50) && (image_num < 50)) { // processes will make multipe images if necessary
                //naming sequential images
                char outputFile[50];
                char imagenumber[3];  // Ensure the buffer is large enough to hold the string
                strcpy(outputFile, outputprefix);
                sprintf(imagenumber, "%d", image_num+1); // Convert i to a string
                strcat(outputFile, imagenumber);
                strcat(outputFile, ".jpg");
                
                xcenter = xcenter - (image_num * .007); //go to the left in later images
                ycenter = ycenter + (image_num * .007); //go up in later images
                xscale = 4 * (50.0/(image_num*8+50.0)); //zoom in more for later images
                max = 1000;

                /// Calculate y scale based on x scale (settable) and image sizes in X and Y (settable)
                yscale = xscale / image_width * image_height;

                // Create a raw image of the appropriate size.
                imgRawImage* img = initRawImage(image_width,image_height);

                // Fill it with a black
                setImageCOLOR(img,0);

                // Compute the Mandelbrot image
                compute_image(img,xcenter-xscale/2,xcenter+xscale/2,ycenter-yscale/2,ycenter+yscale/2,max, num_threads);

                // Save the image in the stated file.
                storeJpegImageFile(img,outputFile);

                // free the mallocs
                freeRawImage(img);
                image_num += num_children;
            }
            
            exit(0); // child exits and doesn't loop
        } else {
            // Parent process
            //does nothing
        }
    }

    // Parent process waits for all children to finish
    for (int j  = 0; j < num_children; j++) {
        wait(NULL);
    }
	return 0;
}




/*
Return the number of iterations at point x, y
in the Mandelbrot space, up to a maximum of max.
*/

int iterations_at_point( double x, double y, int max )
{
	double x0 = x;
	double y0 = y;

	int iter = 0;

	while( (x*x + y*y <= 4) && iter < max ) {

		double xt = x*x - y*y + x0;
		double yt = 2*x*y + y0;

		x = xt;
		y = yt;

		iter++;
	}

	return iter;
}

//compute part of an image (used for multi-threading)
void* compute_part_image(void* arg) {
    thread_args_t* args = (thread_args_t*)arg;
    
	int i,j;

	int width = args->img->width;
	int height = args->img->height;

	// For every pixel in the image...
	for(j=args->start_height;j<args->end_height;j++) {

		for(i=0;i<width;i++) {

			// Determine the point in x,y space for that pixel.
			double x = args->xmin + i*(args->xmax-args->xmin)/width;
			double y = args->ymin + j*(args->ymax-args->ymin)/height;

			// Compute the iterations at that point.
			int iters = iterations_at_point(x,y,args->max);

			// Set the pixel in the bitmap.
			setPixelCOLOR(args->img,i,j,iteration_to_color(iters,args->max));
		}
	}
	return NULL;
}

/*
Compute an entire Mandelbrot image, writing each point to the given bitmap.
Scale the image to the range (xmin-xmax,ymin-ymax), limiting iterations to "max"
*/

void compute_image(imgRawImage* img, double xmin, double xmax, double ymin, double ymax, int max, int num_threads )
{
	pthread_t threads[num_threads]; 
	thread_args_t args[num_threads];
	int result;

	//have to make the args before you start making threads or else issues occur
	for(int i = 0; i < num_threads; i++) {
		args[i].img = img;
        args[i].xmin = xmin;
        args[i].xmax = xmax;
        args[i].ymin = ymin;
        args[i].ymax = ymax;
        args[i].max = max;
        args[i].start_height = (img->height * i) / num_threads; //each thread calculates a chunk of the image
        args[i].end_height = (img->height * (i + 1)) / num_threads; //the next threads chunk is on top of the previous'
	}

	//create each thread that calcultates 1 / num_threads of the image
	for (int i = 0; i < num_threads; i++) {
		result = pthread_create(&threads[i], NULL, compute_part_image, (void*) &args[i]);
        if (result != 0) {
            fprintf(stderr, "Error creating thread %d\n", i);
            exit(1);
        }
	}

	// Wait for each thread to finish
    for (int i = 0; i < num_threads; i++) {
        result = pthread_join(threads[i], NULL);
        if (result != 0) {
            fprintf(stderr, "Error joining thread %d\n", i);
            exit(1);
        }
    }
}


/*
Convert a iteration number to a color.
Here, we just scale to gray with a maximum of imax.
Modify this function to make more interesting colors.
*/
int iteration_to_color( int iters, int max )
{
	int color = 0xFFFFFF*iters/(double)max;
	return color;
}


// Show help message
void show_help()
{
	printf("Use: movie [options]\n");
	printf("Where options are:\n");
	printf("-c <# of processes>    The number of processes that create the images (default=5)\n");
	printf("-t <# of threads>    The number of threads that process the images, can be used with multiple processes (default=1)\n");
	printf("-h          Show this help text.\n");
	printf("\nSome examples are:\n");
	printf("movie.exe -h\n");
	printf("movie.exe -c 10\n");
}

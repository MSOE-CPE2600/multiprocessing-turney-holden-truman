/**********************************
* CPE 2600 121 Lab 11: Multiprocessing
* Filename: movie.c
* Description: Program creates 50 images for a Mandelbot movie with a specified number of processes from the user
*
* Author: Holden Truman
* Date 11/19/2024
***********************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "jpegrw.h"
#include <sys/wait.h>
#include <sys/stat.h>

// local routines
static int iteration_to_color( int i, int max );
static int iterations_at_point( double x, double y, int max );
static void compute_image( imgRawImage *img, double xmin, double xmax,
									double ymin, double ymax, int max );
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

	// For each command line argument given,
	// override the appropriate configuration value.
	while((c = getopt(argc,argv,"c:h"))!=-1) {
		switch(c) 
		{
			case 'c':
				num_children = atof(optarg);
				break;
            case 'h':
				show_help();
				exit(1);
				break;
		}
	}
    printf("Processing 50 images with %d children\n", num_children);

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
                compute_image(img,xcenter-xscale/2,xcenter+xscale/2,ycenter-yscale/2,ycenter+yscale/2,max);

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

/*
Compute an entire Mandelbrot image, writing each point to the given bitmap.
Scale the image to the range (xmin-xmax,ymin-ymax), limiting iterations to "max"
*/

void compute_image(imgRawImage* img, double xmin, double xmax, double ymin, double ymax, int max )
{
	int i,j;

	int width = img->width;
	int height = img->height;

	// For every pixel in the image...

	for(j=0;j<height;j++) {

		for(i=0;i<width;i++) {

			// Determine the point in x,y space for that pixel.
			double x = xmin + i*(xmax-xmin)/width;
			double y = ymin + j*(ymax-ymin)/height;

			// Compute the iterations at that point.
			int iters = iterations_at_point(x,y,max);

			// Set the pixel in the bitmap.
			setPixelCOLOR(img,i,j,iteration_to_color(iters,max));
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
	printf("-h          Show this help text.\n");
	printf("\nSome examples are:\n");
	printf("movie.exe -h\n");
	printf("movie.exe -c 10\n");
}

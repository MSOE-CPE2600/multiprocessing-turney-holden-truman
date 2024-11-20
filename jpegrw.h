/**********************************
* CPE 2600 121 Lab 11: Multiprocessing
* Filename: jpegrw.h
* Description: header file for jpegrw.c
*
* Author: Holden Truman
* Date 11/19/2024
***********************************/

#ifndef JPEGRW_H
#define JPEGRW_H

// a struct to represent a raw image
typedef struct imgRawImage {
	unsigned int numComponents;
	unsigned int width;
    unsigned int height;
	unsigned char* lpData;
} imgRawImage;

// reads in jpeg - allocated memory in imgRawImage - to be freed by caller
imgRawImage* loadJpegImageFile(const char* fname);

// writes out jpeg
int storeJpegImageFile(const imgRawImage* img, const char* lpFilename);

// A few functions to manage raw images
imgRawImage* initRawImage(unsigned int width, unsigned int height);

void freeRawImage(imgRawImage* img);


void setImageCOLOR(imgRawImage* image,unsigned int rgb);

void setImageRGB(imgRawImage* image,unsigned char red,unsigned char green,
							 unsigned char blue);

void setPixelRGB(imgRawImage* image, unsigned int x, unsigned int y,
							 unsigned char red,unsigned char green,
							 unsigned char blue);
							 
void setPixelCOLOR(imgRawImage* image, unsigned int x, unsigned int y, unsigned int rgb);


#endif  /* Compile guard */
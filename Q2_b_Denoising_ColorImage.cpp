// --------------------------------------------------------------------------------------------------
// EE569 Homework Assignment #1
// Date: January 22, 2019
// Name: Suchismita Sahu
// ID: 7688176370
// email: suchisms@usc.edu
// --------------------------------------------------------------------------------------------------

#include<stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <numeric>
#include <algorithm>

using namespace std;

//Functions to find the sum of the array
//For 5*5 Array
float arraysum5(float a[5][5])
{
    float initial_sum  = 0.0;
    for (int i=0 ; i<5 ; i++){
        for(int j=0; j<5; j++){
            initial_sum+=a[i][j];
        }
    }
    return initial_sum;
}

// Median Filter Function 5*5 Filter
void medianfilter(unsigned char image[260][260][3], unsigned char output[256][256][3] ){
    int half = 12;
    int array[25] = {0};
    int count =0;
    int k;

    //Iterating through the image
    for(int chl = 0; chl < 3; chl++){
        for(int i=2; i< 258; i++){
            for(int j=2; j< 258; j++){
                count =0;
                for(int h = -2; h < 3; h++){
                    for(int g = -2; g<3; g++){
                        array[count++]=  (int) image[i+h][j+g][chl];
                    }

                }
                k = 25;
                sort(array, array + k);
                output[i-2][j-2][chl] = (unsigned char) array[half];
            }

        }
    }
}


//Function for Gaussian Denoising Convolution Operation
unsigned char convolveFive(int i, int j, float filter[5][5], unsigned char image[260][260][3], float arraysum, int ch) {
    float sum = 0;
    for (int m = -2; m < 3; m++){
        for (int n = -2; n < 3; n++){
            sum += filter[m+2][n+2]*image[i+m+2][j+n+2][ch];
        }
    }
    //cout<<arraysum<<endl;
    return(unsigned char)(sum/arraysum);
}


int main(int argc, char *argv[]) {

    // Define file pointer and variables
    FILE *file;
    int BytesPerPixel;
    int Size = 256;
    int length = 256;
    int width = 256;

    // Check for proper syntax
    if (argc < 3) {
        cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
        cout << "program_name input_image.raw output_image.raw [BytesPerPixel = 1] [Size = 256]" << endl;
        return 0;
    }

    // Check if image is greyscale or color
    if (argc < 4) {
        BytesPerPixel = 3; // default is grey image
    } else {
        BytesPerPixel = atoi(argv[3]);
        // Check if size is specified
        if (argc >= 5) {
            Size = atoi(argv[4]);
        }
    }

    // Allocate image data array
    unsigned char Imagedata[length][width][BytesPerPixel];

    // Read image (filename specified by first argument) into image data matrix
    if (!(file = fopen(argv[1], "rb"))) {
        cout << "Cannot open file: " << argv[1] << endl;
        exit(1);
    }

    fread(Imagedata, sizeof(unsigned char), length * width * BytesPerPixel, file);
    fclose(file);



    // DENOISING COLOR IMAGE USING GAUSSIAN FILTER AND SUBSEQUENT MEDIAN FILTER

    // Allocating New Image Data Arrays with Boundary Extensions :
    // For 5*5 Filter
    unsigned char NewImagedataFive[260][260][3];

    // Creating New Image Data Array by mirroring the second and third last rows and columns to create length+4*width+4 Bayer Array
    for (int chl = 0; chl < 3; chl++) {
        for (int i = 1; i < 3 + width; i++) { // Rows added to top and bottom
            NewImagedataFive[0][i + 1][chl] = Imagedata[2][i - 1][chl];
            NewImagedataFive[1][i + 1][chl] = Imagedata[1][i - 1][chl];
            NewImagedataFive[length + 3][i][chl] = Imagedata[length - 3][i - 1][chl];
            NewImagedataFive[length + 2][i][chl] = Imagedata[length - 2][i - 1][chl];
        }

        for (int i = 1; i < 3 + length; i++) { // Columns added to right and left
            NewImagedataFive[i + 1][0][chl] = Imagedata[i - 1][2][chl];
            NewImagedataFive[i + 1][1][chl] = Imagedata[i - 1][1][chl];
            NewImagedataFive[i][width + 3][chl] = Imagedata[i - 1][width - 3][chl];
            NewImagedataFive[i][width + 2][chl] = Imagedata[i - 1][width - 2][chl];
        }

        for (int i = 2; i < length + 3; i++) { // Actual Image copy
            for (int j = 2; j < width + 3; j++) {
                NewImagedataFive[i][j][chl] = Imagedata[i - 2][j - 2][chl];
            }
        }

        // Corner elements
        //Corner 1

        NewImagedataFive[0][0][chl] = NewImagedataFive[0][4][chl];
        NewImagedataFive[0][1][chl] = NewImagedataFive[0][3][chl];
        NewImagedataFive[1][0][chl] = NewImagedataFive[1][4][chl];
        NewImagedataFive[1][1][chl] = NewImagedataFive[1][3][chl];

        //Corner 2

        NewImagedataFive[0][width + 3][chl] = NewImagedataFive[0][width - 3][chl];
        NewImagedataFive[0][width + 2][chl] = NewImagedataFive[0][width - 2][chl];
        NewImagedataFive[1][width + 3][chl] = NewImagedataFive[1][width - 3][chl];
        NewImagedataFive[1][width + 2][chl] = NewImagedataFive[1][width - 2][chl];

        // Corner 3

        NewImagedataFive[length + 3][0][chl] = NewImagedataFive[length - 1][0][chl];
        NewImagedataFive[length + 2][0][chl] = NewImagedataFive[length][0][chl];
        NewImagedataFive[length + 3][1][chl] = NewImagedataFive[length + 1][1][chl];
        NewImagedataFive[length + 2][1][chl] = NewImagedataFive[length][1][chl];

        //Corner 4
        NewImagedataFive[length + 3][width + 3][chl] = NewImagedataFive[length + 3][width - 1][chl];
        NewImagedataFive[length + 3][width + 2][chl] = NewImagedataFive[length + 3][width][chl];
        NewImagedataFive[length + 2][width + 3][chl] = NewImagedataFive[length + 2][width - 1][chl];
        NewImagedataFive[length + 3][width + 2][chl] = NewImagedataFive[length + 3][width][chl];

    }

    // Defining the Gaussian Filters for Sigma 1:

    float GaussianFive[5][5] = {{0.003765, 0.015019, 0.023792, 0.015019, 0.003765},
                                {0.015019, 0.059912, 0.094907, 0.059912, 0.015019},
                                {0.023792, 0.094907, 0.150342, 0.094907, 0.023792},
                                {0.015019, 0.059912, 0.094907, 0.059912, 0.015019},
                                {0.003765, 0.015019, 0.023792, 0.015019, 0.003765}};



    // Allocating 3D Intermediate Output Image Data Arrays
    unsigned char OutputImageGaussian5[length][width][3];

    //Implementing Denoising by Gaussian Filter (5*5)
    float sumGaussianFive = arraysum5(GaussianFive);
    for (int i = 0; i < length; i++) {
        for (int j = 0; j < width; j++) {
            OutputImageGaussian5[i][j][0] = convolveFive(i, j, GaussianFive, NewImagedataFive, sumGaussianFive, 0);
            OutputImageGaussian5[i][j][1] = convolveFive(i, j, GaussianFive, NewImagedataFive, sumGaussianFive, 1);
            OutputImageGaussian5[i][j][2] = convolveFive(i, j, GaussianFive, NewImagedataFive, sumGaussianFive, 2);
        }
    }


    // Allocating 3D Output Image Data Arrays
    unsigned char OutputImageMedian5[256][256][3];

    // Implementing Denoising by Median Filter
//    for (int i = 0; i < length; i++) {
//        for (int j = 0; j < width; j++) {
//            OutputImageMedian5[i][j][0] = medianfilter(NewImagedataFive, OutputImageMedian5,0);
//            OutputImageMedian5[i][j][1] = medianfilter(NewImagedataFive, OutputImageMedian5,1);
//            OutputImageMedian5[i][j][2] = medianfilter(NewImagedataFive, OutputImageMedian5,2);
//        }
//    }
    medianfilter(NewImagedataFive, OutputImageMedian5);

    // Write image data (filename specified by second argument) from image data matrix

    if (!(file = fopen(argv[2], "wb"))) {
        cout << "Cannot open file: " << argv[2] << endl;
        exit(1);
    }

    fwrite(OutputImageMedian5, sizeof(unsigned char), length*width*3, file);
    fclose(file);

//    if (!(file = fopen(argv[2], "wb"))) {
//        cout << "Cannot open file: " << argv[2] << endl;
//        exit(1);
//    }
//
//    fwrite(OutputImageGaussian5, sizeof(unsigned char), width * length * 1, file);
//    fclose(file);

    return 0;

}

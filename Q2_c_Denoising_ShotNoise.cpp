// --------------------------------------------------------------------------------------------------
// EE569 Homework Assignment #1
// Date: January 22, 2019
// Name: Suchismita Sahu
// ID: 7688176370
// email: suchisms@usc.edu
// --------------------------------------------------------------------------------------------------

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <fstream>

using namespace std;

//Function to Find Sum of Array
//For 5*5 Array
float arraysum5(float a[5][5])
{
    float initial_sum  = 0.0;
    for (int i=0 ; i<5 ; i++){
        for(int j=0; j<5; j++){
            initial_sum += a[i][j];
        }
    }
    return initial_sum;
}

//Function for Denoising Convolution Operation
// For Gaussian Filter of 5*5
unsigned char convolveFiveGaussian(int i, int j, float filter[5][5], unsigned char image[260][260][1], float arraysum){
    float sum = 0;
    for (int m = -2; m < 3; m++){
        for (int n = -2; n < 3; n++){
            sum += filter[m+2][n+2]*image[i+m+2][j+n+2][0];
        }
    }
    //cout<<arraysum<<endl;
    return(unsigned char)(sum/arraysum);
}

int main(int argc, char *argv[]) {

    // Define file pointer and variables
    FILE *file;
    int BytesPerPixel=1;
    int Size = 256;
    int length = 256;
    int width = 256;

    // Check for proper syntax
    if (argc < 3){
        cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
        cout << "program_name input_image.raw output_image.raw [BytesPerPixel = 1] [Size = 256]" << endl;
        return 0;
    }

    // Check if image is grayscale or color
    if (argc < 4){
        BytesPerPixel = 1; // default is grey image
    }
    else {
        BytesPerPixel = atoi(argv[3]);
        // Check if size is specified
        if (argc >= 5){
            Size = atoi(argv[4]);
        }
    }


    // Allocate image data array
    unsigned char Imagedata[Size][Size][BytesPerPixel];

    // Read image (filename specified by first argument) into image data matrix
    if (!(file=fopen(argv[1],"rb"))) {
        cout << "Cannot open file: " << argv[1] <<endl;
        exit(1);
    }

    fread(Imagedata, sizeof(unsigned char), Size*Size*BytesPerPixel, file);
    fclose(file);

    // Allocating 3D Transform Image Data Array
    unsigned char TransformImage[Size][Size][1];

    // DENOISING SHOT NOISE

    // Taking Anscombe Transform of the Entire Image Pixel by Pixel
    for(int i = 0; i < Size; i++){
        for(int j = 0; j < Size; j++){
            TransformImage[i][j][0] = (unsigned char)(2*pow((Imagedata[i][j][0] + 0.375), 0.5));
        }
    }

    //Implementing Boundary Extension on the Transform Image
    // 2. For 5*5 Filter
    unsigned char NewImagedataFive[260][260][1];

    // Creating New Image Data Array by mirroring the second and third last rows and columns to create length+4*width+4 Bayer Array
    for(int i = 1; i < 3 + width; i++){ // Rows added to top and bottom
        NewImagedataFive[0][i+1][0] = TransformImage[2][i-1][0];
        NewImagedataFive[1][i+1][0] = TransformImage[1][i-1][0];
        NewImagedataFive[length+3][i][0] = TransformImage[length-3][i-1][0];
        NewImagedataFive[length+2][i][0] = TransformImage[length-2][i-1][0];
    }

    for(int i = 1; i < 3 + length; i++){ // Columns added to right and left
        NewImagedataFive[i+1][0][0] = TransformImage[i-1][2][0];
        NewImagedataFive[i+1][1][0] = TransformImage[i-1][1][0];
        NewImagedataFive[i][width+3][0] = TransformImage[i-1][width-3][0];
        NewImagedataFive[i][width+2][0] = TransformImage[i-1][width-2][0];
    }

    for(int i = 2; i < length + 3 ; i++) { // Actual Image copy
        for (int j = 2; j < width + 3; j++) {
            NewImagedataFive[i][j][0] = TransformImage[i-2][j-2][0];
        }
    }

    // Corner elements
    //Corner 1

    NewImagedataFive[0][0][0]=NewImagedataFive[0][4][0];
    NewImagedataFive[0][1][0]=NewImagedataFive[0][3][0];
    NewImagedataFive[1][0][0]=NewImagedataFive[1][4][0];
    NewImagedataFive[1][1][0]=NewImagedataFive[1][3][0];

    //Corner 2

    NewImagedataFive[0][width+3][0]=NewImagedataFive[0][width-3][0];
    NewImagedataFive[0][width+2][0]=NewImagedataFive[0][width-2][0];
    NewImagedataFive[1][width+3][0]=NewImagedataFive[1][width-3][0];
    NewImagedataFive[1][width+2][0]=NewImagedataFive[1][width-2][0];

    // Corner 3

    NewImagedataFive[length+3][0][0]= NewImagedataFive[length-1][0][0];
    NewImagedataFive[length+2][0][0]= NewImagedataFive[length][0][0];
    NewImagedataFive[length+3][1][0]= NewImagedataFive[length+1][1][0];
    NewImagedataFive[length+2][1][0]= NewImagedataFive[length][1][0];

    //Corner 4
    NewImagedataFive[length+3][width+3][0] = NewImagedataFive[length+3][width-1][0];
    NewImagedataFive[length+3][width+2][0] = NewImagedataFive[length+3][width][0];
    NewImagedataFive[length+2][width+3][0] = NewImagedataFive[length+2][width-1][0];
    NewImagedataFive[length+3][width+2][0] = NewImagedataFive[length+3][width][0];

    // Implementing Gaussian Filtering on the Boundary Extended Anscombe Transformed Image

    // Defining the Gaussian Filters for Sigma 1 and Sigma 2:
    float GaussianFive[5][5] = {{0.003765, 0.015019, 0.023792, 0.015019, 0.003765}, {0.015019, 0.059912, 0.094907, 0.059912, 0.015019}, {0.023792, 0.094907, 0.150342, 0.094907, 0.023792}, {0.015019, 0.059912, 0.094907, 0.059912, 0.015019}, {0.003765, 0.015019, 0.023792, 0.015019, 0.003765}};
    float GaussianFive2[5][5] = {{0.023528, 0.033969, 0.038393, 0.033969, 0.023528}, {0.033969, 0.049045, 0.055432, 0.049045, 0.033969}, {0.038393, 0.055432, 0.062651, 0.055432, 0.038393}, {0.033969, 0.049045, 0.055432, 0.049045, 0.033969}, {0.023528, 0.033969, 0.038393, 0.033969, 0.023528}};

    //Sum of the Gaussian Kernels
    float sumGaussianFive = arraysum5(GaussianFive);
    float sumGaussianFive2 = arraysum5(GaussianFive2);

    // Allocating 3D Output Image Data Array
    unsigned char GaussianOutputImage[Size][Size][1];
    unsigned char GaussianOutputImage2[Size][Size][1];

    // Denoising Using 5*5 Gaussian Filter (Sigma 1)
    for(int i = 0; i < length ; i++) {
        for (int j = 0; j < width; j++) {
            GaussianOutputImage[i][j][0] = convolveFiveGaussian(i, j, GaussianFive, NewImagedataFive, sumGaussianFive);
        }
    }

    // Denoising Using 5*5 Gaussian Filter (Sigma 2)
    for(int i = 0; i < length ; i++) {
        for (int j = 0; j < width; j++) {
            GaussianOutputImage2[i][j][0] = convolveFiveGaussian(i, j, GaussianFive, NewImagedataFive, sumGaussianFive2);
        }
    }


    // Allocating 3D Output Image Data Array
    unsigned char OutputImageSigma1_Unbiased[Size][Size][1];
    unsigned char OutputImageSigma2_Unbiased[Size][Size][1];
    unsigned char OutputImageSigma1_Biased[Size][Size][1];
    unsigned char OutputImageSigma2_Biased[Size][Size][1];


    //Implementing Inverse Anscombe Transform

    // Biased - Sigma 1
    for(int i = 0; i < Size; i++){
        for(int j = 0; j < Size; j++){
            OutputImageSigma1_Biased[i][j][0] = (unsigned char)(pow((GaussianOutputImage[i][j][0]/2.0), 2) - 0.375);
        }
    }

    //Biased - Sigma 2
    for(int i = 0; i < Size; i++){
        for(int j = 0; j < Size; j++){
            OutputImageSigma2_Biased[i][j][0] = (unsigned char)(pow((GaussianOutputImage2[i][j][0]/2.0), 2) - 0.375);
        }
    }

    //Unbiased - Sigma 1
    for(int i = 0; i < Size; i++){
        for(int j = 0; j < Size; j++){
            OutputImageSigma1_Unbiased[i][j][0] = (unsigned char)(pow((GaussianOutputImage[i][j][0]/2.0), 2) - 0.125);
        }
    }

    //Unbiased - Sigma 1
    for(int i = 0; i < Size; i++){
        for(int j = 0; j < Size; j++){
            OutputImageSigma2_Unbiased[i][j][0] = (unsigned char)(pow((GaussianOutputImage2[i][j][0]/2.0), 2) - 0.125);
        }
    }



    // Write image data (filename specified by second argument) from image data matrix

    if (!(file=fopen(argv[2],"wb"))) {
        cout << "Cannot open file: " << argv[2] << endl;
        exit(1);
    }

    fwrite(OutputImageSigma2_Unbiased, sizeof(unsigned char), Size*Size*1, file);
    fclose(file);

//    if (!(file=fopen(argv[2],"wb"))) {
//        cout << "Cannot open file: " << argv[2] << endl;
//        exit(1);
//    }
//
//    fwrite(OutputImageSigma2_Biased, sizeof(unsigned char), Size*Size*1, file);
//    fclose(file);
//
//    if (!(file=fopen(argv[2],"wb"))) {
//        cout << "Cannot open file: " << argv[2] << endl;
//        exit(1);
//    }
//
//    fwrite(OutputImageSigma1_Unbiased, sizeof(unsigned char), Size*Size*1, file);
//    fclose(file);
//
//    if (!(file=fopen(argv[2],"wb"))) {
//        cout << "Cannot open file: " << argv[2] << endl;
//        exit(1);
//    }
//
//    fwrite(OutputImageSigma2_Unbiased, sizeof(unsigned char), Size*Size*1, file);
//    fclose(file);

    return 0;
}

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

using namespace std;

// Function to use the Discrete Bilateral Filter Equation for Denoising

//For 3*3 Filter size
unsigned char bilateral_denoising3(int i, int j,unsigned char image[258][258][1]) {
    double pixel = 0.0;
    double sum = 0.0;
    double w;
    double sig_s = 75;
    double sig_c = 10;

    for (int k = -1; k < 2; k++) {
        for (int l = -1; l < 2; l++) {
            w = exp(-(pow(k, 2) + pow(l, 2)) / (2 * sig_c * sig_c) -(pow((image[i][j][0] - image[i + k][j + l][0]), 2) / (2 * sig_s * sig_s)));
            pixel += (image[i + k][j + l][0] * w);
            sum += w;
        }
    }
    return (unsigned char) floor(pixel / sum);
}

unsigned char bilateral_denoising5(int i, int j,unsigned char image[260][260][1]){
    double pixel = 0.0;
    double sum = 0.0;
    double w ;
    double sig_s = 75;
    double sig_c = 10;
    for(int k= -2; k < 3; k++){
        for(int l=-2; l < 3; l++){
            w = exp(-(pow(k,2) + pow(l,2))/(2*sig_c*sig_c) - (pow((image[i][j][0] - image[i+k][j+l][0]),2)/(2*sig_s * sig_s)));
            pixel += (image[i + k][j + l][0] * w);
            sum += w;
        }
    }
    return (unsigned char)floor(pixel/sum);
}

unsigned char bilateral_denoising7(int i, int j,unsigned char image[262][262][1]) {
    double pixel = 0.0;
    double sum = 0.0;
    double w;
    double sig_s = 75;
    double sig_c = 10;

    for (int k = -3; k < 4; k++) {
        for (int l = -3; l < 4; l++) {
            w = exp(-(pow(k, 2) + pow(l, 2)) / (2 * sig_c * sig_c) -(pow((image[i][j][0] - image[i + k][j + l][0]), 2) / (2 * sig_s * sig_s)));
            pixel += (image[i + k][j + l][0] * w);
            sum += w;
        }
    }
    return (unsigned char) floor(pixel / sum);
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

    // Check if image is greyscale or color
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
    unsigned char Imagedata[length][width][BytesPerPixel];

    // Read image (filename specified by first argument) into image data matrix
    if (!(file=fopen(argv[1],"rb"))) {
        cout << "Cannot open file: " << argv[1] <<endl;
        exit(1);
    }

    fread(Imagedata, sizeof(unsigned char), length*width*BytesPerPixel, file);
    fclose(file);



    // DENOISING USING BILATERAL FILTER

    // Allocating New Image Data Arrays with Boundary Extensions :

    // 1. For 3*3 Filter
    unsigned char NewImagedataThree[258][258][1];

    // Creating New Image Data Array by mirroring the second last rows and columns to create length+2*width+2 Bayer Array
    for(int i = 1; i < 1 + width; i++){ // Rows added to top and bottom
        NewImagedataThree[0][i][0] = Imagedata[1][i-1][0];
        NewImagedataThree[length+1][i][0] = Imagedata[length-1][i-1][0];
    }

    for(int i = 1; i < 1 + length; i++){
        NewImagedataThree[i][0][0] = Imagedata[i-1][1][0];
        NewImagedataThree[i][width+1][0] = Imagedata[i-1][width-1][0];
    }

    for(int i = 1; i < length + 1 ; i++) {
        for (int j = 1; j < width + 1; j++) {
            NewImagedataThree[i][j][0] = Imagedata[i-1][j-1][0];
        }
    }

    // Corner elements
    NewImagedataThree[0][0][0]=NewImagedataThree[0][2][0];
    NewImagedataThree[0][width+1][0]=NewImagedataThree[0][width-1][0];
    NewImagedataThree[length+1][0][0]= NewImagedataThree[length+1][2][0];
    NewImagedataThree[length+1][width+1][0] = NewImagedataThree[length+1][width-1][0];


    // 2. For 5*5 Filter
    unsigned char NewImagedataFive[260][260][1];

    // Creating New Image Data Array by mirroring the second and third last rows and columns to create length+4*width+4 Bayer Array
    for(int i = 1; i < 3 + width; i++){ // Rows added to top and bottom
        NewImagedataFive[0][i+1][0] = Imagedata[2][i-1][0];
        NewImagedataFive[1][i+1][0] = Imagedata[1][i-1][0];
        NewImagedataFive[length+3][i][0] = Imagedata[length-3][i-1][0];
        NewImagedataFive[length+2][i][0] = Imagedata[length-2][i-1][0];
    }

    for(int i = 1; i < 3 + length; i++){ // Columns added to right and left
        NewImagedataFive[i+1][0][0] = Imagedata[i-1][2][0];
        NewImagedataFive[i+1][1][0] = Imagedata[i-1][1][0];
        NewImagedataFive[i][width+3][0] = Imagedata[i-1][width-3][0];
        NewImagedataFive[i][width+2][0] = Imagedata[i-1][width-2][0];
    }

    for(int i = 2; i < length + 3 ; i++) { // Actual Image copy
        for (int j = 2; j < width + 3; j++) {
            NewImagedataFive[i][j][0] = Imagedata[i-2][j-2][0];
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


    // 2. For 7*7 Filter
    unsigned char NewImagedataSeven[262][262][1];

    // Creating New Image Data Array by mirroring the second and third last rows and columns to create length+6*width+6 Bayer Array
    for(int i = 1; i < 1 + width; i++){ // Rows added to top and bottom
        NewImagedataSeven[0][i+2][0] = Imagedata[3][i-1][0];
        NewImagedataSeven[1][i+2][0] = Imagedata[2][i-1][0];
        NewImagedataSeven[2][i+2][0] = Imagedata[1][i-1][0];
        NewImagedataSeven[length+5][i+2][0] = Imagedata[length-4][i-1][0];
        NewImagedataSeven[length+4][i+2][0] = Imagedata[length-3][i-1][0];
        NewImagedataSeven[length+3][i+2][0] = Imagedata[length-2][i-1][0];
    }

    for(int i = 1; i < 1 + length; i++){ // Columns added to right and left
        NewImagedataSeven[i+2][0][0] = Imagedata[i-1][3][0];
        NewImagedataSeven[i+2][1][0] = Imagedata[i-1][2][0];
        NewImagedataSeven[i+2][2][0] = Imagedata[i-1][1][0];
        NewImagedataSeven[i+2][width+5][0] = Imagedata[i-1][width-4][0];
        NewImagedataSeven[i+2][width+4][0] = Imagedata[i-1][width-3][0];
        NewImagedataSeven[i+2][width+3][0] = Imagedata[i-1][width-2][0];
    }

    for(int i = 3; i < length + 5 ; i++) { // Actual Image copy
        for (int j = 3; j < width + 5; j++) {
            NewImagedataSeven[i][j][0] = Imagedata[i-3][j-3][0];
        }
    }

    // Corner elements
    //Corner 1

    NewImagedataSeven[0][0][0]=NewImagedataSeven[0][6][0];
    NewImagedataSeven[0][1][0]=NewImagedataSeven[0][5][0];
    NewImagedataSeven[0][2][0]=NewImagedataSeven[0][4][0];
    NewImagedataSeven[1][0][0]=NewImagedataSeven[1][6][0];
    NewImagedataSeven[1][1][0]=NewImagedataSeven[1][5][0];
    NewImagedataSeven[1][2][0]=NewImagedataSeven[1][4][0];
    NewImagedataSeven[2][0][0]=NewImagedataSeven[2][6][0];
    NewImagedataSeven[2][1][0]=NewImagedataSeven[2][5][0];
    NewImagedataSeven[2][2][0]=NewImagedataSeven[2][4][0];


    //Corner 2

    NewImagedataSeven[0][width+5][0]=NewImagedataSeven[0][width-1][0];
    NewImagedataSeven[0][width+4][0]=NewImagedataSeven[0][width][0];
    NewImagedataSeven[0][width+3][0]=NewImagedataSeven[0][width+1][0];
    NewImagedataSeven[1][width+5][0]=NewImagedataSeven[1][width-1][0];
    NewImagedataSeven[1][width+4][0]=NewImagedataSeven[1][width][0];
    NewImagedataSeven[1][width+3][0]=NewImagedataSeven[1][width+1][0];
    NewImagedataSeven[2][width+5][0]=NewImagedataSeven[2][width-1][0];
    NewImagedataSeven[2][width+4][0]=NewImagedataSeven[2][width][0];
    NewImagedataSeven[2][width+3][0]=NewImagedataSeven[2][width+1][0];


    // Corner 3

    NewImagedataSeven[length+5][0][0]= NewImagedataSeven[length+5][6][0];
    NewImagedataSeven[length+4][0][0]= NewImagedataSeven[length+4][6][0];
    NewImagedataSeven[length+3][0][0]= NewImagedataSeven[length+3][6][0];
    NewImagedataSeven[length+5][1][0]= NewImagedataSeven[length+5][5][0];
    NewImagedataSeven[length+4][1][0]= NewImagedataSeven[length+4][5][0];
    NewImagedataSeven[length+3][1][0]= NewImagedataSeven[length+3][5][0];
    NewImagedataSeven[length+5][2][0]= NewImagedataSeven[length+5][4][0];
    NewImagedataSeven[length+4][2][0]= NewImagedataSeven[length+4][4][0];
    NewImagedataSeven[length+3][2][0]= NewImagedataSeven[length+3][4][0];


    //Corner 4

    NewImagedataSeven[length+5][width+5][0] = NewImagedataSeven[length+5][width-1][0];
    NewImagedataSeven[length+4][width+4][0] = NewImagedataSeven[length+4][width][0];
    NewImagedataSeven[length+3][width+3][0] = NewImagedataSeven[length+3][width+1][0];
    NewImagedataSeven[length+5][width+4][0] = NewImagedataSeven[length+5][width][0];
    NewImagedataSeven[length+4][width+5][0] = NewImagedataSeven[length+4][width-1][0];
    NewImagedataSeven[length+5][width+3][0] = NewImagedataSeven[length+5][width+1][0];
    NewImagedataSeven[length+4][width+3][0] = NewImagedataSeven[length+4][width+1][0];
    NewImagedataSeven[length+3][width+4][0] = NewImagedataSeven[length+3][width][0];
    NewImagedataSeven[length+3][width+5][0] = NewImagedataSeven[length+3][width-1][0];

    // Allocating 3D Output Image Data Arrays
    unsigned char OutputImage3[length][width][1];
    unsigned char OutputImage5[length][width][1];
    unsigned char OutputImage7[length][width][1];

    // For 3*3 Filter
    for(int i = 0; i < length; i++){
        for(int j = 0; j < width; j++) {
            OutputImage3[i][j][0] = bilateral_denoising3(i, j, NewImagedataThree);
        }
    }


    // For 5*5 Filter
    for(int i = 0; i < length; i++){
        for(int j = 0; j < width; j++) {
            OutputImage5[i][j][0] = bilateral_denoising5(i, j, NewImagedataFive);
        }
    }

    // For 7*7 Filter
    for(int i = 0; i < length; i++){
        for(int j = 0; j < width; j++) {
            OutputImage7[i][j][0] = bilateral_denoising7(i, j, NewImagedataSeven);
        }
    }

    //PNSR Calculation
    int Max_level = 255;
    double psnr;

    double mse = 0;
    float Sum = 0;
    for (int i = 0;i < 256;i++){
        for (int j = 0;j < 256;j++){
            Sum = (float)(Sum + (pow((OutputImage3[i][j] - Imagedata[i][j]), 2)));
        }
    }

    mse = (1 / (1.0 * 256 * 256)) * Sum;
    psnr = 10 * log10((pow(Max_level, 2)) / (1.0 * mse));

    cout<< "PSNR:"<<psnr<< endl;



    // Write image data (filename specified by second argument) from image data matrix

//    if (!(file=fopen(argv[2],"wb"))) {
//        cout << "Cannot open file: " << argv[2] << endl;
//        exit(1);
//    }
//
//    fwrite(OutputImage3, sizeof(unsigned char), width*length*1, file);
//    fclose(file);


//    if (!(file=fopen(argv[2],"wb"))) {
//        cout << "Cannot open file: " << argv[2] << endl;
//        exit(1);
//    }
//
//    fwrite(OutputImage5, sizeof(unsigned char), width*length*1, file);
//    fclose(file);
//
//
    if (!(file=fopen(argv[2],"wb"))) {
        cout << "Cannot open file: " << argv[2] << endl;
        exit(1);
    }

    fwrite(OutputImage7, sizeof(unsigned char), width*length*1, file);
    fclose(file);


    return 0;
}
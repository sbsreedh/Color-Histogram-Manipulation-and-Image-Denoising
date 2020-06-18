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

using namespace std;

//Functions to find the sum of the array

// For 3*3 Array
float arraysum3(float a[3][3])
{
    float initial_sum  = 0.0;
    for (int i=0 ; i<3 ; i++){
        for(int j=0; j<3; j++){
            initial_sum+=a[i][j];
        }
    }
    return initial_sum;
}

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

//For 7*7 Array
float arraysum7(float a[7][7])
{
    float initial_sum  = 0.0;
    for (int i=0 ; i<7 ; i++){
        for(int j=0; j<7 ; j++){
            initial_sum+=a[i][j];
        }
    }
    return initial_sum;
}

//Functions for Denoising Convolution Operation

// For Linear Filter of 5*5
unsigned char convolveFiveMean(int i, int j, float filter[5][5], unsigned char image[260][260][1]) {
    float sum = 0;
    for (int m = -2; m < 3; m++){
        for (int n = -2; n < 3; n++){
            sum += filter[m+2][n+2]*image[i+m+2][j+n+2][0];
        }
    }
    return(unsigned char)(sum/25);
}

// For Linear Filter of 3*3
unsigned char convolveThreeMean(int i, int j, float filter[3][3], unsigned char image[258][258][1]) {
    float sum = 0;
    for (int m = -1; m < 2; m++){
        for (int n = -1; n < 2; n++){
            sum += filter[m+1][n+1]*image[i+m+1][j+n+1][0];
        }
    }
    return(unsigned char)(sum/9);
}

// For Linear Filter of 7*7
unsigned char convolveSevenMean(int i, int j, float filter[7][7], unsigned char image[262][262][1]) {
    float sum = 0;
    for (int m = -3; m < 4; m++){
        for (int n = -3; n < 4; n++){
            sum += filter[m+3][n+3]*image[i+m+3][j+n+3][0];
        }
    }
    return(unsigned char)(sum/49);
}

// For Gaussian Filter of 5*5
unsigned char convolveFiveGaussian(int i, int j, float filter[5][5], unsigned char image[260][260][1], float arraysum) {
    float sum = 0;
    for (int m = -2; m < 3; m++){
        for (int n = -2; n < 3; n++){
            sum += filter[m+2][n+2]*image[i+m+2][j+n+2][0];
        }
    }
    //cout<<arraysum<<endl;
    return(unsigned char)(sum/arraysum);
}

// For Gaussian Filter of 3*3
unsigned char convolveThreeGaussian(int i, int j, float filter[3][3], unsigned char image[258][258][1], float arraysum) {
    float sum = 0;
    for (int m = -1; m < 2; m++){
        for (int n = -1; n < 2; n++){
            sum += filter[m+1][n+1]*image[i+m+1][j+n+1][0];
        }
    }
    return(unsigned char)(sum/arraysum);
}

// For Gaussian Filter of 7*7
unsigned char convolveSevenGaussian(int i, int j, float filter[7][7], unsigned char image[262][262][1], float arraysum) {
    float sum = 0;
    for (int m = -3; m < 4; m++){
        for (int n = -3; n < 4; n++){
            sum += filter[m+3][n+3]*image[i+m+3][j+n+3][0];
        }
    }
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



    // DENOISING USING LINEAR AND GAUSSIAN FILTERS

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


    // Defining the Linear Filters :
    float Five[5][5] = {{1,1,1,1,1}, {1,1,1,1,1}, {1,1,1,1,1}, {1,1,1,1,1}, {1,1,1,1,1}};
    float Three[3][3] = {{1,1,1}, {1,1,1}, {1,1,1}};
    float Seven[7][7] = {{1,1,1,1,1,1,1}, {1,1,1,1,1,1,1}, {1,1,1,1,1,1,1}, {1,1,1,1,1,1,1},{1,1,1,1,1,1,1}, {1,1,1,1,1,1,1}, {1,1,1,1,1,1,1}};


    // Defining the Gaussian Filters for Sigma 1:

    float GaussianFive[5][5] = {{0.003765, 0.015019, 0.023792, 0.015019, 0.003765}, {0.015019, 0.059912, 0.094907, 0.059912, 0.015019}, {0.023792, 0.094907, 0.150342, 0.094907, 0.023792}, {0.015019, 0.059912, 0.094907, 0.059912, 0.015019}, {0.003765, 0.015019, 0.023792, 0.015019, 0.003765}};
    float GaussianThree[3][3] = {{0.077847, 0.123317, 0.077847}, {0.123317, 0.195346, 0.123317}, {0.077847, 0.123317, 0.077847}};
    float GaussianSeven[7][7] = {{0.000036, 0.000363, 0.001446, 0.002291, 0.001446, 0.000363, 0.000036}, {0.000363, 0.003676, 0.014662, 0.023226, 0.014662, 0.003676, 0.000363}, {0.001446, 0.014662, 0.058488, 0.092651, 0.058488, 0.014662, 0.001446}, {0.002291, 0.023226, 0.092651, 0.146768, 0.092651, 0.023226, 0.002291}, {0.001446, 0.014662, 0.058488, 0.092651, 0.058488, 0.014662, 0.001446}, {0.000363, 0.003676, 0.014662, 0.023226, 0.014662, 0.003676, 0.000363}, {0.000036, 0.000363, 0.001446, 0.002291, 0.001446, 0.000363, 0.000036}};


    float sumGaussianFive = arraysum5(GaussianFive);
    float sumGaussianThree = arraysum3(GaussianThree);
    float sumGaussianSeven = arraysum7(GaussianSeven);

    // Defining the Gaussian Filters for Sigma 2:

    float GaussianFive2[5][5] = {{0.023528, 0.033969, 0.038393, 0.033969, 0.023528}, {0.033969, 0.049045, 0.055432, 0.049045, 0.033969}, {0.038393, 0.055432, 0.062651, 0.055432, 0.038393}, {0.033969, 0.049045, 0.055432, 0.049045, 0.033969}, {0.023528, 0.033969, 0.038393, 0.033969, 0.023528}};
    float GaussianThree2[3][3] = {{0.102059, 0.115349, 0.102059}, {0.115349, 0.130371, 0.115349}, {0.102059, 0.115349, 0.102059}};
    float GaussianSeven2[7][7] = {{0.005084, 0.009377, 0.013539, 0.015302, 0.013539, 0.009377, 0.005084}, {0.009377, 0.017296, 0.024972, 0.028224, 0.024972, 0.017296, 0.009377}, {0.013539, 0.024972, 0.036054, 0.040749, 0.036054, 0.024972, 0.013539}, {0.015302, 0.028224,  0.040749, 0.046056, 0.040749, 0.028224, 0.015302}, {0.013539, 0.024972, 0.036054, 0.040749, 0.036054, 0.024972, 0.013539}, {0.009377, 0.017296, 0.024972, 0.028224, 0.024972, 0.017296, 0.009377}, {0.005084, 0.009377, 0.013539, 0.015302, 0.013539, 0.009377, 0.005084}};

    float sumGaussianFive2 = arraysum5(GaussianFive2);
    float sumGaussianThree2 = arraysum3(GaussianThree2);
    float sumGaussianSeven2 = arraysum7(GaussianSeven2);

    // Allocating 3D Output Image Data Arrays
    unsigned char OutputImageThree[length][width][1];
    unsigned char OutputImageFive[length][width][1];
    unsigned char OutputImageSeven[length][width][1];
    unsigned char OutputImageThreeGaussian[length][width][1];
    unsigned char OutputImageFiveGaussian[length][width][1];
    unsigned char OutputImageSevenGaussian[length][width][1];
    unsigned char OutputImageThreeGaussian2[length][width][1];
    unsigned char OutputImageFiveGaussian2[length][width][1];
    unsigned char OutputImageSevenGaussian2[length][width][1];



    // Denoising Using 3*3 Linear Filter
    for(int i = 0; i < length ; i++) {
        for (int j = 0; j < width; j++) {
            OutputImageThree[i][j][0] = convolveThreeMean(i, j, Three, NewImagedataThree);
        }
    }

    // Denoising Using 3*3 Gaussian Filter
    for(int i = 0; i < length ; i++) {
        for (int j = 0; j < width; j++) {
            OutputImageThreeGaussian[i][j][0] = convolveThreeGaussian(i, j, GaussianThree, NewImagedataThree, sumGaussianThree);
        }
    }


    // Denoising Using 5*5 Linear Filter
    for(int i = 0; i < length ; i++) {
        for (int j = 0; j < width; j++) {
            OutputImageFive[i][j][0] = convolveFiveMean(i, j, Five, NewImagedataFive);
        }
    }

    // Denoising Using 5*5 Gaussian Filter
    for(int i = 0; i < length ; i++) {
        for (int j = 0; j < width; j++) {
            OutputImageFiveGaussian[i][j][0] = convolveFiveGaussian(i, j, GaussianFive, NewImagedataFive, sumGaussianFive);
        }
    }

    // Denoising Using 7*7 Linear Filter
    for(int i = 0; i < length ; i++) {
        for (int j = 0; j < width; j++) {
            OutputImageSeven[i][j][0] = convolveSevenMean(i, j, Seven, NewImagedataSeven);
        }
    }

    // Denoising Using 7*7 Gaussian Filter
    for(int i = 0; i < length ; i++) {
        for (int j = 0; j < width; j++) {
            OutputImageSevenGaussian[i][j][0] = convolveSevenGaussian(i, j, GaussianSeven, NewImagedataSeven,sumGaussianSeven);
        }
    }

    // Denoising Using 3*3 Gaussian Filter, Sigma 2
    for(int i = 0; i < length ; i++) {
        for (int j = 0; j < width; j++) {
            OutputImageThreeGaussian2[i][j][0] = convolveThreeGaussian(i, j, GaussianThree2, NewImagedataThree, sumGaussianThree2);
        }
    }

    // Denoising Using 5*5 Gaussian Filter, Sigma 2
    for(int i = 0; i < length ; i++) {
        for (int j = 0; j < width; j++) {
            OutputImageFiveGaussian2[i][j][0] = convolveFiveGaussian(i, j, GaussianFive2, NewImagedataFive, sumGaussianFive2);
        }
    }

    // Denoising Using 7*7 Gaussian Filter, Sigma 2
    for(int i = 0; i < length ; i++) {
        for (int j = 0; j < width; j++) {
            OutputImageSevenGaussian2[i][j][0] = convolveSevenGaussian(i, j, GaussianSeven2, NewImagedataSeven,sumGaussianSeven2);
        }
    }

    //PNSR Calculation
    int Max_level = 255;
    double psnr;

    double mse = 0;
    float Sum = 0;
    for (int i = 0;i < 256;i++){
        for (int j = 0;j < 256;j++){
            Sum = (float)(Sum + (pow((OutputImageThree[i][j] - Imagedata[i][j]), 2)));
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
//    fwrite(OutputImageThree, sizeof(unsigned char), 256*256*1, file);
//    fclose(file);

//    if (!(file=fopen(argv[2],"wb"))) {
//        cout << "Cannot open file: " << argv[2] << endl;
//        exit(1);
//    }
//
//    fwrite(OutputImageFive, sizeof(unsigned char), width*length*1, file);
//    fclose(file);
//
//    if (!(file=fopen(argv[2],"wb"))) {
//        cout << "Cannot open file: " << argv[2] << endl;
//        exit(1);
//    }
//
//    fwrite(OutputImageSeven, sizeof(unsigned char), width*length*1, file);
//    fclose(file);
//
//    if (!(file=fopen(argv[2],"wb"))) {
//        cout << "Cannot open file: " << argv[2] << endl;
//        exit(1);
//    }
//
//    fwrite(OutputImageThreeGaussian, sizeof(unsigned char), width*length*1, file);
//    fclose(file);
//
//    if (!(file=fopen(argv[2],"wb"))) {
//        cout << "Cannot open file: " << argv[2] << endl;
//        exit(1);
//    }
//
//    fwrite(OutputImageFiveGaussian, sizeof(unsigned char), width*length*1, file);
//    fclose(file);
//
//    if (!(file=fopen(argv[2],"wb"))) {
//        cout << "Cannot open file: " << argv[2] << endl;
//        exit(1);
//    }
//
//    fwrite(OutputImageSevenGaussian, sizeof(unsigned char), width*length*1, file);
//    fclose(file);
//
//    if (!(file=fopen(argv[2],"wb"))) {
//        cout << "Cannot open file: " << argv[2] << endl;
//        exit(1);
//    }
//
//    fwrite(OutputImageThreeGaussian2, sizeof(unsigned char), width*length*1, file);
//    fclose(file);
//
//    if (!(file=fopen(argv[2],"wb"))) {
//        cout << "Cannot open file: " << argv[2] << endl;
//        exit(1);
//    }
//
//    fwrite(OutputImageFiveGaussian2, sizeof(unsigned char), width*length*1, file);
//    fclose(file);
//
    if (!(file=fopen(argv[2],"wb"))) {
        cout << "Cannot open file: " << argv[2] << endl;
        exit(1);
    }

    fwrite(OutputImageSevenGaussian2, sizeof(unsigned char), width*length*1, file);
    fclose(file);

    return 0;
}
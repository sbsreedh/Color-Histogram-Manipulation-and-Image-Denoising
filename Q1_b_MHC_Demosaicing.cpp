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

unsigned char convolve(int i, int j, float filter[5][5], unsigned char image[304][394][1]) {
    float sum = 0;
    for (int m = -2; m < 3; m++){
        for (int n = -2; n < 3; n++){
            sum += filter[m+2][n+2]*image[i+m+2][j+n+2][0];
        }
    }
    return(unsigned char)(sum/8);
}

int main(int argc, char *argv[]) {

    // Define file pointer and variables
    FILE *file;
    int BytesPerPixel=1;
    int length = 300;
    int width = 390;
    cout << "here" << length;
    int Size = 256;

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

    // Malvar-He-Cutler (MHC) Demosaicing

    // Allocating New Image Data Array
    unsigned char NewImagedata[304][394][1];

    // Creating New Image Data Array by mirroring the second and third last rows and columns to create length+4*width+4 Bayer Array
    for(int i = 1; i < 3 + width; i++){ // Rows added to top and bottom
        NewImagedata[0][i+1][0] = Imagedata[2][i-1][0];
        NewImagedata[1][i+1][0] = Imagedata[1][i-1][0];
        NewImagedata[length+3][i][0] = Imagedata[length-3][i-1][0];
        NewImagedata[length+2][i][0] = Imagedata[length-2][i-1][0];
    }

    for(int i = 1; i < 3 + length; i++){ // Columns added to right and left
        NewImagedata[i+1][0][0] = Imagedata[i-1][2][0];
        NewImagedata[i+1][1][0] = Imagedata[i-1][1][0];
        NewImagedata[i][width+3][0] = Imagedata[i-1][width-3][0];
        NewImagedata[i][width+2][0] = Imagedata[i-1][width-2][0];
    }

    for(int i = 2; i < length + 3 ; i++) { // Actual Image copy
        for (int j = 2; j < width + 3; j++) {
            NewImagedata[i][j][0] = Imagedata[i-2][j-2][0];
        }
    }

    // Corner elements
    //Corner 1

    NewImagedata[0][0][0]=NewImagedata[0][4][0];
    NewImagedata[0][1][0]=NewImagedata[0][3][0];
    NewImagedata[1][0][0]=NewImagedata[1][4][0];
    NewImagedata[1][1][0]=NewImagedata[1][3][0];

    //Corner 2

    NewImagedata[0][width+3][0]=NewImagedata[0][width-3][0];
    NewImagedata[0][width+2][0]=NewImagedata[0][width-2][0];
    NewImagedata[1][width+3][0]=NewImagedata[1][width-3][0];
    NewImagedata[1][width+2][0]=NewImagedata[1][width-2][0];

    // Corner 3

    NewImagedata[length+3][0][0]= NewImagedata[length-1][0][0];
    NewImagedata[length+2][0][0]= NewImagedata[length][0][0];
    NewImagedata[length+3][1][0]= NewImagedata[length+1][1][0];
    NewImagedata[length+2][1][0]= NewImagedata[length][1][0];

    //Corner 4
    NewImagedata[length+3][width+3][0] = NewImagedata[length+3][width-1][0];
    NewImagedata[length+3][width+2][0] = NewImagedata[length+3][width][0];
    NewImagedata[length+2][width+3][0] = NewImagedata[length+2][width-1][0];
    NewImagedata[length+3][width+2][0] = NewImagedata[length+3][width][0];



    // Defining the 8 Filters
    float GatRfilter[5][5] = {{0,0,-1,0,0}, {0,0,2,0,0}, {-1,2,4,2,-1}, {0,0,2,0,0}, {0,0,-1,0,0}};
    float GatBfilter[5][5] = {{0,0,-1,0,0}, {0,0,2,0,0}, {-1,2,4,2,-1}, {0,0,2,0,0}, {0,0,-1,0,0}};
    float RatGinRBfilter[5][5] = {{0,0,0.5,0,0}, {0,-1,0,-1,0}, {-1,4,5,4,-1}, {0,-1,0,-1,0}, {0,0,0.5,0,0}};
    float RatGinBRfilter[5][5] = {{0,0,-1,0,0}, {0,-1,4,-1,0}, {0.5,0,5,0,0.5}, {0,-1,4,-1,0}, {0,0,-1,0,0}};
    float RatBinBBfilter[5][5] = {{0,0,-1.5,0,0}, {0,2,0,2,0}, {-1.5,0,6,0,-1.5}, {0,2,0,2,0}, {0,0,-1.5,0,0}};
    float BatGinBRfilter[5][5] = {{0,0,0.5,0,0}, {0,-1,0,-1,0}, {-1,4,5,4,-1}, {0,-1,0,-1,0}, {0,0,0.5,0,0}};
    float BatGinRBfilter[5][5] = {{0,0,-1,0,0}, {0,-1,4,-1,0}, {0.5,0,5,0,0.5}, {0,-1,4,-1,0}, {0,0,-1,0,0}};
    float BatRinRRfilter[5][5] = {{0,0,-1.5,0,0}, {0,2,0,2,0}, {-1.5,0,6,0,-1.5}, {0,2,0,2,0}, {0,0,-1.5,0,0}};



    // Allocating 3D Output Image Data Array
    unsigned char OutputImage[length][width][3];

    // // MHC Convolution steps and Extracting the R G B values for each pixel in the New Image Data Array
    for(int i = 0; i < length ; i++){
        for(int j = 0; j < width ; j++){

            if((i % 2 == 0 && j % 2 == 0)){ //i and j both even
                OutputImage[i][j][0] = convolve(i, j, RatGinRBfilter, NewImagedata);
                OutputImage[i][j][1] = Imagedata[i][j][0];
                OutputImage[i][j][2] = convolve(i, j, BatGinRBfilter, NewImagedata);
            }
            else if(i % 2 ==0 && j % 2 != 0){ //i even j odd
                OutputImage[i][j][0] = Imagedata[i][j][0];
                OutputImage[i][j][1] = convolve(i, j, GatRfilter, NewImagedata);
                OutputImage[i][j][2] = convolve(i, j, BatRinRRfilter, NewImagedata);
            }
            else if(i % 2 != 0 && j % 2 == 0){ //i odd j even
                OutputImage[i][j][0] = convolve(i, j, RatBinBBfilter, NewImagedata);
                OutputImage[i][j][1] = convolve(i, j, GatBfilter, NewImagedata);
                OutputImage[i][j][2] = Imagedata[i][j][0];
            }

            else if(i % 2 != 0 && j % 2 != 0){ //i and j both odd
                OutputImage[i][j][0] = convolve(i, j, RatGinBRfilter, NewImagedata);
                OutputImage[i][j][1] = Imagedata[i][j][0];
                OutputImage[i][j][2] = convolve(i, j, BatGinBRfilter, NewImagedata);

            }

        }

    }

    // Write image data (filename specified by second argument) from image data matrix

    if (!(file=fopen(argv[2],"wb"))) {
        cout << "Cannot open file: " << argv[2] << endl;
        exit(1);
    }

    fwrite(OutputImage, sizeof(unsigned char), width*length*3, file);
    fclose(file);

    return 0;
}

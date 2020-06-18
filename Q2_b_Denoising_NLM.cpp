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

// Functions to Calculate the Non-Local Mean Filtering parameters

//Function to calculate G
float gaussian(int n1, int n2){
    float G;
    float a = 1.0;
    G = (float)(exp(-(pow(n1, 2) + pow(n2, 2)) / (2 * a * a)) / (sqrt(2 * 3.14) * a));
    return G;
}

// NLM Filter Function
//Returns New pixel at the location
float NLM(int i, int j, unsigned char image[282][282][1]) {
    float denom_sum = 0.0;
    float h = 10.0;
    float total_f = 0.0;

    for(int a = i-10; a < i+10; i++) {
        for (int b = j-10; b < j+10; b++) {
            float sum_f = 0.0;
            for (int k = -3; k < 4; k++) {
                for (int l = -3; l < 4; l++) {
                    sum_f += (pow((image[i+k][j+l] - image[a+k][b+l]), 2)* gaussian(k, l));
                }
            }
            total_f += (image[i][j][0]) * exp(-sum_f/(h*h));
            denom_sum+=exp(-sum_f/(h*h));
        }
    }
    return (total_f/denom_sum);
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

    // Allocating New Image Data Arrays with Boundary Extensions :

    // Considering a window of N' = M' = 21 for the Image pixel, we have to extend the boundary of the image by 10 rows and 10 columns on each side
    // Now, for considering N = 7*7 Neighborhood of the first pixel of first window, we need to extend the boundaryof the image by 3 rows and columns
    // Thus, total boundary extension is to be done by 13 rows and columns on each side
    // 256 + 26 = 282

    unsigned char NewImagedata[282][282][1];

    //Implementing Boundary Extension

    // Adding rows and Columns

    for(int j=0; j <13; j++){
        for(int i=13; i< width + 13; i++){
            NewImagedata[length+j+13][i][0] = Imagedata[length-j-2][i-13][0];
        }
    }

    for(int i=0; i<13; i++){
        for(int j=13; j< length + 13; j++){
            NewImagedata[j][width+13+i][0] = Imagedata[j-13][width-i-2][0];
        }
    }

    for(int j=0; j<13; j++){
        for(int i=13; i< width + 13; i++){
            NewImagedata[j][i][0] = Imagedata[13-j][i-13][0];
        }
    }

    for(int i=0; i<13; i++){
        for(int j=13; j< length + 13; j++){
            NewImagedata[j][i][0] = Imagedata[j-13][13-i][0];
        }
    }


    // Corner Elements

    for(int i=0; i< 13; i++){
        for(int j= width + 13; j< width+26; j++){
            NewImagedata[i][j][0] = NewImagedata[i][width + 24-(j-width)][0]; }
    }

    for(int i=0; i< 13; i++){
        for(int j=0; j < 13; j++){
            NewImagedata[i][j][0] = NewImagedata[i][26-j][0]; }
    }




    for(int i= length + 13; i< length + 26; i++){
        for(int j= width+13; j< width+26; j++){
            NewImagedata[i][j][0] = NewImagedata[i][width + 24-(j-width)][0]; }
    }

    for(int i= length+13; i< length+26; i++){
        for(int j=0; j< 13; j++){
            NewImagedata[i][j][0] = NewImagedata[i][26-j][0]; }
    }

    for(int j=0; j<length; j++){
        for(int i=0; i<width; i++){
            NewImagedata[j+13][i+13][0] = Imagedata[j][i][0]; }
    }

    // Allocating 3D Output Image Data Arrays
    unsigned char OutputImage[length][width][1];

    //Implementing NLM Filtering

    // Iterating through the Extended Image nd implementing NLM Filter
    for (int i = 13; i < length + 13; i++){
        for (int j = 13; j < width + 13; j++) {
            OutputImage[i-13][j-13][0] = (unsigned char)NLM(i, j, NewImagedata);
        }
    }

    // Write image data (filename specified by second argument) from image data matrix

    if (!(file=fopen(argv[2],"wb"))) {
        cout << "Cannot open file: " << argv[2] << endl;
        exit(1);
    }

    fwrite(OutputImage, sizeof(unsigned char), 256*256*1, file);
    fclose(file);

    return 0;
}











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

using namespace std;

int main(int argc, char *argv[]) {

    // Define file pointer and variables
    FILE *file;
    int BytesPerPixel=1;
    int length = 300;
    int width = 390;
    //cout << "here" << length;
    int Size = 256;

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
    unsigned char Imagedata[length][width][BytesPerPixel];

    // Read image (filename specified by first argument) into image data matrix
    if (!(file=fopen(argv[1],"rb"))) {
        cout << "Cannot open file: " << argv[1] <<endl;
        exit(1);
    }

    fread(Imagedata, sizeof(unsigned char), length*width*BytesPerPixel, file);
    fclose(file);

    //cout << "bytes"<< BytesPerPixel;

    //Demosaicing using Bilinear Interpolation

    // Allocating New Image Data Array
    unsigned char NewImagedata[length+2][width+2][BytesPerPixel];

    // Creating New Image Data Array by mirroring the second last rows and columns to create length+2*width+2 Bayer Array
    for(int i = 1; i < 1 + width; i++){ // Rows added to top and bottom
        NewImagedata[0][i][0] = Imagedata[1][i-1][0];
        NewImagedata[length+1][i][0] = Imagedata[length-1][i-1][0];
    }

    for(int i = 1; i < 1 + length; i++){
        NewImagedata[i][0][0] = Imagedata[i-1][1][0];
        NewImagedata[i][width+1][0] = Imagedata[i-1][width-1][0];
    }

    for(int i = 1; i < length + 1 ; i++) {
        for (int j = 1; j < width + 1; j++) {
            NewImagedata[i][j][0] = Imagedata[i-1][j-1][0];
        }
    }

    // Corner elements

    NewImagedata[0][0][0]=NewImagedata[0][2][0];
    NewImagedata[0][width+1][0]=NewImagedata[0][width-1][0];
    NewImagedata[length+1][0][0]= NewImagedata[length+1][2][0];
    NewImagedata[length+1][width+1][0] = NewImagedata[length+1][width-1][0];

    // Allocating 3D Output Image Data Array
    unsigned char OutputImage[length][width][3];

    // // Extracting the R G B values for each pixel in the New Image Data Array
    for(int i = 1; i < length + 1 ; i++){
        for(int j = 1; j < width + 1 ; j++){

            if((i-1) % 2 == 0 && (j-1) % 2 == 0){ //i and j both odd
                OutputImage[i-1][j-1][0] = (unsigned char)(0.5*((NewImagedata[i][j-1][0]) + (NewImagedata[i][j+1][0])));
                OutputImage[i-1][j-1][1] = NewImagedata[i][j][0];
                OutputImage[i-1][j-1][2] = (unsigned char)(0.5*((NewImagedata[i-1][j][0]) + (NewImagedata[i+1][j][0])));
            }
            else if((i-1) % 2 ==0 && (j-1) % 2 != 0){ //i odd j even
                OutputImage[i-1][j-1][0] = NewImagedata[i][j][0];
                OutputImage[i-1][j-1][1] = (unsigned char)(0.25*((NewImagedata[i][j-1][0]) + (NewImagedata[i-1][j][0]) + (NewImagedata[i][j+1][0]) + (NewImagedata[i+1][j][0])));
                OutputImage[i-1][j-1][2] = (unsigned char)(0.25*((NewImagedata[i-1][j-1][0]) + (NewImagedata[i-1][j+1][0]) + (NewImagedata[i+1][j-1][0]) + (NewImagedata[i+1][j+1][0])));
            }
            else if((i-1) % 2 != 0 && (j-1) % 2 == 0){ //i even j odd
                OutputImage[i-1][j-1][0] = (unsigned char)(0.25*((NewImagedata[i-1][j-1][0]) + (NewImagedata[i+1][j-1][0]) + (NewImagedata[i-1][j+1][0]) + (NewImagedata[i+1][j+1][0])));
                OutputImage[i-1][j-1][1] = (unsigned char)(0.25*((NewImagedata[i][j-1][0]) + (NewImagedata[i][j+1][0]) + (NewImagedata[i-1][j][0]) + (NewImagedata[i+1][j][0])));
                OutputImage[i-1][j-1][2] = NewImagedata[i][j][0];
            }

            else if((i-1) % 2 != 0 && (j-1) % 2 != 0){ //i and j both even
                OutputImage[i-1][j-1][0] = (unsigned char)(0.5*((NewImagedata[i-1][j][0]) + (NewImagedata[i+1][j][0])));
                OutputImage[i-1][j-1][1] = NewImagedata[i][j][0];
                OutputImage[i-1][j-1][2] = (unsigned char)(0.5*((NewImagedata[i][j-1][0]) + (NewImagedata[i][j+1][0])));

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
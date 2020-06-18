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

int main(int argc, char *argv[]) {

    // Define file pointer and variables
    FILE *file;
    int BytesPerPixel=1;
    int Size = 400;

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

    // Method A
    // Transfer Function Based Histogram Equalization

    // Initialize Intensity
    float hist_intensity[256] = { 0 };
    int pixel;

    //Create Histogram of Image Intensities

    for(int i = 0; i < Size; i++){
        for(int j = 0; j < Size; j++){
            pixel = (int)Imagedata [i][j][0];
            hist_intensity[pixel] += 1;
            //cout << "intensity"<< hist_intensity;
        }
    }

    // Normalize the Image Intensities
    float hist_intensity_Norm[256] = { 0.0 };

    for (int i = 0 ; i < 256 ; i++)
    {
        hist_intensity_Norm[i] = hist_intensity[i] / (160000);
    }

    //Writing Intensity values to text file

//    ofstream myfile ("Histogram_values.txt");
//    if (myfile.is_open())
//    {
//        for(int i = 0; i < 256; i++){
//            myfile << hist_intensity_Norm[i] << " "<< endl ;
//            myfile << " \n";
//        }
//        myfile.close();
//    }
//    else cout << "Unable to open file";

    //Finding CDF of the histogram values

    float CDF[256] = { 0.0 };

    for (int i = 1 ; i < 256 ; i++)
    {
        CDF[i] = CDF[i-1] + hist_intensity_Norm[i];
    }

    //Writing CDF values to text file

//    ofstream myfile1 ("Histogram_CDF_values.txt");
//    if (myfile1.is_open())
//    {
//        for(int i = 0; i < 256; i++){
//            myfile1 << CDF[i] << " "<< endl ;
//            myfile1 << " \n";
//            //myfile1 << " \n";
//
//        }
//        myfile1.close();
//    }
//    else cout << "Unable to open file";

    // Multiplying CDF values by 255

    float temp [256];
    for (int i = 0 ; i < 256 ; i++)
    {
        temp[i] = 255 * CDF[i];
    }


    // Allocating 3D Output Image Data Array
    unsigned char OutputImage[Size][Size][1];


    for(int i = 0; i<Size; i++){
        for(int j=0; j<Size; j++){
            for(int k = 0 ; k < 256; k++){
                if(k == int(Imagedata[i][j][0])){
                    OutputImage[i][j][0] = (unsigned char)(floor(temp[k]));
                }
            }
        }
    }

    //for(int i = 0; i< Size; i++){
      //  for(int j=0; j< Size; j++){
        //    OutputImage[i][j][0] = (unsigned char)floor(CDF[(int)Imagedata[i][j][0]]*(255));
        //}
    //}


    // Write image data (filename specified by second argument) from image data matrix

    if (!(file=fopen(argv[2],"wb"))) {
        cout << "Cannot open file: " << argv[2] << endl;
        exit(1);
    }

    fwrite(OutputImage, sizeof(unsigned char), Size*Size*1, file);
    fclose(file);

    return 0;
}

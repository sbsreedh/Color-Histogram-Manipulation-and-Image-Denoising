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
#include <algorithm>
//#include <map>
//#include <iterator>
//#include <vector>

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


    //Writing Intensity values to text file

//    ofstream myfile ("Histogram_values.txt");
//    if (myfile.is_open())
//    {
//        for(int i = 0; i < 256; i++){
//            myfile << hist_intensity[i] << " "<< endl ;
//            myfile << " \n";
//        }
//        myfile.close();
//    }
//    else cout << "Unable to open file";


    // Method B
    // Cumulative Probability based Histogram Equalization : Bucket Filling Method

    // Number of Buckets = 256
    // Total Number of Pixels =  160000
    // Thus, Number of Pixels per Bucket = 160000/256 = 625

    //int total_bucket[Size*Size];

    int loc1 [Size*Size];
    int loc2 [Size*Size];

    int m = 0;
    int n = 0;
    for (int g = 0; g < 256; g++){
        for (int i = 0 ; i < Size ; i++){
            for (int j = 0 ; j < Size ; j++){
                if (g == (int)Imagedata[i][j][0]){
                    loc1[m] = i;
                    loc2[n] = j;
                    m += 1;
                    n += 1;
                }
            }
        }
    }

    int g = 0 ;
    int pixels_per_bucket = (Size*Size)/256;
    int count = 1;

    // Allocating 3D Output Image Data Array
    unsigned char OutputImage[Size][Size][1];

    for (int p = 0 ; p < Size*Size ; p++){
        if(count % pixels_per_bucket == 0 ){
            count = 1 ;
            g += 1 ;
            OutputImage[loc1[p]][loc2[p]][0] = (unsigned char) g; }
        else{
            OutputImage[loc1[p]][loc2[p]][0] = (unsigned char) g;
            count += 1; }
    }



//    for(int i = 0; i<Size; i++){
//        for(int j = 0; j<Size; j++){
//            total_bucket[(count,i,j)] = Imagedata[i][j][0];
//            count = count + 1;
//        }
//    }
//    // Sorting the total_bucket array
//    sort(total_bucket, total_bucket + (Size*Size));
//
//    //Assigning Pixels in Original Image to corresponding Bucket
//    //int buckets[256] = { 0 };
//
//    //cout << "pixels_per_bucket"<< pixels_per_bucket;
//    for(int i = 0; i < pixels_per_bucket; i++){
//        total_bucket[i] = 0;
//    }


    // Write image data (filename specified by second argument) from image data matrix

    if (!(file=fopen(argv[2],"wb"))) {
        cout << "Cannot open file: " << argv[2] << endl;
        exit(1);
    }

    fwrite(OutputImage, sizeof(unsigned char), Size*Size*1, file);
    fclose(file);

    return 0;
}
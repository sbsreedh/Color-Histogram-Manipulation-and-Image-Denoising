## Objective:
Image demosaicing is an algorithm used to convert grayscale/raw images into color images. Histogram manipulation helps in adjusting contrast of grayscale using its histogram of pixel intensities. Here I have implemented two different types of Image demosaicing:
1. Bilinear Demosaicing
2. MHC Demosaicing

Two Histogram Manipulation methods:
1. Bucket Filling Method
2. Cumulative Frequency Distribution Transfer Method

Image Denoising is the process of removing noise from image. It is an important preprocessing step for further image analysis. Different types of noise that can corrupt image are Gaussian noise, uniform noise, impulse noise, etc. To deal with each of them we have different filters. In this project, I have implemented the following filters on grayscale as well as color images and observe the performance with each of them:

1. Linear filter
2. Bilateral filter
3. Non-local mean (NLM) filter 

## Requirements:
1. Ubuntu/Windows
2. Vscode
3. C++ (g++ compiler)

## Usage Information:
Use below syntax to run respective files:

```
g++ filename.cpp -o filename(main)
./filename inputImageName outputImageName
```

Example:

```
g++ bilinear.cpp
./bilinear cat.raw cat_bilinear_out.raw 1 390 300
```

Thus, For Each Question, Parameters to be passed : 

input_file_name output_file_name

I have hard-coded the Program to handle BytesPerPixel Width Height of each Image. 
For example, for Demosaicing using Bilinear Interpolation Question, Input image size  = 390 * 300 BW Image and Bytes per pixel is 1.

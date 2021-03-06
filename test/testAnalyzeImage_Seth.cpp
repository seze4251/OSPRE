//
//  testAnalyzeImage.cpp
//  testAnalyzeImage
//
//  Created by Anthony on 3/21/2017.
//  Copyright � 2017 Anthony. All rights reserved.
//
/**
 TODO:
 - Merge newly generated analyzeImage code that deals with 13MP images
 **/

#include <iostream>
#include <unistd.h>
#include <exception>
#include <stdio.h>
#include <string>
#include <cmath>
#include <ctime>

// OpenCV
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

// analyzeImage helpers
#include "rt_nonfinite.h"
#include "analyzeImage.h"
#include "analyzeImage_terminate.h"
#include "analyzeImage_initialize.h"
#include "analyzeImage_emxAPI.h"
#include "analyzeImage_emxutil.h"

// OSPRE helpers
#include "PointEarthMoon.h"

// Namespaces
using namespace cv;

// Define common values
#define MOON_RADIUS 1736.0
#define EARTH_RADIUS 6371.0

// Pre Declare Functions:
void readImage(std::string imgFilename, unsigned char* imIn);
void calcRadGuess(double* pxDiam, double* estPos, PointEarthMoon point, double* ans);
int testNominal(bool vOut);

/***
 Main Function
 */
int main(int argc, char* argv[]){
    int nomCase = 1;
    
    bool vOut = 1; // Verbose output
    
    nomCase = testNominal(vOut);
}

/***
 Nominal Test Case
 
 13MP image, 4160 x 3120
 67 px diameter moon in center
 */
int testNominal(bool vOut) {
    if(vOut){
        std::cout << "Starting nominal case" << std::endl;
    }
    
    bool testFailed = 0;
    //Setup time
    time_t rawtime;
    struct tm * timinfo;
    
    double radiusRangeGuess[2] = {157, 167};
    //double radiusRangeGuess[2] = {58, 63};
    double pix_deg[2] = {67, 67};
    double centerPt_data[2];
    int centerPt_size[2];
    double radius;
    double numCirc;
    double alpha;
    double beta;
    double theta;
    double sensitivity = 0.99;
    
    double imgWidth = 1100; double imgHeight = 736;
    
    
    // Initialize function 'analyzeImage' input arguments.
    // Load image with OpenCV
    if(vOut){
        std::cout << "Reading image with OpenCV" << std::endl;
    }
    
    cv::Mat image;
    std::cout << "Starting Image Read" << std::endl;
    image = imread("test/TestImages/blueMoon.jpg", IMREAD_COLOR);
    std::cout << "Finished Image Read" << std::endl;
    
    if(!image.data){
        std::cout << "Could not read image" << std::endl;
        return 0;
    }
    
    emxArray_uint8_T *I;
    emxInitArray_uint8_T(&I, 3);
    cv::Vec3b intensity;
    int sizeimage, counter;
    int rows = image.rows;
    int cols = image.cols;
    sizeimage = rows*cols;
    I->size[0] = rows;
    I->size[1] = cols;
    I->size[2] = 3;
    
    std::cout << "Starting Conversion to EMX array" << std::endl;
    // Convert image into emxArray
    //emxEnsureCapacity((emxArray__common *)I, 0, (int)sizeof(unsigned char));
    
    unsigned char* temp = new unsigned char[2428800];
    
    try{
        counter = 0;
        for (int i=0; i < cols; i++){
            for (int j=0; j < rows; j++){
                intensity = image.at<Vec3b>(j,i);
                uchar blue = intensity.val[0];
                uchar green = intensity.val[1];
                uchar red = intensity.val[2];
                // New Code
                temp[counter] = red;
                temp[counter + sizeimage] = green;
                temp[counter + 2*sizeimage] = blue;
                // Old Code
                //  *(I->data + counter) = red;
                //  *(I->data + counter + sizeimage) = green;
                //  *(I->data + counter + 2*sizeimage) = blue;
                counter++;
            }
        }
    } catch(...) {
        std::cout << "Image conversion failed" << std::endl;
        return 0;
    }
    
    // New Code
    I->data = temp;
    
    FILE* testCC = fopen("./log/testAnalyzeImage.txt","w");
    for (int i = 0; i < 2428800; i++) {
        fprintf(testCC, " %x", I->data[i]);
    }
    fflush(testCC);
    
    std::cout << "Finishedit Conversion to EMX array" << std::endl;
    
    
    std::cout << "Starting call to analyze image" << std::endl;
    std::clock_t start;
    
    printf("dv3 = [%f  %f], sens = %f, pix_deg [%f  %f], I = [%d, %d, %d]\n", radiusRangeGuess[0], radiusRangeGuess[1], sensitivity, pix_deg[0], pix_deg[1], I->size[0], I->size[1], I->size[2]);
    
    try{
        analyzeImage( I, radiusRangeGuess, sensitivity,
                     pix_deg, imgWidth, imgHeight, centerPt_data,
                     centerPt_size, &radius, &numCirc,
                     &alpha, &beta, &theta);
    } catch (const char*){
        std::cout << "Call to analyzeImage failed" << std::endl;
        return 0;
    }
    std::cout << "Finished call to analyze image" << std::endl;
    // Test that radius is within acceptable range
    if(radius<(59.8545-1e-4) || radius>(59.8545+1e-4)){
        std::cout << "Calculated radius is incorrect." << std::endl;
        std::cout << "Expected 59.8545 +/- 1e-4, but instead found: " << radius << std::endl;
        testFailed = 1;
    }
    
    // Test the calculated center point
    if(centerPt_data[0] < (2078.8762-1e-4) || centerPt_data[0] > (2078.8762+1e-4)){
        std::cout << "Calculated X center point is incorrect" << std::endl;
        std::cout << "Expected 2078.8762 +/- 1e-4 but instead found: " << centerPt_data[0] << std::endl;
        testFailed = 1;
    }
    if(centerPt_data[1] < (1559.0764-1e-4) || centerPt_data[1] > (1559.0764+1e-4)){
        std::cout << "Calculated Y center point is incorrect" << std::endl;
        std::cout << "Expected 1559.0764 +/- 1e-4 but instead found: " << centerPt_data[0] << std::endl;
        testFailed = 1;
    }
    
    //Test alpha, beta, theta
    if(alpha < (-0.0168-1e-4) || alpha > (-0.0168+1e-4)){
        std::cout << "Calculated alpha is incorrect" << std::endl;
        std::cout << "Expected -0.0168 +/- 1e-4 but instead found: " << alpha << std::endl;
        testFailed = 1;
    }
    if(beta < (-0.0138-1e-4) || beta > (-0.0138+1e-4)){
        std::cout << "Calculated beta is incorrect" << std::endl;
        std::cout << "Expected -0.0138 +/- 1e-4 but instead found: " << beta << std::endl;
        testFailed = 1;
    }
    if(theta < (1.7867-1e-4) || beta > (1.7867+1e-4)){
        std::cout << "Calculated theta is incorrect" << std::endl;
        std::cout << "Expected 1.7867 +/- 1e-4 but instead found: " << theta << std::endl;
        testFailed = 1;
    }
    
    if(testFailed){
        std::cout << "analyzeImage: Test has failed" << std::endl;
        return 0;
    } else {
        std::cout << "analyzeImage: Test has Passed" << std::endl;
        return 1;
    }
    return 1;
}


/**
 HELPER FUNCTIONS
 */

/*
 calcRadGuess
 TODO:
 - Create emperical function describing estimated radius
 - */
void calcRadGuess(double* pxDiam, double* estPos, PointEarthMoon point, double* ans) {
    /*if (point == PEM_Earth) {
     // Plug in estimated position to Earth emperical function
     ans[0] = pxDiam[0] / 2 - 2;
     ans[1] = pxDiam[1] / 2 + 2;
     } else if (point == PEM_Moon) {
     // Plug in estimated position to Moon emperical function
     ans[0] = pxDiam[0] / 2 - 2;
     ans[1] = pxDiam[1] / 2 + 2;
     }*/
    
    ans[0] = pxDiam[0] / 2 - 2;
    ans[1] = pxDiam[1] / 2 + 2;
    
    /*
     ans[0] = 157;
     ans[1] = 167;
     */
    
}


/***
 --- OUTDATED ---
 readImage
 */
void readImage(std::string imgFilename, unsigned char* imIn) {
    // Get image
    printf("Read Image: Starting Image Read\n");
    cv::Mat image;
    image = cv::imread(imgFilename, cv::IMREAD_COLOR);
    
    
    if (!image.data) {
        printf("Read Image ERROR: Could not open or find the image, Image Name: %s \n", imgFilename.c_str());
        return;
    }
    else {
        printf("Read Image: Image Name Valid, Image Name: %s\n", imgFilename.c_str());
    }
    
    // Allocate variables
    // TODO:
    // - Verify that this is correct
    //unsigned char* imIn = (unsigned char*)imageMessage->getImagePointer();
    cv::Vec3b intensity;
    
    int counter = 0;
    // Loop through image and convert
    for (int i = 0; i < image.cols; i++) {
        for (int j = 0; j < image.rows; j++) {
            intensity = image.at<cv::Vec3b>(j, i);
            uchar blue = intensity.val[0];
            uchar green = intensity.val[1];
            uchar red = intensity.val[2];
            //imIn[counter] = red;
            //imIn[counter + 13431200] = green;
            //imIn[counter + 2 * 13431200] = blue;
            *(imIn + counter) = red;
            *(imIn + counter + 13431200) = green;
            *(imIn + counter + 2*13431200) = blue;
            counter++;
        }
    }
    
    //cv::imshow("Display window", imIn);
    
    //imageMessage->currentImageSize = 2428800;
    printf("Read Image: Finished Image Read\n");
}

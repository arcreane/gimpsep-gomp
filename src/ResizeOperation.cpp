//
// Created by victo on 28/04/2025.
//

#include "ResizeOperation.h"
#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;
void ResizeOperation::apply(Mat& image) {
    if (image.empty()) {
        cerr << "Image is empty" << endl;
        return;
    }
    cout << "enter the scale factor on x: "<<endl;
    double fx;
    while (!(cin >> fx)&& fx>0) {
        cout << "Invalid input. Please enter a positive numeric value for fx ";
        cin.clear();
    }
    cout << "enter the scale factor on y: "<<endl;
    double fy;
    while (!(cin >> fy)&& fy>0) {
        cout << "Invalid input. Please enter a positive numeric value for fy ";
        cin.clear();
    }
    // Resize the image
    Mat resizedImage;
    resize(image, resizedImage, Size(), fx, fy, INTER_LINEAR);
    image = resizedImage;
}
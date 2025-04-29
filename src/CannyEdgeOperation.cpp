//
// Created by victo on 28/04/2025.
//

#include "CannyEdgeOperation.h"
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

void CannyEdgeOperation::apply(Mat& image) {
    if (image.empty()) {
        cerr << "Could not open or find the image" << endl;
        return;
    }

    int lowThreshold, highThreshold;
    cout << "Enter the low threshold for Canny edge detection: ";
    while (!(cin >> lowThreshold)) {
        cout << "Invalid input. Please enter a numeric value for low threshold: ";
        cin.clear();
        cin.ignore();
    }
    cout << "Enter the high threshold for Canny edge detection: ";
    while (!(cin >> highThreshold)) {
        cout << "Invalid input. Please enter a numeric value for high threshold: ";
        cin.clear();
        cin.ignore();
    }

    Canny(image, image, lowThreshold, highThreshold);
}
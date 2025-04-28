//
// Created by victo on 28/04/2025.
//

#include "LightenDarkenOperation.h"
#include <opencv2/opencv.hpp>

void LightenDarkenOperation::apply(Mat& image) {
    // Check if the image is empty
    if (image.empty()) {
        cout << "Image is empty" << endl;
        return;
    }
    cout << "Enter alpha value: " << endl;
    double alpha;
    cin >> alpha;
    cout << "Enter beta value: " << endl;
    double beta;
    cin >> beta;
    // Create a new image to store the result
    Mat newImage = Mat::zeros(image.size(), image.type());

    // Apply the lighten/darken operation
    for (int y = 0; y < image.rows; y++) {
        for (int x = 0; x < image.cols; x++) {
            for (int c = 0; c < 3; c++) {
                newImage.at<Vec3b>(y, x)[c] =
                    saturate_cast<uchar>(alpha * image.at<Vec3b>(y, x)[c] + beta);
            }
        }
    }
    image = newImage;
}
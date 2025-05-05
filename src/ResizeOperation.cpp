//
// Created by victo on 28/04/2025.
//

#include "ResizeOperation.h"
#include <opencv2/opencv.hpp>
using namespace cv;

ResizeOperation::ResizeOperation(const double fx, const double fy) : fx(fx), fy(fy) {}

void ResizeOperation::apply(Mat& image) {
    resize(image, image, Size(), fx, fy);
}

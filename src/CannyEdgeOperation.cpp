#include "CannyEdgeOperation.h"
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

CannyEdgeOperation::CannyEdgeOperation(const int low, const int high, const int kSize)
    : lowThreshold(low), highThreshold(high), kernelSize(kSize) {}

void CannyEdgeOperation::apply(Mat& image) {
    Mat inputGray, edges;

    if (image.channels() > 1) {
        cvtColor(image, inputGray, COLOR_BGR2GRAY);
    } else {
        inputGray = image.clone();
    }

    Canny(inputGray, edges, lowThreshold, highThreshold, kernelSize);

    cvtColor(edges, image, COLOR_GRAY2BGR);
}
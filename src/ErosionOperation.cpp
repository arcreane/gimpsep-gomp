#include "ErosionOperation.h"

using namespace cv;

ErosionOperation::ErosionOperation(const int kSize) : kernelSize(kSize) {}

void ErosionOperation::apply(Mat& image) {
    Mat output;
    const auto kernel = getStructuringElement(MORPH_RECT,
                    Size(2 * kernelSize + 1, 2 * kernelSize + 1),
                    Point(kernelSize, kernelSize));
    erode(image, image, kernel);
}
#include "DilationOperation.h"

using namespace cv;

DilationOperation::DilationOperation(const int kSize) : kernelSize(kSize) {}

void DilationOperation::apply(Mat& image) {
    Mat output;
    const auto kernel = getStructuringElement(MORPH_RECT,
                    Size(2 * kernelSize + 1, 2 * kernelSize + 1),
                    Point(kernelSize, kernelSize));
    dilate(image, image, kernel);
}
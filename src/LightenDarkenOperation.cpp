#include "LightenDarkenOperation.h"

using namespace cv;

LightenDarkenOperation::LightenDarkenOperation(const double f) : factor(f) {}

void LightenDarkenOperation::apply(Mat& image) {
    image.convertTo(image, -1, 1, factor);
}
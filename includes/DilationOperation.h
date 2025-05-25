#ifndef DILATIONOPERATION_H
#define DILATIONOPERATION_H
#include "Operation.h"
#include <opencv2/opencv.hpp>

using namespace cv;

class DilationOperation final : public Operation {
public:
    explicit DilationOperation(int kSize);
    void apply(Mat &image) override;
int kernelSize;
};



#endif //DILATIONOPERATION_H

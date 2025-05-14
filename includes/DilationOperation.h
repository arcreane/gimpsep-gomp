//
// Created by victo on 28/04/2025.
//

#ifndef DILATIONOPERATION_H
#define DILATIONOPERATION_H
#include "Operation.h"
#include <opencv2/opencv.hpp>

using namespace cv;

class DilationOperation final : public Operation {
int kernelSize;
public:
    explicit DilationOperation(int kSize);
    void apply(Mat &image) override;
};



#endif //DILATIONOPERATION_H

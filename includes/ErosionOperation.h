//
// Created by victo on 28/04/2025.
//

#ifndef EROSIONOPERATION_H
#define EROSIONOPERATION_H
#include <Operation.h>
#include <opencv2/opencv.hpp>

using namespace cv;

class ErosionOperation final: public Operation{
int kernelSize;
public:
    explicit ErosionOperation(int kSize);
    void apply(Mat &image) override;
};



#endif //EROSIONOPERATION_H

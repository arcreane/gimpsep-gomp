//
// Created by victo on 28/04/2025.
//

#ifndef LIGHTENDARKENOPERATION_H
#define LIGHTENDARKENOPERATION_H
#include "Operation.h"
#include <opencv2/opencv.hpp>

class LightenDarkenOperation : public Operation {
public:
    void apply(Mat& image) override;
};


#endif //LIGHTENDARKENOPERATION_H

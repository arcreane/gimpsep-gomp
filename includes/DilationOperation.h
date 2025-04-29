//
// Created by victo on 28/04/2025.
//

#ifndef DILATIONOPERATION_H
#define DILATIONOPERATION_H
#include "Operation.h"
#include <opencv2/opencv.hpp>


class DilationOperation final : public Operation {
public:
    void apply(Mat& image) override;
};



#endif //DILATIONOPERATION_H

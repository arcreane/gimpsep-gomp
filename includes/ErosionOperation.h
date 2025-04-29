//
// Created by victo on 28/04/2025.
//

#ifndef EROSIONOPERATION_H
#define EROSIONOPERATION_H
#include <Operation.h>
#include <opencv2/opencv.hpp>


class ErosionOperation final: public Operation{
public:
    void apply(Mat &image) override;
};



#endif //EROSIONOPERATION_H

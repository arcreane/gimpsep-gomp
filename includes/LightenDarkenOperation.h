#ifndef LIGHTENDARKENOPERATION_H
#define LIGHTENDARKENOPERATION_H
#include "Operation.h"
#include <opencv2/opencv.hpp>

class LightenDarkenOperation final: public Operation {
public:
    void apply(Mat& image) override;
    explicit LightenDarkenOperation(double f);
double factor;
};


#endif //LIGHTENDARKENOPERATION_H

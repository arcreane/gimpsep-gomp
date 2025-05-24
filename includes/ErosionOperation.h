#ifndef EROSIONOPERATION_H
#define EROSIONOPERATION_H
#include <Operation.h>
#include <opencv2/opencv.hpp>

using namespace cv;

class ErosionOperation final: public Operation{
public:
    void apply(Mat &image) override;
    explicit ErosionOperation(int kSize);
int kernelSize;
};



#endif //EROSIONOPERATION_H

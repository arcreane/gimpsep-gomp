#ifndef CANNYEDGEOPERATION_H
#define CANNYEDGEOPERATION_H
#include <Operation.h>


class CannyEdgeOperation final: public Operation{
public:
    int lowThreshold;
    int highThreshold;
    int kernelSize;
    explicit CannyEdgeOperation(int low, int high, int kSize);
    void apply(Mat& image) override;
};



#endif //CANNYEDGEOPERATION_H

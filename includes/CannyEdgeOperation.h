#ifndef CANNYEDGEOPERATION_H
#define CANNYEDGEOPERATION_H
#include <Operation.h>


class CannyEdgeOperation final: public Operation{
public:
    void apply(Mat& image) override;
    explicit CannyEdgeOperation(int low, int high, int kSize);
private:
    int lowThreshold;
    int highThreshold;
    int kernelSize;
};



#endif //CANNYEDGEOPERATION_H

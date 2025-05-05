//
// Created by victo on 28/04/2025.
//

#ifndef CANNYEDGEOPERATION_H
#define CANNYEDGEOPERATION_H
#include <Operation.h>


class CannyEdgeOperation final: public Operation{
private:
    int lowThreshold;
    int highThreshold;
    int kernelSize;
public:
    explicit CannyEdgeOperation(int low, int high, int kSize);
    void apply(Mat& image) override;
};



#endif //CANNYEDGEOPERATION_H

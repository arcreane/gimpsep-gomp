#ifndef RESIZEOPERATION_H
#define RESIZEOPERATION_H
#include "Operation.h"


class ResizeOperation final : public Operation{
double fx;
double fy;
public:
    explicit ResizeOperation(double fx, double fy);

    void apply(Mat& image) override;
};



#endif //RESIZEOPERATION_H
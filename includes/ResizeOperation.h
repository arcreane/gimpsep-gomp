//
// Created by victo on 28/04/2025.
//

#ifndef RESIZEOPERATION_H
#define RESIZEOPERATION_H
#include "Operation.h"


class ResizeOperation: public Operation{
public:
    void apply(Mat& image) override;
};



#endif //RESIZEOPERATION_H

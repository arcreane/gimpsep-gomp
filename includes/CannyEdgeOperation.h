//
// Created by victo on 28/04/2025.
//

#ifndef CANNYEDGEOPERATION_H
#define CANNYEDGEOPERATION_H
#include <Operation.h>


class CannyEdgeOperation final: public Operation{
public:
    void apply(Mat& image) override;
};



#endif //CANNYEDGEOPERATION_H

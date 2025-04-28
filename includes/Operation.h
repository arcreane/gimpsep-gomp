//
// Created by victo on 28/04/2025.
//

#ifndef OPERATION_H
#define OPERATION_H
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class Operation {
    public:
      virtual void apply(Mat& image) = 0;
      virtual ~Operation() {}
};

#endif //OPERATION_H

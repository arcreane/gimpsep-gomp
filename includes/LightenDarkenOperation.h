//
// Created by victo on 28/04/2025.
//

#ifndef LIGHTENDARKENOPERATION_H
#define LIGHTENDARKENOPERATION_H
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
class LightenDarkenOperation {
public:
    // Constructor
    LightenDarkenOperation() = default;
    // Destructor
    ~LightenDarkenOperation() = default;
    // Method to apply the operation
    void apply(Mat& image);

};



#endif //LIGHTENDARKENOPERATION_H

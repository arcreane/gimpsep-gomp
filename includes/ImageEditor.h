#ifndef IMAGEEDITOR_H
#define IMAGEEDITOR_H

#include <opencv2/opencv.hpp>
#include <iostream>
#include "Operation.h"

using namespace cv;

class ImageEditor {
private:
    Mat currentImage;
    Mat modifiedImage;
public:
    bool loadImage(const std::string& filepath);
    [[nodiscard]] bool saveImage(const std::string& filepath) const;
    void applyOperation(const std::shared_ptr<Operation>& operation);
    void displayImage(const std::string& windowName) const;
    void resetToOriginal();
    [[nodiscard]] Mat getOriginalImage() const;
    [[nodiscard]] Mat getResultImage() const;
};



#endif //IMAGEEDITOR_H

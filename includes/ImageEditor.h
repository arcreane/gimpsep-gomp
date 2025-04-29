//
// Created by victo on 28/04/2025.
//

#ifndef IMAGEEDITOR_H
#define IMAGEEDITOR_H

#include <opencv2/opencv.hpp>
#include <iostream>
#include "Operation.h"

class ImageEditor {
    private:
        cv::Mat currentImage;
    public:
        bool loadImage(const std::string& filepath);
        [[nodiscard]] bool saveImage(const std::string& filepath) const;
        void applyOperation(std::shared_ptr<Operation> operation);
        void displayImage(const std::string& windowName) const;
};



#endif //IMAGEEDITOR_H

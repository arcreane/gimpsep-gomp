//
// Created by victo on 28/04/2025.
//

#include "ImageEditor.h"
#include "Operation.h"
#include <filesystem>

ImageEditor::ImageEditor() = default;

ImageEditor::~ImageEditor() = default;

bool ImageEditor::loadImage(const std::string &filepath) {
    const Mat image = imread(filepath, IMREAD_COLOR);
    if (image.empty()) {
        std::cerr << "Could not open or find the image" << std::endl;
        return false;
    }
    currentImage = image;
    return true;
}

bool ImageEditor::saveImage(const std::string &filepath) const {
    if (currentImage.empty()) {
        std::cerr << "No image loaded to save" << std::endl;
        return false;
    }

    std::string correctedFilepath = !filepath.empty() ? filepath : "images/output.png";

    if (!imwrite(correctedFilepath, currentImage)) {
        std::cerr << "Could not save the image at " << correctedFilepath << std::endl;
        return false;
    }
    std::cout << "Saving image to: " << correctedFilepath << std::endl;
    return true;
}

void ImageEditor::applyOperation(std::shared_ptr<Operation> operation) {
    if (currentImage.empty()) {
        std::cerr << "No image loaded" << std::endl;
        return;
    }
    operation->apply(currentImage);
}

void ImageEditor::displayImage(const std::string& windowName) const {
    if (currentImage.empty()) {
        std::cerr << "No image loaded" << std::endl;
        return;
    }
    cv::imshow(windowName, currentImage);
    cv::waitKey(0);
}

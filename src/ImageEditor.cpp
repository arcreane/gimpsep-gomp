#include "ImageEditor.h"
#include "Operation.h"
#include <filesystem>

using namespace std;
using namespace cv;

bool ImageEditor::loadImage(const std::string &filepath) {
    Mat image = imread(filepath, IMREAD_COLOR);
    if (image.empty()) {
        std::cerr << "Could not open or find the image" << std::endl;
        return false;
    }
    Mat resizedImage;
    constexpr int maxWidth = 1920;
    constexpr int maxHeight = 1080;
    if (image.cols > maxWidth || image.rows > maxHeight) {
        const double scale = std::min(static_cast<double>(maxWidth) / image.cols, static_cast<double>(maxHeight) / image.rows);
        resize(image, resizedImage, Size(), scale, scale, INTER_LINEAR);
    }
    image = resizedImage.empty() ? image : resizedImage;
    currentImage = image;
    modifiedImage = image.clone();
    return true;
}

void ImageEditor::saveImage(const std::string &filepath) const {
    if (modifiedImage.empty()) {
        std::cerr << "No image loaded to save" << std::endl;
    }

    if (!imwrite(filepath, modifiedImage)) {
        std::cerr << "Could not save the image at " << filepath << std::endl;
    }
}

void ImageEditor::applyOperation(const std::shared_ptr<Operation>& operation) {
    addOperationToUndoStack();
    operation->apply(modifiedImage);
}

void ImageEditor::displayImage(const std::string& windowName) const {
    if (modifiedImage.empty()) {
        std::cerr << "No image loaded" << std::endl;
        return;
    }
    imshow(windowName, modifiedImage);
    waitKey(0);
    destroyAllWindows();
}

void ImageEditor::setOriginalImage(const Mat& image) {
    currentImage = image.clone();
    modifiedImage = image.clone();
}

void ImageEditor::resetToOriginal() {
    modifiedImage = currentImage.clone();
}

Mat ImageEditor::getOriginalImage() const {
    return currentImage.clone();
}

Mat ImageEditor::getResultImage() const {
    return modifiedImage.clone();
}

void ImageEditor::setImage(const Mat& image) {
    modifiedImage = image.clone();
}

void ImageEditor::setLockedImage(const Mat& image) {
    lockedImage = image.clone();
}

Mat ImageEditor::getLockedImage() const {
    return lockedImage.clone();
}

void ImageEditor::addOperationToUndoStack() {
    undoStack.push_back(modifiedImage.clone());
}

void ImageEditor::undoLastOperation() {
    if (!undoStack.empty()) {
        undoStack.pop_back();
        if (!undoStack.empty()) {
            setImage(undoStack.back());
        } else {
            setImage(currentImage.clone());
        }
    } else {
        std::cerr << "No operations to undo" << std::endl;
    }
}
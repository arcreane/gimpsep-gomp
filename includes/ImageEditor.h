#ifndef IMAGEEDITOR_H
#define IMAGEEDITOR_H

#include <opencv2/opencv.hpp>
#include "Operation.h"

using namespace cv;

class ImageEditor {
    Mat currentImage;
    Mat modifiedImage;
    Mat lockedImage;
    vector<Mat> undoStack;
public:
    bool loadImage(const std::string& filepath);
    void saveImage(const std::string& filepath) const;
    void applyOperation(const std::shared_ptr<Operation>& operation);
    void displayImage(const std::string& windowName) const;
    void resetToOriginal();
    void setOriginalImage(const Mat& image);
    void setImage(const Mat& image);
    void setLockedImage(const Mat& image);
    void addOperationToUndoStack();
    void undoLastOperation();
    [[nodiscard]] Mat getOriginalImage() const;
    [[nodiscard]] Mat getResultImage() const;
    [[nodiscard]] Mat getLockedImage() const;
};



#endif //IMAGEEDITOR_H
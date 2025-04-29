//
// Created by victo on 28/04/2025.
//

#include "DilationOperation.h"

void DilationOperation::apply(Mat& image) {
    if ( image.empty() ) {
        cerr << "Could not open or find the image" << endl;
        return;
    }
    int dilationSize;
    cout << "Enter the dilation size (positive integer): ";
    while (!(cin >> dilationSize) || dilationSize < 0) {
        cout << "Invalid input. Please enter a positive integer for dilation size: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    const Mat element = getStructuringElement(MORPH_RECT,
                                        Size(2 * dilationSize + 1, 2 * dilationSize + 1),
                                        Point(dilationSize, dilationSize));
    dilate(image, image, element);
}
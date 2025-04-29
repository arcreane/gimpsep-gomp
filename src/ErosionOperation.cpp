//
// Created by victo on 28/04/2025.
//

#include "ErosionOperation.h"

void ErosionOperation::apply(Mat &image) {
    if ( image.empty() ) {
        cerr << "Could not open or find the image" << endl;
        return;
    }
    int erosionSize;
    cout << "Enter the erosion size (positive integer): ";
    while (!(cin >> erosionSize) || erosionSize < 0) {
        cout << "Invalid input. Please enter a positive integer for erosion size: ";
        cin.clear();
    }

    const Mat element = getStructuringElement(MORPH_RECT,
                                         Size(2 * erosionSize + 1, 2 * erosionSize + 1),
                                         Point(erosionSize, erosionSize));
    erode(image, image, element);
}

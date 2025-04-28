//
// Created by victo on 28/04/2025.
//

#include "LightenDarkenOperation.h"

void LightenDarkenOperation::apply(Mat &image) {
    if ( image.empty() ) {
        cout << "Empty image" << endl;
    }

    double alpha;
    double beta;
    cout << "Enter the alpha value (must be positive): ";
    while (!(cin >> alpha)) {
        cout << "Invalid input. Please enter a numeric value for alpha: ";
        cin.clear();
    }
    cout << "Enter the beta value (can be negative): ";
    while (!(cin >> beta)) {
        cout << "Invalid input. Please enter a numeric value for beta: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    for(int y=0; y<image.rows; y++) {
        for(int x=0; x<image.cols; x++) {
            for(int c=0; c<3; c++) {
                image.at<Vec3b>(y,x)[c] =
                  saturate_cast<uchar>( alpha*( image.at<Vec3b>(y,x)[c] ) + beta );
            }
        }
    }
    image;

}

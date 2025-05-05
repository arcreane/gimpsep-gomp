#include "ImageEditor.h"
#include "DilationOperation.h"
#include "ErosionOperation.h"
#include "ResizeOperation.h"
#include "LightenDarkenOperation.h"
#include "CannyEdgeOperation.h"
#include "PanoramaStitcher.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <memory>

using namespace std;
using namespace cv;

int brightness = 100;
int erosionSize = 1;
int dilationSize = 1;
int resizeFactor = 100;
int cannyThreshold1 = 50;
int cannyThreshold2 = 150;

Mat original;
ImageEditor editor;
shared_ptr<Operation> currentOperation;

void update() {
    if (currentOperation) {
        editor.resetToOriginal();
        editor.applyOperation(currentOperation);
        imshow("Image Editor", editor.getResultImage());
    }
}

void setupBrightness() {
    destroyWindow("Image Editor");
    namedWindow("Image Editor", WINDOW_AUTOSIZE);
    createTrackbar("Brightness", "Image Editor", &brightness, 200, [](int, void*) {
        currentOperation = make_shared<LightenDarkenOperation>(brightness - 100);
        update();
    });
    setTrackbarPos("Brightness", "Image Editor", 100);
    update();
}

void setupErosion() {
    destroyWindow("Image Editor");
    namedWindow("Image Editor", WINDOW_AUTOSIZE);
    createTrackbar("Erosion Size", "Image Editor", &erosionSize, 20, [](int, void*) {
        currentOperation = make_shared<ErosionOperation>(erosionSize);
        update();
    });
    update();
}

void setupDilation() {
    destroyWindow("Image Editor");
    namedWindow("Image Editor", WINDOW_AUTOSIZE);
    createTrackbar("Dilation Size", "Image Editor", &dilationSize, 20, [](int, void*) {
        currentOperation = make_shared<DilationOperation>(dilationSize);
        update();
    });
    update();
}

void setupResize() {
    destroyWindow("Image Editor");
    namedWindow("Image Editor", WINDOW_AUTOSIZE);
    createTrackbar("Resize %", "Image Editor", &resizeFactor, 300, [](int, void*) {
        double scale = resizeFactor / 100.0;
        currentOperation = make_shared<ResizeOperation>(scale, scale);
        update();
    });
    setTrackbarPos("Resize %", "Image Editor", resizeFactor);
    update();
}

void setupCanny() {
    destroyWindow("Image Editor");
    namedWindow("Image Editor", WINDOW_AUTOSIZE);
    createTrackbar("Canny Thresh1", "Image Editor", &cannyThreshold1, 255, [](int, void*) {
        currentOperation = make_shared<CannyEdgeOperation>(cannyThreshold1, cannyThreshold2, 3);
        update();
    });
    createTrackbar("Canny Thresh2", "Image Editor", &cannyThreshold2, 255, [](int, void*) {
        currentOperation = make_shared<CannyEdgeOperation>(cannyThreshold1, cannyThreshold2, 3);
        update();
    });
    // Initial values are fetched once to apply before sliders move
    currentOperation = make_shared<CannyEdgeOperation>(cannyThreshold1, cannyThreshold2, 3);
    update();
}

void setupPanorama() {
    destroyWindow("Image Editor");
    namedWindow("Image Editor", WINDOW_AUTOSIZE);
    vector<string> paths = {
        // vector images to stitch
    };
    currentOperation = make_shared<PanoramaStitcher>(paths);
    editor.resetToOriginal();
    editor.applyOperation(currentOperation);
    imshow("Image Editor", editor.getResultImage());
}

int main(int argc, char** argv) {
    string imagePath;
    cout << "Enter image path to load: ";
    cin >> imagePath;

    if (!editor.loadImage(imagePath)) {
        cerr << "Failed to load image: " << imagePath << endl;
        return -1;
    }

    original = editor.getOriginalImage();
    namedWindow("Image Editor", WINDOW_AUTOSIZE);
    imshow("Image Editor", original);

    cout << "Keyboard Controls:\n";
    cout << "b - Brightness\ne - Erosion\nd - Dilation\nr - Resize\nc - Canny\np - Panorama\ns - Save\nESC - Exit\n";

    while (true) {
        int key = waitKey(0);
        if (key == 27) break; // ESC to quit
        switch (key) {
            case 'b': setupBrightness(); break;
            case 'e': setupErosion(); break;
            case 'd': setupDilation(); break;
            case 'r': setupResize(); break;
            case 'c': setupCanny(); break;
            case 'p': setupPanorama(); break;
            case 's':
                imwrite("images/output.jpg", editor.getResultImage());
                cout << "Saved to output.jpg" << endl;
                break;
        }
    }

    destroyAllWindows();
    return 0;
}

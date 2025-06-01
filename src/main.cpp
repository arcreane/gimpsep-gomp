#include <filesystem>
#include <iostream>
#include <memory>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include "ImageEditor.h"
#include "DilationOperation.h"
#include "ErosionOperation.h"
#include "ResizeOperation.h"
#include "LightenDarkenOperation.h"
#include "CannyEdgeOperation.h"
#include "PanoramaStitcher.h"
#include "tinyfiledialogs.h"

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
bool showHelp = false;
string initialWorkingDir;
Rect btnEdit, btnPanorama;

void overlayInstructions(Mat &img) {
    const vector<string> lines = showHelp
        ? vector<string>{
            "[b] Brightness", "[e] Erosion", "[d] Dilation", "[r] Resize", "[c] Canny",
            "[p] Panorama", "[s] Save", "[u] Undo", "[l] Lock", "[h] Toggle Help", "[ESC] Exit"
        }
        : vector<string>{"[h] Toggle Help"};

    constexpr int padding = 10;
    constexpr int lineHeight = 20;
    constexpr int width = 220;
    const int height = lineHeight * static_cast<int>(lines.size());

    rectangle(img, Point(0, 0), Point(width, height + padding), Scalar(255, 255, 255), FILLED);
    addWeighted(img(Rect(0, 0, width, height + padding)), 0.7, img(Rect(0, 0, width, height + padding)), 0.3, 0, img(Rect(0, 0, width, height + padding)));

    for (int i = 0; i < lines.size(); ++i) {
        putText(img, lines[i], Point(10, 20 + i * lineHeight), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0), 1);
    }
}

void update() {
    if (currentOperation) {
        const Mat base = editor.getLockedImage().empty()
                             ? editor.getOriginalImage().clone()
                             : editor.getLockedImage().clone();

        Mat working = base.clone();
        currentOperation->apply(working);
        editor.setImage(working);
    }

    Mat display = editor.getResultImage().clone();
    overlayInstructions(display);
    imshow("Image Editor", display);
}

void setupBrightness() {
    destroyWindow("Image Editor");
    namedWindow("Image Editor", WINDOW_AUTOSIZE);
    createTrackbar("Brightness", "Image Editor", &brightness, 200, [](int, void *) {
        currentOperation = make_shared<LightenDarkenOperation>(brightness - 100);
        update();
    });
    setTrackbarPos("Brightness", "Image Editor", 100);
    update();
}

void setupErosion() {
    destroyWindow("Image Editor");
    namedWindow("Image Editor", WINDOW_AUTOSIZE);
    createTrackbar("Erosion Size", "Image Editor", &erosionSize, 20, [](int, void *) {
        currentOperation = make_shared<ErosionOperation>(erosionSize);
        update();
    });
    update();
}

void setupDilation() {
    destroyWindow("Image Editor");
    namedWindow("Image Editor", WINDOW_AUTOSIZE);
    createTrackbar("Dilation Size", "Image Editor", &dilationSize, 20, [](int, void *) {
        currentOperation = make_shared<DilationOperation>(dilationSize);
        update();
    });
    update();
}

void setupResize() {
    destroyWindow("Image Editor");
    namedWindow("Image Editor", WINDOW_AUTOSIZE);
    createTrackbar("Resize %", "Image Editor", &resizeFactor, 300, [](int, void *) {
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
    createTrackbar("Canny Thresh1", "Image Editor", &cannyThreshold1, 255, [](int, void *) {
        currentOperation = make_shared<CannyEdgeOperation>(cannyThreshold1, cannyThreshold2, 3);
        update();
    });
    createTrackbar("Canny Thresh2", "Image Editor", &cannyThreshold2, 255, [](int, void *) {
        currentOperation = make_shared<CannyEdgeOperation>(cannyThreshold1, cannyThreshold2, 3);
        update();
    });
    currentOperation = make_shared<CannyEdgeOperation>(cannyThreshold1, cannyThreshold2, 3);
    update();
}

void setupPanorama(const vector<string> &paths) {
    const auto panoramaStitcher = make_shared<PanoramaStitcher>(paths);
    Mat resultImage = editor.getResultImage();
    panoramaStitcher->apply(resultImage);
    editor.setOriginalImage(resultImage.clone());
    currentOperation.reset();
    Mat display = editor.getResultImage().clone();
    overlayInstructions(display);
    imshow("Image Editor", display);
}

enum class StartupChoice { None, EditImage, Panorama };

StartupChoice startupChoice = StartupChoice::None;

void onMouseClick(int event, int x, int y, int, void *) {
    if (event != EVENT_LBUTTONDOWN) return;

    if (btnEdit.contains(Point(x, y)))
        startupChoice = StartupChoice::EditImage;
    else if (btnPanorama.contains(Point(x, y)))
        startupChoice = StartupChoice::Panorama;
}

void showMainImageWithButtons(const Mat& mainImage) {
    int buttonHeight = 50;
    int padding = 20;

    int canvasWidth = max(mainImage.cols + 2 * padding, 400);
    int canvasHeight = mainImage.rows + buttonHeight + 3 * padding;

    Mat canvas(canvasHeight, canvasWidth, CV_8UC3, Scalar(0, 0, 0));

    int x = (canvas.cols - mainImage.cols) / 2;
    int y = padding;

    mainImage.copyTo(canvas(Rect(x, y, mainImage.cols, mainImage.rows)));

    int buttonWidth = 150;
    int spacing = 30;

    int yBtnTop = mainImage.rows + 2 * padding;
    btnEdit = Rect((canvas.cols - 2 * buttonWidth - spacing) / 2, yBtnTop, buttonWidth, buttonHeight);
    btnPanorama = Rect(btnEdit.x + buttonWidth + spacing, yBtnTop, buttonWidth, buttonHeight);

    rectangle(canvas, btnEdit, Scalar(100, 200, 255), FILLED);
    putText(canvas, "Edit Image", btnEdit.tl() + Point(20, 35), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 0, 0), 2);

    rectangle(canvas, btnPanorama, Scalar(100, 255, 150), FILLED);
    putText(canvas, "Panorama", btnPanorama.tl() + Point(25, 35), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 0, 0), 2);

    namedWindow("Image Editor Menu");
    setMouseCallback("Image Editor Menu", onMouseClick);
    imshow("Image Editor Menu", canvas);
}

int main() {
    initialWorkingDir = filesystem::current_path().string();

    Mat gimpLogo = imread("./images/image_gimp.png", IMREAD_UNCHANGED);
    if (gimpLogo.empty()) {
        cerr << "Erreur : l'image ./images/gimp/gimp.png est introuvable ou invalide." << endl;
        return -1;
    }

    Mat smallLogo;
    resize(gimpLogo, smallLogo, Size(), 0.4, 0.4);

    showMainImageWithButtons(smallLogo);

    while (startupChoice == StartupChoice::None) {
        if (waitKey(30) == 27) {
            destroyWindow("Image Editor Menu");
            return 0;
        }
    }

    destroyWindow("Image Editor Menu");

    if (startupChoice == StartupChoice::EditImage) {
        const char* filterPatterns[] = { "*.png", "*.jpg", "*.jpeg", "*.bmp" };
        const char* path = tinyfd_openFileDialog("Select an image", "", 4, filterPatterns, "Image files", 0);

        if (!path || !editor.loadImage(path)) {
            cerr << "Failed to load image.\n";
            return -1;
        }

        original = editor.getOriginalImage();
        namedWindow("Image Editor", WINDOW_AUTOSIZE);
        Mat display = original.clone();
        overlayInstructions(display);
        imshow("Image Editor", display);

    } else if (startupChoice == StartupChoice::Panorama) {
        const char* filterPatterns[] = { "*.png", "*.jpg", "*.jpeg", "*.bmp" };
        const char* paths = tinyfd_openFileDialog("Select images", "", 4, filterPatterns, "Image files", 1);

        if (!paths || strlen(paths) == 0) {
            cerr << "No files selected.\n";
            return -1;
        }

        vector<string> fileList;
        stringstream ss(paths);
        string token;
        while (getline(ss, token, '|')) {
            fileList.push_back(token);
        }

        editor.loadImage(fileList[0]);
        original = editor.getOriginalImage();
        namedWindow("Image Editor", WINDOW_AUTOSIZE);
        Mat display = original.clone();
        overlayInstructions(display);
        imshow("Image Editor", display);

        setupPanorama(fileList);
    }

    while (getWindowProperty("Image Editor", WND_PROP_VISIBLE) >= 1) {
        int key = waitKey(0);
        if (key == 27) break;
        switch (key) {
            case 'b': setupBrightness(); break;
            case 'e': setupErosion(); break;
            case 'd': setupDilation(); break;
            case 'r': setupResize(); break;
            case 'c': setupCanny(); break;
            case 's': {
                filesystem::current_path(initialWorkingDir);
                const char* savePath = tinyfd_saveFileDialog("Save image as", "output.png", 0, nullptr, nullptr);
                if (!savePath) break;
                editor.saveImage(savePath);
                break;
            }
            case 'u':
                editor.undoLastOperation();
                update();
                break;
            case 'l':
                editor.setLockedImage(editor.getResultImage().clone());
                break;
            case 'o':
                editor.setLockedImage(Mat());
                editor.resetToOriginal();
                update();
                currentOperation.reset();
                break;
            case 'h':
                showHelp = !showHelp;
                update();
                break;
        }
    }

    destroyAllWindows();
    return 0;
}

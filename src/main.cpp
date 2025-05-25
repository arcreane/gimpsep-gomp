#include <filesystem>
#include "ImageEditor.h"
#include "DilationOperation.h"
#include "ErosionOperation.h"
#include "ResizeOperation.h"
#include "LightenDarkenOperation.h"
#include "CannyEdgeOperation.h"
#include "PanoramaStitcher.h"
#include "FileHelper.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <memory>
#include <vector>

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
std::string initialWorkingDir;
cv::Rect btnEdit, btnPanorama;


void overlayInstructions(Mat &img) {
    const vector<string> lines = showHelp
                                     ? vector<string>{
                                         "[b] Brightness", "[e] Erosion", "[d] Dilation", "[r] Resize", "[c] Canny",
                                         "[p] Panorama", "[s] Save", "[u] Undo", "[l] Lock", "[h] Toggle Help",
                                         "[ESC] Exit"
                                     }
                                     : vector<string>{"[h] Toggle Help"};

    constexpr int padding = 10;
    constexpr int lineHeight = 20;
    constexpr int width = 220;
    const int height = lineHeight * static_cast<int>(lines.size());

    rectangle(img, Point(0, 0), Point(width, height + padding),
              Scalar(255, 255, 255), FILLED);
    addWeighted(img(Rect(0, 0, width, height + padding)),
                0.7, img(Rect(0, 0, width, height + padding)), 0.3, 0,
                img(Rect(0, 0, width, height + padding)));

    for (int i = 0; i < size(lines); ++i) {
        putText(img, lines[i], Point(10, 20 + i * lineHeight),
                FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0), 1);
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


void showMainImageWithButtons(const cv::Mat& mainImage) {

    int buttonHeight = 50;
    int padding = 20;

    int canvasWidth = std::max(mainImage.cols + 2 * padding, 400);
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
    initialWorkingDir = std::filesystem::current_path().string();

    cv::Mat gimpLogo = cv::imread("../images/image_gimp.png", cv::IMREAD_UNCHANGED);
    if (gimpLogo.empty()) {
        std::cerr << "Erreur : l'image ./images/gimp/gimp.png est introuvable ou invalide." << std::endl;
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
        if (std::string path = FileHelper::openFileDialog(); path.empty() || !editor.loadImage(path)) {
            cerr << "Failed to load image.\n";
            return -1;
        }

        original = editor.getOriginalImage();
        namedWindow("Image Editor", WINDOW_AUTOSIZE);
        Mat display = original.clone();
        overlayInstructions(display);
        imshow("Image Editor", display);
    } else if (startupChoice == StartupChoice::Panorama) {
        const std::vector<std::string> paths = FileHelper::openMultipleFilesDialog();
        if (paths.empty()) {
            cerr << "No files selected.\n";
            return -1;
        }
        editor.loadImage(paths[0]);

        original = editor.getOriginalImage();
        namedWindow("Image Editor", WINDOW_AUTOSIZE);
        Mat display = original.clone();
        overlayInstructions(display);
        imshow("Image Editor", display);

        setupPanorama(paths);
    } else {
        cerr << "Invalid option.\n";
        return -1;
    }

    while (getWindowProperty("Image Editor", WND_PROP_VISIBLE) >= 1) {
        const int key = waitKey(0);
        if (key == 27) break;
        switch (key) {
            case 'b': setupBrightness();
                break;
            case 'e': setupErosion();
                break;
            case 'd': setupDilation();
                break;
            case 'r': setupResize();
                break;
            case 'c': setupCanny();
                break;
            case 's': {
                std::filesystem::current_path(initialWorkingDir);
                std::string savePath = FileHelper::saveFileDialog();
                if (savePath.empty()) {
                    break;
                }
                editor.saveImage(savePath);
                break;
            }
            case 'u': {
                editor.undoLastOperation();
                Mat restored = editor.getResultImage().clone();
                overlayInstructions(restored);
                imshow("Image Editor", restored);
                break;
            }
            case 'l':
                editor.setLockedImage(editor.getResultImage().clone());
                break;
            case 'o': {
                editor.setLockedImage(Mat());
                editor.resetToOriginal();
                Mat restored = editor.getResultImage().clone();
                overlayInstructions(restored);
                imshow("Image Editor", restored);
                currentOperation.reset();
                Mat resetView = editor.getResultImage().clone();
                overlayInstructions(resetView);
                imshow("Image Editor", resetView);
                break;
            }
            case 'h':
                showHelp = !showHelp;
                update();
                break;
            default: ;
        }
    }

    destroyAllWindows();
    return 0;
}

//
// Created by victo on 29/04/2025.
//

#include "PanoramaStitcher.h"
#include <filesystem>
namespace fs = std::filesystem;

PanoramaStitcher::PanoramaStitcher() {
    do {
        int choice;
        std::cout << "Enter 1 to input a folder or 2 to input image paths one by one: ";
        std::cin >> choice;
        if (choice == 1) {
            std::string folderPath;
            std::cout << "Enter the folder path containing images: ";
            std::cin >> folderPath;

            for (const auto& entry : fs::directory_iterator(folderPath)) {
                if (entry.is_regular_file()) {
                    if (std::string extension = entry.path().extension().string(); extension == ".jpg" || extension == ".png" || extension == ".bmp") {
                        imagePaths.push_back(entry.path().string());
                    }
                }
            }
        } else if (choice == 2) {
            int n;
            std::cout << "Enter number of images to stitch: ";
            std::cin >> n;
            for (int i = 0; i < n; ++i) {
                std::string path;
                std::cout << "Enter path for image " << (i + 1) << ": ";
                std::cin >> path;
                imagePaths.push_back(path);
            }
        }
        cout << "You have entered " << imagePaths.size() << " images." << endl;
    } while (imagePaths.empty());
}

void PanoramaStitcher::apply(Mat& image) {
    vector<Mat> images;
    for (const auto& path : imagePaths) {
        Mat img = imread(path, IMREAD_COLOR);
        if (img.empty()) {
            cerr << "Could not open or find the image at " << path << endl;
            return;
        }
        images.push_back(img);
    }

    for (size_t i = 0; i < images.size(); ++i) {
        for (size_t j = i + 1; j < images.size(); ++j) {
            if (images[i].size() != images[j].size()) {
                cerr << "Error: Images have different sizes and cannot be compared." << endl;
                continue;
            }
            Mat gray1, gray2;
            cvtColor(images[i], gray1, COLOR_BGR2GRAY);
            cvtColor(images[j], gray2, COLOR_BGR2GRAY);
            if (countNonZero(gray1 != gray2) == 0) {
                cerr << "Error: Identical images provided for stitching." << endl;
                return;
            }
        }
    }

    const Ptr<Stitcher> stitcher = Stitcher::create(Stitcher::PANORAMA);
    Mat stitchedImage;

    if (const Stitcher::Status status = stitcher->stitch(images, stitchedImage); status != Stitcher::OK) {
        cerr << "Error during stitching: " << static_cast<int>(status) << endl;
        switch (status) {
            case Stitcher::ERR_NEED_MORE_IMGS:
                cerr << "Not enough images for stitching." << endl;
                break;
            case Stitcher::ERR_HOMOGRAPHY_EST_FAIL:
                cerr << "Homography estimation failed." << endl;
                break;
            case Stitcher::ERR_CAMERA_PARAMS_ADJUST_FAIL:
                cerr << "Camera parameters adjustment failed." << endl;
                break;
            default:
                cerr << "Unknown error." << endl;
        }
        return;
    }

    image = stitchedImage;
    cout << "Stitching completed successfully." << endl;
}


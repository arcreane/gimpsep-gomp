//
// Created by victo on 28/04/2025.
//

#include <opencv2/stitching.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include "PanoramaStitcher.h"

using namespace cv;
using namespace std;

PanoramaStitcher::PanoramaStitcher(const std::vector<std::string>& paths)
    : imagePaths(paths) {}

void PanoramaStitcher::apply(Mat& image) {
    std::vector<Mat> images;

    for (const auto& path : imagePaths) {
        if (Mat img = imread(path); !img.empty()) {
            images.push_back(img);
        } else {
            cerr << "Warning: Couldn't load " << path << endl;
        }
    }

    if (images.size() < 2) {
        cerr << "Need at least 2 images for panorama" << endl;
        return;
    }

    Mat pano;
    const Ptr<Stitcher> stitcher = Stitcher::create(Stitcher::PANORAMA);

    if (const Stitcher::Status status = stitcher->stitch(images, pano); status != Stitcher::OK) {
        cerr << "Stitching failed, error code: " << status << endl;
        return;
    }

    constexpr int maxWidth = 1920;
    constexpr int maxHeight = 1080;
    if (pano.cols > maxWidth || pano.rows > maxHeight) {
        const double scale = std::min(static_cast<double>(maxWidth) / pano.cols, static_cast<double>(maxHeight) / pano.rows);
        resize(pano, pano, Size(), scale, scale, INTER_LINEAR);
    }

    image = pano.clone();
}

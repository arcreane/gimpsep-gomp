#include "PanoramaStitcher.h"
#include <opencv2/stitching.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

PanoramaStitcher::PanoramaStitcher(const std::vector<std::string>& paths)
    : imagePaths(paths) {}

void PanoramaStitcher::apply(cv::Mat& image) {
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
        cerr << "Stitching failed, error code: " << static_cast<int>(status) << endl;
        return;
    }

    image = pano.clone();
}

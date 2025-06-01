#include <opencv2/stitching.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <stack>
#include "PanoramaStitcher.h"

using namespace cv;
using namespace std;

PanoramaStitcher::PanoramaStitcher(const vector<string>& paths)
    : imagePaths(paths) {}

void PanoramaStitcher::apply(Mat& image) {
    vector<Mat> images;

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
        const double scale = min(static_cast<double>(maxWidth) / pano.cols, static_cast<double>(maxHeight) / pano.rows);
        resize(pano, pano, Size(), scale, scale, INTER_LINEAR);
    }

    image = cropBlackBorder(pano);
}

Mat PanoramaStitcher::cropBlackBorder(const Mat& image) {
    Mat gray, binary;
    cvtColor(image, gray, COLOR_BGR2GRAY);
    threshold(gray, binary, 10, 1, THRESH_BINARY);

    const int rows = binary.rows;
    const int cols = binary.cols;
    vector<vector<int>> heights(rows, vector<int>(cols, 0));

    for (int x = 0; x < cols; ++x) {
        heights[0][x] = binary.at<uchar>(0, x);
        for (int y = 1; y < rows; ++y) {
            heights[y][x] = binary.at<uchar>(y, x) ? heights[y - 1][x] + 1 : 0;
        }
    }

    int maxArea = 0, x0 = 0, y0 = 0, w = 0, h = 0;
    for (int y = 0; y < rows; ++y) {
        stack<int> s;
        int x = 0;
        vector<int> height = heights[y];
        height.push_back(0);
        while (x <= cols) {
            int curHeight = (x < cols) ? height[x] : 0;
            if (s.empty() || curHeight >= height[s.top()]) {
                s.push(x++);
            } else {
                int top = s.top(); s.pop();
                int width = s.empty() ? x : x - s.top() - 1;
                int area = height[top] * width;
                if (area > maxArea) {
                    maxArea = area;
                    x0 = s.empty() ? 0 : s.top() + 1;
                    y0 = y - height[top] + 1;
                    w = width;
                    h = height[top];
                }
            }
        }
    }

    if (w <= 0 || h <= 0 ||
        x0 < 0 || y0 < 0 ||
        x0 + w > image.cols || y0 + h > image.rows) {
        std::cerr << "Invalid crop region. Returning original image.\n";
        return image.clone();
        }

    return image(Rect(x0, y0, w, h)).clone();
}


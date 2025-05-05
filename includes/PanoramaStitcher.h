#ifndef PANORAMA_STITCHER_H
#define PANORAMA_STITCHER_H

#include "Operation.h"
#include <vector>
#include <string>

using namespace cv;
class PanoramaStitcher final : public Operation {
private:
  std::vector<std::string> imagePaths;

public:
  explicit PanoramaStitcher(const std::vector<std::string>& paths);
  void apply(Mat& image) override;
};

#endif // PANORAMA_STITCHER_H

//
// Created by victo on 29/04/2025.
//

#ifndef PANORAMASTICHER_H
#define PANORAMASTICHER_H

#include "Operation.h"
#include <vector>
#include <string>


class PanoramaSticher final: public Operation {
private:
  std::vector<std::string> imagePaths;
public:
  PanoramaSticher();
  void apply(cv::Mat& image) override;
};



#endif //PANORAMASTICHER_H

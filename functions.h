#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <opencv2/core.hpp>

void modifyROI(const cv::Size imgSize, cv::Rect &rect);

int getMaxId(const cv::Mat &line);

#endif // FUNCTIONS_H

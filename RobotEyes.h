#ifndef ROBOTEYES_H
#define ROBOTEYES_H

#include <opencv2/core.hpp>

class RobotEyes
{
public:
    RobotEyes();
    
    void input(const cv::Mat &frame_l, const cv::Mat &frame_r);
    
    void output();
    
private:
    cv::Mat m_frameL;
    cv::Mat m_frameR;
    
    void processSingle(const cv::Mat &img, std::vector<cv::Rect> &faceRects, 
                       std::vector<std::string> &genders, std::vector<std::array<int,2>> &ages);
    
    void processPair(const cv::Mat &frameL, const cv::Mat &frameR, 
                     const std::vector<cv::Rect> &faceRects_l, 
                     std::vector<std::array<cv::Rect,2>> &faceRectPairs);
};

#endif // ROBOTEYES_H

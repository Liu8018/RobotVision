#include "functions.h"

void modifyROI(const cv::Size imgSize, cv::Rect &rect)
{
    if(rect.x > imgSize.width-1 || rect.y > imgSize.height-1)
    {
        rect = cv::Rect();
        return;
    }
    
    if(rect.x < 0)
        rect.x = 0;
    if(rect.y < 0)
        rect.y = 0;
    if(rect.br().x > imgSize.width-1)
        rect.width = imgSize.width-1-rect.x;
    if(rect.br().y > imgSize.height-1)
        rect.height = imgSize.height-1-rect.y;
}

int getMaxId(const cv::Mat &line)
{
    double minVal,maxVal;
    int minIdx[2],maxIdx[2];
    
    cv::minMaxIdx(line,&minVal,&maxVal,minIdx,maxIdx);
    
    return maxIdx[1];
}

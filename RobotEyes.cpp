#include "RobotEyes.h"
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "FaceDetection/FaceDetection.h"
#include "InfoPrediction/InfoPrediction.h"
#include "functions.h"
#include "TargetMatcher.h"

RobotEyes::RobotEyes()
{
    
}

void RobotEyes::input(const cv::Mat &frame_l, const cv::Mat &frame_r)
{
    frame_l.copyTo(m_frameL);
    frame_r.copyTo(m_frameR);
}

void RobotEyes::output()
{
    //处理左帧
    std::vector<cv::Rect> faceRects_l;
    std::vector<std::string> genders;
    std::vector<std::array<int,2>> ages;
    processSingle(m_frameL,faceRects_l,genders,ages);
    
    //test
    cv::Mat frameL_test = m_frameL.clone();
    for(size_t i=0;i<faceRects_l.size();i++)
    {
        cv::rectangle(frameL_test,faceRects_l[i],cv::Scalar(255,0,0),2);
        cv::putText(frameL_test,genders[i],faceRects_l[i].tl(),1,1,cv::Scalar(0,255,0));
        std::string ageText = std::to_string(ages[i][0]) + "-" + std::to_string(ages[i][1]);
        cv::putText(frameL_test,ageText,faceRects_l[i].br(),1,1,cv::Scalar(0,0,255));
    }
    cv::namedWindow("frame_l",0);
    cv::imshow("frame_l",frameL_test);
    
    //利用左帧处理结果优化右帧处理
    std::vector<std::array<cv::Rect,2>> faceRectPairs;
    processPair(m_frameL,m_frameR,faceRects_l,faceRectPairs);
    
    //计算距离
    std::vector<float> dists(faceRectPairs.size());
    for(size_t i=0;i<faceRectPairs.size();i++)
    {
        float dist = std::abs(faceRectPairs[i][0].x - faceRectPairs[i][1].x);
        dist = 1/dist;
        dist *= 20;
        
        dists[i] = dist;
    }
    
    //test
    cv::Mat frameR_test = m_frameR.clone();
    for(size_t i=0;i<faceRectPairs.size();i++)
    {
        cv::rectangle(frameR_test,faceRectPairs[i][1],cv::Scalar(255,0,0),2);
        
        cv::putText(frameR_test,genders[i],faceRectPairs[i][1].tl(),1,1,cv::Scalar(0,255,0));
        
        std::string ageText = std::to_string(ages[i][0]) + "-" + std::to_string(ages[i][1]);
        cv::putText(frameR_test,ageText,faceRectPairs[i][1].br(),1,1,cv::Scalar(0,0,255));
        
        cv::putText(frameR_test,std::to_string(dists[i]).substr(0,4),
                    cv::Point(faceRectPairs[i][1].x,faceRectPairs[i][1].br().y),1,1,cv::Scalar(200,100,220));
    }
    cv::namedWindow("frame_r",0);
    cv::imshow("frame_r",frameR_test);
}

void RobotEyes::processSingle(const cv::Mat &img, std::vector<cv::Rect> &faceRects, 
                              std::vector<std::string> &genders, std::vector<std::array<int,2>> &ages)
{
    //检测人脸
    g_faceDT.detect(img,faceRects);
    
    //防止检测框超出边界
    for(std::vector<cv::Rect>::iterator it=faceRects.begin();it != faceRects.end();)
    {
        modifyROI(img.size(),*it);
        if(it->empty())
            it = faceRects.erase(it);
        else
            it++;
    }
    
    //根据人脸提取信息
    genders.resize(faceRects.size());
    ages.resize(faceRects.size());
    for(size_t i=0;i<faceRects.size();i++)
    {
        cv::Mat faceROI = img(faceRects[i]);
        g_infoPR.getInfo(faceROI,genders[i],ages[i][0],ages[i][1]);
    }
}

void RobotEyes::processPair(const cv::Mat &frameL, const cv::Mat &frameR, 
                            const std::vector<cv::Rect> &faceRects_l, 
                            std::vector<std::array<cv::Rect, 2> > &faceRectPairs)
{
    faceRectPairs.clear();
    
    for(size_t i=0;i<faceRects_l.size();i++)
    {
        cv::Mat target = frameL(faceRects_l[i]);
        int targetCols = target.cols;
        int targetRows = target.rows;
        
        //根据left faceRect的上下界在右帧取rowRange(适当扩充上下界)
        int eh = 8;
        int lowLimit = faceRects_l[i].y-eh<0? 0:faceRects_l[i].y-eh;
        int highLimit = faceRects_l[i].br().y+eh>frameR.rows? frameR.rows:faceRects_l[i].br().y+eh;
        cv::Mat image = frameR.rowRange(lowLimit,highLimit);
        
        //适当缩小target和image，加速运算
        int resizeRows = 20;
        float resizeRatio = resizeRows/(float)image.rows;
        cv::resize(target,target,cv::Size(),resizeRatio,resizeRatio);
        cv::resize(image,image,cv::Size(),resizeRatio,resizeRatio);
        
        //模板匹配
        cv::Mat matchResult;
        cv::matchTemplate(image,target,matchResult,cv::TM_SQDIFF);
        
        double minVal, maxVal;
        cv::Point minLoc, maxLoc, matchLoc;
        cv::minMaxLoc( matchResult, &minVal, &maxVal, &minLoc, &maxLoc, cv::Mat() );
        matchLoc = minLoc;
        
        //变换回原坐标系
        matchLoc.x /= resizeRatio;
        matchLoc.y /= resizeRatio;
        matchLoc.y += lowLimit;
        
        std::array<cv::Rect,2> rectPair;
        rectPair[0] = faceRects_l[i];
        rectPair[1] = cv::Rect(matchLoc, cv::Size(targetCols, targetRows));
        faceRectPairs.push_back(rectPair);
        
        /*
        cv::rectangle( image, cv::Rect(matchLoc, cv::Size(target.cols, target.rows) ), cv::Scalar(0, 0, 255), 2, 8, 0 );
        
        cv::namedWindow("Target detection",0);
        cv::imshow("Target detection",image);
        */
    }
}

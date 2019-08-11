#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>

#include "functions.h"

#include "FaceDetection/FaceDetection.h"
#include "InfoPrediction/InfoPrediction.h"

int main(int argc, char ** argv)
{
    if(argc < 2)
    {
        std::cout<<"command-line options error"<<std::endl;
        return 0;
    }
    
    cv::VideoCapture cap(argv[1]);
    if(!cap.isOpened())
    {
        std::cout<<"failed to open "<<argv[1]<<std::endl;
        return 0;
    }
    cap.set(cv::CAP_PROP_FRAME_WIDTH,640);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT,240);
    
    cv::Mat frame;
    while(1)
    {
        cap >> frame;
        frame = frame.colRange(0,frame.cols/2);
        
        std::vector<cv::Rect> faceRects;
        g_faceDT.detect(frame,faceRects);
        
        std::vector<std::string> genders(faceRects.size());
        std::vector<std::array<int,2>> ages(faceRects.size());
        for(size_t i=0;i<faceRects.size();i++)
        {
            modifyROI(frame.size(),faceRects[i]);
            
            cv::Mat faceROI = frame(faceRects[i]);
            g_infoPR.getInfo(faceROI,genders[i],ages[i][0],ages[i][1]);
            
            cv::rectangle(frame,faceRects[i],cv::Scalar(255,0,0),2);
            cv::putText(frame,genders[i],faceRects[i].tl(),1,1,cv::Scalar(0,255,0));
            std::string ageText = std::to_string(ages[i][0]) + "-" + std::to_string(ages[i][1]);
            cv::putText(frame,ageText,faceRects[i].br(),1,1,cv::Scalar(0,0,255));
        }
        
        //show
        cv::namedWindow("frame",0);
        cv::imshow("frame",frame);
        
        if(cv::waitKey(1) == 'q')
            break;
    }
    
    return 0;
}

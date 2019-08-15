#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>

#include "RobotEyes.h"

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
    //cap.set(cv::CAP_PROP_FRAME_WIDTH,640);
    //cap.set(cv::CAP_PROP_FRAME_HEIGHT,240);
    cap.set(cv::CAP_PROP_FRAME_WIDTH,1280);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT,480);
    
    RobotEyes eye;
    
    cv::Mat frame;
    while(1)
    {
        cap >> frame;
        //cv::flip(frame,frame,1);
        
        cv::namedWindow("src frame",0);
        cv::imshow("src frame",frame);
        
        cv::Mat frameL = frame.colRange(0,frame.cols/2);
        cv::Mat frameR = frame.colRange(frame.cols/2,frame.cols);
        
        eye.input(frameL,frameR);
        eye.output();
        
        if(cv::waitKey(1) == 'q')
            break;
    }
    
    return 0;
}

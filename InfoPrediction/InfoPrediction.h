#ifndef INFOPREDICTION_H
#define INFOPREDICTION_H

#include <opencv2/dnn.hpp>

class InfoPrediction
{
public:
    InfoPrediction();
    
    void getInfo(const cv::Mat &face, std::string &gender, int &age_l, int &age_h);
    
private:
    cv::dnn::Net m_ageNet;
    cv::dnn::Net m_genderNet;
};

extern InfoPrediction g_infoPR;

#endif // INFOPREDICTION_H

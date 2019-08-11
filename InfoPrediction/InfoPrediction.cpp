#include "InfoPrediction.h"
#include <iostream>

#include "functions.h"

const std::string ageProtoPath = "./data/age_deploy.prototxt";
const std::string ageModelPath = "./data/age_net.caffemodel";

const std::string genderProtoPath = "./data/gender_deploy.prototxt";
const std::string genderModelPath = "./data/gender_net.caffemodel";

const int ageList[8][2] = {{0,2}, {4,6}, {8,12}, {15,20}, {25,32}, {38,43}, {48,53}, {60,100}};
const std::string genderList[2] = {"Male", "Female"};

InfoPrediction g_infoPR;

InfoPrediction::InfoPrediction()
{
    
}

void InfoPrediction::getInfo(const cv::Mat &face, std::string &gender, int &age_l, int &age_h)
{
    if(m_ageNet.empty())
        m_ageNet = cv::dnn::readNet(ageModelPath,ageProtoPath);
    if(m_genderNet.empty())
        m_genderNet = cv::dnn::readNet(genderModelPath,genderProtoPath);
    
    cv::Scalar MODEL_MEAN_VALUES(78.4263377603, 87.7689143744, 114.895847746);
    cv::Mat blob = cv::dnn::blobFromImage(face,1.0, cv::Size(227, 227), MODEL_MEAN_VALUES, false);
    
    m_ageNet.setInput(blob);
    cv::Mat agePred = m_ageNet.forward();
    int ageLevel = getMaxId(agePred);
    age_l = ageList[ageLevel][0];
    age_h = ageList[ageLevel][1];
    
    m_genderNet.setInput(blob);
    cv::Mat genderPred = m_genderNet.forward();
    gender = genderList[getMaxId(genderPred)];
}

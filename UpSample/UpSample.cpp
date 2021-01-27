#include "UpSample.h"
#include <QDebug>
#include <QString>
#include <iostream>
#include <string>
#include <opencv2/dnn_superres.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include "opencv2/opencv.hpp"

QString UpSamplePlugin::name(){
    return "Upsample";
}

void UpSamplePlugin::edit(const cv::Mat &input, cv::Mat &output){
    // dl model at https://github.com/opencv/opencv_contrib/tree/master/modules/dnn_superres
    std::string path = "models/FSRCNN_x2.pb";
//    qDebug() << QString("models/FSRCNN_x2.pb");

    cv::dnn_superres::DnnSuperResImpl sr;

//    qDebug()<<"reading  model";
    sr.readModel(path);
//    qDebug()<<"seting  model";
    sr.setModel("fsrcnn", 2);
    cv::Mat result;
//    qDebug()<<"doing upsampling";
    sr.upsample(input, result);
    output=result;
}

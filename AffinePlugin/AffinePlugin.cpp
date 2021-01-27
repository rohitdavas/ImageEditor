#include "AffinePlugin.h"

QString AffinePlugin::name(){
    return "Affine";
}


void AffinePlugin::edit(const cv::Mat &input, cv::Mat &output){
    cv::Point2f triangleA[3];
    cv::Point2f triangleB[3];

    // triangle 1
    triangleA[0] = cv::Point2f(0,0);
    triangleA[1] = cv::Point2f(1,0);
    triangleA[2] = cv::Point2f(0,1);

    // triangle 2
    triangleB[0] = cv::Point2f(0,0);
    triangleB[1] = cv::Point2f(1,0);
    triangleB[2] = cv::Point2f(1,1);

    cv::Mat affinematrix = cv::getAffineTransform(triangleA, triangleB);
    cv::Mat result;
    cv::warpAffine(input,
                   result,
                   affinematrix,
                   input.size(),
                   cv::INTER_CUBIC,
                   cv::BORDER_CONSTANT);
    output = result;

}

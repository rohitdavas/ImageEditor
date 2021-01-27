#include <QString>
#include "erode_plugin.h"
#include <opencv2/opencv.hpp>
#include<opencv2/imgproc.hpp>

QString ErodePlugin::name(){
    return "Erode";
}

void ErodePlugin::edit(const cv::Mat &input, cv::Mat &output){
    /* documentation here
    https://docs.opencv.org/3.4/d4/d86/group__imgproc__filter.html#gaeb1e0c1033e3f6b891a25d0511362aeb

    void cv::erode 	( 	InputArray  	src,
                        OutputArray  	dst,
                        InputArray  	kernel, // if cv::Mat() is used then a 3*3 kernel is used.
                        Point  	anchor = Point(-1,-1),
                        int  	iterations = 1,
                        int  	borderType = BORDER_CONSTANT,
                        const Scalar &  	borderValue = morphologyDefaultBorderValue()
                    )

*/
    cv::erode(input, output, cv::Mat());
}

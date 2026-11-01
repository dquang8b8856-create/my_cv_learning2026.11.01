#include "demo_roi.h"
#include <iostream>

namespace demo {

cv::Mat roi(const cv::Mat& src, const std::string& outDir)
{
    cv::Rect box(580, 255, 580, 921);
    cv::Mat out = src(box).clone();

    cv::imwrite(outDir + "01_roi.bmp", out);
    std::cout << "[ROI] " << out.cols << " x " << out.rows << std::endl;
    return out;
}

}
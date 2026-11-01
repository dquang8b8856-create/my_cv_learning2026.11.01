#include "demo_hsv.h"
#include <iostream>

namespace demo {

cv::Mat hsv(const cv::Mat& src, const std::string& outDir)
{
    cv::Mat hsvImg, m1, m2, mask, redOnly;

    cv::cvtColor(src, hsvImg, cv::COLOR_BGR2HSV);
    // 红色的 H 跨 0 点，要分两段
    cv::inRange(hsvImg, cv::Scalar(0,   80, 60), cv::Scalar(12,  255, 255), m1);
    cv::inRange(hsvImg, cv::Scalar(168, 80, 60), cv::Scalar(180, 255, 255), m2);
    cv::bitwise_or(m1, m2, mask);
    cv::bitwise_and(src, src, redOnly, mask);

    cv::imwrite(outDir + "03_red_only.bmp", redOnly);
    std::cout << "[HSV] 红色像素: " << cv::countNonZero(mask) << std::endl;
    return redOnly;
}

}
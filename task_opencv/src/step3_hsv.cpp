#include "step3_hsv.h"
#include <iostream>

namespace demo {

cv::Mat step3_hsv(const cv::Mat& img, const std::string& out)
{
    cv::Mat hsv, m1, m2, maskRed, redOnly;

    cv::cvtColor(img, hsv, cv::COLOR_BGR2HSV);
    cv::inRange(hsv, cv::Scalar(0,   80, 60), cv::Scalar(12,  255, 255), m1);
    cv::inRange(hsv, cv::Scalar(168, 80, 60), cv::Scalar(180, 255, 255), m2);
    cv::bitwise_or(m1, m2, maskRed);
    cv::bitwise_and(img, img, redOnly, maskRed);

    cv::imwrite(out + "03_red_only.jpg", redOnly);
    std::cout << "[3] 红色像素: " << cv::countNonZero(maskRed) << std::endl;
    return redOnly;
}

}
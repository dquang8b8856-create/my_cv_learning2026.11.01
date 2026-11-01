#include "step5_canny.h"
#include <iostream>

namespace demo {

cv::Mat step5_canny(const cv::Mat& img, const std::string& out)
{
    cv::Mat gray, blur, edges;

    cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(gray, blur, cv::Size(11, 11), 0);
    cv::Canny(blur, edges, 60, 160);

    cv::imwrite(out + "05_canny.png", edges);
    std::cout << "[5] 边缘像素: " << cv::countNonZero(edges) << std::endl;
    return edges;
}

}
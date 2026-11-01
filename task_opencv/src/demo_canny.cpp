#include "demo_canny.h"
#include <iostream>

namespace demo {

cv::Mat canny(const cv::Mat& src, const std::string& outDir)
{
    cv::Mat gray, blur, edges;

    cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(gray, blur, cv::Size(11, 11), 0);
    cv::Canny(blur, edges, 60, 160);

    cv::imwrite(outDir + "05_canny.png", edges);
    std::cout << "[Canny] 边缘像素: " << cv::countNonZero(edges) << std::endl;
    return edges;
}

}
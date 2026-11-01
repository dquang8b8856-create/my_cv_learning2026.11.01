#include "demo_blurbinary.h"
#include <iostream>

namespace demo {

cv::Mat blurbinary(const cv::Mat& src, const std::string& outDir)
{
    cv::Mat gray, blur, binRaw, binBlur;

    cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(gray, blur, cv::Size(11, 11), 0);

    double t1 = cv::threshold(gray, binRaw,  0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
    double t2 = cv::threshold(blur, binBlur, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

    cv::imwrite(outDir + "04_binary_raw.bmp",  binRaw);
    cv::imwrite(outDir + "04_binary_blur.bmp", binBlur);
    std::cout << "[二值化] Otsu: " << t1 << " -> " << t2 << std::endl;
    return binBlur;
}

}
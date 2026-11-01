#include "step4_blurbinary.h"
#include <iostream>

namespace demo {

cv::Mat step4_blurbinary(const cv::Mat& img, const std::string& out)
{
    cv::Mat gray, blur, binRaw, binBlur;

    cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(gray, blur, cv::Size(11, 11), 0);

    double t1 = cv::threshold(gray, binRaw,  0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
    double t2 = cv::threshold(blur, binBlur, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

    cv::imwrite(out + "04_binary_raw.png",  binRaw);
    cv::imwrite(out + "04_binary_blur.png", binBlur);
    std::cout << "[4] Otsu: " << t1 << " -> " << t2 << std::endl;
    return binBlur;
}

}
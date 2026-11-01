#include "step4_blurbinary.h"
#include <iostream>

namespace demo {

cv::Mat step4_blurbinary(const cv::Mat& img, const std::string& out, cv::Mat& blurOut)
{
    cv::Mat gray, binRaw, binBlur;

    cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(gray, blurOut, cv::Size(11, 11), 0);   // 模糊结果交给 step5

    double t1 = cv::threshold(gray, binRaw,  0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
    double t2 = cv::threshold(blurOut, binBlur, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

    cv::imwrite(out + "04_binary_raw.png",  binRaw);
    cv::imwrite(out + "04_binary_blur.png", binBlur);
    std::cout << "[4] Otsu: " << t1 << " -> " << t2 << std::endl;
    return binBlur;
}

}
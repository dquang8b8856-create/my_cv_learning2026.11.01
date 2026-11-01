#include "step2_resize.h"
#include <iostream>

namespace demo {

cv::Mat step2_resize(const cv::Mat& img, const std::string& out)
{
    cv::Mat small;
    cv::resize(img, small, cv::Size(), 0.5, 0.5);

    cv::imwrite(out + "02_small.jpg", small);
    std::cout << "[2] 缩放: " << small.cols << " x " << small.rows << std::endl;
    return small;
}

}
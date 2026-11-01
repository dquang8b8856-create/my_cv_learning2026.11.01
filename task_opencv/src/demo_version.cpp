#include "demo_version.h"
#include <iostream>

namespace demo {

cv::Mat version(const cv::Mat& src, const std::string&)
{
    std::cout << "[版本] OpenCV " << CV_VERSION << std::endl;
    return src.clone();
}

}
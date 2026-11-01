#include "step1_roi.h"
#include <iostream>

namespace demo {

cv::Mat step1_roi(const cv::Mat& img, const std::string& out)
{
    cv::Rect box(580, 255, 580, 921);
    cv::Mat roi = img(box);

    cv::imwrite(out + "01_roi.jpg", roi);
    std::cout << "[1] ROI: " << roi.cols << " x " << roi.rows << std::endl;
    return roi;
}

}
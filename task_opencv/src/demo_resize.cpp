#include "demo_resize.h"
#include <iostream>

namespace demo {

cv::Mat resize(const cv::Mat& src, const std::string& outDir)
{
    cv::Mat out;
    cv::resize(src, out, cv::Size(), 0.5, 0.5, cv::INTER_AREA);

    cv::imwrite(outDir + "02_resize.bmp", out);
    std::cout << "[缩放] " << out.cols << " x " << out.rows << std::endl;
    return out;
}

}
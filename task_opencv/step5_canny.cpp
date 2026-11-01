#include "step5_canny.h"
#include <iostream>

namespace demo {

cv::Mat step5_canny(const cv::Mat& blurGray, const std::string& out)
{
    cv::Mat edges;
    cv::Canny(blurGray, edges, 60, 160);

    cv::imwrite(out + "05_canny.png", edges);
    std::cout << "[5] 边缘像素: " << cv::countNonZero(edges) << std::endl;
    return edges;
}

}
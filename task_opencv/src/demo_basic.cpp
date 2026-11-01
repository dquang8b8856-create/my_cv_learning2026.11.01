#include "demo_basic.h"
#include <iostream>

namespace demo {

cv::Mat basic(const cv::Mat&, const std::string& outDir)
{
    cv::Mat img(300, 400, CV_8UC3, cv::Scalar(40, 40, 40));
    cv::circle(img, cv::Point(200, 150), 80, cv::Scalar(0, 200, 255), 4);
    cv::putText(img, "OpenCV", cv::Point(150, 160),
                cv::FONT_HERSHEY_SIMPLEX, 1.2, cv::Scalar(255, 255, 255), 2);

    cv::imwrite(outDir + "00_basic.bmp", img);
    std::cout << "[基础] 图像尺寸 " << img.cols << " x " << img.rows << std::endl;
    return img;
}

}
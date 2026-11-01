/**
 * ⑤ Canny 边缘检测
 * 输入是 step4 已经做好的「模糊灰度图」，不再重复灰度化/模糊
 */
#pragma once
#include <opencv2/opencv.hpp>
#include <string>

namespace demo {
cv::Mat step5_canny(const cv::Mat& blurGray, const std::string& outDir);
}
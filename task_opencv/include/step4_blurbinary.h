/**
 * ④ 高斯模糊 + Otsu 二值化
 * 顺便通过 blurOut 把「模糊后的灰度图」交给下一步用，
 * 避免 Canny 那边重复做一次灰度化 + 高斯模糊
 */
#pragma once
#include <opencv2/opencv.hpp>
#include <string>

namespace demo {
cv::Mat step4_blurbinary(const cv::Mat& src, const std::string& outDir, cv::Mat& blurOut);
}
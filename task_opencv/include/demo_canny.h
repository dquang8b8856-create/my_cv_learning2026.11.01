/**
 * Canny 边缘检测
 * 处理原图并把结果写入 outDir，返回结果图（供主程序显示）
 */
#pragma once
#include <opencv2/opencv.hpp>
#include <string>

namespace demo {
cv::Mat canny(const cv::Mat& src, const std::string& outDir);
}
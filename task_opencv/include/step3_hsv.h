/**
 * ③ HSV 转换 + 筛选红色
 * 从原图 src 计算，结果写进 outDir，并返回结果图
 */
#pragma once
#include <opencv2/opencv.hpp>
#include <string>

namespace demo {
cv::Mat step3_hsv(const cv::Mat& src, const std::string& outDir);
}
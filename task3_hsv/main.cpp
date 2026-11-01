/**
 * ============================================================
 *  文件：main.cpp
 *  项目：进阶③ · HSV 转换与颜色筛选
 *  功能：BGR→HSV，用两段 inRange 筛出红色墙面
 *  环境：Ubuntu 24.04 + OpenCV 4.11 + CMake 3.28 + g++ 13.3
 *  运行：cmake -S . -B build && cmake --build build && ./build/task3_hsv
 * ============================================================
 */
#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

#ifndef SRC_DIR
#define SRC_DIR "."
#endif

int main() {
    cv::Mat img = cv::imread(std::string(SRC_DIR) + "/train.jpg");
    if (img.empty()) { std::cout << "读不到图片" << std::endl; return 1; }

    cv::Mat hsv, m1, m2, mask, redOnly;
    cv::cvtColor(img, hsv, cv::COLOR_BGR2HSV);
    cv::inRange(hsv, cv::Scalar(0,   80, 60), cv::Scalar(12,  255, 255), m1);  // 红 H 靠近 0
    cv::inRange(hsv, cv::Scalar(168, 80, 60), cv::Scalar(180, 255, 255), m2);  // 红 H 靠近 180
    cv::bitwise_or(m1, m2, mask);
    cv::bitwise_and(img, img, redOnly, mask);

    cv::imwrite(std::string(SRC_DIR) + "/result_hsv.jpg",       hsv);
    cv::imwrite(std::string(SRC_DIR) + "/result_red_mask.png",  mask);
    cv::imwrite(std::string(SRC_DIR) + "/result_red_only.jpg",  redOnly);

    double ratio = 100.0 * cv::countNonZero(mask) / (img.cols * img.rows);
    std::cout << "红色像素: " << cv::countNonZero(mask) << std::endl;
    std::cout << "红色占比: " << ratio << " %" << std::endl;
    return 0;
}
/**
 * ============================================================
 *  文件：main.cpp
 *  项目：进阶② · 图像缩放
 *  功能：按 0.5 倍缩放，INTER_AREA 插值
 *  环境：Ubuntu 24.04 + OpenCV 4.11 + CMake 3.28 + g++ 13.3
 *  运行：cmake -S . -B build && cmake --build build && ./build/task2_resize
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

    cv::Mat small;
    cv::resize(img, small, cv::Size(), 0.5, 0.5, cv::INTER_AREA);
    cv::imwrite(std::string(SRC_DIR) + "/result_resize.jpg", small);

    std::cout << "原图  : " << img.cols << " x " << img.rows << std::endl;
    std::cout << "缩放后: " << small.cols << " x " << small.rows << std::endl;
    return 0;
}
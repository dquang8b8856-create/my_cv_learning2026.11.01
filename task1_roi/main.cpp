/**
 * ============================================================
 *  文件：main.cpp
 *  项目：进阶① · ROI 裁剪
 *  功能：对原图裁剪指定矩形区域并保存
 *  环境：Ubuntu 24.04 + OpenCV 4.11 + CMake 3.28 + g++ 13.3
 *  运行：cmake -S . -B build && cmake --build build && ./build/task1_roi
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

    cv::Rect box(580, 255, 580, 921);          // 裁剪区域 x,y,w,h
    cv::Mat roi = img(box);
    cv::imwrite(std::string(SRC_DIR) + "/result_roi.jpg", roi);

    std::cout << "原图: " << img.cols << " x " << img.rows << std::endl;
    std::cout << "ROI : " << roi.cols << " x " << roi.rows << std::endl;
    return 0;
}
/**
 * ============================================================
 *  文件：main.cpp
 *  项目：进阶⑤ · Canny 边缘提取
 *  功能：高斯降噪后 Canny，提取并保存边缘图
 *  环境：Ubuntu 24.04 + OpenCV 4.11 + CMake 3.28 + g++ 13.3
 *  运行：cmake -S . -B build && cmake --build build && ./build/task5_canny
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

    cv::Mat gray, blur, edges;
    cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(gray, blur, cv::Size(5, 5), 0);   // Canny 前先降噪
    cv::Canny(blur, edges, 60, 160);

    cv::imwrite(std::string(SRC_DIR) + "/result_canny.png", edges);

    std::cout << "Canny 边缘像素: " << cv::countNonZero(edges) << std::endl;
    return 0;
}
/**
 * ============================================================
 *  文件：main.cpp
 *  项目：进阶④ · 高斯模糊前后二值化对比
 *  功能：Otsu 自动阈值二值化，对比模糊前后差异
 *  环境：Ubuntu 24.04 + OpenCV 4.11 + CMake 3.28 + g++ 13.3
 *  运行：cmake -S . -B build && cmake --build build && ./build/task4_blur_binary
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

    cv::Mat gray, blur, binRaw, binBlur;
    cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(gray, blur, cv::Size(11, 11), 0);

    double t1 = cv::threshold(gray, binRaw,  0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
    double t2 = cv::threshold(blur, binBlur, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

    cv::imwrite(std::string(SRC_DIR) + "/result_binary_raw.png",  binRaw);
    cv::imwrite(std::string(SRC_DIR) + "/result_binary_blur.png", binBlur);
    cv::imwrite(std::string(SRC_DIR) + "/result_blur.jpg",        blur);

    std::cout << "Otsu 阈值 原图: " << t1 << std::endl;
    std::cout << "Otsu 阈值 模糊: " << t2 << std::endl;
    cv::Mat diff;
    cv::compare(binRaw, binBlur, diff, cv::CMP_NE);
    std::cout << "两种二值化差异像素: " << cv::countNonZero(diff) << std::endl;
    return 0;
}
/**
 * ============================================================
 *  OpenCV 最小视觉任务 · 总入口（6 个文件）
 *  main.cpp 负责读图、依次调用 5 个步骤、统一收尾
 *    ① step1_roi        ROI 裁剪
 *    ② step2_resize     图像缩放
 *    ③ step3_hsv        HSV 转换 + 筛选红色
 *    ④ step4_blurbinary 高斯模糊 + Otsu 二值化
 *    ⑤ step5_canny      Canny 边缘检测
 *  环境：Ubuntu 24.04 + OpenCV 4.11 + CMake
 *  运行：cmake -S . -B build && cmake --build build && ./build/vision_demo
 * ============================================================
 */
#include <iostream>
#include <string>

#include "step1_roi.h"
#include "step2_resize.h"
#include "step3_hsv.h"
#include "step4_blurbinary.h"
#include "step5_canny.h"

#ifndef SRC_DIR
#define SRC_DIR "."
#endif

int main() {
    std::cout << "=== OpenCV " << CV_VERSION << " ===" << std::endl;

    cv::Mat img = cv::imread(std::string(SRC_DIR) + "/train.jpg");
    if (img.empty()) { std::cout << "读不到图片\n"; return 1; }
    std::cout << "原图: " << img.cols << " x " << img.rows << std::endl;

    std::string out = std::string(SRC_DIR) + "/results/";

    cv::Mat roi   = demo::step1_roi(img, out);
    cv::Mat small = demo::step2_resize(img, out);
    cv::Mat red   = demo::step3_hsv(img, out);

    // step4 把模糊后的灰度图交出来，step5 直接拿去做 Canny，
    // 不重复做一次灰度化 + 高斯模糊
    cv::Mat blurGray;
    cv::Mat bin   = demo::step4_blurbinary(img, out, blurGray);
    cv::Mat edges = demo::step5_canny(blurGray, out);

    cv::imwrite(out + "00_original.jpg", img);

    std::cout << "全部完成，结果在 results/" << std::endl;
    return 0;
}
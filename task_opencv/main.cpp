/**
 * ============================================================
 *  OpenCV 最小视觉任务 · 总入口
 *  用一个大文件把 8 个小模块（每个都有 .h + .cpp）串起来：
 *    1 demo_version     输出版本号
 *    2 demo_basic       基础绘制（画圆 + 文字）
 *    3 demo_roi         ROI 裁剪
 *    4 demo_resize      图像缩放
 *    5 demo_hsv         HSV 转换 + 红色提取
 *    6 demo_blurbinary  高斯模糊 + Otsu 二值化对比
 *    7 demo_canny       Canny 边缘检测
 *    8 demo_components  连通域统计
 *  环境：Ubuntu 24.04 + OpenCV 4.11 + CMake
 *  运行：cmake -S . -B build && cmake --build build && ./build/vision_demo
 * ============================================================
 */
#include <iostream>
#include <string>

#include "demo_basic.h"
#include "demo_blurbinary.h"
#include "demo_canny.h"
#include "demo_components.h"
#include "demo_hsv.h"
#include "demo_resize.h"
#include "demo_roi.h"
#include "demo_version.h"

#ifndef SRC_DIR
#define SRC_DIR "."
#endif

int main()
{
    const std::string base = std::string(SRC_DIR);
    const std::string out  = base + "/results/";

    std::cout << "=== OpenCV 8 个模块演示 ===" << std::endl;

    cv::Mat src = cv::imread(base + "/train.jpg");
    if (src.empty())
    {
        std::cout << "读不到图片: " << base << "/train.jpg" << std::endl;
        return 1;
    }
    std::cout << "原图: " << src.cols << " x " << src.rows << std::endl;

    // 依次调用 8 个模块
    cv::Mat r1 = demo::version(src, out);
    cv::Mat r2 = demo::basic(src, out);
    cv::Mat r3 = demo::roi(src, out);
    cv::Mat r4 = demo::resize(src, out);
    cv::Mat r5 = demo::hsv(src, out);
    cv::Mat r6 = demo::blurbinary(src, out);
    cv::Mat r7 = demo::canny(src, out);
    cv::Mat r8 = demo::components(src, out);

    std::cout << "\n全部完成，结果已写入 results/" << std::endl;

    // 弹窗显示（按任意键关闭）
    cv::imshow("0 基础绘制", r2);
    cv::imshow("1 ROI", r3);
    cv::imshow("2 缩放", r4);
    cv::imshow("3 红色提取", r5);
    cv::imshow("4 二值化(模糊后)", r6);
    cv::imshow("5 Canny", r7);
    cv::imshow("6 连通域", r8);

    std::cout << "按任意键关闭窗口..." << std::endl;
    cv::waitKey(0);
    cv::destroyAllWindows();

    return 0;
}
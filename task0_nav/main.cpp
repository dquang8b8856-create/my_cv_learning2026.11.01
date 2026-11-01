/**
 * ============================================================
 *  最小视觉小任务 —— 导航方向
 *  功能：二值化分离可通行区域与障碍，统计连通域并按面积排序
 *  环境：Ubuntu 24.04 + OpenCV 4.11 + CMake 3.28 + g++ 13.3
 *  运行：cmake -S . -B build && cmake --build build && ./build/task0_nav
 * ============================================================
 */
#include <opencv2/opencv.hpp>

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#ifndef SRC_DIR
#define SRC_DIR "."
#endif

int main() {
    std::cout << "=== 导航方向 · 二值化 + 连通域统计 ===" << std::endl;

    cv::Mat img = cv::imread(std::string(SRC_DIR) + "/train.jpg");
    if (img.empty()) { std::cout << "读不到图片" << std::endl; return 1; }
    std::cout << "原图: " << img.cols << " x " << img.rows << std::endl;
    const std::string out = std::string(SRC_DIR) + "/";

    // 1. 灰度 → 高斯模糊 → Otsu 二值化（亮=障碍，暗=可通行）
    cv::Mat gray, blur, bin;
    cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(gray, blur, cv::Size(11, 11), 0);
    double th = cv::threshold(blur, bin, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
    std::cout << "Otsu 阈值: " << th << std::endl;

    // 2. 连通域统计
    cv::Mat labels, stats, centroids;
    int n = cv::connectedComponentsWithStats(bin, labels, stats, centroids, 8, CV_32S);
    int total = img.cols * img.rows;
    int obstacle = cv::countNonZero(bin);
    std::cout << "连通域总数: " << (n - 1) << std::endl;
    std::cout << "障碍像素: " << obstacle << " (" << 100.0 * obstacle / total << "%)" << std::endl;
    std::cout << "可通行像素: " << (total - obstacle) << " ("
              << 100.0 * (total - obstacle) / total << "%)" << std::endl;

    // 3. 按面积排序（0 号是背景）
    std::vector<int> idx;
    for (int i = 1; i < n; ++i) idx.push_back(i);
    std::sort(idx.begin(), idx.end(), [&](int a, int b) {
        return stats.at<int>(a, cv::CC_STAT_AREA) > stats.at<int>(b, cv::CC_STAT_AREA);
    });

    // 4. 画出前 5 大区域
    std::cout << "\n前 5 大障碍区域:" << std::endl;
    cv::Mat vis = img.clone();
    int top = std::min<int>(5, static_cast<int>(idx.size()));
    for (int k = 0; k < top; ++k) {
        int i = idx[k];
        int area = stats.at<int>(i, cv::CC_STAT_AREA);
        cv::Rect r(stats.at<int>(i, cv::CC_STAT_LEFT),  stats.at<int>(i, cv::CC_STAT_TOP),
                   stats.at<int>(i, cv::CC_STAT_WIDTH), stats.at<int>(i, cv::CC_STAT_HEIGHT));
        std::cout << "  #" << (k + 1) << " 面积=" << area
                  << " 外接框=(" << r.x << "," << r.y << "," << r.width << "," << r.height << ")"
                  << " 质心=(" << (int)centroids.at<double>(i, 0)
                  << "," << (int)centroids.at<double>(i, 1) << ")" << std::endl;

        cv::rectangle(vis, r, cv::Scalar(0, 255, 0), 4);
        cv::putText(vis, "#" + std::to_string(k + 1) + " " + std::to_string(area),
                    cv::Point(r.x + 8, std::max(34, r.y + 34)),
                    cv::FONT_HERSHEY_SIMPLEX, 1.1, cv::Scalar(0, 255, 0), 3);
    }

    // 5. 保存
    cv::imwrite(out + "result_binary.png", bin);
    cv::imwrite(out + "result_regions.jpg", vis);
    std::cout << "\n已保存: result_binary.png / result_regions.jpg" << std::endl;
    return 0;
}
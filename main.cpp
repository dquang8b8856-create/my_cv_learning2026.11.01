/**
 * ============================================================
 *  最小视觉小任务 —— 导航方向
 *  主任务：二值化分离可通行区域与障碍 + 连通域像素统计
 *  进阶：① ROI 裁剪  ② 缩放  ③ HSV 筛色  ④ 模糊前后二值化  ⑤ Canny
 *  环境：Ubuntu 24.04 + OpenCV 4.11 + CMake 3.28 + g++ 13.3
 *  运行：cmake -S . -B build && cmake --build build && ./build/vision_demo
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
    std::cout << "=== OpenCV " << CV_VERSION << " ===" << std::endl;

    cv::Mat img = cv::imread(std::string(SRC_DIR) + "/train.jpg");
    if (img.empty()) { std::cout << "读不到图片" << std::endl; return 1; }
    const std::string out = std::string(SRC_DIR) + "/";
    std::cout << "原图: " << img.cols << " x " << img.rows << std::endl;

    // ================= 进阶 ① ROI 裁剪 =================
    cv::Rect box(580, 255, 580, 921);
    cv::Mat roi = img(box);
    cv::imwrite(out + "result_roi.jpg", roi);
    std::cout << "[1] ROI: " << roi.cols << " x " << roi.rows << std::endl;

    // ================= 进阶 ② 缩放 =================
    cv::Mat small;
    cv::resize(img, small, cv::Size(), 0.5, 0.5, cv::INTER_AREA);
    cv::imwrite(out + "result_resize.jpg", small);
    std::cout << "[2] 缩放: " << small.cols << " x " << small.rows << std::endl;

    // ================= 进阶 ③ HSV + 筛红色 =================
    cv::Mat hsv, m1, m2, maskRed, redOnly;
    cv::cvtColor(img, hsv, cv::COLOR_BGR2HSV);
    cv::inRange(hsv, cv::Scalar(0,   80, 60), cv::Scalar(12,  255, 255), m1);  // 红色 H 靠近 0
    cv::inRange(hsv, cv::Scalar(168, 80, 60), cv::Scalar(180, 255, 255), m2);  // 红色 H 靠近 180
    cv::bitwise_or(m1, m2, maskRed);
    cv::bitwise_and(img, img, redOnly, maskRed);
    cv::imwrite(out + "result_red_only.jpg", redOnly);
    std::cout << "[3] 红色占比: "
              << 100.0 * cv::countNonZero(maskRed) / (img.cols * img.rows) << " %" << std::endl;

    // ================= 进阶 ④ 高斯模糊 + 二值化 =================
    cv::Mat gray, blur, binRaw, binBlur;
    cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(gray, blur, cv::Size(11, 11), 0);
    double t1 = cv::threshold(gray, binRaw,  0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
    double t2 = cv::threshold(blur, binBlur, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
    cv::imwrite(out + "result_binary_raw.png",  binRaw);
    cv::imwrite(out + "result_binary_blur.png", binBlur);
    std::cout << "[4] Otsu 阈值: " << t1 << " -> " << t2 << std::endl;

    // ================= 进阶 ⑤ Canny 边缘 =================
    cv::Mat edges;
    cv::Canny(blur, edges, 60, 160);
    cv::imwrite(out + "result_canny.png", edges);
    std::cout << "[5] Canny 边缘像素: " << cv::countNonZero(edges) << std::endl;

    // ================= 主任务：连通域统计 =================
    cv::Mat labels, stats, centroids;
    int n = cv::connectedComponentsWithStats(binBlur, labels, stats, centroids, 8, CV_32S);
    int total = img.cols * img.rows;
    int obstacle = cv::countNonZero(binBlur);

    std::vector<int> idx;
    for (int i = 1; i < n; ++i) idx.push_back(i);
    std::sort(idx.begin(), idx.end(), [&](int a, int b) {
        return stats.at<int>(a, cv::CC_STAT_AREA) > stats.at<int>(b, cv::CC_STAT_AREA);
    });

    std::cout << "\n[主任务] 连通域: " << (n - 1)
              << "，障碍像素 " << obstacle << " (" << 100.0 * obstacle / total << "%)"
              << "，可通行 " << 100.0 * (total - obstacle) / total << "%" << std::endl;

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
    cv::imwrite(out + "result_regions.jpg", vis);

    std::cout << "\n已保存 6 张结果图到 " << out << std::endl;
    return 0;
}
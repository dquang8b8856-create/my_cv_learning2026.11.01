#include "demo_components.h"
#include <algorithm>
#include <iostream>
#include <vector>

namespace demo {

cv::Mat components(const cv::Mat& src, const std::string& outDir)
{
    cv::Mat gray, blur, bin;
    cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(gray, blur, cv::Size(11, 11), 0);
    cv::threshold(blur, bin, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

    cv::Mat labels, stats, centroids;
    int n = cv::connectedComponentsWithStats(bin, labels, stats, centroids, 8, CV_32S);

    std::vector<int> idx;
    for (int i = 1; i < n; ++i) idx.push_back(i);
    std::sort(idx.begin(), idx.end(), [&](int a, int b) {
        return stats.at<int>(a, cv::CC_STAT_AREA) > stats.at<int>(b, cv::CC_STAT_AREA);
    });

    cv::Mat vis = src.clone();
    int top = std::min<int>(5, static_cast<int>(idx.size()));
    for (int k = 0; k < top; ++k)
    {
        int i = idx[k];
        int area = stats.at<int>(i, cv::CC_STAT_AREA);
        cv::Rect r(stats.at<int>(i, cv::CC_STAT_LEFT),  stats.at<int>(i, cv::CC_STAT_TOP),
                   stats.at<int>(i, cv::CC_STAT_WIDTH), stats.at<int>(i, cv::CC_STAT_HEIGHT));

        cv::rectangle(vis, r, cv::Scalar(0, 255, 0), 4);
        cv::putText(vis, "#" + std::to_string(k + 1) + " " + std::to_string(area),
                    cv::Point(r.x + 8, std::max(34, r.y + 34)),
                    cv::FONT_HERSHEY_SIMPLEX, 1.1, cv::Scalar(0, 255, 0), 3);
    }

    cv::imwrite(outDir + "06_components.jpg", vis);
    std::cout << "[连通域] 共 " << (n - 1) << " 个" << std::endl;
    return vis;
}

}
# 计算机视觉课程 · 知识点总结（markdown01）

> 覆盖本仓库四个部分：CMake 构建、OpenCV 图像处理、C++/STL 编程、开发环境与 Git。
> 每节末尾附「踩过的坑」，都是实际调试中真实遇到的。

---

## 一、CMake 构建系统

### 1.1 CMake 是什么

CMake 本身不编译代码，它**读取 `CMakeLists.txt`，生成对应平台的构建文件**（Linux 下是 Makefile/Ninja，Windows 下是 .sln/.vcxproj），再由 make/ninja/MSBuild 真正编译。

```
CMakeLists.txt ──cmake──> Makefile ──make──> 可执行文件
```

好处：一份配置，Windows / Linux 通用，不用手写一长串 `g++ -I... -L... -l...`。

### 1.2 最小工程（3 行）

```cmake
cmake_minimum_required(VERSION 3.10)   # 要求的最低 CMake 版本
project(hello)                          # 工程名
add_executable(hello main.cpp)          # 用 main.cpp 生成可执行文件 hello
```

构建流程：

```bash
cmake -S . -B build        # -S 源码目录  -B 构建目录（生成 Makefile 到 build/）
cmake --build build        # 真正编译
./build/hello              # 运行
```

> `-S` / `-B` 是现代写法；老写法是先 `mkdir build && cd build && cmake ..`。

### 1.3 链接 OpenCV（5 行）

```cmake
cmake_minimum_required(VERSION 3.10)
project(opencv_test)

find_package(OpenCV REQUIRED)                              # 找 OpenCV，找不到就报错
add_executable(opencv_test main.cpp)
target_link_libraries(opencv_test PRIVATE ${OpenCV_LIBS})   # 链接库
```

`find_package` 成功后会提供两个变量：

| 变量 | 含义 |
|---|---|
| `OpenCV_INCLUDE_DIRS` | 头文件目录（如 `/usr/include/opencv4`） |
| `OpenCV_LIBS` | 要链接的库列表（core / imgproc / imgcodecs / highgui ...） |

### 1.4 多文件工程

```cmake
add_executable(vision_demo
  main.cpp
  src/step1_roi.cpp
  src/step2_resize.cpp
  ...
)
target_include_directories(vision_demo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
```

**文件写多了不会自动加**——新增 .cpp 必须在这里列出来，否则链接时会报「未定义的引用」。

### 1.5 把路径编进程序

```cmake
target_compile_definitions(vision_demo PRIVATE SRC_DIR="${CMAKE_CURRENT_SOURCE_DIR}")
```

```cpp
#ifndef SRC_DIR
#define SRC_DIR "."
#endif
cv::imread(std::string(SRC_DIR) + "/train.jpg");
```

这样不管从哪个目录运行程序，都能找到图片（否则在 `build/` 里运行就找不到）。

### 1.6 其他常用命令

| 命令 | 作用 |
|---|---|
| `set(CMAKE_CXX_STANDARD 17)` | 指定 C++ 标准 |
| `if(MSVC) add_compile_options(/utf-8) endif()` | Windows 下源码里有中文必须加，否则乱码报错 |
| `file(MAKE_DIRECTORY .../results)` | 配置阶段建好输出目录（`imwrite` 不会自动建目录） |
| `message(STATUS "...")` | 配置时打印信息，调试用 |
| `-DCMAKE_BUILD_TYPE=Release` | 指定构建类型 |

### 1.7 踩过的坑

| 现象 | 原因 | 解决 |
|---|---|---|
| `undefined reference to cv::...` | 没链接库 | 加 `target_link_libraries(... ${OpenCV_LIBS})` |
| `fatal error: opencv2/opencv.hpp: No such file` | 找不到头文件 | `libopencv-dev` 没装，或没加 `target_include_directories` |
| Windows 下 `LNK2019 ... debug_build_guard` | Debug/Release 混用 | 统一 `-DCMAKE_BUILD_TYPE=Release` |
| 程序里中文乱码/报奇怪的语法错误 | 源文件 UTF-8，MSVC 默认 GBK | `add_compile_options(/utf-8)` |
| 在 `build/` 里运行读不到图片 | 相对路径 | 用 `SRC_DIR` 宏传绝对路径 |
| 改了 `add_executable` 但没生效 | 该行写在 `add_executable` 之后 | 编译选项要写在目标定义之前 |

---

## 二、OpenCV 图像处理

### 2.1 Mat：OpenCV 的图像容器

```cpp
cv::Mat img = cv::imread("train.jpg");   // 读图，失败返回空 Mat
if (img.empty()) { /* 处理失败 */ }

img.cols;      // 宽
img.rows;      // 高
img.channels();// 通道数（彩色 3，灰度 1）
```

- 图像在内存中是**行优先**的二维数组；
- `cv::Mat` 是**引用计数**的：`cv::Mat b = a;` 只是浅拷贝（共享数据），要独立副本用 `a.clone()`。

### 2.2 读写与显示

```cpp
cv::Mat img = cv::imread("in.jpg");     // 读
cv::imwrite("out.png", img);            // 写，格式由扩展名决定
cv::imshow("窗口名", img);              // 显示
cv::waitKey(0);                          // 等待按键（0 = 一直等）
cv::destroyAllWindows();                 // 关闭所有窗口
```

> `imshow` 必须有图形界面；服务器/无显示器环境会报错。

### 2.3 颜色空间

| 空间 | 通道 | 说明 |
|---|---|---|
| BGR | 3 | OpenCV 默认顺序（**不是 RGB**），每个 0~255 |
| GRAY | 1 | 灰度，0 黑 ~ 255 白 |
| HSV | 3 | H 色调 0~180、S 饱和度 0~255、V 明度 0~255 |

```cpp
cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);
cv::cvtColor(img, hsv,  cv::COLOR_BGR2HSV);
```

**为什么筛颜色要用 HSV**：BGR 三个通道都随光照变化，很难用一个区间描述「红色」；HSV 把「是什么颜色」和「多亮」分开了，阈值稳定得多。

### 2.4 ROI 裁剪与缩放

```cpp
cv::Rect box(580, 255, 580, 921);   // x, y, width, height
cv::Mat roi = img(box).clone();     // 注意 clone()，否则和原图共享内存

cv::resize(img, small, cv::Size(), 0.5, 0.5, cv::INTER_AREA);  // 缩小用 INTER_AREA
```

### 2.5 颜色筛选：inRange

```cpp
cv::inRange(hsv, cv::Scalar(0, 80, 60), cv::Scalar(12, 255, 255), m1);    // 红 H 靠近 0
cv::inRange(hsv, cv::Scalar(168, 80, 60), cv::Scalar(180, 255, 255), m2); // 红 H 靠近 180
cv::bitwise_or(m1, m2, mask);                       // 两段合并
cv::bitwise_and(img, img, redOnly, mask);           // 只保留红色区域
```

**关键点：红色的 H 跨 0 点**（0~12 和 168~180 两段），只写一段会漏掉一半。这是 HSV 筛红色最容易错的地方。

### 2.6 灰度化 → 高斯模糊 → 二值化

```cpp
cv::Mat gray, blur, bin;
cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);
cv::GaussianBlur(gray, blur, cv::Size(11, 11), 0);       // 核必须是奇数
double th = cv::threshold(blur, bin, 0, 255,
                          cv::THRESH_BINARY | cv::THRESH_OTSU);
```

- **Otsu**：自动找最佳阈值，把「类间方差最大」的那个值当作阈值；
- 阈值参数传 `0` 即可（Otsu 会覆盖它），返回值就是它算出的阈值；
- **模糊的作用**：抑制噪声和纹理，让二值化后的连通区域更完整（本作业里阈值从 78 降到 75，差异 10 万像素基本都是墙面纹理）。

### 2.7 边缘检测：Canny

```cpp
cv::Canny(blur, edges, 60, 160);   // 低阈值, 高阈值
```

- 输入应该是**已经模糊过的灰度图**（Canny 内部也有 Sobel，但对噪声仍敏感）；
- 双阈值：强边缘（>160）保留，弱边缘（60~160）只有连着强边缘才保留。

### 2.8 连通域统计

```cpp
cv::Mat labels, stats, centroids;
int n = cv::connectedComponentsWithStats(bin, labels, stats, centroids, 8, CV_32S);
```

| 输出 | 内容 |
|---|---|
| `n` | 连通域个数（**包含 0 号背景**，所以前景是 n-1 个） |
| `labels` | 每个像素属于哪个连通域（同尺寸的标签图） |
| `stats` | 每个连通域的统计信息，`stats.at<int>(i, 字段)` |
| `centroids` | 每个连通域的质心，`centroids.at<double>(i, 0/1)` |

`stats` 的字段常量：

| 常量 | 含义 |
|---|---|
| `cv::CC_STAT_LEFT` / `TOP` | 外接矩形左上角 x / y |
| `cv::CC_STAT_WIDTH` / `HEIGHT` | 外接矩形宽 / 高 |
| `cv::CC_STAT_AREA` | 像素个数 |

**面积排序找主体**：把连通域按 `AREA` 从大到小排序，最大的通常是背景/墙面，第二大的往往就是画面主体（本作业里 #2 面积 131347，正好框住雕像）。

### 2.9 在图上画框和文字

```cpp
cv::rectangle(img, rect, cv::Scalar(0, 255, 0), 4);          // 绿色框，线宽 4
cv::putText(img, "text", cv::Point(x, y),
            cv::FONT_HERSHEY_SIMPLEX, 1.1, cv::Scalar(0, 255, 0), 3);
```

> 注意 `Scalar` 的顺序是 **B, G, R**，`(0,255,0)` 是绿色。

### 2.10 踩过的坑

| 现象 | 原因 | 解决 |
|---|---|---|
| `imwrite` 没报错但没生成文件 | 目录不存在 | 先 `file(MAKE_DIRECTORY ...)` 或手动建目录 |
| 红色只筛出一半 | H 跨 0 点 | 两段 `inRange` 再 `bitwise_or` |
| 二值化后满屏碎点 | 墙面纹理/噪声 | 先高斯模糊再 Otsu |
| Canny 结果很脏 | 直接对原图做 | 先 `GaussianBlur` 再 `Canny` |
| `stats.at<int>` 崩溃 | 用了错误的类型（`stats` 是 CV_32S，必须 `at<int>`；`centroids` 是 CV_64F，必须 `at<double>`） | 按类型取 |
| 遍历连通域时从 0 开始 | 0 号是背景 | 从 1 开始 |

---

## 三、C++ 与 STL

### 3.1 vector 与排序去重

```cpp
std::vector<int> arr;
int x;
while (std::cin >> x) arr.push_back(x);   // 读到 EOF

std::sort(arr.begin(), arr.end());         // 默认升序

// 方法一：unique + erase（必须先排序）
arr.erase(std::unique(arr.begin(), arr.end()), arr.end());

// 方法二：相邻比较，自己攒一个新数组
for (size_t i = 1; i < arr.size(); ++i)
    if (arr[i] != arr[i - 1]) result.push_back(arr[i]);
```

- `std::unique` 只是把**相邻**的重复元素移到后面，返回去重后末尾的迭代器，**不改变 size**，所以要配合 `erase`；
- 没排序就 `unique`，去不干净。

### 3.2 stringstream 分词

```cpp
std::string line;
std::getline(std::cin, line);      // 读一整行（包含空格）

std::stringstream ss(line);
std::string word;
std::vector<std::string> words;
while (ss >> word) words.push_back(word);   // 按空白自动切分
```

`getline` 和 `>>` 的区别：`>>` 遇到空格就停，`getline` 读到换行。

### 3.3 逆序遍历的坑

```cpp
for (int i = words.size() - 1; i >= 0; i--)   // ⚠️ 有隐患
```

`words.size()` 返回 `size_t`（无符号），**空 vector 时 `size()-1` 会变成极大值**，循环直接越界。

安全写法：

```cpp
for (int i = static_cast<int>(words.size()) - 1; i >= 0; --i)
// 或者
for (size_t i = words.size(); i-- > 0; )
```

### 3.4 输入读法的兼容性

题目样例 `5 3 1 2 1` 有歧义：`5` 是个数还是数据？

```cpp
// 先全读进来，再判断第一个数是不是「个数」
while (std::cin >> x) arr.push_back(x);
if (!arr.empty() && (long long)arr[0] == (long long)arr.size() - 1)
    arr.erase(arr.begin());
```

两种输入都能正确处理，避免因为样例理解偏差而错。

### 3.5 输出格式：行尾空格

```cpp
for (size_t i = 0; i < arr.size(); ++i)
{
    if (i) std::cout << " ";     // 第一个元素前不加空格
    std::cout << arr[i];
}
std::cout << std::endl;
```

判题系统做严格文本比对时，行尾多一个空格也算错。

### 3.6 类与对象：深拷贝 vs 浅拷贝

```cpp
class Student {
private:
    std::string name, id;
    int* scores;        // 动态数组
    int numScores;
public:
    Student(std::string n, std::string i, int num, int s1, int s2, int s3);
    Student(const Student& other);   // 拷贝构造（深拷贝）
    ~Student();                      // 析构（释放内存）
    void print();
    void setScore(int index, int score);
};
```

**深拷贝**：重新 `new` 一块内存，逐元素复制。

```cpp
Student::Student(const Student& other)
{
    name = other.name;
    id = other.id;
    numScores = other.numScores;
    scores = new int[numScores];                 // 自己的内存
    for (int i = 0; i < numScores; i++)
        scores[i] = other.scores[i];             // 逐元素复制
}
```

**如果交给编译器默认的浅拷贝**：

```cpp
stu2.scores = stu1.scores;    // 只复制指针，指向同一块内存
```

后果有两个：

1. 改 `stu1` 的成绩会连带改掉 `stu2`，两个对象不再独立；
2. 析构时两个对象对**同一块内存各 `delete[]` 一次** → `double free or corruption`，程序崩溃。

**结论：类里管理了动态内存，就必须自己写深拷贝构造（必要时还要写拷贝赋值运算符，遵循「三法则」）。**

### 3.7 踩过的坑

| 现象 | 原因 | 解决 |
|---|---|---|
| 输出 `1 1 1 2 3`（多了一个 1） | `cin >> x` 失败后 x 保留旧值仍被 push | 检查读取循环的条件 |
| 编译警告 `C4267 size_t → int` | 类型窄化 | `static_cast<int>` |
| 程序结束崩溃 / 报 double free | 浅拷贝后重复释放 | 写深拷贝构造函数 |
| 构造函数里 `scores[0..2]` 硬编码 | 传 `num != 3` 就越界 | 用数组/vector 传参 |

---

## 四、开发环境

### 4.1 虚拟机环境（Ubuntu 24.04）

| 组件 | 版本 | 安装 |
|---|---|---|
| g++ | 13.3.0 | `sudo apt install build-essential` |
| CMake | 3.28.3 | `sudo apt install cmake` |
| OpenCV | 4.11.0 | `sudo apt install libopencv-dev` |

检查是否装好：

```bash
cmake --version
g++ --version
pkg-config --modversion opencv4
```

### 4.2 VS Code 插件

| 插件 | 作用 |
|---|---|
| **CMake Tools** | 底部状态栏提供 Select a Kit / Build / Run |
| **C/C++** | 语法提示、调试（IntelliSense） |

使用流程：打开文件夹 → 状态栏 `Select a Kit` 选 `GCC x86_64-linux-gnu` → `F7` 编译 → `Shift+F5` 运行。

### 4.3 常见问题

| 现象 | 原因 | 解决 |
|---|---|---|
| `#include <opencv2/opencv.hpp>` 爆红，但能编译 | IntelliSense 不知道 OpenCV 头文件在哪 | 建 `.vscode/c_cpp_properties.json`，`includePath` 加 `/usr/include/opencv4` |
| 右上角 ▶ 运行失败 | 那是 **Code Runner** 插件，只会 `g++ main.cpp`，不认识 CMake | 用终端 `cmake --build build && ./build/xxx`，或底部状态栏的 Run |
| `spawn ninja ENOENT` | 没装 ninja，CMake Tools 自动退回 Unix Makefiles | 不影响，可忽略 |
| `生成的生成配置不包含活动生成配置` | CMake 没配置过 / 没选 Kit | 先 `CMake: Configure`，或忽略（不影响编译） |
| 共享文件夹不见了 | VMware 临时挂载，重启失效 | `sudo vmhgfs-fuse .host:/ /mnt/hgfs -o allow_other`，或写进 `/etc/fstab` |

---

## 五、Git 与 GitHub

### 5.1 基本流程

```bash
git init                                   # 初始化仓库
git add .                                  # 暂存所有改动
git commit -m "说明"                       # 提交
git branch -M main                         # 分支改名 main
git remote add origin <仓库地址>           # 关联远程
git push -u origin main                    # 首次推送
git push                                   # 之后推送
```

查看状态：

```bash
git status          # 哪些文件改了
git log --oneline   # 提交历史
git ls-files        # 仓库里跟踪的文件
git diff            # 具体改动
```

### 5.2 删除与恢复

```bash
git rm -r 目录/            # 删除并暂存
git rm --cached 文件       # 只从仓库移除，保留本地
git checkout <提交> -- 路径 # 从历史恢复某个文件
git revert <提交>          # 生成一个反向提交（安全）
```

> 只要提交过，删掉的东西都能从历史里找回来。

### 5.3 SSH 推送（绕开 HTTPS 被墙）

```bash
ssh-keygen -t ed25519 -C "备注" -f 密钥路径 -N ""
# 把 xxx.pub 的内容贴到 GitHub → Settings → SSH and GPG keys

ssh -T -p 443 git@ssh.github.com      # 测试连通
git clone ssh://git@ssh.github.com:443/用户名/仓库.git
```

端口 **443** 是 GitHub 提供的 SSH over HTTPS 端口，很多网络会放行 443 但拦截 22。

### 5.4 .gitignore

```
build/
*.o
*.exe
```

编译产物不进仓库，仓库只留源码和必要资源。

---

## 六、整体知识地图

```
                 ┌──────────────┐
                 │ CMakeLists   │  构建配置
                 └──────┬───────┘
                        │ cmake
                 ┌──────▼───────┐
                 │ 编译 + 链接   │  g++ / MSVC
                 └──────┬───────┘
                        │
        ┌───────────────┼────────────────┐
        │               │                │
   ┌────▼────┐    ┌─────▼─────┐   ┌──────▼──────┐
   │ C++/STL │    │  OpenCV   │   │  VS Code    │
   │ 容器/类 │    │ 图像处理   │   │ 插件/调试    │
   └────┬────┘    └─────┬─────┘   └──────┬──────┘
        │               │                │
        └───────────────┼────────────────┘
                        │
                 ┌──────▼───────┐
                 │ Git / GitHub │  代码托管
                 └──────────────┘
```

**一条完整的视觉任务链路**：

```
读图 imread
  → 转灰度/HSV cvtColor
  → 去噪 GaussianBlur
  → 分割 threshold / inRange
  → 形态学/边缘 Canny
  → 找区域 connectedComponentsWithStats / findContours
  → 画框标注 rectangle / putText
  → 保存 imwrite
```

---

## 附：常用参数速查

| 项目 | 本作业取值 | 说明 |
|---|---|---|
| 高斯核 | 11×11 | 必须是奇数，越大越模糊 |
| Otsu 阈值 | 78（原图）/ 75（模糊后） | 自动计算 |
| Canny 双阈值 | 60 / 160 | 低 : 高 ≈ 1 : 2~3 |
| 红色 H 范围 | 0~12，168~180 | 跨 0 点，两段合并 |
| S / V 下限 | 80 / 60 | 滤掉灰白和过暗像素 |
| ROI 区域 | (580,255) 起，580×921 | x, y, w, h |
| 缩放 | 0.5 倍，INTER_AREA | 缩小用面积插值 |
| 连通域连通性 | 8 | 8 邻域 |

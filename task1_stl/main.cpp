/**
 * ============================================================
 *  第 1 题 · STL 排序与去重
 *  功能：读入若干整数存入 vector，先输出排序结果，再输出去重结果
 *        去重用「相邻比较」自行实现（也可用 set）
 *  说明：输入兼容两种写法
 *        ① 直接给数据：5 3 1 2 1
 *        ② 先给个数 n：5 3 1 2 1 5
 *  环境：C++17 + CMake
 * ============================================================
 */
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main()
{
    // 先把所有整数读进来
    vector<int> arr;
    int x;
    while (cin >> x)
    {
        arr.push_back(x);
    }

    // 若第一个数刚好等于后面元素的个数，说明它是「个数 n」，去掉它
    if (!arr.empty() && (long long)arr[0] == (long long)arr.size() - 1)
    {
        arr.erase(arr.begin());
    }

    // 排序
    sort(arr.begin(), arr.end());

    for (size_t i = 0; i < arr.size(); i++)
    {
        if (i) cout << " ";
        cout << arr[i];
    }
    cout << endl;

    // 去重：相邻比较
    vector<int> result;

    if (!arr.empty())
    {
        result.push_back(arr[0]);

        for (size_t i = 1; i < arr.size(); i++)
        {
            if (arr[i] != arr[i - 1])
            {
                result.push_back(arr[i]);
            }
        }
    }

    for (size_t i = 0; i < result.size(); i++)
    {
        if (i) cout << " ";
        cout << result[i];
    }
    cout << endl;

    return 0;
}
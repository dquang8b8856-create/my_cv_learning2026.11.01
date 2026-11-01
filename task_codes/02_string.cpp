/**
 * ============================================================
 *  第 2 题 · 字符串处理
 *  功能：读入一行英文句子，统计单词个数，并按逆序输出每个单词
 *        用 getline + stringstream 拆分单词
 *  环境：C++17 + CMake
 * ============================================================
 */
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

int main()
{
    string s;
    getline(cin, s);

    stringstream ss(s);
    string word;
    vector<string> words;

    while (ss >> word)
    {
        words.push_back(word);
    }

    cout << "单词数量：" << words.size() << endl;

    for (int i = words.size() - 1; i >= 0; i--)
    {
        cout << words[i] << " ";
    }

    cout << endl;

    return 0;
}
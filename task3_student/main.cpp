/**
 * ============================================================
 *  第 3 题 · 类与对象进阶
 *  功能：Student 类（动态成绩数组 / 深拷贝构造 / 析构 / 打印）
 *        验证 stu1 与 stu2 互不影响，证明深拷贝生效
 *  环境：C++17 + CMake
 * ============================================================
 */
#include <iostream>
#include <string>
using namespace std;

class Student
{
private:
    string name;
    string id;
    int* scores;
    int numScores;

public:
    Student(string n, string i, int num, int s1, int s2, int s3)
    {
        name = n;
        id = i;
        numScores = num;

        scores = new int[numScores];

        scores[0] = s1;
        scores[1] = s2;
        scores[2] = s3;
    }

    Student(const Student& other)
    {
        name = other.name;
        id = other.id;
        numScores = other.numScores;

        scores = new int[numScores];

        for (int i = 0; i < numScores; i++)
        {
            scores[i] = other.scores[i];
        }
    }

    ~Student()
    {
        delete[] scores;
    }

    void print()
    {
        cout << name << " " << id << " ";

        for (int i = 0; i < numScores; i++)
        {
            cout << scores[i] << " ";
        }

        cout << endl;
    }

    void setScore(int index, int score)
    {
        if (index >= 0 && index < numScores)
        {
            scores[index] = score;
        }
    }
};

int main()
{
    Student stu1("张三", "2025001", 3, 85, 90, 78);

    Student stu2 = stu1;

    cout << "复制前：" << endl;
    cout << "stu1：";
    stu1.print();

    cout << "stu2：";
    stu2.print();

    stu1.setScore(0, 60);

    cout << "\n修改 stu1 第一门成绩后：" << endl;
    cout << "stu1：";
    stu1.print();

    cout << "stu2：";
    stu2.print();

    return 0;
}
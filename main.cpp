#include "StudentManager.h"

#include <iostream>
#include <limits>

using namespace std;

/*
 * 显示主菜单
 */
void showMainMenu()
{
    cout << endl;
    cout << "========================================" << endl;
    cout << "          学生成绩管理系统" << endl;
    cout << "========================================" << endl;
    cout << "1. 录入学生信息" << endl;
    cout << "2. 查询学生信息" << endl;
    cout << "3. 修改学生成绩" << endl;
    cout << "4. 输出全部学生信息" << endl;
    cout << "5. 输出学生总分排名" << endl;
    cout << "6. 删除学生信息" << endl;
    cout << "0. 退出系统" << endl;
    cout << "========================================" << endl;
}

/*
 * 读取主菜单选项
 *
 * 处理输入字母等非法情况，避免菜单进入死循环。
 */
bool readMenuChoice(int& choice)
{
    cout << "请输入菜单选项：";

    if (cin >> choice)
    {
        // 清除行末换行符，方便后续使用 getline()
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return true;
    }

    // 输入流已经结束，直接结束程序
    if (cin.eof())
    {
        cout << endl << "输入结束，程序退出。" << endl;
        return false;
    }

    cout << "输入无效，请输入一个整数。" << endl;
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    return true;
}

int main()
{
    // StudentManager 内部会自动创建和管理
    // StudentList、StudentBST、NameHash 和 RankList
    StudentManager manager;

    int choice = -1;

    while (true)
    {
        showMainMenu();

        if (!readMenuChoice(choice))
        {
            break;
        }

        switch (choice)
        {
        case 1:
            manager.addStudent();
            break;

        case 2:
            manager.queryStudent();
            break;

        case 3:
            manager.modifyScores();
            break;

        case 4:
            manager.displayAll();
            break;

        case 5:
            manager.displayRanking();
            break;

        case 6:
            manager.removeStudent();
            break;

        case 0:
            cout << "感谢使用学生成绩管理系统！" << endl;
            return 0;

        default:
            cout << "菜单选项无效，请重新输入。" << endl;
            break;
        }
    }

    return 0;
}
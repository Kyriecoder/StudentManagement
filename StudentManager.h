#ifndef STUDENTMANAGER_H
#define STUDENTMANAGER_H

#include "Student.h"
#include "StudentList.h"
#include "StudentBST.h"
#include "NameHash.h"
#include "RankList.h"

#include <string>

using namespace std;

/*
 * StudentManager 类：学生成绩管理系统的协调类
 *
 * 负责把四个数据结构组合起来：
 * 1. StudentList：保存完整学生数据；
 * 2. StudentBST：按学号建立索引；
 * 3. NameHash：按姓名建立索引；
 * 4. RankList：维护总分排名。
 *
 * main.cpp 只调用 StudentManager 的公有函数，
 * 不直接操作各个结构的内部指针。
 */
class StudentManager
{
public:
    StudentManager();

    /*
     * 各成员对象会由默认析构函数自动释放：
     * rankList、nameHash、studentBST、studentList。
     */
    ~StudentManager() = default;

    /*
     * StudentManager 中包含不可复制的数据结构，
     * 因此禁止复制整个管理对象。
     */
    StudentManager(const StudentManager&) = delete;
    StudentManager& operator=(const StudentManager&) = delete;

    // 录入一个学生
    void addStudent();

    // 查询学生子菜单，可根据学号或姓名查询
    void queryStudent();

    // 按学号查询学生
    void queryById();

    // 按姓名查询全部同名学生
    void queryByName();

    // 按学号修改四门课程成绩
    void modifyScores();

    // 按学号删除学生
    void removeStudent();

    // 按主链表顺序输出全部学生
    void displayAll();

    // 按总分排名输出全部学生
    void displayRanking();

private:
    // 学生主链表，保存所有完整学生信息
    StudentList studentList;

    // 按学号建立的 BST 索引
    StudentBST studentBST;

    // 按姓名建立的哈希表索引
    NameHash nameHash;

    // 按总分维护的排名链
    RankList rankList;

    /*
     * 用于给新学生分配录入顺序。
     * 总分相同时，orderNo 小的学生排在前面。
     */
    int nextOrderNo;

    /*
     * 标记排名链是否已经建立：
     *
     * false：还没有进行过完整排名，
     *        新录入的学生暂时不加入排名链；
     *
     * true： 排名链已经建立，
     *        新增、修改和删除都需要同步调整排名。
     */
    bool rankBuilt;

    // 如果没有建立排名，则使用归并排序建立一次
    void ensureRankBuilt();

    /*
     * 读取一个整数。
     * 输入失败时会清空输入状态，等待重新输入。
     */
    bool readInt(const string& prompt, int& value) const;

    /*
     * 读取一门成绩。
     * 会一直循环，直到输入 0～100 之间的整数。
     */
    bool readScore(const string& prompt, int& score) const;

    // 读取一行字符串，例如学号、姓名和性别
    string readLine(const string& prompt) const;
};

#endif

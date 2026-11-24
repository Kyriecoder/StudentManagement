#ifndef STUDENT_H
#define STUDENT_H

#include <iostream>
#include <string>


using namespace std;

/*
 * Student 类：保存一个学生的基本信息、四门课程成绩、总分和排名
 *
 * 说明：
 * 1. 学生完整信息只在 Student 对象中保存一次；
 * 2. 后续 BST、HASH 和排名链都保存指向 StudentNode 的指针，
 *    不重复保存 Student 对象；
 * 3. totalScore 由四门成绩计算得到，不允许外部随意修改；
 * 4. rank 为 0 表示该学生还没有进行排名计算。
 /*

 */
class Student
{
public:
    // 无参构造函数，将所有数据初始化为空字符串或 0
    Student();

    /*
     * 带参构造函数
     *
     * 参数：
     * studentId    学号，作为学生的唯一标识
     * name         姓名，可以重复
     * gender       性别
     * english      英语成绩
     * math         数学成绩
     * programDesign 程序设计成绩
     * dataStructure 数据结构成绩
     * orderNo      录入顺序，用于总分相同时保持稳定顺序
     *
     * 注意：
     * 调用构造函数的代码应先使用 isValidScore() 检查四门成绩。
     */
    Student(const string& studentId,
        const string& name,
        const string& gender,
        int english,
        int math,
        int programDesign,
        int dataStructure,
        int orderNo = 0);

    /*
     * 获取学生的基本信息。
     * 返回 const 引用可以避免复制整个字符串，同时禁止通过返回值修改数据。
     */
    const string& getStudentId() const;
    const string& getName() const;
    const string& getGender() const;

    /*
     * 获取四门课程成绩。
     * 每门成绩都应在 0～100 之间。
     */
    int getEnglishScore() const;
    int getMathScore() const;
    int getProgramDesignScore() const;
    int getDataStructureScore() const;

    // 获取由四门成绩计算得到的总分
    int getTotalScore() const;

    // 获取排名，0 表示尚未排名
    int getRank() const;

    /*
     * 获取录入顺序。
     * 排序时若总分相同，可以按照 orderNo 从小到大排列，
     * 从而保证相同成绩学生的先后顺序不被打乱。
     */
    int getOrderNo() const;

    
    /*
     * 一次性修改四门成绩。
     *
     * 修改规则：
     * 1. 四门成绩必须全部合法；
     * 2. 任意一门不合法时，四门成绩都不修改，保证数据一致；
     * 3. 修改成功后自动重新计算总分；
     * 4. 返回值 true 表示修改成功，false 表示失败。
     */
    bool setScores(int english,
        int math,
        int programDesign,
        int dataStructure);

    /*
     * 设置学生排名。
     * 排名从 1 开始，若传入负数则统一保存为 0。
     */
    void setRank(int rank);

    /*
     * 设置录入顺序。
     * 该值由 StudentManager 在录入学生时统一分配，
     * 主要用于稳定排序。
     */
    void setOrderNo(int orderNo);

    /*
     * 静态函数：检查单门成绩是否合法。
     *
     * 合法范围：0～100。
     * 因为不依赖具体学生对象，所以声明为 static。
     */
    static bool isValidScore(int score);

    /*
     * 输出一个学生的信息。
     * display() 只负责打印，不负责查询、修改或排名。
     */
    void display() const;

private:
    // 学号，唯一标识一个学生，不允许重复
    string studentId;

    // 姓名，允许重复，例如可以有多个“张三”
    string name;

    // 性别
    string gender;

    // 四门课程成绩，每门范围为 0～100
    int englishScore;
    int mathScore;
    int programDesignScore;
    int dataStructureScore;

    // 四门课程成绩之和，由 setScores() 自动维护
    int totalScore;

    // 当前排名，0 表示尚未排名
    int rank;

    // 录入顺序，用于总分相同时保持稳定顺序
    int orderNo;
};

#endif
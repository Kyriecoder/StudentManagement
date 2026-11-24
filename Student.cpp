#include "Student.h"

/*
 * 无参构造函数
 *
 * 将字符串初始化为空，将数值初始化为 0。
 * 后续如果需要先创建对象、再填入数据，可以使用这个构造函数。
 */
Student::Student()
    : studentId(""),
    name(""),
    gender(""),
    englishScore(0),
    mathScore(0),
    programDesignScore(0),
    dataStructureScore(0),
    totalScore(0),
    rank(0),
    orderNo(0)
{
}

/*
 * 带参构造函数
 *
 * 根据传入的四门成绩直接计算总分。
 * 学号、姓名和性别在对象创建时确定，之后不提供修改函数，
 * 因为题目要求修改时只修改四门课程成绩。
 */
Student::Student(const string& studentId,
    const string& name,
    const string& gender,
    int english,
    int math,
    int programDesign,
    int dataStructure,
    int orderNo)
    : studentId(studentId),
    name(name),
    gender(gender),
    englishScore(english),
    mathScore(math),
    programDesignScore(programDesign),
    dataStructureScore(dataStructure),
    totalScore(english + math + programDesign + dataStructure),
    rank(0),
    orderNo(orderNo)
{
}

/*
 * 获取学号。
 * 返回 const 引用，避免复制字符串，同时防止外部修改学号。
 */
const string& Student::getStudentId() const
{
    return studentId;
}

/*
 * 获取姓名。
 * 姓名允许重复，姓名查询会由 HASH 表找出所有同名学生。
 */
const string& Student::getName() const
{
    return name;
}

// 获取性别
const string& Student::getGender() const
{
    return gender;
}

// 获取英语成绩
int Student::getEnglishScore() const
{
    return englishScore;
}

// 获取数学成绩
int Student::getMathScore() const
{
    return mathScore;
}

// 获取程序设计成绩
int Student::getProgramDesignScore() const
{
    return programDesignScore;
}

// 获取数据结构成绩
int Student::getDataStructureScore() const
{
    return dataStructureScore;
}

// 获取总分
int Student::getTotalScore() const
{
    return totalScore;
}

// 获取当前排名
int Student::getRank() const
{
    return rank;
}

// 获取录入顺序
int Student::getOrderNo() const
{
    return orderNo;
}

/*
 * 修改四门成绩
 *
 * 执行步骤：
 * 1. 检查四门成绩是否都在 0～100 之间；
 * 2. 只要有一门不合法，立即返回 false；
 * 3. 四门成绩全部合法后，再统一修改成员变量；
 * 4. 重新计算总分，保证成绩和总分始终一致。
 *
 * 这种写法可以避免出现“英语改成功、数学修改失败”
 * 导致对象内部数据不一致的情况。
 */
bool Student::setScores(int english,
    int math,
    int programDesign,
    int dataStructure)
{
    if (!isValidScore(english) ||
        !isValidScore(math) ||
        !isValidScore(programDesign) ||
        !isValidScore(dataStructure))
    {
        return false;
    }

    englishScore = english;
    mathScore = math;
    programDesignScore = programDesign;
    dataStructureScore = dataStructure;

    // 四门成绩都修改成功后重新计算总分
    totalScore = englishScore
        + mathScore
        + programDesignScore
        + dataStructureScore;

    return true;
}

/*
 * 设置排名
 *
 * 有效排名从 1 开始。
 * 如果传入负数，则说明排名无效，统一保存为 0。
 */
void Student::setRank(int rank)
{
    if (rank < 0)
    {
        this->rank = 0;
    }
    else
    {
        this->rank = rank;
    }
}

/*
 * 设置录入顺序
 *
 * orderNo 由 StudentManager 在录入学生时统一生成。
 * 总分相同的同学共享相同排名，
 * 但输出时仍可以按照 orderNo 保持录入时的先后顺序。
 */
void Student::setOrderNo(int orderNo)
{
    if (orderNo < 0)
    {
        this->orderNo = 0;
    }
    else
    {
        this->orderNo = orderNo;
    }
}

/*
 * 静态成绩检查函数
 *
 * 返回 true：成绩在 0～100 之间；
 * 返回 false：成绩小于 0 或大于 100。
 *
 * 因为成绩范围检查不依赖某个具体学生对象，
 * 所以这里使用 static 成员函数。
 */
bool Student::isValidScore(int score)
{
    return score >= 0 && score <= 100;
}

/*
 * 显示一个学生的信息
 *
 * 当前将全部信息输出在同一行中。
 * rank 为 0 时仍然显示 0，表示该学生尚未参与排名计算。
 */
void Student::display() const
{
    cout << "学号：" << studentId
        << "  姓名：" << name
        << "  性别：" << gender
        << "  英语：" << englishScore
        << "  数学：" << mathScore
        << "  程序设计：" << programDesignScore
        << "  数据结构：" << dataStructureScore
        << "  总分：" << totalScore
        << "  排名：" << rank
        << endl;
}
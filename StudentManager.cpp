#include "StudentManager.h"

#include <iostream>
#include <limits>

/*
 * StudentManager 构造函数
 *
 * nextOrderNo 从 1 开始，
 * rankBuilt 初始为 false，表示排名链还没有建立。
 */
StudentManager::StudentManager()
    : studentList(),
    studentBST(),
    nameHash(),
    rankList(),
    nextOrderNo(1),
    rankBuilt(false)
{
}

/*
 * 读取一个整数
 *
 * 使用循环处理非数字输入，避免输入失败后菜单陷入死循环。
 * 输入失败时：
 * 1. 清除 cin 的错误状态；
 * 2. 丢弃当前行剩余内容；
 * 3. 提示用户重新输入。
 */
bool StudentManager::readInt(const string& prompt, int& value) const
{
    while (true)
    {
        cout << prompt;

        if (cin >> value)
        {
            // 清除行末换行符，方便后面使用 getline()
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return true;
        }

        // 输入流已经结束，无法继续读取
        if (cin.eof())
        {
            cout << endl << "输入已结束。" << endl;
            return false;
        }

        cout << "输入不是整数，请重新输入。" << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

/*
 * 读取一门成绩
 *
 * 成绩必须是整数，并且范围在 0～100 之间。
 * 不合法时会一直提示重新输入。
 */
bool StudentManager::readScore(const string& prompt, int& score) const
{
    while (true)
    {
        int inputScore = 0;

        if (!readInt(prompt, inputScore))
        {
            return false;
        }

        if (Student::isValidScore(inputScore))
        {
            score = inputScore;
            return true;
        }

        cout << "成绩必须在 0～100 之间，请重新输入。" << endl;
    }
}

/*
 * 读取一行字符串
 *
 * 学号使用 string，可以保留前导零；
 * 姓名也可以正常读取带空格的字符串。
 */
string StudentManager::readLine(const string& prompt) const
{
    cout << prompt;

    string value;
    getline(cin, value);

    return value;
}

/*
 * 录入学生
 *
 * 执行过程：
 * 1. 读取学号、姓名和性别；
 * 2. 检查学号和姓名是否为空；
 * 3. 通过 BST 检查学号是否重复；
 * 4. 读取四门成绩；
 * 5. 创建 Student 对象；
 * 6. 加入学生主链表；
 * 7. 加入 BST 和姓名 HASH；
 * 8. 如果排名链已经建立，再加入排名链。
 */
void StudentManager::addStudent()
{
    string studentId = readLine("请输入学号：");
    string name = readLine("请输入姓名：");
    string gender = readLine("请输入性别：");

    if (studentId.empty())
    {
        cout << "学号不能为空。" << endl;
        return;
    }

    if (name.empty())
    {
        cout << "姓名不能为空。" << endl;
        return;
    }

    if (gender.empty())
    {
        cout << "性别不能为空。" << endl;
        return;
    }

    // 学号是唯一关键字，BST 中找到相同学号就不能继续录入
    if (studentBST.findById(studentId) != nullptr)
    {
        cout << "该学号已经存在，不能重复录入。" << endl;
        return;
    }

    int english = 0;
    int math = 0;
    int programDesign = 0;
    int dataStructure = 0;

    if (!readScore("请输入英语成绩：", english) ||
        !readScore("请输入数学成绩：", math) ||
        !readScore("请输入程序设计成绩：", programDesign) ||
        !readScore("请输入数据结构成绩：", dataStructure))
    {
        return;
    }

    /*
     * 创建 Student 对象。
     * nextOrderNo 用于同分时保持录入顺序。
     */
    Student student(studentId,
        name,
        gender,
        english,
        math,
        programDesign,
        dataStructure,
        nextOrderNo);

    /*
     * 当前的 StudentList::append() 返回 void，
     * 所以加入主链表后再根据学号取得新节点指针。
     */
    studentList.append(student);
    StudentNode* newNode = studentList.findById(studentId);

    if (newNode == nullptr)
    {
        cout << "学生节点创建失败。" << endl;
        return;
    }

    /*
     * 插入 BST。
     * 正常情况下前面的重复检查已经通过，这里插入应当成功。
     * 如果仍然失败，需要撤销主链表中的新节点。
     */
    if (!studentBST.insert(newNode))
    {
        studentList.removeById(studentId);
        cout << "插入学号索引失败。" << endl;
        return;
    }

    /*
     * 插入姓名 HASH。
     * HASH 允许同名学生，所以不同学生即使姓名相同也能插入。
     */
    if (!nameHash.insert(newNode))
    {
        studentBST.removeById(studentId);
        studentList.removeById(studentId);
        cout << "插入姓名索引失败。" << endl;
        return;
    }

    /*
     * 如果排名链已经建立，
     * 新学生需要立即插入正确排名位置。
     */
    if (rankBuilt && !rankList.insertStudent(newNode))
    {
        nameHash.remove(newNode);
        studentBST.removeById(studentId);
        studentList.removeById(studentId);
        cout << "插入排名链失败。" << endl;
        return;
    }

    // 所有结构都插入成功后才增加录入顺序
    ++nextOrderNo;
    if (!rankBuilt)
    {
        ensureRankBuilt();
    }

    cout << "学生录入成功。" << endl;
    newNode->data.display();
}

/*
 * 按学号查询学生
 */
void StudentManager::queryById()
{
    string studentId = readLine("请输入要查询的学号：");

    StudentNode* studentNode = studentBST.findById(studentId);

    if (studentNode == nullptr)
    {
        cout << "没有找到该学号对应的学生。" << endl;
        return;
    }

    studentNode->data.display();
}

/*
 * 按姓名查询全部同名学生
 *
 * NameHash::displayByName() 会遍历完整的哈希桶，
 * 并输出全部姓名相同的学生。
 */
void StudentManager::queryByName()
{
    string name = readLine("请输入要查询的姓名：");

    int count = nameHash.displayByName(name);

    if (count > 0)
    {
        cout << "共找到 " << count << " 名姓名为“"
            << name << "”的学生。" << endl;
    }
}

/*
 * 查询学生子菜单
 */
void StudentManager::queryStudent()
{
    while (true)
    {
        cout << endl;
        cout << "------ 学生查询 ------" << endl;
        cout << "1. 按学号查询" << endl;
        cout << "2. 按姓名查询" << endl;
        cout << "0. 返回上级菜单" << endl;

        int choice = 0;

        if (!readInt("请选择：", choice))
        {
            return;
        }

        switch (choice)
        {
        case 1:
            queryById();
            break;

        case 2:
            queryByName();
            break;

        case 0:
            return;

        default:
            cout << "输入无效，请重新选择。" << endl;
        }
    }
}

/*
 * 按学号修改四门成绩
 *
 * 学号和姓名不能修改，因此 BST 和 HASH 不需要重建。
 * 如果总分变了，并且排名链已经建立，
 * 再调用 RankList::updateStudent() 调整排名。
 */
void StudentManager::modifyScores()
{
    string studentId = readLine("请输入要修改成绩的学号：");

    StudentNode* studentNode = studentBST.findById(studentId);

    if (studentNode == nullptr)
    {
        cout << "没有找到该学号对应的学生。" << endl;
        return;
    }

    cout << "当前信息：" << endl;
    studentNode->data.display();

    int oldTotal = studentNode->data.getTotalScore();

    int english = 0;
    int math = 0;
    int programDesign = 0;
    int dataStructure = 0;

    if (!readScore("请输入新的英语成绩：", english) ||
        !readScore("请输入新的数学成绩：", math) ||
        !readScore("请输入新的程序设计成绩：", programDesign) ||
        !readScore("请输入新的数据结构成绩：", dataStructure))
    {
        return;
    }

    /*
     * setScores() 会先检查四门成绩，
     * 全部合法后统一修改，并自动重新计算总分。
     */
    if (!studentNode->data.setScores(english,
        math,
        programDesign,
        dataStructure))
    {
        cout << "成绩不合法，修改失败。" << endl;
        return;
    }

    int newTotal = studentNode->data.getTotalScore();

    /*
     * 排名链已经建立时，立即调整该学生在排名链中的位置。
     * 如果还没有建立排名，不用处理，
     * 以后调用 buildFromList() 时会使用最新总分。
     */
    if (rankBuilt)
    {
        rankList.updateStudent(studentNode);
    }

    cout << "成绩修改成功。" << endl;
    cout << "原总分：" << oldTotal
        << "，新总分：" << newTotal << endl;

    studentNode->data.display();
}

/*
 * 按学号删除学生
 *
 * 删除顺序：
 * 1. 从排名链删除；
 * 2. 从姓名 HASH 删除；
 * 3. 从学号 BST 删除；
 * 4. 最后从学生主链表删除 StudentNode。
 */
void StudentManager::removeStudent()
{
    string studentId = readLine("请输入要删除的学号：");

    StudentNode* studentNode = studentBST.findById(studentId);

    if (studentNode == nullptr)
    {
        cout << "没有找到该学号对应的学生。" << endl;
        return;
    }

    /*
     * 保存删除前的信息，方便输出提示。
     * 删除主链表节点后，studentNode 会变成悬空指针，
     * 因此下面要先把需要的信息保存下来。
     */
    string studentName = studentNode->data.getName();

    if (rankBuilt)
    {
        rankList.removeStudent(studentNode);
    }

    nameHash.remove(studentNode);
    studentBST.removeById(studentId);
    studentList.removeById(studentId);

    cout << "已删除学生：" << studentName
        << "，学号：" << studentId << endl;
}

/*
 * 输出全部学生
 *
 * 按照学生主链表的录入顺序输出，
 * 不会按照总分重新排序。
 */
void StudentManager::displayAll()
{
    cout << endl;
    cout << "------ 全部学生信息 ------" << endl;

    studentList.displayAll();
}

/*
 * 保证排名链已经建立
 *
 * 第一次查看排名时才建立完整排名，
 * 使用 RankList::buildFromList() 进行一次稳定二路归并排序。
 * 以后排名链已经存在，不再重复排序。
 */
void StudentManager::ensureRankBuilt()
{
    if (!rankBuilt)
    {
        rankList.buildFromList(studentList);
        rankBuilt = true;
    }
}

/*
 * 输出总分排名
 *
 * 如果排名链尚未建立，先建立排名；
 * 然后直接遍历排名链输出，不重新排序。
 */
void StudentManager::displayRanking()
{
    ensureRankBuilt();

    cout << endl;
    cout << "------ 学生总分排名 ------" << endl;

    rankList.displayRanking();
}
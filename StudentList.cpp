#include "StudentList.h"

/*
 * StudentNode 构造函数
 *
 * 参数 student 会被复制到当前节点的 data 中。
 * 这里真正保存一份完整的学生信息。
 */
StudentNode::StudentNode(const Student& student)
    : data(student),
    next(nullptr)
{
}

/*
 * StudentList 构造函数
 *
 * 初始状态：
 * 1. 没有头节点；
 * 2. 没有尾节点；
 * 3. 学生数量为 0。
 */
StudentList::StudentList()
    : head(nullptr),
    tail(nullptr),
    studentCount(0)
{
}

/*
 * StudentList 析构函数
 *
 * 程序结束或 StudentList 对象被销毁时，
 * 调用 clear() 逐个释放学生节点，防止内存泄漏。
 */
StudentList::~StudentList()
{
    clear();
}

// 如果头指针为空，说明链表中没有任何学生节点
bool StudentList::isEmpty() const
{
    return head == nullptr;
}

// 返回当前链表中的学生数量
int StudentList::getSize() const
{
    return studentCount;
}

/*
 * 在链表尾部添加学生
 *
 * 执行过程：
 * 1. 根据学生对象创建一个新节点；
 * 2. 如果链表为空，新节点同时作为头节点和尾节点；
 * 3. 如果链表不为空，把新节点接到尾节点后面；
 * 4. 更新尾指针和学生数量。
 *
 * 使用 tail 尾指针后，不需要每次从头遍历找尾部，
 * 所以插入操作的时间复杂度为 O(1)。
 */
void StudentList::append(const Student& student)
{
    StudentNode* newNode = new StudentNode(student);

    if (head == nullptr)
    {
        // 原链表为空，新节点既是头节点，也是尾节点
        head = newNode;
        tail = newNode;
    }
    else
    {
        // 原链表不为空，把新节点接在最后一个节点后面
        tail->next = newNode;
        tail = newNode;
    }

    ++studentCount;
}

/*
 * 根据学号查找学生节点
 *
 * 从链表头开始逐个比较学号。
 * 找到后返回对应节点指针，找不到返回 nullptr。
 *
 * 时间复杂度为 O(n)。
 * 由于项目要求按学号查询使用 BST，
 * 所以该函数主要供删除、测试或辅助逻辑使用。
 */
StudentNode* StudentList::findById(const string& studentId) const
{
    StudentNode* current = head;

    while (current != nullptr)
    {
        if (current->data.getStudentId() == studentId)
        {
            return current;
        }

        current = current->next;
    }

    return nullptr;
}

/*
 * 根据学号删除学生节点
 *
 * 需要同时记录当前节点和前一个节点：
 * current 用来找到目标节点；
 * previous 用来重新连接链表。
 *
 * 删除情况分为三种：
 * 1. 删除头节点；
 * 2. 删除中间节点；
 * 3. 删除尾节点。
 */
bool StudentList::removeById(const string& studentId)
{
    StudentNode* current = head;
    StudentNode* previous = nullptr;

    // 查找待删除节点，同时记录它的前一个节点
    while (current != nullptr &&
        current->data.getStudentId() != studentId)
    {
        previous = current;
        current = current->next;
    }

    // 遍历结束仍未找到，说明学号不存在
    if (current == nullptr)
    {
        return false;
    }

    if (previous == nullptr)
    {
        // 待删除节点是头节点
        head = current->next;
    }
    else
    {
        // 待删除节点是中间节点或尾节点
        previous->next = current->next;
    }

    // 如果删除的是尾节点，需要把 tail 指向前一个节点
    if (current == tail)
    {
        tail = previous;
    }

    // 释放学生节点
    delete current;

    --studentCount;

    return true;
}

/*
 * 输出全部学生信息
 *
 * 从链表头开始遍历，
 * 对每个学生节点调用 Student::display()。
 */
void StudentList::displayAll() const
{
    if (head == nullptr)
    {
        cout << "当前没有学生信息。" << endl;
        return;
    }

    StudentNode* current = head;

    while (current != nullptr)
    {
        current->data.display();
        current = current->next;
    }
}

/*
 * 清空学生主链表
 *
 * 逐个保存下一个节点，
 * 然后删除当前节点，避免删除后无法继续遍历。
 */
void StudentList::clear()
{
    StudentNode* current = head;

    while (current != nullptr)
    {
        StudentNode* nextNode = current->next;
        delete current;
        current = nextNode;
    }

    head = nullptr;
    tail = nullptr;
    studentCount = 0;
}

// 返回链表头节点，供排名链建立和全表遍历使用
StudentNode* StudentList::getHead() const
{
    return head;
}
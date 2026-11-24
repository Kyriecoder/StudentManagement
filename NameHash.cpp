#include "NameHash.h"
#include "StudentList.h"
#include <iostream>

// 为静态常量提供定义，兼容不同的 C++ 编译环境
const int NameHash::TABLE_SIZE;

/*
 * HashNode 构造函数
 *
 * name 是哈希关键字；
 * student 指向主链表中的学生节点。
 */
HashNode::HashNode(const string& name, StudentNode* student)
    : name(name),
    student(student),
    next(nullptr)
{
}

/*
 * NameHash 构造函数
 *
 * 将所有哈希桶初始化为空指针。
 * 哈希表中还没有任何学生索引，因此 nodeCount 为 0。
 */
NameHash::NameHash()
    : nodeCount(0)
{
    for (int i = 0; i < TABLE_SIZE; ++i)
    {
        buckets[i] = nullptr;
    }
}

/*
 * NameHash 析构函数
 *
 * 调用 clear() 释放哈希索引节点。
 * 注意不会释放 buckets 中 student 指针指向的学生节点。
 */
NameHash::~NameHash()
{
    clear();
}

/*
 * 判断哈希表是否为空
 *
 * nodeCount 保存索引节点数量，
 * 如果为 0，说明没有任何学生姓名索引。
 */
bool NameHash::isEmpty() const
{
    return nodeCount == 0;
}

// 返回姓名哈希表中的索引节点数量
int NameHash::getSize() const
{
    return nodeCount;
}

/*
 * 姓名哈希函数
 *
 * 基本思路：
 * 从哈希值 0 开始，依次取出姓名中的每个字符，
 * 使用 hash = hash * 31 + 字符 更新哈希值。
 *
 * 下面先把字符转换成 unsigned char，
 * 避免某些平台上 char 是有符号类型时产生负数。
 *
 * 最后对桶数量取余，得到桶下标。
 */
unsigned int NameHash::hashName(const string& name) const
{
    unsigned int hashValue = 0;

    for (size_t i = 0; i < name.size(); ++i)
    {
        unsigned char ch = static_cast<unsigned char>(name[i]);
        hashValue = hashValue * 31 + ch;
    }

    return hashValue % TABLE_SIZE;
}

/*
 * 插入姓名索引
 *
 * 执行过程：
 * 1. 检查学生节点指针是否为空；
 * 2. 从学生节点中取得姓名；
 * 3. 计算姓名对应的哈希桶下标；
 * 4. 检查这个学生是否已经插入过；
 * 5. 创建 HashNode，并插入桶链表头部。
 *
 * 不同学生姓名相同是合法的，
 * 它们会保存为多个不同的 HashNode。
 */
bool NameHash::insert(StudentNode* studentNode)
{
    if (studentNode == nullptr)
    {
        return false;
    }

    const string& name = studentNode->data.getName();
    int index = static_cast<int>(hashName(name));

    /*
     * 检查同一个学生节点是否已经被插入。
     *
     * 这里比较的是 StudentNode 指针，
     * 不是比较姓名。
     * 同名但是不同学生节点，仍然可以正常插入。
     */
    HashNode* current = buckets[index];

    while (current != nullptr)
    {
        if (current->student == studentNode)
        {
            return false;
        }

        current = current->next;
    }

    // 创建新的哈希索引节点
    HashNode* newNode = new HashNode(name, studentNode);

    // 头插法插入当前哈希桶
    newNode->next = buckets[index];
    buckets[index] = newNode;

    ++nodeCount;

    return true;
}

/*
 * 按姓名输出全部同名学生
 *
 * 执行过程：
 * 1. 根据姓名计算哈希桶下标；
 * 2. 遍历整个桶；
 * 3. 因为不同姓名可能发生哈希冲突，
 *    所以必须比较完整姓名；
 * 4. 姓名完全相同时，输出对应学生。
 *
 * 找到第一个学生不能立即停止，
 * 因为可能有多个学生姓名相同。
 */
int NameHash::displayByName(const string& name) const
{
    int index = static_cast<int>(hashName(name));
    HashNode* current = buckets[index];

    int foundCount = 0;

    while (current != nullptr)
    {
        /*
         * 桶中可能保存了其他姓名，
         * 所以这里比较完整姓名，
         * 不能只根据桶下标判断是否匹配。
         */
        if (current->name == name)
        {
            if (current->student != nullptr)
            {
                current->student->data.display();
                ++foundCount;
            }
        }

        current = current->next;
    }

    if (foundCount == 0)
    {
        cout << "没有找到姓名为“" << name << "”的学生。" << endl;
    }

    return foundCount;
}

/*
 * 根据学生节点指针删除姓名索引
 *
 * 执行过程：
 * 1. 从学生节点中取得姓名；
 * 2. 计算对应的哈希桶；
 * 3. 在桶链表中查找 student 指针相同的节点；
 * 4. 将该 HashNode 从链表中摘下并释放。
 *
 * 注意：
 * 只删除 HashNode，不删除 StudentNode。
 * 因此必须先从 HASH 中删除索引，
 * 最后才能删除主链表中的学生节点。
 */
bool NameHash::remove(StudentNode* studentNode)
{
    if (studentNode == nullptr)
    {
        return false;
    }

    const string& name = studentNode->data.getName();
    int index = static_cast<int>(hashName(name));

    HashNode* current = buckets[index];
    HashNode* previous = nullptr;

    while (current != nullptr)
    {
        if (current->student == studentNode)
        {
            // 如果待删除节点是桶链表的头节点
            if (previous == nullptr)
            {
                buckets[index] = current->next;
            }
            else
            {
                // 待删除节点在链表的中间或末尾
                previous->next = current->next;
            }

            delete current;
            --nodeCount;

            return true;
        }

        previous = current;
        current = current->next;
    }

    return false;
}

/*
 * 清空姓名哈希表
 *
 * 遍历每一个哈希桶，
 * 逐个释放桶中的 HashNode。
 *
 * 这里只释放索引节点，
 * 不会释放学生主链表中的 StudentNode。
 */
void NameHash::clear()
{
    for (int i = 0; i < TABLE_SIZE; ++i)
    {
        HashNode* current = buckets[i];

        while (current != nullptr)
        {
            HashNode* nextNode = current->next;
            delete current;
            current = nextNode;
        }

        buckets[i] = nullptr;
    }

    nodeCount = 0;
}
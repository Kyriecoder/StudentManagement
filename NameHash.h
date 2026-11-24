#ifndef NAMEHASH_H
#define NAMEHASH_H

#include <string>

using namespace std;

// NameHash 只保存 StudentNode 指针，不需要在头文件中包含完整定义
class StudentNode;

/*
 * HashNode 类：姓名哈希表中的索引节点
 *
 * 每个节点只保存：
 * 1. 姓名关键字；
 * 2. 指向主链表学生节点的指针；
 * 3. 指向同一个哈希桶中下一个节点的指针。
 *
 * 不在哈希节点中重复保存学号、成绩、总分等信息。
 */
class HashNode
{
public:
    // 哈希表关键字：学生姓名
    string name;

    // 指向主链表中的学生节点
    // HASH 只建立索引，不拥有这个学生节点
    StudentNode* student;

    // 同一个哈希桶中的下一个节点
    HashNode* next;

    // 根据姓名和学生节点指针创建一个哈希索引节点
    HashNode(const string& name, StudentNode* student);
};


/*
 * NameHash 类：按姓名组织的哈希表
 *
 * 采用链地址法处理冲突。
 *
 * 链地址法比开放地址法更适合本项目，原因如下：
 * 1. 姓名允许重复，每个同名学生可以保存为一个独立节点；
 * 2. 有冲突时可以直接挂到链表后面；
 * 3. 删除索引时不需要使用“墓碑标记”；
 * 4. 查询同名时，可以遍历整个桶找出全部符合条件的学生。
 */
class NameHash
{
public:
    // 哈希桶数量，使用质数有利于让哈希值分布得更加均匀
    static const int TABLE_SIZE = 101;

    // 构造一个空的姓名哈希表
    NameHash();

    /*
     * 析构函数。
     * 释放所有 HashNode，但不释放学生主链表中的 StudentNode。
     */
    ~NameHash();

    /*
     * 禁止复制哈希表。
     *
     * HashNode 之间通过链表连接，如果直接复制，
     * 两张哈希表可能共享同一批索引节点，
     * 析构时容易发生重复释放。
     */
    NameHash(const NameHash&) = delete;
    NameHash& operator=(const NameHash&) = delete;

    // 判断哈希表是否为空
    bool isEmpty() const;

    // 返回姓名哈希表中的索引节点数量
    int getSize() const;

    /*
     * 插入一个学生索引。
     *
     * 姓名允许重复，所以不同学生即使姓名相同也可以插入。
     * 如果同一个 StudentNode 被重复插入，则返回 false。
     *
     * 插入成功返回 true。
     */
    bool insert(StudentNode* studentNode);

    /*
     * 根据姓名输出全部同名学生。
     *
     * 先计算姓名对应的哈希桶，
     * 然后遍历整个桶并比较完整姓名字符串。
     *
     * 返回值：找到的学生数量。
     */
    int displayByName(const string& name) const;

    /*
     * 根据学生节点指针删除姓名索引。
     *
     * 删除 StudentManager 中的学生时，
     * 已经通过 BST 或其他方式取得了 StudentNode 指针，
     * 所以可以直接用指针删除，避免遍历整个哈希表。
     *
     * 这里只删除 HashNode，不删除 StudentNode。
     * 删除成功返回 true，未找到返回 false。
     */
    bool remove(StudentNode* studentNode);

    // 清空全部哈希桶，并释放所有 HashNode
    void clear();

private:
    // 哈希桶数组
    // 每个元素保存一条冲突链表的头节点
    HashNode* buckets[TABLE_SIZE];

    // 当前索引节点数量
    int nodeCount;

    /*
     * 计算姓名对应的哈希桶下标。
     *
     * 这里使用常见的字符串哈希方式：
     * hash = hash * 31 + 当前字符
     *
     * 最后对 TABLE_SIZE 取余，得到桶下标。
     */
    unsigned int hashName(const string& name) const;
};

#endif
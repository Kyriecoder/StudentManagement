#pragma once
#ifndef STUDENTBST_H
#define STUDENTBST_H

#include <string>

using namespace std;

// BST 只需要知道 StudentNode 是一个类，暂时不需要包含完整定义
class StudentNode;

/*
 * BSTNode 类：二叉搜索树节点
 *
 * 每个节点只保存：
 * 1. 学号关键字；
 * 2. 指向主链表中对应学生节点的指针。
 *
 * 不在 BST 节点中重复保存姓名、成绩、总分等信息，
 * 这样可以减少数据重复。
 */
class BSTNode
{
public:
    // BST 的关键字：学号
    string studentId;

    // 指向学生主链表中对应的学生节点
    // BST 只建立索引，不拥有这个学生节点
    StudentNode* student;

    // 左子树和右子树
    BSTNode* left;
    BSTNode* right;

    // 根据学号和学生节点指针创建一个 BST 节点
    BSTNode(const string& studentId, StudentNode* student);
};


/*
 * StudentBST 类：按学号组织的二叉搜索树
 *
 * 主要功能：
 * 1. 插入学生索引；
 * 2. 按学号查询学生；
 * 3. 按学号删除学生索引；
 * 4. 中序遍历输出所有学生；
 * 5. 清空 BST。
 *
 * 说明：
 * BST 中学号不允许重复。
 * 如果插入已经存在的学号，insert() 会返回 false。
 */
class StudentBST
{
public:
    // 构造一棵空 BST
    StudentBST();

    // 释放所有 BST 节点，但不释放学生主链表节点
    ~StudentBST();

    /*
     * 禁止复制 BST。
     *
     * BST 节点内部使用指针连接，
     * 如果直接复制，两棵树可能共享节点，
     * 析构时容易出现重复释放的问题。
     */
    StudentBST(const StudentBST&) = delete;
    StudentBST& operator=(const StudentBST&) = delete;

    // 判断 BST 是否为空
    bool isEmpty() const;

    // 返回 BST 中保存的学生索引数量
    int getSize() const;

    /*
     * 插入一个学生索引。
     *
     * 参数 studentNode 指向主链表中的学生节点。
     *
     * 插入成功返回 true；
     * 如果学号已经存在，或者传入空指针，返回 false。
     */
    bool insert(StudentNode* studentNode);

    /*
     * 根据学号查询学生。
     *
     * 找到时返回对应的 StudentNode 指针；
     * 找不到时返回 nullptr。
     *
     * 时间复杂度取决于树的高度：
     * 平均为 O(log n)，最坏情况下为 O(n)。
     */
    StudentNode* findById(const string& studentId) const;

    /*
     * 根据学号删除 BST 索引。
     *
     * 只删除 BSTNode，不删除 StudentNode。
     * 主链表、HASH 和排名链中的相应数据，
     * 应由 StudentManager 按顺序依次删除。
     *
     * 删除成功返回 true，未找到返回 false。
     */
    bool removeById(const string& studentId);

    /*
     * 中序遍历 BST。
     *
     * 由于按学号建立 BST，
     * 中序遍历时会按学号从小到大输出学生信息。
     */
    void displayInOrder() const;

    // 清空整棵 BST，并释放全部 BSTNode
    void clear();

private:
    // BST 的根节点
    BSTNode* root;

    // BST 节点数量
    int nodeCount;

    /*
     * 递归插入辅助函数。
     *
     * 使用 BSTNode*& 引用参数，
     * 当 node 为空时可以直接修改父节点的 left 或 right 指针。
     */
    bool insertNode(BSTNode*& node,
        const string& studentId,
        StudentNode* studentNode);

    /*
     * 递归删除辅助函数。
     *
     * removed 用来记录本次删除是否成功，
     * 避免在多层递归中重复判断。
     */
    BSTNode* removeNode(BSTNode*& node,
        const string& studentId,
        bool& removed);

    // 递归释放一棵子树
    void clearNode(BSTNode* node);

    // 递归中序遍历一棵子树
    void displayInOrderNode(BSTNode* node) const;
};

#endif
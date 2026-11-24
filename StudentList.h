#pragma once		
#ifndef STUDENTLIST_H
#define STUDENTLIST_H

#include "Student.h"
#include <iostream>
#include <string>

using namespace std;

/*
 * StudentNode 类：学生主链表中的一个节点
 *
 * data 保存完整的 Student 对象；
 * next 指向下一个学生节点。
 *
 * 这里把 data 和 next 设置为 public，
 * 是为了让 StudentList 能直接访问节点内容，
 * 也方便 BST、HASH 和排名链通过指针取得学生数据。
 */
class StudentNode
{
public:
    // 一个完整的学生对象
    Student data;

    // 指向链表中的下一个学生节点
    StudentNode* next;

    // 构造函数：根据一个学生对象创建链表节点
    explicit StudentNode(const Student& student);
};


/*
 * StudentList 类：学生主链表
 *
 * 主要功能：
 * 1. 在链表尾部添加学生；
 * 2. 根据学号查找学生节点；
 * 3. 根据学号删除学生节点；
 * 4. 输出全部学生信息；
 * 5. 清空链表并释放学生节点。
 *
 * 内存管理说明：
 * StudentList 拥有所有 StudentNode，
 * 所以 StudentList 释放时负责 delete 学生节点。
 * BST、HASH 和排名链只保存学生节点指针，不能释放这些学生节点。
 */
class StudentList
{
public:
    // 构造空链表
    StudentList();

    // 析构函数：释放链表中全部学生节点
    ~StudentList();

    /*
     * 禁止复制链表。
     *
     * 因为链表中保存的是裸指针，如果直接复制，
     * 两个链表会指向同一批节点，析构时可能重复释放内存。
     * 当前项目中不需要复制 StudentList，所以直接禁止复制。
     */
    StudentList(const StudentList&) = delete;
    StudentList& operator=(const StudentList&) = delete;

    // 判断链表是否为空
    bool isEmpty() const;

    // 返回链表中学生节点的数量
    int getSize() const;

    /*
     * 在链表尾部添加一个学生。
     *
     * 由于保存了 tail 尾指针，
     * 正常添加操作的时间复杂度为 O(1)。
     *
     * 注意：
     * 学号是否重复应由 StudentManager 通过 BST 检查，
     * 这里不进行 O(n) 的重复检查。
     */
    void append(const Student& student);

    /*
     * 根据学号查找学生节点。
     *
     * 这是链表的顺序查找，时间复杂度为 O(n)。
     * 正式按学号查询应使用 BST，这个函数主要用于辅助操作。
     *
     * 找到时返回学生节点指针；
     * 找不到时返回 nullptr。
     */
    StudentNode* findById(const string& studentId) const;

    /*
     * 根据学号删除学生节点。
     *
     * 删除成功时释放该学生节点并返回 true；
     * 找不到对应学号时返回 false。
     *
     * 注意：
     * 正式删除学生时，应先删除 BST、HASH 和排名链中的引用，
     * 最后再调用这个函数删除主链表节点。
     */
    bool removeById(const string& studentId);

    // 按照主链表顺序输出全部学生信息
    void displayAll() const;

    // 清空学生主链表，并释放全部学生节点
    void clear();

    /*
     * 返回链表头节点。
     *
     * 建立排名链时，可以从链表头开始遍历全部学生。
     * 调用者只能读取或遍历节点，不能随意 delete 头节点。
     */
    StudentNode* getHead() const;

private:
    // 指向链表第一个学生节点
    StudentNode* head;

    // 指向链表最后一个学生节点，方便 O(1) 尾部插入
    StudentNode* tail;

    // 当前学生数量
    int studentCount;
};

#endif

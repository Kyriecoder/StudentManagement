#ifndef RANKLIST_H
#define RANKLIST_H

#include "StudentList.h"

using namespace std;

/*
 * RankNode 类：排名链节点
 *
 * 排名链不复制完整的 Student 数据，
 * 只保存指向主链表学生节点的指针。
 */
class RankNode
{
public:
    // 指向主链表中的学生节点
    // RankList 不拥有这个学生节点
    StudentNode* student;

    // 前一个排名节点和后一个排名节点
    RankNode* prev;
    RankNode* next;

    // 根据学生节点指针创建排名节点
    explicit RankNode(StudentNode* student);
};


/*
 * RankList 类：按总分维护学生排名
 *
 * 排序规则：
 * 1. 总分高的排在前面；
 * 2. 总分相同时，录入顺序小的排在前面；
 * 3. 排名采用竞赛排名：
 *    成绩为 95、90、90、85 时，
 *    排名为 1、2、2、4。
 */
class RankList
{
public:
    RankList();
    ~RankList();

    /*
     * 禁止复制。
     * 排名节点使用裸指针连接，
     * 复制后容易出现两个排名链共享节点和重复释放的问题。
     */
    RankList(const RankList&) = delete;
    RankList& operator=(const RankList&) = delete;

    bool isEmpty() const;
    int getSize() const;

    /*
     * 根据学生主链表建立排名链。
     *
     * 会先释放旧的排名节点，再根据主链表创建新的排名节点，
     * 然后使用自己实现的稳定二路归并排序。
     *
     * 这个方法一般用于第一次生成排名，
     * 修改成绩、插入学生和删除学生后不应再次调用。
     */
    void buildFromList(const StudentList& studentList);

    /*
     * 把一个新学生插入排名链。
     *
     * 插入时直接寻找合适位置，不调用完整排序。
     * 插入后会重新计算受影响的排名。
     */
    bool insertStudent(StudentNode* studentNode);

    /*
     * 从排名链中删除一个学生。
     *
     * 只删除 RankNode，不删除 StudentNode。
     * 删除后会重新计算受影响的排名。
     */
    bool removeStudent(StudentNode* studentNode);

    /*
     * 学生四门成绩修改完成后调用。
     *
     * 此时 Student 中的总分已经改变，
     * 本函数会把该学生的 RankNode 从原位置摘下，
     * 再根据新总分插入新位置。
     *
     * 不重新排序整条排名链。
     */
    bool updateStudent(StudentNode* studentNode);

    /*
     * 按排名顺序输出全部学生。
     * 返回值是实际输出的学生数量。
     */
    int displayRanking() const;

    // 清空排名链，并释放所有 RankNode
    void clear();

private:
    // 排名链的头节点和尾节点
    RankNode* head;
    RankNode* tail;

    // 排名节点数量
    int nodeCount;

    /*
     * 根据学生指针查找排名节点。
     * 主要用于阻止重复插入，以及修改成绩前定位原位置。
     */
    RankNode* findRankNode(StudentNode* studentNode) const;

    /*
     * 判断 first 是否应该排在 second 前面。
     *
     * 返回 true 的条件：
     * 1. first 的总分更高；
     * 2. 总分相同时，first 的录入顺序更小。
     */
    bool comesBefore(const StudentNode* first,
        const StudentNode* second) const;

    // 递归二路归并排序
    RankNode* mergeSort(RankNode* listHead);

    // 合并两条已经有序的排名链
    RankNode* merge(RankNode* left, RankNode* right);

    // 从排名链开头重新计算所有排名
    void refreshAllRanks();

    /*
     * 从 start 开始重新计算后面的排名。
     *
     * startPosition 是 start 在整条排名链中的第几位，
     * 从 1 开始计算。
     *
     * 只修改 start 及其后面的排名，
     * 前面的排名保持不变。
     */
    void refreshRanksFrom(RankNode* start, int startPosition);
};

#endif

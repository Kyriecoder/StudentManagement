#include "RankList.h"
#include <iostream>

/*
 * RankNode 构造函数
 *
 * 排名节点只保存学生节点指针，
 * 构造完成后前后指针都为空。
 */
RankNode::RankNode(StudentNode* student)
    : student(student),
    prev(nullptr),
    next(nullptr)
{
}

// 构造空排名链
RankList::RankList()
    : head(nullptr),
    tail(nullptr),
    nodeCount(0)
{
}

// 析构时释放所有排名节点
RankList::~RankList()
{
    clear();
}

// 判断排名链是否为空
bool RankList::isEmpty() const
{
    return head == nullptr;
}

// 返回排名节点数量
int RankList::getSize() const
{
    return nodeCount;
}

/*
 * 根据学生主链表建立排名链
 *
 * 执行步骤：
 * 1. 清空旧排名节点；
 * 2. 按主链表顺序为每个学生创建一个 RankNode；
 * 3. 对这些 RankNode 进行稳定二路归并排序；
 * 4. 设置排名链尾指针；
 * 5. 计算所有学生的排名。
 *
 * 注意：
 * 这里使用的是自定义 mergeSort()，
 * 没有调用 std::sort() 等现成排序函数。
 */
void RankList::buildFromList(const StudentList& studentList)
{
    clear();

    StudentNode* currentStudent = studentList.getHead();

    // 先按主链表原来的顺序建立排名节点
    while (currentStudent != nullptr)
    {
        RankNode* newNode = new RankNode(currentStudent);

        if (head == nullptr)
        {
            head = newNode;
            tail = newNode;
        }
        else
        {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }

        ++nodeCount;
        currentStudent = currentStudent->next;
    }

    // 使用稳定二路归并排序建立正式排名顺序
    head = mergeSort(head);

    if (head != nullptr)
    {
        head->prev = nullptr;
        tail = head;

        while (tail->next != nullptr)
        {
            tail = tail->next;
        }

        tail->next = nullptr;
    }
    else
    {
        tail = nullptr;
    }

    refreshAllRanks();
}

/*
 * 根据学生节点指针查找排名节点
 *
 * 时间复杂度 O(n)。
 * 只在局部更新时使用，不用于按姓名或学号查询。
 */
RankNode* RankList::findRankNode(StudentNode* studentNode) const
{
    if (studentNode == nullptr)
    {
        return nullptr;
    }

    RankNode* current = head;

    while (current != nullptr)
    {
        if (current->student == studentNode)
        {
            return current;
        }

        current = current->next;
    }

    return nullptr;
}

/*
 * 判断 first 是否应该排在 second 前面
 *
 * 排序优先级：
 * 1. 总分高的人在前；
 * 2. 总分相同时，orderNo 小的人在前。
 *
 * orderNo 由 StudentManager 在录入学生时分配，
 * 保证同分学生可以保持录入顺序。
 */
bool RankList::comesBefore(const StudentNode* first,
    const StudentNode* second) const
{
    if (first == nullptr || second == nullptr)
    {
        return false;
    }

    int firstTotal = first->data.getTotalScore();
    int secondTotal = second->data.getTotalScore();

    if (firstTotal != secondTotal)
    {
        return firstTotal > secondTotal;
    }

    return first->data.getOrderNo() < second->data.getOrderNo();
}

/*
 * 把一个新学生插入排名链
 *
 * 执行过程：
 * 1. 检查指针是否为空；
 * 2. 检查该学生是否已经在排名链中；
 * 3. 从前往后寻找第一个应排在新学生后面的节点；
 * 4. 把新节点插入该位置；
 * 5. 从插入位置开始重新计算排名。
 *
 * 整个过程不调用完整排序。
 */
bool RankList::insertStudent(StudentNode* studentNode)
{
    if (studentNode == nullptr)
    {
        return false;
    }

    // 防止同一个学生被重复加入排名链
    if (findRankNode(studentNode) != nullptr)
    {
        return false;
    }

    RankNode* newNode = new RankNode(studentNode);
    RankNode* current = head;
    int position = 1;

    /*
     * 找到第一个不应排在 newNode 前面的节点。
     * 所有应该排在新节点前面的节点都跳过。
     */
    while (current != nullptr &&
        comesBefore(current->student, studentNode))
    {
        current = current->next;
        ++position;
    }

    // 把 newNode 插入 current 前面
    newNode->prev = (current != nullptr) ? current->prev : tail;
    newNode->next = current;

    if (newNode->prev != nullptr)
    {
        newNode->prev->next = newNode;
    }
    else
    {
        head = newNode;
    }

    if (current != nullptr)
    {
        current->prev = newNode;
    }
    else
    {
        tail = newNode;
    }

    ++nodeCount;

    // 新节点插入后，从该位置开始重算排名
    refreshRanksFrom(newNode, position);

    return true;
}

/*
 * 从排名链中删除一个学生
 *
 * 只删除 RankNode，不删除 StudentNode。
 *
 * 删除后，原来位于后面的学生可能整体前移，
 * 因此从被删除位置开始重新计算排名。
 */
bool RankList::removeStudent(StudentNode* studentNode)
{
    if (studentNode == nullptr)
    {
        return false;
    }

    RankNode* current = head;
    int position = 1;

    while (current != nullptr &&
        current->student != studentNode)
    {
        current = current->next;
        ++position;
    }

    if (current == nullptr)
    {
        return false;
    }

    RankNode* previous = current->prev;
    RankNode* successor = current->next;

    if (previous != nullptr)
    {
        previous->next = successor;
    }
    else
    {
        head = successor;
    }

    if (successor != nullptr)
    {
        successor->prev = previous;
    }
    else
    {
        tail = previous;
    }

    delete current;
    --nodeCount;

    /*
     * 删除后，原位置由 successor 接替。
     * 因此从 successor 所在位置开始重算排名。
     */
    if (successor != nullptr)
    {
        refreshRanksFrom(successor, position);
    }

    return true;
}

/*
 * 学生成绩修改后更新排名
 *
 * 前提：
 * Student 中的四门成绩和总分已经通过 setScores() 修改完成。
 *
 * 执行过程：
 * 1. 找到学生原来的 RankNode；
 * 2. 把它从原位置摘下，但暂时不释放；
 * 3. 根据新总分重新寻找插入位置；
 * 4. 重新接回排名链；
 * 5. 从最早受影响的位置开始重算排名。
 *
 * 这条排名链不会进行完整重新排序。
 */
bool RankList::updateStudent(StudentNode* studentNode)
{
    if (studentNode == nullptr)
    {
        return false;
    }

    RankNode* current = head;
    int oldPosition = 1;

    // 找到学生原来的排名节点
    while (current != nullptr &&
        current->student != studentNode)
    {
        current = current->next;
        ++oldPosition;
    }

    if (current == nullptr)
    {
        return false;
    }

    RankNode* oldPrevious = current->prev;
    RankNode* oldNext = current->next;

    // 先把节点从原位置摘下
    if (oldPrevious != nullptr)
    {
        oldPrevious->next = oldNext;
    }
    else
    {
        head = oldNext;
    }

    if (oldNext != nullptr)
    {
        oldNext->prev = oldPrevious;
    }
    else
    {
        tail = oldPrevious;
    }

    current->prev = nullptr;
    current->next = nullptr;
    --nodeCount;

    // 根据修改后的总分，重新寻找插入位置
    RankNode* scan = head;
    int newPosition = 1;

    while (scan != nullptr &&
        comesBefore(scan->student, studentNode))
    {
        scan = scan->next;
        ++newPosition;
    }

    current->prev = (scan != nullptr) ? scan->prev : tail;
    current->next = scan;

    if (current->prev != nullptr)
    {
        current->prev->next = current;
    }
    else
    {
        head = current;
    }

    if (scan != nullptr)
    {
        scan->prev = current;
    }
    else
    {
        tail = current;
    }

    ++nodeCount;

    /*
     * 需要确定最早受影响的位置：
     *
     * 如果新位置在旧位置之前，从新位置开始重算；
     * 如果新位置在旧位置之后，旧位置的后继是最早受影响节点。
     */
    RankNode* start = nullptr;
    int startPosition = 1;

    if (oldNext == nullptr || newPosition <= oldPosition)
    {
        start = current;
        startPosition = newPosition;
    }
    else
    {
        start = oldNext;
        startPosition = oldPosition;
    }

    refreshRanksFrom(start, startPosition);

    return true;
}

/*
 * 稳定二路归并排序
 *
 * 先使用快慢指针把链表分成两半，
 * 分别排序左右两半，
 * 最后用 merge() 合并。
 *
 * 时间复杂度为 O(n log n)。
 */
RankNode* RankList::mergeSort(RankNode* listHead)
{
    if (listHead == nullptr || listHead->next == nullptr)
    {
        if (listHead != nullptr)
        {
            listHead->prev = nullptr;
        }

        return listHead;
    }

    // 使用快慢指针找到链表中点
    RankNode* slow = listHead;
    RankNode* fast = listHead->next;

    while (fast != nullptr && fast->next != nullptr)
    {
        slow = slow->next;
        fast = fast->next->next;
    }

    RankNode* rightHead = slow->next;
    slow->next = nullptr;

    if (rightHead != nullptr)
    {
        rightHead->prev = nullptr;
    }

    RankNode* leftSorted = mergeSort(listHead);
    RankNode* rightSorted = mergeSort(rightHead);

    return merge(leftSorted, rightSorted);
}

/*
 * 合并两条有序排名链
 *
 * 如果两个节点排序位置相同，
 * 优先取左链节点，从而保持稳定排序。
 */
RankNode* RankList::merge(RankNode* left, RankNode* right)
{
    RankNode* resultHead = nullptr;
    RankNode* resultTail = nullptr;

    while (left != nullptr && right != nullptr)
    {
        RankNode* chosen = nullptr;

        /*
         * 只有右边节点明确应该排在左边节点前面时，
         * 才选择右侧节点。
         *
         * 两边完全相同时选择左侧节点，
         * 保证原来的先后顺序不改变。
         */
        if (comesBefore(right->student, left->student))
        {
            chosen = right;
            right = right->next;
        }
        else
        {
            chosen = left;
            left = left->next;
        }

        chosen->next = nullptr;
        chosen->prev = resultTail;

        if (resultHead == nullptr)
        {
            resultHead = chosen;
        }
        else
        {
            resultTail->next = chosen;
        }

        resultTail = chosen;
    }

    RankNode* remaining = (left != nullptr) ? left : right;

    while (remaining != nullptr)
    {
        RankNode* chosen = remaining;
        remaining = remaining->next;

        chosen->next = nullptr;
        chosen->prev = resultTail;

        if (resultHead == nullptr)
        {
            resultHead = chosen;
        }
        else
        {
            resultTail->next = chosen;
        }

        resultTail = chosen;
    }

    if (resultHead != nullptr)
    {
        resultHead->prev = nullptr;
    }

    if (resultTail != nullptr)
    {
        resultTail->next = nullptr;
    }

    return resultHead;
}

/*
 * 从排名链开头重新计算全部排名
 *
 * 排名规则：
 *   95、90、90、85
 *   排名为 1、2、2、4
 *
 * 当当前总分等于前一个总分时，
 * 使用前一个学生的排名；
 * 否则排名等于当前学生在顺序中的位置。
 */
void RankList::refreshAllRanks()
{
    if (head == nullptr || head->student == nullptr)
    {
        return;
    }

    int position = 1;//位置
	int currentRank = 1;//当前排名
	int previousTotal = head->student->data.getTotalScore();//前一个总分

    head->student->data.setRank(currentRank);

    RankNode* current = head->next;

    while (current != nullptr)
    {
        ++position;

        int currentTotal = current->student->data.getTotalScore();

        if (currentTotal != previousTotal)
        {
            currentRank = position;
            previousTotal = currentTotal;
        }

        current->student->data.setRank(currentRank);
        current = current->next;
    }
}

/*
 * 从指定节点开始重新计算排名
 *
 * startPosition 是 start 在整条排名链中的位置。
 * start 前面的排名已经正确，因此不需要重新计算。
 */
void RankList::refreshRanksFrom(RankNode* start, int startPosition)
{
    if (start == nullptr || start->student == nullptr)
    {
        return;
    }

    int previousTotal = start->student->data.getTotalScore();
    int currentRank = startPosition;

    /*
     * 如果前一个学生和 start 总分相同，
     * start 应直接使用前一个学生的排名。
     */
    if (start->prev != nullptr &&
        start->prev->student != nullptr &&
        start->prev->student->data.getTotalScore() == previousTotal)
    {
        currentRank = start->prev->student->data.getRank();
    }

    start->student->data.setRank(currentRank);

    int position = startPosition;
    RankNode* current = start->next;

    while (current != nullptr)
    {
        ++position;

        int currentTotal = current->student->data.getTotalScore();

        if (currentTotal != previousTotal)
        {
            currentRank = position;
            previousTotal = currentTotal;
        }

        current->student->data.setRank(currentRank);
        current = current->next;
    }
}

/*
 * 按排名顺序输出全部学生
 *
 * 直接遍历已经维护好的排名链，
 * 不会再调用排序函数。
 */
int RankList::displayRanking() const
{
    if (head == nullptr)
    {
        cout << "当前没有学生排名信息。" << endl;
        return 0;
    }

    int count = 0;
    RankNode* current = head;

    while (current != nullptr)
    {
        if (current->student != nullptr)
        {
            current->student->data.display();
            ++count;
        }

        current = current->next;
    }

    return count;
}

// 清空排名链，只释放 RankNode
void RankList::clear()
{
    RankNode* current = head;

    while (current != nullptr)
    {
        RankNode* nextNode = current->next;
        delete current;
        current = nextNode;
    }

    head = nullptr;
    tail = nullptr;
    nodeCount = 0;
}
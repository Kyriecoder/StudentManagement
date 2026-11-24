#include "StudentBST.h"
#include "StudentList.h"
#include <iostream>

/*
 * BSTNode 构造函数
 *
 * studentId 是索引关键字；
 * student 是指向主链表学生节点的指针。
 */
BSTNode::BSTNode(const string& studentId, StudentNode* student)
    : studentId(studentId),
    student(student),
    left(nullptr),
    right(nullptr)
{
}

/*
 * StudentBST 构造函数
 *
 * 初始状态没有根节点，节点数量为 0。
 */
StudentBST::StudentBST()
    : root(nullptr),
    nodeCount(0)
{
}

/*
 * StudentBST 析构函数
 *
 * clear() 会删除所有 BSTNode，
 * 但不会删除 BSTNode 中 student 指针指向的学生节点。
 */
StudentBST::~StudentBST()
{
    clear();
}

// 判断 BST 是否为空
bool StudentBST::isEmpty() const
{
    return root == nullptr;
}

// 返回 BST 节点数量
int StudentBST::getSize() const
{
    return nodeCount;
}

/*
 * 插入学生索引
 *
 * 执行过程：
 * 1. 检查学生节点指针是否为空；
 * 2. 从学生节点中取得学号；
 * 3. 调用递归函数插入 BST；
 * 4. 插入成功后节点数量加一。
 */
bool StudentBST::insert(StudentNode* studentNode)
{
    if (studentNode == nullptr)
    {
        return false;
    }

    const string& studentId = studentNode->data.getStudentId();

    bool inserted = insertNode(root, studentId, studentNode);

    if (inserted)
    {
        ++nodeCount;
    }

    return inserted;
}

/*
 * 递归插入辅助函数
 *
 * 根据学号比较结果决定插入左子树还是右子树：
 * 1. 新学号小于当前学号：插入左子树；
 * 2. 新学号大于当前学号：插入右子树；
 * 3. 学号相同：说明重复，不允许插入。
 *
 * 使用 BSTNode*& node 的原因是：
 * node 可能是 root，也可能是父节点的 left 或 right。
 * 当 node 为空时，直接给 node 赋值就能把新节点接到树上。
 */
bool StudentBST::insertNode(BSTNode*& node,
    const string& studentId,
    StudentNode* studentNode)
{
    if (node == nullptr)
    {
        node = new BSTNode(studentId, studentNode);
        return true;
    }

    if (studentId < node->studentId)
    {
        return insertNode(node->left, studentId, studentNode);
    }
    else if (studentId > node->studentId)
    {
        return insertNode(node->right, studentId, studentNode);
    }
    else
    {
        // 学号相同，BST 不允许保存重复关键字
        return false;
    }
}

/*
 * 根据学号查询学生
 *
 * 从根节点开始迭代查找：
 * 1. 目标学号更小，进入左子树；
 * 2. 目标学号更大，进入右子树；
 * 3. 学号相等，返回学生节点指针。
 *
 * 找到的只是索引节点中的指针，
 * 学生完整信息仍然保存在主链表中。
 */
StudentNode* StudentBST::findById(const string& studentId) const
{
    BSTNode* current = root;

    while (current != nullptr)
    {
        if (studentId < current->studentId)
        {
            current = current->left;
        }
        else if (studentId > current->studentId)
        {
            current = current->right;
        }
        else
        {
            return current->student;
        }
    }

    return nullptr;
}

/*
 * 根据学号删除 BST 索引
 *
 * removeNode() 负责真正删除 BSTNode。
 * 删除成功后，将节点数量减一。
 */
bool StudentBST::removeById(const string& studentId)
{
    bool removed = false;

    root = removeNode(root, studentId, removed);

    if (removed)
    {
        --nodeCount;
    }

    return removed;
}

/*
 * 递归删除辅助函数
 *
 * 删除分为以下四种情况：
 * 1. 没有找到节点，直接返回；
 * 2. 待删除节点是叶子节点；
 * 3. 待删除节点只有一个孩子；
 * 4. 待删除节点有两个孩子。
 *
 * 有两个孩子时，选择右子树中的最小节点作为后继节点，
 * 把后继节点的学号和学生指针复制到当前节点，
 * 然后再从右子树中删除那个后继节点。
 */
BSTNode* StudentBST::removeNode(BSTNode*& node,
    const string& studentId,
    bool& removed)
{
    if (node == nullptr)
    {
        return nullptr;
    }

    if (studentId < node->studentId)
    {
        node->left = removeNode(node->left, studentId, removed);
    }
    else if (studentId > node->studentId)
    {
        node->right = removeNode(node->right, studentId, removed);
    }
    else
    {
        // 找到待删除的 BST 节点
        removed = true;

        // 情况一：没有孩子，直接删除当前 BST 节点
        if (node->left == nullptr && node->right == nullptr)
        {
            delete node;
            return nullptr;
        }

        // 情况二：只有右孩子，用右孩子替代当前节点
        if (node->left == nullptr)
        {
            BSTNode* rightChild = node->right;
            delete node;
            return rightChild;
        }

        // 情况三：只有左孩子，用左孩子替代当前节点
        if (node->right == nullptr)
        {
            BSTNode* leftChild = node->left;
            delete node;
            return leftChild;
        }

        /*
         * 情况四：有两个孩子。
         * 找到右子树中的最小学号节点作为后继节点。
         */
        BSTNode* successor = node->right;

        while (successor->left != nullptr)
        {
            successor = successor->left;
        }

        // 用后继节点的关键字和学生指针覆盖当前节点
        node->studentId = successor->studentId;
        node->student = successor->student;

        /*
         * 原来的后继节点要删除。
         * 这里再次调用 removeNode(),
         * 它会命中“没有左孩子”的情况。
         */
        node->right = removeNode(node->right,
            successor->studentId,
            removed);
    }

    return node;
}

/*
 * 中序遍历输出全部学生
 *
 * 由于学号在 BST 中按顺序排列，
 * 中序遍历会按照学号从小到大输出。
 */
void StudentBST::displayInOrder() const
{
    if (root == nullptr)
    {
        cout << "BST 中暂无学生索引。" << endl;
        return;
    }

    displayInOrderNode(root);
}

/*
 * 递归中序遍历辅助函数
 *
 * 遍历顺序：
 * 左子树 -> 当前节点 -> 右子树
 *
 * 注意：
 * 这里只读取学生节点，不删除学生节点。
 */
void StudentBST::displayInOrderNode(BSTNode* node) const
{
    if (node == nullptr)
    {
        return;
    }

    displayInOrderNode(node->left);

    if (node->student != nullptr)
    {
        node->student->data.display();
    }

    displayInOrderNode(node->right);
}

/*
 * 清空 BST
 *
 * 释放全部 BSTNode，
 * 但不会 delete node->student。
 */
void StudentBST::clear()
{
    clearNode(root);
    root = nullptr;
    nodeCount = 0;
}

/*
 * 递归释放一棵子树
 *
 * 采用后序遍历：
 * 先释放左子树，再释放右子树，最后释放当前节点。
 */
void StudentBST::clearNode(BSTNode* node)
{
    if (node == nullptr)
    {
        return;
    }

    clearNode(node->left);
    clearNode(node->right);
    delete node;
}
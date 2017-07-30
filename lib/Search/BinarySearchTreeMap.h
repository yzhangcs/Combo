/*******************************************************************************
 * BinarySearchTreeMap.h
 *
 * Author: zhangyu
 * Date: 2017.5.24
 ******************************************************************************/

#pragma once
#include <algorithm>
#include <cassert>
#include <cmath>
#include <functional>
#include <iostream>
#include <iterator>
#include "ArrayQueue.h"
#include "ArrayStack.h"

/**
 * 使用模板实现的二叉查找树（Map）.
 * 实现了中序遍历树的前向迭代器.
 */
template<typename K, typename V>
class BinarySearchTreeMap
{
    using Comparator = std::function<bool(const K& lhs, const K& rhs)>;
private:
    struct Node
    {
        K key;
        V val;
        int size; // 以该结点为根的子树中的结点数
        Node* lft;
        Node* rht; // 左右子女指针
        Node(K key, V val) :
            key(std::move(key)), 
            val(std::move(val)), 
            size(1), lft(nullptr), rht(nullptr) {}
    };
    Comparator less; // 比较器
    Node* root; // 根结点

    int size(Node* x) const;
    int height(Node* x) const;
    int rank(Node* x, const K& key) const;
    Node* copy(Node* x) const;
    Node* select(Node* x, int i) const;
    Node* find(Node* x, const K& key) const;
    Node* minKey(Node* x) const;
    Node* maxKey(Node* x) const;
    Node* put(Node* x, const K& key, const V& val);
    Node* remove(Node* x, const K& key);
    Node* removeMin(Node* x);
    Node* removeMax(Node* x);
    void preOrder(Node* x, ArrayQueue<K>& queue) const;
    void inOrder(Node* x, ArrayQueue<K>& queue) const;
    void postOrder(Node* x, ArrayQueue<K>& queue) const;
    void print(Node* x) const; // 打印树的结构
    void destroy(Node* x);
    bool isBST() const { return isBST(root); } // 检查当前树是否为二叉查找树
    bool isBST(Node* x) const; // 检查指定根结点的树是否为二叉查找树
public:
    explicit BinarySearchTreeMap(Comparator comp = std::less<K>()) :
        root(nullptr), less(comp) {} // 构造函数
    BinarySearchTreeMap(const BinarySearchTreeMap& that); // 复制构造函数
    BinarySearchTreeMap(BinarySearchTreeMap&& that); // 移动构造函数
    ~BinarySearchTreeMap() { destroy(root); } // 析构函数

    int size() const { return size(root); } // 返回树当前大小
    int height() const { return height(root); } // 返回树高度
    int rank(const K& key) const { return rank(root, key); }
    bool isEmpty() const { return size(root) == 0; } // 判断是否为空树
    // 判断树中是否存在指定的键
    bool contains(const K& key) const { return find(root, key) != nullptr; }
    K keyOf(int i) const; // 得到升序排列下第i个键
    V valueOf(const K& key) const; // 得到指定键对应的值
    K minKey() const; // 得到最小键及其值
    K maxKey() const; // 得到最大键及其值
    void put(K key, V val); // 添加指定键值对到
    void remove(const K& key); // 移除指定键值对
    void removeMin(); // 移除最小键值对
    void removeMax(); // 移除最小键值对
    void preOrder(ArrayQueue<K>& queue) const { preOrder(root, queue); } // 前序遍历
    void inOrder(ArrayQueue<K>& queue) const { inOrder(root, queue); } // 中序遍历
    void postOrder(ArrayQueue<K>& queue) const { postOrder(root, queue); } // 后序遍历
    void levelOrder(ArrayQueue<K>& queue) const; // 层次序遍历
    void print() const { print(root); } // 打印树结构
    void swap(BinarySearchTreeMap& that); // 内容与另一个BinarySearchTreeMap对象交换
    void clear() { destroy(root); } // 清空树
    
    BinarySearchTreeMap& operator=(BinarySearchTreeMap that);
    template <typename T, typename U>
    friend std::ostream& operator<<(std::ostream& os, const BinarySearchTreeMap<T, U>& tree);

    class iterator : public std::iterator<std::forward_iterator_tag, K>
    {
    private:
        ArrayStack<Node*> stack; // 内部辅助栈
        Node* i;
    public:
        iterator() : i(nullptr) {}
        iterator(Node* x) : i(x)
        {
            while (i != nullptr)
            {
                stack.push(i);
                i = i->lft;
            } // 遍历到最小键所在结点，经过的结点放入栈中
            stack.pop(i); // 此时i指向最小键所在结点
        }
        ~iterator() {}

        K& operator*() const
        { return i->key; }
        bool operator==(const iterator& that) const
        { return i == that.i; }
        bool operator!=(const iterator& that) const
        { return i != that.i; }
        iterator& operator++()
        {
            // 若有右子树，寻找右子树最小结点，并将所有经过的结点入栈
            for (Node* i = i->rht; i != nullptr; i = i->lft)
                stack.push(i);
            if (!stack.pop(i)) i = nullptr; // 后继为父结点或右子树最小结点，否则为空
            return *this;
        }
        const iterator operator++(int)
        {
            iterator tmp(*this);

            // 若有右子树，寻找右子树最小结点，并将所有经过的结点入栈
            for (Node* i = i->rht; i != nullptr; i = i->lft)
                stack.push(i);
            if (!stack.pop(i)) i = nullptr; // 后继为父结点或右子树最小结点，否则为空
            return tmp;
        }
    };
    iterator begin() const { return root; }
    iterator end() const { return nullptr; }
};

/**
 * 二叉查找树复制构造函数.
 * 复制另一个树作为这个树的初始化.
 *
 * @param that: 被复制的树
 */
template<typename K, typename V>
BinarySearchTreeMap<K, V>::BinarySearchTreeMap(const BinarySearchTreeMap& that)
{
    less = that.less;
    root = copy(that.root);
}

/**
 * 二叉查找树移动构造函数.
 * 移动另一个树，其资源所有权转移到新创建的对象.
 *
 * @param that: 被移动的树
 */
template<typename K, typename V>
BinarySearchTreeMap<K, V>::BinarySearchTreeMap(BinarySearchTreeMap&& that)
{
    less = that.less;
    root = that.root;
    that.root = nullptr; // 指向空指针，退出被析构
}

/**
 * 返回以结点x为根的子树的结点数.
 *
 * @param x: 指定结点指针，指向的结点作为某个子树的根结点
 * @return 以结点x为根的子树的结点数
 */
template<typename K, typename V>
int BinarySearchTreeMap<K, V>::size(Node* x) const
{
    if (x == nullptr) return 0;
    else              return x->size;
}

/**
 * 返回以结点x为根的子树的高度.
 *
 * @param x: 指定结点指针，指向的结点作为某个子树的根结点
 * @return 以结点x为根的子树的高度
 */
template<typename K, typename V>
int BinarySearchTreeMap<K, V>::height(Node* x) const
{
    // 左右子树较高者加上本身
    if (x == nullptr) return 0;
    else              return 1 + std::max(height(x->lft), height(x->rht));
}

/**
 * 返回严格小于指定键的所有键的数量.
 * 当指定结点为空时返回0.
 *
 * @param x: 指定结点指针，指向的结点作为某个子树的根结点
 *        key: 要查找的键
 * @return 严格小于指定键的所有键的数量.
 */
template<typename K, typename V>
int BinarySearchTreeMap<K, V>::rank(Node* x, const K& key) const
{
    if (x == nullptr) return 0;
    if      (less(key, x->key)) return rank(x->lft, key);
    else if (less(x->key, key)) return size(x->lft) + 1 + rank(x->rht, key);
    else                        return size(x->lft);
}

/**
 * 复制以结点x为根的子树.
 *
 * @param x: 指定结点指针，指向的结点作为某个子树的根结点
 * @return 复制子树的根结点
 */
template<typename K, typename V>
typename BinarySearchTreeMap<K, V>::Node* BinarySearchTreeMap<K, V>::copy(Node* x) const
{
    if (x == nullptr) return nullptr;

    Node* y = new Node(x->key, x->val);

    y->lft = copy(x->lft);
    y->rht = copy(x->rht);
    return y;
}

/**
 * 得到升序排列下第i个键.
 *
 * @param x: 指定结点指针，指向的结点作为某个子树的根结点
 *        key: 指定键序列
 * @return x: 找到指定键，返回指向包含该键的结点的指针
 *         nullptr: 定位失败，返回空指针
 */
template<typename K, typename V>
typename BinarySearchTreeMap<K, V>::Node* BinarySearchTreeMap<K, V>::
    select(Node* x, int i) const
{
    if (x == nullptr) return nullptr;

    int j = size(x->lft);

    if      (i < j) return select(x->lft, i);
    else if (j > i) return select(x->rht, i - j - 1);
    else            return x;
}

/**
 * 从以结点x为根的子树开始查找，得到指定键的结点.
 *
 * @param x: 指定结点指针，指向的结点作为某个子树的根结点
 *        key: 要查找的键
 * @return x: 找到指定键，返回指向包含该键的结点的指针
 *         nullptr: 定位失败，返回空指针
 */
template<typename K, typename V>
typename BinarySearchTreeMap<K, V>::Node* BinarySearchTreeMap<K, V>::
    find(Node* x, const K& key) const
{
    if (x == nullptr) return nullptr;
    if      (less(key, x->key)) return find(x->lft, key);
    else if (less(x->key, key)) return find(x->rht, key);
    else                        return x;
}

/**
 * 从以结点x为根的子树开始查找，找到包含最小键的结点.
 *
 * @param x: 指定结点指针，指向的结点作为某个子树的根结点
 * @return 指向包含最小键的结点的指针
 */
template<typename K, typename V>
typename BinarySearchTreeMap<K, V>::Node* BinarySearchTreeMap<K, V>::
    minKey(Node* x) const
{
    if (x->lft == nullptr) return x;
    else                   return minKey(x->lft);
}

/**
 * 从以结点x为根的子树开始查找，找到包含最大键的结点.
 *
 * @param x: 指定结点指针，指向的结点作为某个子树的根结点
 * @return 指向包含最大键的结点的指针
 */
template<typename K, typename V>
typename BinarySearchTreeMap<K, V>::Node* BinarySearchTreeMap<K, V>::
    maxKey(Node* x) const
{
    if (x->rht == nullptr) return x;
    else                   return maxKey(x->rht);
}

/**
 * 将键值放入以结点x为根的子树中.
 *
 * @param x: 指定结点指针，指向的结点作为某个子树的根结点
 *        key: 键
 *        val: 键
 * @return 插入该键值对后的子树的根结点
 */
template<typename K, typename V>
typename BinarySearchTreeMap<K, V>::Node* BinarySearchTreeMap<K, V>::
    put(Node* x, const K& key, const V& val)
{
    if (x == nullptr) return new Node(key, val); // 创建新结点，以该键值对赋值
    if      (less(key, x->key)) x->lft = put(x->lft, key, val); // 小于根放入左子树
    else if (less(x->key, key)) x->rht = put(x->rht, key, val); // 大于根放入右子树
    else                        x->val = val; // 等于根修改原值
    x->size = size(x->lft) + 1 + size(x->rht); // 更新结点中的size值
    return x;
}

/**
 * 在以结点x为根的子树中查找指定键，并移除该键值对.
 *
 * @param x: 指定结点指针，指向的结点作为某个子树的根结点
 *        key: 要移除的键
 * @return 移除键值对后的子树的根结点
 */
template<typename K, typename V>
typename BinarySearchTreeMap<K, V>::Node* BinarySearchTreeMap<K, V>::
    remove(Node* x, const K& key)
{
    if (x == nullptr) return nullptr;
    // 查找结点
    if      (less(key, x->key)) x->lft = remove(x->lft, key);
    else if (less(x->key, key)) x->rht = remove(x->rht, key);
    else
    {
        // 左右子树均不为空，后继结点填补（即右子树下的最小结点）
        if (x->rht != nullptr && x->lft != nullptr)
        {
            Node* succ = minKey(x->rht); // 总是选后继结点导致树不平衡

            x->key = succ->key;
            x->val = succ->val;
            x->rht = removeMin(x->rht); // 删除后继结点
        }
        else
        {
            Node* tmp = x;

            if      (x->rht == nullptr) x = x->lft; // 右子树为空，左子女填补
            else if (x->lft == nullptr) x = x->rht; // 左子树为空，右子女填补
            delete tmp; // 释放掉原结点
            return x; // 无需更新结点计数
        }
    }
    x->size = size(x->lft) + 1 + size(x->rht);
    return x;
}

/**
 * 移除以结点x为根的子树中的最小键值对.
 *
 * @param x: 指定结点指针，指向的结点作为某个子树的根结点
 * @return 移除最小键值对后的子树的根结点
 */
template<typename K, typename V>
typename BinarySearchTreeMap<K, V>::Node* BinarySearchTreeMap<K, V>::
    removeMin(Node* x)
{
    if (x->lft == nullptr)
    {
        Node* tmp = x;

        x = x->rht;
        delete tmp;
        return x;
    }
    x->lft = removeMin(x->lft);
    x->size = size(x->lft) + 1 + size(x->rht);
    return x;
}

/**
 * 移除以结点x为根的子树中的最大键值对.
 *
 * @param x: 指定结点指针，指向的结点作为某个子树的根结点
 * @return 移除最大键值对后的子树的根结点
 */
template<typename K, typename V>
typename BinarySearchTreeMap<K, V>::Node* BinarySearchTreeMap<K, V>::
    removeMax(Node* x)
{
    if (x->rht == nullptr)
    {
        Node* tmp = x;

        x = x->lft;
        delete tmp;
        return x;
    }
    x->rht = removeMax(x->rht);
    x->size = size(x->lft) + 1 + size(x->rht);
    return x;
}

/**
 * 前序遍历以结点x为根的子树，将所有键存入队列.
 *
 * @param x: 指定结点指针，指向的结点作为某个子树的根结点
 *        queue: 存储键的队列
 */
template<typename K, typename V>
void BinarySearchTreeMap<K, V>::preOrder(Node* x, ArrayQueue<K>& queue) const
{
    if (x == nullptr) return;
    queue.enqueue(x->key);
    preOrder(x->lft, queue);
    preOrder(x->rht, queue);
}

/**
 * 中序遍历以结点x为根的子树，将所有键存入队列.
 *
 * @param x: 指定结点指针，指向的结点作为某个子树的根结点
 *        queue: 存储键的队列
 */
template<typename K, typename V>
void BinarySearchTreeMap<K, V>::inOrder(Node* x, ArrayQueue<K>& queue) const
{
    if (x == nullptr) return;
    preOrder(x->lft, queue);
    queue.enqueue(x->key);
    preOrder(x->rht, queue);
}

/**
 * 后序遍历以结点x为根的子树，将所有键存入队列.
 *
 * @param x: 指定结点指针，指向的结点作为某个子树的根结点
 *        queue: 存储键的队列
 */
template<typename K, typename V>
void BinarySearchTreeMap<K, V>::postOrder(Node* x, ArrayQueue<K>& queue) const
{
    if (x == nullptr) return;
    preOrder(x->lft, queue);
    preOrder(x->rht, queue);
    queue.enqueue(x->key);
}

/**
 * 打印以结点x为根的子树的形状.
 * 树中键值以"*"符号代替.
 * 根据子树存在与否，共┌┴┐、┌┘、└┐以及空四种形状.
 *
 * @param x: 指定结点指针，指向的结点作为某个子树的根结点
 */
template<typename K, typename V>
void BinarySearchTreeMap<K, V>::print(Node* x) const
{
    if (isEmpty()) return;

    ArrayQueue<Node*> queue; // 辅助队列存放下一层结点
    int h = height();
    int num = 0;
    int width = 0;

    std::cout << "Height of tree: " << h << std::endl;
    if (h > 6) // 树高大于6不打印
    {
        std::cout << "The tree is too big to be printed" << std::endl;
        return;
    }
    queue.enqueue(root);
    while ((h--) > 0)
    {
        num = queue.size();
        width = (int) exp2(h) - 1;
        for (int j = 0; j < num; ++j)
        {
            Node* x = nullptr;

            x = queue.dequeue();
            for (int k = 0; k < width; ++k) std::cout << " ";
            if (x == nullptr)
            {
                std::cout << " ";
                queue.enqueue(nullptr);
                queue.enqueue(nullptr);
            }
            else
            {
                std::cout << "*";
                queue.enqueue(x->lft);
                queue.enqueue(x->rht);
            }
            for (int k = 0; k < width; ++k) std::cout << " ";
            std::cout << " ";
        }
        std::cout << std::endl;
        num = queue.size();
        if (num == 0) continue;
        width = (int) exp2(h - 1) - 1;
        for (int j = 0; j < num; j += 2)
        {
            Node* lft = nullptr;
            Node* rht = nullptr;

            lft = queue.dequeue();
            rht = queue.dequeue();
            for (int k = 0; k < width; ++k) std::cout << " ";
            if (lft != nullptr && rht != nullptr)
            {
                std::cout << "┌";
                for (int k = 0; k < width; ++k) std::cout << "─";
                std::cout << "┴";
                for (int k = 0; k < width; ++k) std::cout << "─";
                std::cout << "┐";
            }
            else if (lft != nullptr)
            {
                std::cout << "┌";
                for (int k = 0; k < width; ++k) std::cout << "─";
                std::cout << "┘";
                for (int k = 0; k < width; ++k) std::cout << " ";
                std::cout << " ";
            }
            else if (rht != nullptr)
            {
                std::cout << " ";
                for (int k = 0; k < width; ++k) std::cout << " ";
                std::cout << "└";
                for (int k = 0; k < width; ++k) std::cout << "─";
                std::cout << "┐";
            }
            else
            {
                std::cout << " ";
                for (int k = 0; k < width; ++k) std::cout << " ";
                std::cout << " ";
                for (int k = 0; k < width; ++k) std::cout << " ";
                std::cout << " ";
            }
            for (int k = 0; k < width; ++k) std::cout << " ";
            std::cout << " ";
            queue.enqueue(lft);
            queue.enqueue(rht);
        }
        std::cout << std::endl;
    }
}

/**
 * 销毁以结点x为根的子树.
 *
 * @param x: 指定结点指针，指向的结点作为某个子树的根结点
 */
template<typename K, typename V>
void BinarySearchTreeMap<K, V>::destroy(Node* x)
{
    if (x != nullptr)
    {
        destroy(x->lft);
        destroy(x->rht);
        delete x;
    }
}

/**
 * 检查当前树是否正确.
 *
 * @param x: 指定结点指针，指向的结点作为某个子树的根结点
 * @return true: 正确
 *         false: 不正确，当前二叉查找树不合法
 */
template<typename K, typename V>
bool BinarySearchTreeMap<K, V>::isBST(Node* x) const
{
    if (x == nullptr) return true;
    if (x->lft != nullptr && less(x->key, x->lft->key)) return false;
    if (x->rht != nullptr && less(x->rht->key, x->key)) return false;
    return isBST(x->lft) && isBST(x->rht);
}

/**
 * 得到排名为i的键.
 *
 * @param i: 指定的键排名
 *        key: 得到的键
 * @return true: 获取键成功
 *         false: 获取键失败
 */
template<typename K, typename V>
K BinarySearchTreeMap<K, V>::keyOf(int i) const
{
    assert(i >= 0 && i < size());

    Node* x = select(root, i);

    if (x == nullptr) return false;
    return x->key;
}

/**
 * 得到指定键对应的值.
 * 键不能为空.
 *
 * @param key: 指定键
 * @return 指定键对应的值
 * @throws std::invalid_argument: 指定键不存在
 */
template<typename K, typename V>
V BinarySearchTreeMap<K, V>::valueOf(const K& key) const
{
    Node* x = find(root, key);

    if (x == nullptr) 
        throw std::invalid_argument("TreeMap key does not exist.");
    return x->val;
}

/**
 * 得到最小键.
 *
 * @return key: 得到的最小键
 * @throws std::out_of_range: 树映射空
 */
template<typename K, typename V>
K BinarySearchTreeMap<K, V>::minKey() const
{
    if (isEmpty()) 
        throw std::out_of_range("TreeMap underflow.");
    return minKey(root)->key;
}

/**
 * 得到最大键.
 *
 * @return key: 得到的最大键
 * @throws std::out_of_range: 树映射空
 */
template<typename K, typename V>
K BinarySearchTreeMap<K, V>::maxKey() const
{
    if (isEmpty()) 
        throw std::out_of_range("TreeMap underflow.");
    return maxKey(root)->key;
}

/**
 * 添加指定键值对到树中.
 * 首先查找整个树，若查找的键不存在，则插入新键值对.
 * 若查找的键存在，则让新值代替旧值.
 * 加入的键和值不能为空.
 *
 * @param key: 指定键
 *        val: 键对应的值
 */
template<typename K, typename V>
void BinarySearchTreeMap<K, V>::put(K key, V val)
{
    root = put(root, key, val);
    // assert(isBST());
}

/**
 * 移除树中指定键值对.
 *
 * @param key: 要移除的键
 */
template<typename K, typename V>
void BinarySearchTreeMap<K, V>::remove(const K& key)
{
    if (isEmpty()) return;
    root = remove(root, key);
    // assert(isBST());
}

/**
 * 移除树中最小键值对.
 */
template<typename K, typename V>
void BinarySearchTreeMap<K, V>::removeMin()
{
    if (isEmpty()) return;
    root = removeMin(root);
    // assert(isBST());
}

/**
 * 移除树中最大键值对.
 */
template<typename K, typename V>
void BinarySearchTreeMap<K, V>::removeMax()
{
    if (isEmpty()) return;
    root = removeMax(root);
    // assert(isBST());
}

/**
 * 按层次序遍历树，遍历的键放入队列.
 *
 * @param queue: 存储键的队列
 */
template<typename K, typename V>
void BinarySearchTreeMap<K, V>::levelOrder(ArrayQueue<K>& queue) const
{
    ArrayQueue<Node*> tmpq;

    tmpq.enqueue(root);
    while (!tmpq.isEmpty())
    {
        Node* x = tmpq.dequeue();

        if (x == nullptr) continue;
        queue.enqueue(x->key);
        tmpq.enqueue(x->lft);
        tmpq.enqueue(x->rht);
    }
}

/**
 * 交换当前BinarySearchTreeMap对象和另一个BinarySearchTreeMap对象.
 *
 * @param that: BinarySearchTreeMap对象that
 */
template<typename K, typename V>
void BinarySearchTreeMap<K, V>::swap(BinarySearchTreeMap<K, V>& that)
{
    using std::swap; // 如果没有针对类型的特化swap，则使用std::swap
    swap(root, that.root);
}

/**
 * =操作符重载.
 * 让当前BinarySearchTreeMap对象等于另一个BinarySearchTreeMap对象.
 *
 * @param that: BinarySearchTreeMap对象that
 * @return 当前BinarySearchTreeMap对象
 */
template<typename K, typename V>
BinarySearchTreeMap<K, V>& BinarySearchTreeMap<K, V>::
    operator=(BinarySearchTreeMap<K, V> that)
{
    swap(that); // *this与that互相交换，退出时that被析构
    return *this;
}

/**
 * <<操作符重载，按中序打印所有键.
 *
 * @param os: 输出流对象
 *        tree: 二叉查找树
 * @return 输出流对象
 */
template<typename K, typename V>
std::ostream& operator<<(std::ostream& os, const BinarySearchTreeMap<K, V>& tree)
{
    for (auto i : tree)
        os << i << " ";
    return os;
}

/**
 * 交换两个BinarySearchTreeMap对象.
 *
 * @param lhs: BinarySearchTreeMap对象lhs
 *        rhs: BinarySearchTreeMap对象rhs
 */
template<typename K, typename V>
void swap(BinarySearchTreeMap<K, V>& lhs, BinarySearchTreeMap<K, V>& rhs)
{
    lhs.swap(rhs);
}

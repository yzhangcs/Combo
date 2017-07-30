/*******************************************************************************
 * RedBlackTreeMap.h
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
#include "RedBlackTreeMap.h"
#include "ArrayStack.h"

/**
 * 使用模板实现的红黑树（Map）.
 * 红黑树是一种平衡二叉查找树，并与2-3树一一对应，其中两个结点之间的红链接
 * 对应2-3树中的3-结点（n-结点是指有n条子链的结点），即如果把红链接放平，
 * 就成为一个具有2个键和3个子女的结点.
 * 一个结点左右均为红链，这对应一个临时的4-结点（红链放平有3个键和4个子女）.
 * 红链接（或者一对父子结点，子结点的颜色是红色）都是左倾斜的，黑链接是普通链接.
 * 任何结点都不同时与两条红链接相连（允许有这样的临时结点存在，但是最终要做平衡处理）.
 * 红黑树是完美黑色平衡的，即任意空链接到根结点的路径上的黑链接数量相同.
 * 实现了中序遍历树的前向迭代器.
 */
template<typename K, typename V>
class RedBlackTreeMap
{
    using Comparator = std::function<bool(const K& lhs, const K& rhs)>;
private:
    static const bool RED = true;
    static const bool BLACK = false;

    struct Node
    {
        K key;
        V val;
        int size; // 以该结点为根的子树中的结点数
        bool colour; // 结点颜色，指和父结点之间链接的颜色
        Node* lft;
        Node* rht; // 左右子女指针        
        Node(K key, V val, bool colour) :
            key(std::move(key)), 
            val(std::move(val)),
            size(1), colour(colour), lft(nullptr), rht(nullptr) {}
    };
    Comparator less; // 比较器
    Node* root; // 根结点

    int size(Node* x) const;
    int height(Node* x) const;
    int rank(Node* x, const K& key) const;
    Node* copy(Node* x) const;
    Node* find(Node* x, int i) const;
    Node* find(Node* x, const K& key) const;
    Node* minKey(Node* x) const;
    Node* maxKey(Node* x) const;
    Node* put(Node* x, const K& key, const V& val);
    Node* remove(Node* x, const K& key);
    Node* removeMin(Node* x);
    Node* removeMax(Node* x);
    Node* rotateLft(Node* x);
    Node* rotateRht(Node* x);
    Node* moveRedLft(Node* x);
    Node* moveRedRht(Node* x);
    Node* balance(Node* x);    
    Node* flipColours(Node* x);
    void preOrder(Node* x, RedBlackTreeMap<K>& queue) const;
    void inOrder(Node* x, RedBlackTreeMap<K>& queue) const;
    void postOrder(Node* x, RedBlackTreeMap<K>& queue) const;
    void print(Node* x) const; // 打印树的结构
    void destroy(Node* x);
    // 判断父子链的颜色（结点颜色）是否为红色，空链为黑色
    bool isRed(Node* x) const { return x == nullptr ? false : x->colour == RED; }
    bool isBST() const { return isBST(root); } // 检查当前树是否为二叉查找树
    bool isBST(Node* x) const; // 检查指定根结点的树是否为二叉查找树
    bool is23Tree() const { return is23Tree(root); } // 检查当前树是否为2-3树
    bool is23Tree(Node* x) const; // 检查指定根结点的树是否为2-3树
    bool isBalanced() const; // 检查树是否黑色平衡
    bool isBalanced(Node* x, int black) const; // 检查指定根结点的树是否黑色平衡
    bool equal_to(const K& lhs, const K& rhs) const
    { return !less(lhs, rhs) && !less(rhs, lhs); }
public:
    explicit RedBlackTreeMap(Comparator comp = std::less<K>()) :
        root(nullptr), less(comp) {} // 构造函数
    RedBlackTreeMap(const RedBlackTreeMap& that); // 复制构造函数
    RedBlackTreeMap(RedBlackTreeMap&& that); // 移动构造函数
    ~RedBlackTreeMap() { destroy(root); } // 析构函数

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
    void preOrder(RedBlackTreeMap<K>& queue) const { preOrder(root, queue); } // 前序遍历
    void inOrder(RedBlackTreeMap<K>& queue) const { inOrder(root, queue); } // 中序遍历
    void postOrder(RedBlackTreeMap<K>& queue) const { postOrder(root, queue); } // 后序遍历
    void levelOrder(RedBlackTreeMap<K>& queue) const; // 层次序遍历
    void print() const { print(root); } // 打印树结构
    void swap(RedBlackTreeMap& that); // 内容与另一个RedBlackTreeMap对象交换
    void clear() { destroy(root); } // 清空树
    
    RedBlackTreeMap& operator=(RedBlackTreeMap that);
    template <typename T, typename U>
    friend std::ostream& operator<<(std::ostream& os, const RedBlackTreeMap<T, U>& Set);

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
 * 红黑树复制构造函数.
 * 复制另一个树作为这个树的初始化.
 *
 * @param that: 被复制的红黑树
 */
template<typename K, typename V>
RedBlackTreeMap<K, V>::RedBlackTreeMap(const RedBlackTreeMap& that)
{
    less = that.less;
    root = copy(that.root);
}

/**
 * 红黑树移动构造函数.
 * 移动另一个树，其资源所有权转移到新创建的对象.
 *
 * @param that: 被移动的树
 */
template<typename K, typename V>
RedBlackTreeMap<K, V>::RedBlackTreeMap(RedBlackTreeMap&& that)
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
int RedBlackTreeMap<K, V>::size(Node* x) const
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
int RedBlackTreeMap<K, V>::height(Node* x) const
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
int RedBlackTreeMap<K, V>::rank(Node* x, const K& key) const
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
typename RedBlackTreeMap<K, V>::Node* RedBlackTreeMap<K, V>::
    copy(Node* x) const
{
    if (x == nullptr) return nullptr;

    Node* y = new Node(x->key, x->val, x->colour);

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
typename RedBlackTreeMap<K, V>::Node* RedBlackTreeMap<K, V>::
    find(Node* x, int i) const
{
    if (x == nullptr) return nullptr;

    int j = size(x->lft);

    if      (i < j) return find(x->lft, i);
    else if (j > i) return find(x->rht, i - j - 1);
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
typename RedBlackTreeMap<K, V>::Node* RedBlackTreeMap<K, V>::
    find(Node* x, const K& key) const
{
    if (x == nullptr) return nullptr;
    if      (less(key, x->key)) return find(x->lft,  key);
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
typename RedBlackTreeMap<K, V>::Node* RedBlackTreeMap<K, V>::
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
typename RedBlackTreeMap<K, V>::Node* RedBlackTreeMap<K, V>::
    maxKey(Node* x) const
{
    if (x->rht == nullptr) return x;
    else                   return maxKey(x->rht);
}

/**
 * 将键值放入以结点x为根的子树中.
 * 新插入的结点颜色设为红.
 *
 * @param x: 指定结点指针，指向的结点作为某个子树的根结点
 *        key: 键
 *        val: 键
 * @return 插入该键值对后的子树的根结点
 */
template<typename K, typename V>
typename RedBlackTreeMap<K, V>::Node* RedBlackTreeMap<K, V>::
    put(Node* x, const K& key, const V& val)
{
    // 创建新结点，以该键值对赋值，新插入的结点颜色为红
    if (x == nullptr) return new Node(key, val, RED); 
    // 与二叉查找树相同的操作
    if      (less(key, x->key)) x->lft = put(x->lft, key, val); // 小于根放入左子树
    else if (less(x->key, key)) x->rht = put(x->rht, key, val); // 大于根放入右子树
    else                        x->val = val; // 等于根修改原值
    x = balance(x); // 自底向上平衡，变换有2条红链的结点（拆分4-结点）
    return x;
}

/**
 * 在以结点x为根的子树中查找指定键，并移除该键值对.
 * 删除黑色结点会导致黑不平衡，因此自顶向下删除时，保证结点具有红链，即在3-结点或4-结点中.
 *
 * @param x: 指定结点指针，指向的结点作为某个子树的根结点
 *        key: 要移除的键
 * @return 移除键值对后的子树的根结点
 */
template<typename K, typename V>
typename RedBlackTreeMap<K, V>::Node* RedBlackTreeMap<K, V>::
    remove(Node* x, const K& key)
{
    // 指定键小于x的键，准备向左走
    if (less(key, x->key))
    {
        if (x->lft == nullptr) return x; // 左边为空则返回
        // 左边是2-结点，左结点没有与红链相连，则左边增加红链
        if (!isRed(x->lft) && !isRed(x->lft->lft))
            x = moveRedLft(x);
        x->lft = remove(x->lft, key);
    }
    else 
    {
        if (isRed(x->lft))
            x = rotateRht(x);
        // 被查找的键在树的底部可以直接删除
        if (equal_to(key, x->key) && (x->rht == nullptr))
        {
            delete x;
            return nullptr;
        }
        if (x->rht == nullptr) return x; // 右边为空则返回
        // 右边为2-结点，右结点没有与红链相连，则右边增加红链
        if (!isRed(x->rht) && !isRed(x->rht->lft))
            x = moveRedRht(x);
        // 找到键且键不在底部，则与后继结点交换，删除后继结点
        if (equal_to(key, x->key)) 
        {
            Node* min = minKey(x->rht);

            x->key = min->key;
            x->val = min->val;
            x->rht = removeMin(x->rht);
        }
        else x->rht = remove(x->rht, key);
    }
    return balance(x); // 自底向上平衡，变换有2条红链的结点（拆分4-结点）
}

/**
 * 移除以结点x为根的子树中的最小键值对.
 * 删除黑色结点会导致黑不平衡，因此自顶向下删除时，保证结点具有红链，即在3-结点或4-结点中.
 *
 * @param x: 指定结点指针，指向的结点作为某个子树的根结点
 * @return 移除最小键值对后的子树的根结点
 */
template<typename K, typename V>
typename RedBlackTreeMap<K, V>::Node* RedBlackTreeMap<K, V>::
    removeMin(Node* x)
{
    // x的颜色必为红
    if (x->lft == nullptr)
    {
        delete x; // 找到最小即删除
        // 平衡的红黑树不存在左空右非空的情况，因此x没有右子树
        return nullptr;
    }
    // 左链为黑且左结点的左链为黑，左边为2-结点，
    // 则通过移动或者合并，使左边多一条红链，成为3-结点或临时4-结点的一部分
    if (!isRed(x->lft) && !isRed(x->lft->lft))
        x = moveRedLft(x);
    x->lft = removeMin(x->lft);
    return balance(x); // 自底向上平衡，变换有2条红链的结点（拆分4-结点）
}

/**
 * 移除以结点x为根的子树中的最大键值对.
 * 删除黑色结点会导致黑不平衡，因此自顶向下删除时，保证结点具有红链，即在3-结点或4-结点中.
 *
 * @param x: 指定结点指针，指向的结点作为某个子树的根结点
 * @return 移除最大键值对后的子树的根结点
 */
template<typename K, typename V>
typename RedBlackTreeMap<K, V>::Node* RedBlackTreeMap<K, V>::
    removeMax(Node* x)
{
    // 相比removeMin多一个右旋操作，这使红链右倾，保证删除时结点在红链上，且左子女为空
    if (isRed(x->lft))
        x = rotateRht(x);
    if (x->rht == nullptr)
    {
        delete x; // 找到最大即删除
        return nullptr;
    }
    // 右边没有红链，为2-结点，则通过移动或者合并，增加右边红链
    if (!isRed(x->rht) && !isRed(x->rht->lft))
        x = moveRedRht(x);
    x->rht = removeMax(x->rht);
    return balance(x); // 自底向上平衡，变换有2条红链的结点（拆分4-结点）
}

/**
 * 使右倾的链接向左倾斜.
 *
 * @param x: 指定结点指针
 * @return 旋转后重定向的结点指针
 */
template<typename K, typename V>
typename RedBlackTreeMap<K, V>::Node* RedBlackTreeMap<K, V>::
    rotateLft(Node* x)
{
    assert(x != nullptr && isRed(x->rht)); // x的右链应当为红

    Node* y = x->rht;

    x->rht = y->lft; // y的左子树给x作为其右子树
    y->lft = x; // x变成y的左子树
    y->colour = x->colour; // 保留原来的颜色
    x->colour = RED; // x变成红色
    return y; // y则取代x
}

/**
 * 使左倾的链接向右倾斜.
 *
 * @param x: 指定结点指针
 * @return 旋转后重定向的结点指针
 */
template<typename K, typename V>
typename RedBlackTreeMap<K, V>::Node* RedBlackTreeMap<K, V>::
    rotateRht(Node* x)
{
    assert(x != nullptr && isRed(x->lft)); // x的左链应当为红

    Node* y = x->lft;

    x->lft = y->rht; // y的右子树给x作为其左子树
    y->rht = x; // x变成y的右子树
    y->colour = x->colour; // 保留原来的颜色
    x->colour = RED; // x变成红色
    return y; // y则取代x
}

/**
 * 当前结点为红色，同时左链和左结点的左链为黑，使左链或其中一个子女成为红色.
 * 即要么使左边合并成一个4-结点，要么使左边成为3-结点.
 * 左边增加的结点来自于父结点，若右边不是2-结点，那么右边还要移一个到父结点.
 * 
 * @param x: 指定结点指针，指向的结点作为某个子树的根结点
 *
 * @return 移除后重定向的结点指针
 */
template<typename K, typename V>
typename RedBlackTreeMap<K, V>::Node* RedBlackTreeMap<K, V>::
    moveRedLft(Node* x)
{
    assert(x != nullptr);
    assert(isRed(x) && !isRed(x->lft) && !isRed(x->lft->lft));
    flipColours(x); // 颜色翻转，x的左右结点变红，与x合并，成为临时4-结点
    // 若右结点的左链为红，即右边不是2-结点，则移动右边一个键
    if (isRed(x->rht->lft)) 
    { // 右边移到父结点，父结点移到左边，左边成为3-结点
        x->rht = rotateRht(x->rht);
        x = rotateLft(x);
        flipColours(x);
    }
    return x;
}

/**
 * 当前结点为红色，同时右链和右结点的左链为黑，使右链或其中一个子女成为红色.
 * 即要么使右边合并成一个4-结点，要么使右边成为3-结点.
 * 右边增加的结点来自于父结点，若左边不是2-结点，那么左边还要移一个到父结点.
 *
 * @param x: 指定结点指针，指向的结点作为某个子树的根结点
 * @return 移除后重定向的结点指针
 */
template<typename K, typename V>
typename RedBlackTreeMap<K, V>::Node* RedBlackTreeMap<K, V>::
    moveRedRht(Node* x)
{
    assert(x != nullptr);
    assert(isRed(x) && !isRed(x->rht) && !isRed(x->rht->lft));
    flipColours(x); // 颜色翻转，x的左右结点变红，与x合并，成为临时4-结点
    // 若左结点的左链为红，即左边不是2-结点，则移动左边一个键
    if (isRed(x->lft->lft))
    {
        x = rotateRht(x);
        flipColours(x);
    }
    return x;
}

/**
 * 修正链接，使指定结点下的子树恢复完美黑平衡.
 * 修正分为四种情况，其中左黑右黑为正常情况，不须修正，而左黑右红、左红右红
 * 和左红左子红（两条红链）需要通过旋转来修正.
 * 其余复杂的情况应该在子结点的时候被处理，这样每个结点只需修正简单的三种即可恢复平衡.
 * 平衡操作是自底向上被调用.
 *
 * @param x: 指定结点指针，指向的结点作为某个子树的根结点
 * @return 修正链接后的重定向的结点指针
 */
template<typename K, typename V>
typename RedBlackTreeMap<K, V>::Node* RedBlackTreeMap<K, V>::
    balance(Node* x)
{
    assert(x != nullptr);
    if (isRed(x->rht) && !isRed(x->lft))     x = rotateLft(x); // 左黑右红，左旋转
    if (isRed(x->lft) && isRed(x->lft->lft)) x = rotateRht(x); // 左红左子红，右旋转后左红右红，继续处理    
    if (isRed(x->lft) && isRed(x->rht))      flipColours(x); // 左红右红，翻转颜色，等于分拆4-结点
    x->size = size(x->lft) + 1 + size(x->rht); // 更新结点中的size值
    return x;
}

/**
 * 翻转指定结点及其子女的颜色，结点和子女均不能为空.
 * 当且仅当结点红左右子女为黑，或者结点黑左右子女为红时，颜色才能被翻转.
 *
 * @param x: 指定结点指针
 * @return 翻转颜色之后的结点指针
 */
template<typename K, typename V>
typename RedBlackTreeMap<K, V>::Node* RedBlackTreeMap<K, V>::
    flipColours(Node* x)
{
    assert(x != nullptr && x->lft != nullptr && x->rht != nullptr);
    assert ((!isRed(x) && isRed(x->lft) && isRed(x->rht)) ||
            (isRed(x) && !isRed(x->lft) && !isRed(x->rht)));
    x->colour = !x->colour; // 结点与子女的颜色全部翻转
    x->lft->colour = !x->lft->colour;
    x->rht->colour = !x->rht->colour;
    return x;
}

/**
 * 前序遍历以结点x为根的子树，将所有键存入队列.
 *
 * @param x: 指定结点指针，指向的结点作为某个子树的根结点
 *        queue: 存储键的队列
 */
template<typename K, typename V>
void RedBlackTreeMap<K, V>::preOrder(Node* x, RedBlackTreeMap<K>& queue) const
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
void RedBlackTreeMap<K, V>::inOrder(Node* x, RedBlackTreeMap<K>& queue) const
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
void RedBlackTreeMap<K, V>::postOrder(Node* x, RedBlackTreeMap<K>& queue) const
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
void RedBlackTreeMap<K, V>::print(Node* x) const
{
    if (isEmpty()) return;

    RedBlackTreeMap<Node*> queue; // 辅助队列存放下一层结点
    int h = height();
    int num = 0;
    int width = 0;

    std::cout << "Height of tree: " << h << std::endl;
    if (h > 7) // 树高大于7不打印
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
                // std::cout << x->key;
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
                isRed(lft) ? std::cout << "┎" : std::cout << "┌";
                for (int k = 0; k < width; ++k) std::cout << "─";
                std::cout << "┴";
                for (int k = 0; k < width; ++k) std::cout << "─";
                isRed(rht) ? std::cout << "┒" : std::cout << "┐";
            }
            else if (lft != nullptr)
            {
                isRed(lft) ? std::cout << "┎" : std::cout << "┌";
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
                isRed(rht) ? std::cout << "┒" : std::cout << "┐";
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
void RedBlackTreeMap<K, V>::destroy(Node* x)
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
bool RedBlackTreeMap<K, V>::isBST(Node* x) const
{
    if (x == nullptr) return true;
    if (x->lft != nullptr && less(x->key, x->lft->key)) return false;
    if (x->rht != nullptr && less(x->rht->key, x->key)) return false;
    return isBST(x->lft) && isBST(x->rht);
}

/**
 * 检查指定根结点的树是否为2-3树.
 * 如果是2-3树，则没有红色的右链（红黑树的实现是左倾斜的），
 * 且每个结点最多连接一条红色左链（没有4-结点未被分解）.
 *
 * @param x: 指定结点指针，指向的结点作为某个子树的根结点
 * @return true: 是2-3树
 *         false: 不是2-3树
 */
template<typename K, typename V>
bool RedBlackTreeMap<K, V>::is23Tree(Node* x) const
{
    if (x == nullptr)                           return true;
    if (isRed(x->rht))                          return false;
    if (x != root && isRed(x) && isRed(x->lft)) return false;
    return is23Tree(x->lft) && is23Tree(x->rht);
}

/**
 * 检查树是否黑色平衡.
 *
 * @return true: 树黑色平衡
 *         false: 树黑色不平衡
 */
template<typename K, typename V>
bool RedBlackTreeMap<K, V>::isBalanced() const
{
    Node* x = root;
    int black = 0; // 黑链的数量

    while (x != nullptr)
    {
        if (!isRed(x)) black++;
        x = x->lft;
    }
    return isBalanced(root, black);
}

/**
 * 检查在以指定结点为根的树上，其每条根到叶子的路径上的黑链树是否等于指定的值.
 *
 * @param x: 指定结点指针，指向的结点作为某个子树的根结点
 *        black: 指定的黑链的数量
 * @return true: 等于指定黑链数
 *         false: 不等于指定黑链数
 */
template<typename K, typename V>
bool RedBlackTreeMap<K, V>::isBalanced(Node* x, int black) const
{
    if (x == nullptr) return black == 0;
    if (!isRed(x)) black--; // 是黑链，下一层递归之前黑链数先减一
    return isBalanced(x->lft, black) && isBalanced(x->rht, black);
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
K RedBlackTreeMap<K, V>::keyOf(int i) const
{
    assert(i >= 0 && i < size());

    Node* x = find(root, i);

    if (x == nullptr) return false;
    key = x->key;
    return true;
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
V RedBlackTreeMap<K, V>::valueOf(const K& key) const
{
    Node* x = find(root, key);

    if (x == nullptr)
        throw std::invalid_argument("TreeMap key does not exist.");
    return x->val;
}

/**
 * 得到最小键.
 *
 * @param key: 得到的最小键
 *        val: 键对应的值
 * @return true: 获取最小键成功
 *         false: 获取最小键失败
 * @throws std::out_of_range: 树映射空
 */
template<typename K, typename V>
bool RedBlackTreeMap<K, V>::minKey() const
{
    if (isEmpty()) 
        throw std::out_of_range("TreeMap underflow.");

    Node* x = minKey(root);

    key = x->key;
    val = x->val;
    return true;
}

/**
 * 得到最大键.
 *
 * @param key: 得到的最大键
 *        val: 键对应的值
 * @return true: 获取最大键成功
 *         false: 获取最大键失败
 * @throws std::out_of_range: 树映射空
 */
template<typename K, typename V>
bool RedBlackTreeMap<K, V>::maxKey() const
{
    if (isEmpty()) 
        throw std::out_of_range("TreeMap underflow.");

    Node* x = maxKey(root);

    key = x->key;
    val = x->val;
    return true;
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
void RedBlackTreeMap<K, V>::put(K key, V val)
{
    root = put(root, key, val);
    root->colour = BLACK; // 保证根结点为黑色
    // assert(isBST() && is23Tree() && isBalanced());
}

/**
 * 移除树中指定键值对.
 *
 * @param key: 要移除的键
 */
template<typename K, typename V>
void RedBlackTreeMap<K, V>::remove(const K& key)
{
    if (isEmpty()) return;

    // 根左右子女均为黑，设置根为红
    if (!isRed(root->lft) && !isRed(root->rht))
        root->colour = RED;
    root = remove(root, key);
    if (!isEmpty()) root->colour = BLACK; // 保证根结点为黑
    // assert(isBST() && is23Tree() && isBalanced());
}

/**
 * 移除树中最小键值对.
 */
template<typename K, typename V>
void RedBlackTreeMap<K, V>::removeMin()
{
    if (isEmpty()) return;

    // 根左右子女均为黑，设置根为红
    if (!isRed(root->lft) && !isRed(root->rht))
        root->colour = RED;
    root = removeMin(root);
    if (!isEmpty()) root->colour = BLACK; // 保证根结点为黑
    // assert(isBST() && is23Tree() && isBalanced());
}

/**
 * 移除树中最大键值对.
 */
template<typename K, typename V>
void RedBlackTreeMap<K, V>::removeMax()
{
    if (isEmpty()) return;

    // 根左右子女均为黑，设置根为红
    if (!isRed(root->lft) && !isRed(root->rht))
        root->colour = RED;
    root = removeMax(root);
    if (!isEmpty()) root->colour = BLACK; // 保证根结点为黑
    // assert(isBST() && is23Tree() && isBalanced());
}

/**
 * 按层次序遍历树，遍历的键放入队列.
 *
 * @param queue: 存储键的队列
 */
template<typename K, typename V>
void RedBlackTreeMap<K, V>::levelOrder(ArrayQueue<K>& queue) const
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
 * 交换当前RedBlackTreeMap对象和另一个RedBlackTreeMap对象.
 *
 * @param that: RedBlackTreeMap对象that
 */
template<typename K, typename V>
void RedBlackTreeMap<K, V>::swap(RedBlackTreeMap<K, V>& that)
{
    using std::swap; // 如果没有针对类型的特化swap，则使用std::swap
    swap(root, that.root);
}

/**
 * =操作符重载.
 * 让当前RedBlackTreeMap对象等于另一个RedBlackTreeMap对象.
 *
 * @param that: RedBlackTreeMap对象that
 * @return 当前RedBlackTreeMap对象
 */
template<typename K, typename V>
RedBlackTreeMap<K, V>& RedBlackTreeMap<K, V>::
    operator=(RedBlackTreeMap<K, V> that)
{
    swap(that); // *this与that互相交换，退出时that被析构
    return *this;
}

/**
 * <<操作符重载，按中序打印所有键.
 *
 * @param os: 输出流对象
 *        tree: 红黑树对象
 * @return 输出流对象
 */
template<typename K, typename V>
std::ostream& operator<<(std::ostream& os, const RedBlackTreeMap<K, V>& tree)
{
    for (auto i : tree)
        os << i << " ";
    return os;
}

/**
 * 交换两个RedBlackTreeMap对象.
 *
 * @param lhs: RedBlackTreeMap对象lhs
 *        rhs: RedBlackTreeMap对象rhs
 */
template<typename K, typename V>
void swap(RedBlackTreeMap<K, V>& lhs, RedBlackTreeMap<K, V>& rhs)
{
    lhs.swap(rhs);
}
/*******************************************************************************
 * List.h
 *
 * Author: zhangyu
 * Date: 2017.4.25
 ******************************************************************************/

#pragma once
#include <iostream>
#include <iterator>

namespace combo
{

/**
 * 使用模板实现的链表.
 * 实现了链表的双向迭代器.
 */
template<typename E>
class List
{
	using iterator = class ListIterator;

    struct Node
    {
        E elem;
        Node* prev;
        Node* next;
        Node() : elem(), prev(this), next(this) {}
        Node(E elem) : elem(std::move(elem)), prev(this), next(this) {}
    };
public:
    List() : n(0), sentinel(new Node) {}
    List(const List& that);
    List(List&& that) noexcept;
    ~List();

    // 返回链表元素的数量
    int size() const { return n; }
    // 判断是否为空链表
    bool empty() const { return n == 0; }
    // 添加元素到指定位置
    void insert(iterator pos, E elem);
    // 添加元素到链表头部
    void insert_front(E elem);
    // 添加元素到链表尾部
    void insert_back(E elem);
    // 移除指定位置的元素
    void remove(iterator pos);
    // 移除链表头部元素
    void remove_front();
    // 移除链表尾部元素
    void remove_back();
    // 返回链表头部元素的引用
    E& front() { return const_cast<E&>(static_cast<const List&>(*this).front()); }
    // 返回链表头部元素的const引用
    const E& front() const;
    // 返回链表尾部元素的引用
    E& back() { return const_cast<E&>(static_cast<const List&>(*this).back()); }
    // 返回链表尾部元素的const引用
    const E& back() const;
    // 内容与另一个LinkedList对象交换
    void swap(List& that);
    // 清空链表
    void clear();

    List& operator=(List that);
    List& operator+=(const List& that);
    template<typename T>
    friend List<T> operator+(List<T> lhs, const List<T>& rhs);
    template <typename T>
    friend bool operator==(const List<T>& lhs, const List<T>& rhs);
    template <typename T>
    friend bool operator!=(const List<T>& lhs, const List<T>& rhs);
    template<typename T>
    friend std::ostream& operator<<(std::ostream& os, const List<T>& list);

    // 链表不支持随机访问
    class iterator : public std::iterator<std::bidirectional_iterator_tag, E>
    {
        friend class List;
    private:
        Node* pn;
    public:
        iterator() : pn(nullptr) {}
        iterator(Node* x) : pn(x) {}
        iterator(const iterator& that) : pn(that.pn) {}
        ~iterator() {}

        E& operator*() const
        { return pn->elem; }
        E* operator->() const
        { return &pn->elem; }
        iterator& operator++()
        {
            pn = pn->next;
            return *this;
        }
        iterator operator++(int)
        {
            iterator tmp(*this);
            ++*this;
            return tmp;
        }
        iterator& operator--()
        {
            pn = pn->prev;
            return *this;
        }
        iterator operator--(int)
        {
            iterator tmp(*this);
            --*this;
            return tmp;
        }
        bool operator==(const iterator& that) const
        { return pn == that.pn; }
        bool operator!=(const iterator& that) const
        { return pn != that.pn; }
    };
    iterator begin() const { return iterator(sentinel->next); }
    iterator end() const { return iterator(sentinel); }
private:
    int n; // 链表大小
    Node* sentinel; // 哨兵指针

    // 定位指定元素
    Node* locate(int i) const;
    // 检查索引是否合法
    bool valid(int i) const { return i >= 0 && i < n; }
};

/**
 * 链表复制构造函数.
 * 复制另一个链表作为这个链表的初始化.
 *
 * @param that: 被复制的链表
 */
template<typename E>
List<E>::List(const List& that)
{
    n = 0;
    sentinel = new Node();
    for (auto i : that)
        insert_back(i);
}

/**
 * 链表移动构造函数.
 * 移动另一个链表，其资源所有权转移到新创建的对象.
 *
 * @param that: 被移动的链表
 */
template<typename E>
List<E>::List(List&& that) noexcept
{
    n = that.n;
    sentinel = that.sentinel;
    that.sentinel = nullptr; // 指向空指针，退出被析构
}

/**
 * 链表析构函数.
 */
template<typename E>
List<E>::~List()
{
    clear();
    delete sentinel;
    sentinel = nullptr;
}

/**
 * 定位指定位置的元素.
 * 返回指向该位置元素的指针.
 *
 * @param i: 指定元素的索引
 * @return 指向该位置元素的指针
 * @throws std::out_of_range: 索引不合法
 */
template<typename E>
typename List<E>::Node* List<E>::locate(int i) const
{
    if (!valid(i))
        throw std::out_of_range("List::locate() index out of range.");
    if (i < (n >> 1)) return std::next(begin(), i).pn;
    else              return std::prev(end(), n - i).pn;
}

/**
 * 添加元素到链表指定位置.
 *
 * @param i: 要添加元素的索引
 *        elem: 要添加的元素
 * @throws std::out_of_range: 索引不合法
 */
template<typename E>
void List<E>::insert(iterator pos, E elem)
{
    Node* prec = nullptr;
    Node* succ = nullptr; // 指定位置的前驱和后继
    Node* pnew = nullptr;

    if (i == n) succ = sentinel;
    else        succ = locate(i);
    pnew = new Node(std::move(elem));
    prec = succ->prev;
    prec->next = pnew;
    pnew->prev = prec;
    pnew->next = succ;
    succ->prev = pnew;
    n++;
}

/**
 * 添加元素到链表头部.
 *
 * @param elem: 要添加的元素
 */
template<typename E>
void List<E>::insert_front(E elem)
{
    Node* succ = sentinel->next;
    Node* pnew = new Node(std::move(elem));

    sentinel->next = pnew;
    pnew->prev = sentinel;
    pnew->next = succ;
    succ->prev = pnew;
    n++;
}

/**
 * 添加元素到链表尾部.
 *
 * @param elem: 要添加的元素
 */
template<typename E>
void List<E>::insert_back(E elem)
{
    Node* prec = sentinel->prev;
    Node* pnew = new Node(std::move(elem));

    prec->next = pnew;
    pnew->prev = prec;
    pnew->next = sentinel;
    sentinel->prev = pnew;
    n++;
}

/**
 * 移除链表中指定位置的元素.
 *
 * @param i: 要移除元素的索引
 * @throws std::out_of_range: 索引不合法
 */
template<typename E>
void List<E>::remove(iterator pos)
{
    Node* pold = locate(i);
    Node* prec = pold->prev;
    Node* succ = pold->next; // 指定位置的前驱和后继

    prec->next = succ;
    succ->prev = prec;
    delete pold;
    n--;
}

/**
 * 移除链表头部元素.
 *
 * @throws std::out_of_range: 队空
 */
template<typename E>
void List<E>::remove_front()
{
    if (empty())
        throw std::out_of_range("List::remove_front");

    Node* pold = sentinel->next;
    Node* succ = pold->next;

    sentinel->next = succ;
    succ->prev = sentinel;
    delete pold;
    n--;
}

/**
 * 移除链表尾部元素.
 *
 * @throws std::out_of_range: 队空
 */
template<typename E>
void List<E>::remove_back()
{
    if (empty())
        throw std::out_of_range("List::remove_back");

    Node* pold = sentinel->prev;
    Node* prec = pold->prev;

    prec->next = sentinel;
    sentinel->prev = prec;
    delete pold;
    n--;
}

/**
 * 返回链表头部元素的const引用.
 *
 * @return 链表头部元素的const引用
 * @throws std::out_of_range: 链表为空
 */
template<typename E>
const E& List<E>::front() const
{
    if (empty())
        throw std::out_of_range("List::front");
    return *begin();
}

/**
 * 返回链表尾部元素的const引用.
 *
 * @return 链表尾部元素的const引用
 * @throws std::out_of_range: 链表为空
 */
template<typename E>
const E& List<E>::back() const
{
    if (empty())
        throw std::out_of_range("List::back");
    return *std::prev(end());
}

/**
 * 交换当前List对象和另一个List对象.
 *
 * @param that: List对象that
 */
template<typename E>
void List<E>::swap(List<E>& that)
{
    using std::swap;
    swap(n, that.n);
    swap(sentinel, that.sentinel);
}

/**
 * 清空该链表元素.
 */
template<typename E>
void List<E>::clear()
{
    if (empty()) return;
    if (sentinel == nullptr) return;

    Node* current = sentinel->next;
    // 释放每个结点内存
    while (current != sentinel)
    {
        sentinel->next = current->next;
        delete current;
        current = sentinel->next;
    }
    sentinel->prev = sentinel;
    n = 0;
}

/**
 * =操作符重载.
 * 让当前List对象等于给定List对象that.
 *
 * @param that: List对象that
 * @return 当前List对象
 */
template<typename E>
List<E>& List<E>::operator=(List<E> that)
{
    swap(that);
    return *this;
}

/**
 * +=操作符重载.
 * 复制另一个对象所有元素,添加到当前对象.
 *
 * @param that: List对象that
 * @return 当前List对象
 */
template<typename E>
List<E>& List<E>::operator+=(const List<E>& that)
{
    for (auto i : that)
        insert_back(i);
    return *this;
}

/**
 * +操作符重载.
 * 返回一个包含lhs和rhs所有元素的对象.
 *
 * @param lhs: List对象lhs
 *        rhs: List对象rhs
 * @return 包含lhs和rhs所有元素的List对象
 */
template<typename E>
List<E> operator+(List<E> lhs, const List<E>& rhs)
{
    lhs += rhs;
    return lhs;
}

/**
 * ==操作符重载函数，比较两个List对象是否相等.
 *
 * @param lhs: List对象lhs
 *        rhs: List对象rhs
 * @return true: 相等
 *         false: 不等
 */
template<typename E>
bool operator==(const List<E>& lhs, const List<E>& rhs)
{
    if (&lhs == &rhs)             return true;
    if (lhs.size() != rhs.size()) return false;
    return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

/**
 * !=操作符重载函数，比较两个List对象是否不等.
 *
 * @param lhs: List对象lhs
 *        rhs: List对象rhs
 * @return true: 不等
 *         false: 相等
 */
template<typename E>
bool operator!=(const List<E>& lhs, const List<E>& rhs)
{
    return !(lhs == rhs);
}

/**
 * <<操作符重载函数，打印所有链表元素.
 *
 * @param os: 输出流对象
 *        list: 要输出的链表
 * @return 输出流对象
 */
template<typename E>
std::ostream& operator<<(std::ostream& os, const List<E>& list)
{
    for (auto i : list)
        os << i << " ";
    return os;
}

/**
 * 交换两个List对象.
 *
 * @param lhs: List对象lhs
 *        rhs: List对象rhs
 */
template<typename E>
void swap(List<E>& lhs, List<E>& rhs)
{
    lhs.swap(rhs);
}

} // namespace combo

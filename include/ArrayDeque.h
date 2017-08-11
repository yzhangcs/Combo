/*******************************************************************************
 * ArrayDeque.h
 *
 * Author: zhangyu
 * Date: 2017.4.23
 ******************************************************************************/

#pragma once
#include <cassert>
#include <iostream>
#include <iterator>

/**
 * 使用模板实现的双端队列. 
 * 由动态连续数组存储双端队列. 
 * 实现了数组双端队列的双向迭代器.
 */
template<typename E>
class ArrayDeque
{
    static const int DEFAULT_CAPACITY = 10; // 默认双端队列容量
private:
    int n;    // 队列大小
    int N;    // 队列容量
    int head; // 队首索引
    int tail; // 队尾索引
    E* pd;    // 双端队列指针
    
    void resize(int size); // 调整双端队列容量
public:
    explicit ArrayDeque(int cap = DEFAULT_CAPACITY); 
    ArrayDeque(const ArrayDeque& that);
    ArrayDeque(ArrayDeque&& that) noexcept;
    ~ArrayDeque() { delete[] pd; }

    int size() const { return n; } // 返回双端队列当前大小
    int capacity() const { return N; } // 返回双端队列容量
    bool empty() const { return n == 0; } // 判断是否为空双端队列
    void insertFront(E elem); // 添加元素到队首
    void insertBack(E elem); // 添加元素到队尾
    void endeque(E elem) { insertBack(std::move(elem)); } // 入队函数
    E removeFront(); // 队首元素出队
    E removeBack(); // 队尾元素出队
    E dequeue() { return removeFront(); } // 出队函数
    E& front(); // 返回队首引用
    E& back(); // 返回队尾引用
    void swap(ArrayDeque& that); // 内容与另一个ArrayDeque对象交换
    void clear(); // 清空双端队列，不释放空间，双端队列容量不变
    
    ArrayDeque& operator=(ArrayDeque that);
    template <typename T>
    friend bool operator==(const ArrayDeque<T>& lhs, const ArrayDeque<T>& rhs);
    template <typename T>
    friend bool operator!=(const ArrayDeque<T>& lhs, const ArrayDeque<T>& rhs);
    template <typename T>
    friend std::ostream& operator<<(std::ostream& os, const ArrayDeque<T>& deque);

    class iterator : public std::iterator<std::bidirectional_iterator_tag, E>
    {
    private:
        const ArrayDeque* deque;
        int i;
    public:
        iterator() : deque(nullptr), i(0) {}
        iterator(const ArrayDeque* deque, int i) : deque(deque), i(i) {}
        iterator(const iterator& that) : deque(that.deque), i(that.i) {}
        ~iterator() {}

        E& operator*() const
        { return deque->pd[(deque->head + i) % deque->N]; }
        E* operator->() const
        { return &deque->pd[(deque->head + i) % deque->N]; }
        iterator& operator++()
        {
            i++;
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
            i--;
            return *this;
        }
        iterator operator--(int)
        {
            iterator tmp(*this);
            --*this;
            return tmp;
        }
        bool operator==(const iterator& that) const
        { return deque == that.deque && i == that.i; }
        bool operator!=(const iterator& that) const
        { return deque != that.deque || i != that.i; }
    };
    iterator begin() const { return iterator(this, 0); }
    iterator end() const { return iterator(this, n); }
};

/**
 * 数组双端队列构造函数，初始化双端队列.
 * 双端队列初始容量为10.
 *
 * @param cap: 指定双端队列容量
 */
template<typename E>
ArrayDeque<E>::ArrayDeque(int cap)
{
    n = 0;
    N = cap; // 默认容量为10
    head = 0;
    tail = 0;
    pd = new E[N];
}

/**
 * 数组双端队列复制构造函数.
 * 复制另一个双端队列作为初始化的值.
 *
 * @param that: 被复制的双端队列
 */
template<typename E>
ArrayDeque<E>::ArrayDeque(const ArrayDeque& that)
{
    n = that.n;
    N = that.N;
    head = that.head;
    tail = that.tail;
    pd = new E[N];
    std::copy(that.begin(), that.end(), begin());
}

/**
 * 数组双端队列移动构造函数.
 * 移动另一个双端队列，其资源所有权转移到新创建的对象.
 *
 * @param that: 被移动的双端队列
 */
template<typename E>
ArrayDeque<E>::ArrayDeque(ArrayDeque&& that) noexcept
{
    n = that.n;
    N = that.N;
    head = that.head;
    tail = that.tail;
    pd = that.pd;
    that.pd = nullptr; // 指向空指针，退出被析构
}

/**
 * 创建指定容量的新双端队列，并移动所有元素到新双端队列当中.
 *
 * @param size: 新双端队列容量
 */
template<typename E>
void ArrayDeque<E>::resize(int size)
{
    // 保证新的容量不小于双端队列当前大小
    assert(size >= n); 
    
    ArrayDeque tmp(size);
    // 将所有元素移动到临时双端队列
    std::move(begin(), end(), tmp.begin());
    tmp.n = n; 
    tmp.head = 0; 
    tmp.tail = n; // 设置临时双端队列的大小和首尾索引
    swap(tmp); // *this与tmp互相交换，退出时tmp被析构
}

/**
 * 添加元素到队首.
 * 当双端队列达到最大容量，扩容双端队列到两倍容量后，再将元素入队.
 *
 * @param elem: 要添加到队首的元素
 */
template<typename E>
void ArrayDeque<E>::insertFront(E elem)
{
    if (n == N)
        resize(N * 2);
    if (--head < 0) head = N - 1;
    // 移动元素入队
    pd[head] = std::move(elem);
    n++;
}

/**
 * 添加元素到队尾.
 * 当双端队列达到最大容量，扩容双端队列到两倍容量后，再将元素入队.
 *
 * @param elem: 要添加到队尾的元素
 */
template<typename E>
void ArrayDeque<E>::insertBack(E elem)
{
    if (n == N)
        resize(N * 2);
    pd[tail++] = std::move(elem);
    if (tail == N) tail = 0;
    n++;
}

/**
 * 让队首元素出队.
 * 当双端队列达到1/4容量，缩小双端队列容量.
 *
 * @return 出队队首
 * @throws std::out_of_range: 队空
 */
template<typename E>
E ArrayDeque<E>::removeFront()
{
    if (empty()) 
        throw std::out_of_range("ArrayDeque::removeFront() underflow.");

    E tmp = pd[head++];

    if (head == N) head = 0;
    n--;
    // 保证双端队列始终约为半满状态，保证n>0
    if (n > 0 && n == N / 4) 
        resize(N / 2);
    return tmp; // 发生NRVO
}

/**
 * 让队尾元素出队.
 * 当双端队列达到1/4容量，缩小双端队列容量.
 *
 * @return 出队队尾
 * @throws std::out_of_range: 队空
 */
template<typename E>
E ArrayDeque<E>::removeBack()
{
    if (empty()) 
        throw std::out_of_range("ArrayDeque::removeBack() underflow.");
    if (--tail < 0) 
        tail = N - 1;

    E tmp = pd[tail];

    n--;
    // 保证双端队列始终约为半满状态，保证n>0
    if (n > 0 && n == N / 4)
        resize(N / 2);
    return tmp; // 发生NRVO
}

/**
 * 返回队首引用.
 *
 * @return 队首引用
 * @throws std::out_of_range: 队空
 */
template<typename E>
E& ArrayDeque<E>::front()
{
    if (empty()) 
        throw std::out_of_range("ArrayDeque::front() underflow.");
    return *begin();
}

/**
 * 返回队尾引用.
 *
 * @return 队尾引用
 * @throws std::out_of_range: 队空
 */
template<typename E>
E& ArrayDeque<E>::back()
{
    if (empty()) 
        throw std::out_of_range("ArrayDeque::back() underflow.");
    return *std::prev(end());
}

/**
 * 交换当前ArrayDeque对象和另一个ArrayDeque对象.
 *
 * @param that: ArrayDeque对象that
 */
template<typename E>
void ArrayDeque<E>::swap(ArrayDeque<E>& that)
{
    // 如果没有针对类型的特化swap，则使用std::swap
    using std::swap; 
    swap(n, that.n);
    swap(head, that.head);
    swap(tail, that.tail);
    swap(N, that.N);
    swap(pd, that.pd);
}

/**
 * 清空该双端队列元素.
 * 不释放空间，双端队列容量保持不变.
 */
template<typename E>
void ArrayDeque<E>::clear()
{    
    n = 0;
    head = 0;
    tail = 0;
}

/**
 * =操作符重载.
 * 让当前ArrayDeque对象等于给定ArrayDeque对象that.
 *
 * @param that: ArrayDeque对象that
 * @return 当前ArrayDeque对象
 */
template<typename E>
ArrayDeque<E>& ArrayDeque<E>::operator=(ArrayDeque<E> that)
{
    // *this与that互相交换，退出时that被析构
    swap(that);
    return *this;
}

/**
 * ==操作符重载函数，比较两个ArrayDeque对象是否相等.
 *
 * @param lhs: ArrayDeque对象lhs
 *        rhs: ArrayDeque对象rhs
 * @return true: 相等
 *         false: 不等
 */
template<typename E>
bool operator==(const ArrayDeque<E>& lhs, const ArrayDeque<E>& rhs)
{
    if (&lhs == &rhs)             return true;
    if (lhs.size() != rhs.size()) return false;
    return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

/**
 * !=操作符重载函数，比较两个ArrayDeque对象是否不等.
 *
 * @param lhs: ArrayDeque对象lhs
 *        rhs: ArrayDeque对象rhs
 * @return true: 不等
 *         false: 相等
 */
template<typename E>
bool operator!=(const ArrayDeque<E>& lhs, const ArrayDeque<E>& rhs)
{
    return !(lhs == rhs);
}

/**
 * <<操作符重载函数，打印所有双端队列元素.
 *
 * @param os: 输出流对象
 *        deque: 要输出的双端队列
 * @return 输出流对象
 */
template<typename E>
std::ostream& operator<<(std::ostream& os, const ArrayDeque<E>& deque)
{
    for (auto i : deque)
        os << i << " ";
    return os;
}

/**
 * 交换两个ArrayDeque对象.
 *
 * @param lhs: ArrayDeque对象lhs
 *        rhs: ArrayDeque对象rhs
 */
template<typename E>
void swap(ArrayDeque<E>& lhs, ArrayDeque<E>& rhs)
{
    lhs.swap(rhs);
}

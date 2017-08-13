/*******************************************************************************
 * Deque.h
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
 * 实现了双端队列的随机访问迭代器.
 */
template<typename E>
class Deque
{
    static const int DEFAULT_CAPACITY = 10; // 默认双端队列容量
private:
    int n;    // 队列大小
    int N;    // 队列容量
    int head; // 队首索引
    int tail; // 队尾索引
    E* pd;    // 双端队列指针
    
    // 调整双端队列容量
    void resize(int size); 
public:
    explicit Deque(int cap = DEFAULT_CAPACITY); 
    Deque(const Deque& that);
    Deque(Deque&& that) noexcept;
    ~Deque() { delete[] pd; }

    // 返回双端队列当前大小
    int size() const { return n; } 
    // 返回双端队列容量
    int capacity() const { return N; } 
    // 判断是否为空双端队列
    bool empty() const { return n == 0; } 
    // 添加元素到队首
    void insert_front(E elem); 
    // 添加元素到队尾
    void insert_back(E elem); 
    // 队首元素出队
    E remove_front(); 
    // 队尾元素出队
    E remove_Back(); 
    // 返回队首引用
    E& front() { return const_cast<E&>(static_cast<const Deque&>(*this).front()); } 
    // 返回const队首引用
    const E& front() const; 
    // 返回队尾引用
    E& back() { return const_cast<E&>(static_cast<const Deque&>(*this).back()); } 
    // 返回const队尾引用
    const E& back() const; 
    // 内容与另一个Deque对象交换
    void swap(Deque& that); 
    // 清空双端队列，不释放空间，双端队列容量不变
    void clear(); 
    
    Deque& operator=(Deque that);
    template <typename T>
    friend bool operator==(const Deque<T>& lhs, const Deque<T>& rhs);
    template <typename T>
    friend bool operator!=(const Deque<T>& lhs, const Deque<T>& rhs);
    template <typename T>
    friend std::ostream& operator<<(std::ostream& os, const Deque<T>& deque);

    class iterator : public std::iterator<std::random_access_iterator_tag, E>
    {
    private:
        const Deque* deque;
        int i;
    public:
        iterator() : deque(nullptr), i(0) {}
        iterator(const Deque* deque, int i) : deque(deque), i(i) {}
        iterator(const iterator& that) : deque(that.deque), i(that.i) {}
        ~iterator() {}

        E& operator*() const
        { return deque->pd[i]; }
        E* operator->() const
        { return &deque->pd[i]; }
        E& operator[](int pos) const
        { return deque->pd[i + pos]; }
        iterator& operator++()
        {
            if (i == deque->N - 1) i = 0;
            else                  i++;
            return *this;
        }
        iterator operator++(int)
        {
            iterator tmp(*this);
            ++*this;
            return tmp;
        }        
        iterator operator+(int pos)
        { return iterator(deque, i + pos); }
        iterator operator+=(int pos)
        {
            i += pos;
            if (i > deque->N - 1) i -= deque->N;
            return *this;
        }
        iterator& operator--()
        {
            if (i == 0) i = deque->N - 1;
            else        i--;
            return *this;
        }
        iterator operator--(int)
        {
            iterator tmp(*this);
            --*this;
            return tmp;
        }
        iterator operator-(int pos) 
        { return iterator(deque, i - pos); }
        iterator operator-=(int pos) 
        {
            i -= pos;
            if (i < 0) i += deque->N;
            return *this;
        }        
        int operator-(const iterator& that) 
        {
            if (deque != that.deque)
                throw std::invalid_argument("Deque::iterator::operator-() invalid iterator.");
            return i - that.i;
        }
        bool operator==(const iterator& that) const
        { return deque == that.deque && i == that.i; }
        bool operator!=(const iterator& that) const
        { return deque != that.deque || i != that.i; }
        bool operator<(const iterator& that) const
        {
            if (deque != that.deque)
                throw std::invalid_argument("Deque::iterator::operator<() invalid iterator.");
            return i < that.i;
        }
        bool operator<=(const iterator& that) const
        {
            if (deque != that.deque)
                throw std::invalid_argument("Deque::iterator::operator<=() invalid iterator.");
            return i <= that.i;
        }
        bool operator>(const iterator& that) const
        {
            if (deque != that.deque)
                throw std::invalid_argument("Deque::iterator::operator>() invalid iterator.");
            return i > that.i;
        }
        bool operator>=(const iterator& that) const
        {
            if (deque != that.deque)
                throw std::invalid_argument("Deque::iterator::operator>=() invalid iterator.");
            return i >= that.i;
        }
    };
    iterator begin() const { return iterator(this, head); }
    iterator end() const { return iterator(this, tail); }
};

/**
 * 双端队列构造函数，初始化双端队列.
 * 双端队列初始容量为10.
 *
 * @param cap: 指定双端队列容量
 */
template<typename E>
Deque<E>::Deque(int cap)
{
    n = 0;
    N = cap; // 默认容量为10
    head = 0;
    tail = 0;
    pd = new E[N];
}

/**
 * 双端队列复制构造函数.
 * 复制另一个双端队列作为初始化的值.
 *
 * @param that: 被复制的双端队列
 */
template<typename E>
Deque<E>::Deque(const Deque& that)
{
    n = that.n;
    N = that.N;
    head = that.head;
    tail = that.tail;
    pd = new E[N];
    std::copy(that.begin(), that.end(), begin());
}

/**
 * 双端队列移动构造函数.
 * 移动另一个双端队列，其资源所有权转移到新创建的对象.
 *
 * @param that: 被移动的双端队列
 */
template<typename E>
Deque<E>::Deque(Deque&& that) noexcept
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
void Deque<E>::resize(int size)
{
    // 保证新的容量不小于双端队列当前大小
    assert(size >= n); 
    
    Deque tmp(size);
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
 * @param elem: 要添加的元素
 */
template<typename E>
void Deque<E>::insert_front(E elem)
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
 * @param elem: 要添加的元素
 */
template<typename E>
void Deque<E>::insert_back(E elem)
{
    if (n == N)
        resize(N * 2);
    pd[tail++] = std::move(elem);
    if (tail == N) tail = 0;
    n++;
}

/**
 * 移除队首元素.
 * 当双端队列达到1/4容量，缩小双端队列容量.
 *
 * @return 移除的元素
 * @throws std::out_of_range: 队空
 */
template<typename E>
E Deque<E>::remove_front()
{
    if (empty()) 
        throw std::out_of_range("Deque::remove_front() underflow.");

    E tmp = pd[head++];

    if (head == N) head = 0;
    n--;
    // 保证双端队列始终约为半满状态，保证n>0
    if (n > 0 && n == N / 4) 
        resize(N / 2);
    return tmp; // 发生NRVO
}

/**
 * 移除队尾元素.
 * 当双端队列达到1/4容量，缩小双端队列容量.
 *
 * @return 移除的元素
 * @throws std::out_of_range: 队空
 */
template<typename E>
E Deque<E>::remove_Back()
{
    if (empty()) 
        throw std::out_of_range("Deque::remove_Back() underflow.");
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
 * 返回const队首引用.
 *
 * @return const队首引用
 * @throws std::out_of_range: 队空
 */
template<typename E>
const E& Deque<E>::front() const
{
    if (empty()) 
        throw std::out_of_range("Deque::front() underflow.");
    return *begin();
}

/**
 * 返回const队尾引用.
 *
 * @return const队尾引用
 * @throws std::out_of_range: 队空
 */
template<typename E>
const E& Deque<E>::back() const
{
    if (empty()) 
        throw std::out_of_range("Deque::back() underflow.");
    return *std::prev(end());
}

/**
 * 交换当前Deque对象和另一个Deque对象.
 *
 * @param that: Deque对象that
 */
template<typename E>
void Deque<E>::swap(Deque<E>& that)
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
void Deque<E>::clear()
{    
    n = 0;
    head = 0;
    tail = 0;
}

/**
 * =操作符重载.
 * 让当前Deque对象等于给定Deque对象that.
 *
 * @param that: Deque对象that
 * @return 当前Deque对象
 */
template<typename E>
Deque<E>& Deque<E>::operator=(Deque<E> that)
{
    // *this与that互相交换，退出时that被析构
    swap(that);
    return *this;
}

/**
 * ==操作符重载函数，比较两个Deque对象是否相等.
 *
 * @param lhs: Deque对象lhs
 *        rhs: Deque对象rhs
 * @return true: 相等
 *         false: 不等
 */
template<typename E>
bool operator==(const Deque<E>& lhs, const Deque<E>& rhs)
{
    if (&lhs == &rhs)             return true;
    if (lhs.size() != rhs.size()) return false;
    return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

/**
 * !=操作符重载函数，比较两个Deque对象是否不等.
 *
 * @param lhs: Deque对象lhs
 *        rhs: Deque对象rhs
 * @return true: 不等
 *         false: 相等
 */
template<typename E>
bool operator!=(const Deque<E>& lhs, const Deque<E>& rhs)
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
std::ostream& operator<<(std::ostream& os, const Deque<E>& deque)
{
    for (auto i : deque)
        os << i << " ";
    return os;
}

/**
 * 交换两个Deque对象.
 *
 * @param lhs: Deque对象lhs
 *        rhs: Deque对象rhs
 */
template<typename E>
void swap(Deque<E>& lhs, Deque<E>& rhs)
{
    lhs.swap(rhs);
}

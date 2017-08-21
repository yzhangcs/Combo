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

template <class E>
struct Block
{
    int N;
    E* pb;

    Block() : N(0), pb(nullptr) {}
    Block(int count) : N(count), pb(new E[N]) {}
    Block(const Block& that) : N(that.N), pb(new E[N])
    { std::copy(that.pb, that.pb + that.N, pb); }
    Block(Block&& that) noexcept : N(that.N), pb(that.pb)
    { that.pb = nullptr; }
    ~Block() { delete[] pb; }
    E& operator[](int i) { return pb[i]; }
    Block& operator=(Block that)
    {
        swap(N, that.N);
        swap(pb, that.pb);
        return *this;
    }
};

template <class E>
class DequeIterator : public std::iterator<std::random_access_iterator_tag, E>
{
private:
    Block* block;
    int current;
public:
    DequeIterator() : block(nullptr), current(0) {}
    DequeIterator(Block* block, int current) : block(block), current(current) {}
    DequeIterator(const DequeIterator& that) : block(that.block), current(that.current) {}
    ~DequeIterator() {}

    E& operator*() const
    { return block[current]; }
    E* operator->() const
    { return &block[current]; }
    E& operator[](int n) const
    { return *(*this + n); }
    DequeIterator& operator++()
    {
        ++current;
        if (current == block->N)
        {
            block++;
            current = 0;
        }
        return *this;
    }
    DequeIterator operator++(int)
    {
        DequeIterator tmp(*this);
        ++*this;
        return tmp;
    }
    DequeIterator operator+(int n)
    {
        DequeIterator tmp(*this);
        return tmp += n;
    }
    DequeIterator& operator+=(int n)
    {
        // 相对于当前区块头部的偏移
        int offset = n + current;

        if (offset >= 0 && offset < block->N)
            current += n;
        else
        {
            int block_offset = offset < 0 ? -((-offset - 1) / block->N) - 1
                                          : offset / block->N;
            block += block_offset;
            current = offset - block_offset * block->N;
        }
        return *this;
    }
    DequeIterator& operator--()
    {
        if (current == 0)
        {
            --block;
            current = block->N;
        }
        --current;
        return *this;
    }
    DequeIterator operator--(int)
    {
        DequeIterator tmp(*this);
        --*this;
        return tmp;
    }
    DequeIterator operator-(int n)
    {
        DequeIterator tmp(*this);
        return tmp += -n;
    }
    DequeIterator& operator-=(int n)
    { return *this += -n; }
    int operator-(const DequeIterator& that)
    { return that.block->N - that.current + block->N * (block - that.block - 1) + current; }
    bool operator==(const DequeIterator& that) const
    { return block == that.block && current == that.current; }
    bool operator!=(const DequeIterator& that) const
    { return !(*this == that); }
    bool operator<(const DequeIterator& that) const
    { return block == that.block ? current < that.current : block < that.block; }
    bool operator>(const DequeIterator& that) const
    { return that < *this; }
    bool operator<=(const DequeIterator& that) const
    { return !(that < *this); }
    bool operator>=(const DequeIterator& that) const
    { return !(*this < that); }
};

/**
 * 使用模板实现的双端队列.
 * 由动态连续数组存储双端队列.
 * 实现了双端队列的随机访问迭代器.
 */
template<typename E>
class Deque
{
    static const int BLOCK_SIZE = 10; // 区块大小
    static const int DEFAULT_MAP_SIZE = 10; // 默认映射大小
    static const int DEFAULT_CAPACITY = 10; // 默认双端队列容量

    using iterator = DequeIterator<E>;
private:
    int M;         // 区块个数
    Block<E>* map; // 区块映射
    iterator head; // 队首迭代器
    iterator tail; // 队尾迭代器

    // 调整映射大小
    void reserve_map();
    // 调整映射大小
    void reserve_block(int count);
public:
    explicit Deque(int count = DEFAULT_CAPACITY);
    Deque(const Deque& that);
    Deque(Deque&& that) noexcept;
    ~Deque() { delete[] map; }

    // 返回双端队列当前大小
    int size() const { return tail - head; }
    // 判断是否为空双端队列
    bool empty() const { return head == tail; }
    // 添加元素到队首
    void insert_front(E elem);
    // 添加元素到队尾
    void insert_back(E elem);
    // 队首元素出队
    E remove_front();
    // 队尾元素出队
    E remove_back();
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

    iterator begin() const { return head; }
    iterator end() const { return tail; }
};

/**
 * 双端队列构造函数，初始化双端队列.
 * 双端队列初始容量为10.
 *
 * @param count: 指定双端队列容量
 */
template<typename E>
Deque<E>::Deque(int count)
{
    // 在满足指定容量所需的最少区块数和默认映射大小中取较大值
    M = std::max(count / BLOCK_SIZE + 1, DEFAULT_MAP_SIZE);
    map = new Block<E>[M];
    // 分配中央区块
    map[M / 2] = Block(BLOCK_SIZE);
    // 头尾迭代器指向中央区块的中间，便于向两端扩展
    head = iterator(map + M / 2, BLOCK_SIZE / 2);
    tail = iterator(map + M / 2, BLOCK_SIZE / 2);
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
    M = that.M;
    map = new Block[M];
    // 迭代器的位置偏移相同
    head = iterator(map + that.head.block - that.map, that.head.current);
    tail = iterator(map + that.tail.block - that.map, that.tail.current);
    std::copy(that.head, that.tail, head);
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
    M = that.M
    map = that.map;
    head = that.head;
    tail = that.tail;
    that.map = nullptr;
    that.head = nullptr;
    that.tail = nullptr; // 指向空指针，退出被析构
}

/**
 * 创建指定容量的新双端队列，并移动所有元素到新双端队列当中.
 *
 * @param size: 新双端队列容量
 */
template<typename E>
void Deque<E>::reserve_map(int count)
{
    // 保证新的容量不小于双端队列当前大小
    assert(count >= size());

    M = count / BLOCK_SIZE + 1;
    map = new Block[M];
    // 头尾迭代器指向中央区域，便于向两端扩展
    head = iterator(map + M / 2, BLOCK_SIZE / 2);
    tail = iterator(map + M / 2, BLOCK_SIZE / 2);

    Deque tmp(count);
    // 将所有元素移动到临时双端队列
    std::move(begin, end, tmp.begin);
    tmp.head = 0;
    tmp.tail = n; // 设置头尾迭代器
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
    // 头迭代器指向的区块满，则分配并指向新区块
    if (head.current == 0)
    {
        // 没有多余映射区块，则扩容映射到两倍
        if (head.block == map)
            reserve_map(2 * M);
        --head.block;
        head.block = Block(BLOCK_SIZE);
        head.current = BLOCK_SIZE;
    }
    --head.current;
    head.block[head.current] = std::move(elem);
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
    // 尾迭代器指向的区块满，则分配并指向新区块
    if (tail.current == BLOCK_SIZE)
    {
        // 映射满，则扩容映射到两倍
        if (tail.block == map + M - 1)
            reserve_map(2 * M);
        ++tail.block;
        tail.block = Block(BLOCK_SIZE);
        tail.current = 0;
    }
    tail.block[tail.current] = std::move(elem);
    ++tail.current;
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
        throw std::out_of_range("Deque::remove_front");

    E tmp = *head;

    head.current++;
    if (head.current == BLOCK_SIZE)
    {
        // 释放空区块
        head.block = Block();
        ++head.block;
        head.current = 0;
        // // 保证映射始终约为半满状态
        // if (n > 0 && n == N / 4)
        //     reserve_map(N / 2);
    }
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
E Deque<E>::remove_back()
{
    if (empty())
        throw std::out_of_range("Deque::remove_back");

    if (tail.current == 0)
    {
        // 释放空区块
        tail.block = Block();
        --tail.block;
        tail.current = BLOCK_SIZE;
        // // 保证映射始终约为半满状态
        // if (n > 0 && n == N / 4)
        //     reserve_map(N / 2);
    }
    --tail.current;
    return *tail;
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
        throw std::out_of_range("Deque::front");
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
        throw std::out_of_range("Deque::back");
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

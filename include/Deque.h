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

// 根据元素类型大小计算一个区块所能容纳的元素个数
static constexpr int block_size(int size) { return size < 256 ? 4096 / size : 16;}

template <class E>
class DequeIterator : public std::iterator<std::random_access_iterator_tag, E>
{
    static const int BLOCK_SIZE = block_size(sizeof(E));
private:
    E** block; // 区块映射指针
    E* head; // 区块头指针
    E* tail; // 区块尾指针
    E* current; // 区块当前位置指针

    void set_block(E** new_block)
    {
        block = new_block;
        head = *block;
        tail = head + BLOCK_SIZE;
    }
public:
    DequeIterator() : block(nullptr), head(nullptr), tail(nullptr), current(nullptr) {}
    DequeIterator(E** block, E* current) :
        block(block), head(*block), tail(head + BLOCK_SIZE), current(current) {}
    DequeIterator(const DequeIterator& that) :
        block(that.block), head(that.head), tail(that.tail), current(that.current) {}
    ~DequeIterator() {}

    E& operator*() const
    { return *current; }
    E* operator->() const
    { return current; }
    E& operator[](int n) const
    { return *(*this + n); }
    DequeIterator& operator++()
    {
        ++current;
        // 当前区块满，则跳到下一个区块
        if (current == tail)
        {
            set_block(block + 1);
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
        int offset = n + current - head;

        if (offset >= 0 && offset < BLOCK_SIZE)
            current += n;
        else
        {
            int block_offset = offset < 0 ? (offset + 1) / BLOCK_SIZE - 1
                                        : offset / BLOCK_SIZE;
            set_block(block + block_offset);
            current = head + offset - block_offset * BLOCK_SIZE)；
        }
        return *this;
    }
    DequeIterator& operator--()
    {
        if (current == head)
            set_block(block - 1)
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
    { return BLOCK_SIZE * (block - that.block) - that.current - that.head + current - head; }
    bool operator==(const DequeIterator& that) const
    { return current == that.current; }
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
private:
    static const int BLOCK_SIZE = block_size(sizeof(E)); // 区块大小
    static const int DEFAULT_MAP_SIZE = 10; // 默认映射大小
    static const int DEFAULT_CAPACITY = 10; // 默认双端队列容量

    using data_allocator = std::allocator<T>;
    using map_allocator = std::allocator<T*>;
public:
    using iterator = DequeIterator<E>;
private:
    int M;   // 区块个数
    E** map; // 区块映射
    iterator it_bg; // 队首迭代器
    iterator it_ed; // 队尾迭代器

    // 初始化映射
    void initialize_map(int count);
    // 创建指定映射范围的区块
    void create_blocks(E** block_head, E** block_tail);
    // 销毁指定映射范围的区块
    void destroy_blocks(E** block_head, E** block_tail);
    // 调整映射大小
    void reserve_map();
public:
    Deque() { initialize_map(0); }
    explicit Deque(int count, const E& value = E());
    Deque(const Deque& that);
    Deque(Deque&& that) noexcept;
    ~Deque() { delete[] map; }

    // 返回双端队列当前大小
    int size() const { return it_ed - it_bg; }
    // 判断是否为空双端队列
    bool empty() const { return it_bg == it_ed; }
    // 添加元素到指定位置
    void insert(E elem);
    // 添加元素到队首
    void insert_front(E elem);
    // 添加元素到队尾
    void insert_back(E elem);
    // 移除指定位置的元素
    void remove(int i);
    // 队首元素出队
    void remove_front();
    // 队尾元素出队
    void remove_back();
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

    iterator begin() const { return it_bg; }
    iterator end() const { return it_ed; }
};

/**
 * 双端队列构造函数，初始化双端队列.
 *
 * @param count: 指定双端队列容量
 */
template<typename E>
Deque<E>::Deque(int count, const E& value)
{
    // 初始化满足指定容量的映射
    initialize_map(count);
    uninitialized_fill(it_bg, it_ed, value);
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
    // 初始化满足that大小的映射
    initialize_map(that.size());
    uninitialized_copy(that.it_bg, that.it_ed, it_bg);
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
    it_bg = that.it_bg;
    it_ed = that.it_ed;
    that.map = nullptr; // 指向空指针，退出被析构
}

/**
 * 初始化映射.
 * 根据指定容量创建相应范围的区块并初始化迭代器.
 *
 * @param count: 元素容量
 */
void initialize_map(int count)
{
    // 满足指定容量所需的最少区块数
    int num_blocks = count / BLOCK_SIZE + 1;
    // 映射容量为num_blocks + 2和DEFAULT_MAP_SIZE中的较大值
    M = std::max(num_blocks + 2, DEFAULT_MAP_SIZE);
    map = map_allocator::allocate(M);
    // 映射两端剩余容量相同
    E** block_head = map + (M - num_blocks) / 2;
    E** block_tail = block_head + num_blocks;
    // 创建区块，区块映射位于中央位置，便于向两端扩展
    create_blocks(block_head, block_tail);
    it_bg = iterator(block_head, *block_head);
    it_ed = iterator(block_tail - 1, *(block_tail - 1) + count % BLOCK_SIZE);
}

/**
 * 创建[block_head, block_tail)映射范围指向的区块.
 *
 * @param block_head: 起始映射指针
 * @param block_tail: 结束映射指针
 */
template<typename E>
Deque<E>::void create_blocks(E** block_head, E** block_tail)
{
    E** i;
    // Note: commit or rollback
    try
    {
        for (i = block_head; i < block_tail; ++i)
            *i = data_allocator::allocate(BLOCK_SIZE);
    }
    catch(...)
    {
        destroy_blocks(block_head, i)
    }
}

/**
 * 销毁[block_head, block_tail)映射范围指向的区块.
 *
 * @param block_head: 起始映射指针
 * @param block_tail: 结束映射指针
 */
template<typename E>
Deque<E>::void destroy_blocks(E** block_head, E** block_tail)
{
    for (i = block_head; i < block_tail; ++i)
        data_allocator::deallocate(*i, BLOCK_SIZE);
}

/**
 * @param size: 新双端队列容量
 */
template<typename E>
void Deque<E>::reserve_map(int map_size)
{
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
    if (it_bg.current == it_bg.head)
    {
        // 映射满，则扩容映射到两倍
        if (it_bg.block == map)
            reserve_map(2 * M);
        it_bg.block - 1 = data_allocator::allocate(BLOCK_SIZE);
        it_bg.set_block(it_bg.block - 1);
        it_bg.current = it_bg.tail;
    }
    --it_bg.current;
    data_allocator::construct(it_bg.current, std::move(elem));
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
    if (it_ed.current == BLOCK_SIZE)
    {
        // 映射满，则扩容映射到两倍
        if (it_ed.block == map + M - 1)
            reserve_map(2 * M);
        it_ed.block + 1 = data_allocator::allocate(BLOCK_SIZE);
        it_ed.set_block(it_ed.block + 1);
        it_ed.current = it_ed.head;
    }
    data_allocator::construct(it_ed.current, std::move(elem));
    ++it_ed.current;
}

/**
 * 移除队首元素.
 * 当双端队列达到1/4容量，缩小双端队列容量.
 *
 * @throws std::out_of_range: 队空
 */
template<typename E>
void Deque<E>::remove_front()
{
    if (empty())
        throw std::out_of_range("Deque::remove_front");
    data_allocator::destroy(it_bg.current);
    it_bg.current++;
    if (it_bg.current == it_bg.tail)
    {
        // 释放空区块
        data_allocator::deallocate(*it_bg.block);
        it_bg.set_block(it_bg.block + 1);
        it_bg.current = it_bg.head;
        // // 保证映射始终约为半满状态
        // if (n > 0 && n == N / 4)
        //     reserve_map(N / 2);
    }
}

/**
 * 移除队尾元素.
 * 当双端队列达到1/4容量，缩小双端队列容量.
 *
 * @throws std::out_of_range: 队空
 */
template<typename E>
void Deque<E>::remove_back()
{
    if (empty())
        throw std::out_of_range("Deque::remove_back");
    if (it_ed.current == it_ed.head)
    {
        // 释放空区块
        data_allocator::deallocate(*it_ed.block);
        it_ed.set_block(it_ed.block - 1);
        it_ed.current = it_ed.tail;
        // // 保证映射始终约为半满状态
        // if (n > 0 && n == N / 4)
        //     reserve_map(N / 2);
    }
    --it_ed.current;
    data_allocator::destroy(it_end.current);
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
    swap(it_bg, that.head);
    swap(it_ed, that.tail);
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
    it_bg = 0;
    it_ed = 0;
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

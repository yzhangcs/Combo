/*******************************************************************************
 * Deque.h
 *
 * Author: zhangyu
 * Date: 2017.4.23
 ******************************************************************************/

#pragma once
#include <iostream>
#include <iterator>

// 根据元素类型大小计算一个区块所能容纳的元素个数
static constexpr int block_size(int size) { return size < 256 ? 4096 / size : 16;}

template <class E>
class Deque;

template <class E>
class DequeIterator : public std::iterator<std::random_access_iterator_tag, E>
{
    friend class Deque<E>;

    static const int BLOCK_SIZE = block_size(sizeof(E));
private:
    E** block;  // 区块映射指针
    E* current; // 区块当前位置指针
    E* head; // 区块头指针
    E* tail; // 区块尾指针

    // 跳转到指定区块，current的修改交给调用者
    void set_block(E** new_block)
    {
        block = new_block;
        head = *block;
        tail = head + BLOCK_SIZE;
    }
public:
    DequeIterator() : block(nullptr), current(nullptr), head(nullptr), tail(nullptr) {}
    DequeIterator(E** block, E* current) :
        block(block), current(current), head(*block), tail(head + BLOCK_SIZE) {}
    DequeIterator(const DequeIterator& that) :
        block(that.block), current(that.current), head(that.head), tail(that.tail) {}
    ~DequeIterator() {}

    E& operator*() const
    { return *current; }
    E* operator->() const
    { return current; }
    E& operator[](int i) const
    { return *(*this + i); }
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
            current = head + offset - block_offset * BLOCK_SIZE;
        }
        return *this;
    }
    DequeIterator& operator--()
    {
        if (current == head)
            set_block(block - 1);
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

    using data_allocator        = std::allocator<E>;
    using map_allocator         = std::allocator<E*>;
    // std::allocator::construct & destroy was deprecated in C++17
    // using data_allocator_traits = std::allocator_traits<data_allocator>;
    // using map_allocator_traits  = std::allocator_traits<map_allocator>;
public:
    using iterator               = DequeIterator<E>;
    using const_iterator         = DequeIterator<const E>;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
private:
    int M;   // 区块个数
    E** map; // 区块映射
    iterator it_bg; // 队首迭代器
    iterator it_ed; // 队尾迭代器

    // Note: 将map视为「Vector of blocks」，map的操作类似于Vector
    // 初始化映射
    void initialize_map(int count);
    // 重新安排映射的容量
    void reserve_map(int new_count, bool at_front);
    // 添加区块到指定区块映射范围
    void insert_block(E** block_begin, E** block_end);
    // 添加区块到区块映射头部
    void insert_block_at_front();
    // 添加区块到区块映射尾部
    void insert_block_at_back();
    // 移除指定区块映射范围的区块
    void remove_block(E** block_begin, E** block_end);
    // 移除区块映射头部的区块
    void remove_block_at_front();
    // 移除区块映射尾部的区块
    void remove_block_at_back();
    // 检查迭代器是否合法
    bool valid(int i) const { return i >= 0 && i < size(); }
public:
    Deque() { initialize_map(0); }
    explicit Deque(int count, const E& value = E());
    Deque(const Deque& that);
    Deque(Deque&& that) noexcept;
    ~Deque();
    Deque& operator=(Deque that);

    // 判断是否为空双端队列
    bool empty() const { return it_bg == it_ed; }
    // 返回双端队列当前大小
    int size() const { return it_ed - it_bg; }
    // 收缩双端队列，移除过剩容量
    void shrink_to_fit();

    // 返回const队首引用
    const E& front() const;
    // 返回const队尾引用
    const E& back() const;
    // 返回指定位置元素的const引用，带边界检查
    const E& at(int i) const;
    // 返回指定位置元素的const引用，无边界检查
    const E& operator[](int i) const { return it_bg[i]; }
    // 返回队首引用
    E& front() { return const_cast<E&>(static_cast<const Deque&>(*this).front()); }
    // 返回队尾引用
    E& back() { return const_cast<E&>(static_cast<const Deque&>(*this).back()); }
    // 返回指定位置元素的引用，带边界检查
    E& at(int i) { return const_cast<E&>(static_cast<const Deque&>(*this).at(i)); }
    // 返回指定位置元素的引用，无边界检查
    E& operator[](int i) { return const_cast<E&>(static_cast<const Deque&>(*this)[i]); }

    // 添加元素到队首
    void insert_front(E elem);
    // 添加元素到队尾
    void insert_back(E elem);
    // 添加元素到迭代器指定的位置
    void insert(iterator pos, E elem);
    // 队首元素出队
    void remove_front();
    // 队尾元素出队
    void remove_back();
    // 移除迭代器指定位置的元素
    void remove(iterator pos);
    // 内容与另一个Deque对象交换
    void swap(Deque& that);
    // 清空双端队列
    void clear();

    iterator                 begin()       noexcept { return it_bg; }
    const_iterator           begin() const noexcept { return it_bg; }
    const_iterator          cbegin() const noexcept { return it_bg; }
    iterator                   end()       noexcept { return it_ed; }
    const_iterator             end() const noexcept { return it_ed; }
    const_iterator            cend() const noexcept { return it_ed; }
    reverse_iterator        rbegin()       noexcept { return       reverse_iterator(it_ed); }
    const_reverse_iterator  rbegin() const noexcept { return const_reverse_iterator(it_ed); }
    const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(it_ed); }
    reverse_iterator          rend()       noexcept { return       reverse_iterator(it_bg); }
    const_reverse_iterator    rend() const noexcept { return const_reverse_iterator(it_bg); }
    const_reverse_iterator   crend() const noexcept { return const_reverse_iterator(it_bg); }


    template <typename T>
    friend bool operator==(const Deque<T>& lhs, const Deque<T>& rhs);
    template <typename T>
    friend bool operator!=(const Deque<T>& lhs, const Deque<T>& rhs);
    template <typename T>
    friend std::ostream& operator<<(std::ostream& os, const Deque<T>& deque);
};

/**
 * 双端队列构造函数.
 * 创建并用指定值初始化指定容量的双端队列.
 *
 * @param count: 指定双端队列容量
 * @param value: 用于初始化双端队列的值，不指定时是默认构造的值
 */
template<typename E>
Deque<E>::Deque(int count, const E& value)
{
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
    M = that.M;
    map = that.map;
    it_bg = that.it_bg;
    it_ed = that.it_ed;
    that.map = nullptr; // 指向空指针，退出被析构
}

/**
 * 双端队列析构函数函数.
 */
template<typename E>
Deque<E>::~Deque()
{
    // 析构掉所有区块内的元素
    if (it_bg.block == it_ed.block)
        data_allocator::destroy(it_bg.current, it_ed.current);
    else
    {
        data_allocator::destroy(it_bg.current, it_ed.tail);
        for (E** i = it_bg.block + 1; i < it_ed.block; ++i)
            data_allocator::destroy(*i, *i + BLOCK_SIZE);
        data_allocator::destroy(it_ed.head, it_ed.current);
    }
    // 移除所有的区块
    remove_block(it_bg.block, it_ed.block + 1);
    // 释放映射空间
    map_allocator::deallocate(map, M);
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
 * 初始化映射.
 * 根据指定容量分配相应范围的区块，并初始化迭代器.
 * 初始化后的映射指向的区块是未构造的.
 *
 * @param count: 元素容量
 */
template<typename E>
void Deque<E>::initialize_map(int count)
{
    // 满足指定容量所需的最少区块数
    int num_blocks = count / BLOCK_SIZE + 1;
    // 映射容量为num_blocks + 2和DEFAULT_MAP_SIZE中的较大值
    M = std::max(num_blocks + 2, DEFAULT_MAP_SIZE);
    map = map_allocator::allocate(M);
    // 映射两端剩余容量相同
    E** block_begin = map + (M - num_blocks) / 2;
    E** block_end = block_begin + num_blocks;
    // 分配区块，区块映射位于中央位置，便于向两端扩展
    insert_block(block_begin, block_end);
    it_bg = iterator(block_begin, *block_begin);
    it_ed = iterator(block_end - 1, *(block_end - 1) + count % BLOCK_SIZE);
}

/**
 * 重新安排映射的容量.
 *
 * @param new_count: 新的映射容量
 * @param at_front: 标识是否将新增的容量安排在映射头部
 */
template<typename E>
void Deque<E>::reserve_map(int new_count, bool at_front)
{
    // 如果新的容量小于当前映射容量，则映射不改变
    if (new_count > M)
    {
        E** new_map = map_allocator::allocate(new_count);
        // 如果at_front，则将新增容量安排在映射头部，否则头部剩余容量不变
        E** new_block_begin = new_map + (it_bg.block - map)
                                     + (at_front ? new_count - M : 0);
        // 复制区块映射指针到新的映射，不改变区块
        std::copy(it_bg.block, it_ed.block + 1, new_block_begin);
        map_allocator::deallocate(map, M);
        map = new_map;
        M = new_count;
        it_bg.set_block(new_block_begin);
        it_ed.set_block(new_block_begin + size() - 1);
    }
}

/**
 * 添加区块到指定映射范围.
 * 添加的区块是未构造的.
 *
 * @param block_begin: 区块起始位置（包含）
 * @param block_end: 区块结束位置（不包含）
 */
template<typename E>
void Deque<E>::insert_block(E** block_begin, E** block_end)
{
    E** i;
    // Note: commit or rollback
    try
    {
        for (i = block_begin; i < block_end; ++i)
            *i = data_allocator::allocate(BLOCK_SIZE);
    }
    catch(...)
    {
        remove_block(block_begin, i);
    }
}

/**
 * 添加区块到区块映射头部.
 * 添加的区块是未构造的.
 */
template<typename E>
void Deque<E>::insert_block_at_front()
{
    // 头部映射满，则扩容映射到两倍，新增容量安排在头部
    if (it_bg.block == map)
        reserve_map(2 * M, true);
    it_bg.block - 1 = data_allocator::allocate(BLOCK_SIZE);
    // 重置头迭代器的指向
    it_bg.set_block(it_bg.block - 1);
    it_bg.current = it_bg.tail;
}

/**
 * 添加区块到区块映射尾部.
 * 添加的区块是未构造的.
 */
template<typename E>
void Deque<E>::insert_block_at_back()
{
    // 尾部映射满，则扩容映射到两倍，新增容量安排在尾部
    if (it_ed.block == map + M - 1)
        reserve_map(2 * M, false);
    it_ed.block + 1 = data_allocator::allocate(BLOCK_SIZE);
    // 重置尾迭代器的指向
    it_ed.set_block(it_ed.block + 1);
    it_ed.current = it_ed.head;
}

/**
 * 移除[block_begin, block_end)映射范围指向的区块.
 * 移除前的区块已经析构完成.
 *
 * @param block_begin: 区块起始位置（包含）
 * @param block_end: 区块结束位置（不包含）
 */
template<typename E>
void Deque<E>::remove_block(E** block_begin, E** block_end)
{
    for (E** i = block_begin; i < block_end; ++i)
        data_allocator::deallocate(*i, BLOCK_SIZE);
}

/**
 * 移除区块映射头部的区块.
 * 移除前的区块已经析构完成.
 */
template<typename E>
void Deque<E>::remove_block_at_front()
{
    // 释放空区块
    data_allocator::deallocate(*it_bg.block);
    // 重置头迭代器的指向
    it_bg.set_block(it_bg.block + 1);
    it_bg.current = it_bg.head;
}

/**
 * 移除区块映射尾部的区块.
 * 移除前的区块已经析构完成.
 */
template<typename E>
void Deque<E>::remove_block_at_back()
{
    // 释放空区块
    data_allocator::deallocate(*it_ed.block);
    // 重置尾迭代器的指向
    it_ed.set_block(it_ed.block - 1);
    it_ed.current = it_ed.tail;
}

/**
 * 收缩双端队列，移除过剩容量.
 */
template<typename E>
void Deque<E>::shrink_to_fit()
{
    // Note: g++在头尾区块的剩余容量之和大于一个区块时会选择收缩
    //       一个区块的容量，这个操作的代价很高昂.
    //       因此这里选择仅释放多余的映射容量，不负责调整区块.
    int new_count = it_ed.block + 1 - it_bg.block + 2;

    E** new_map = map_allocator::allocate(new_count);
    std::copy(it_bg.block, it_ed.block + 1, new_map + 1);
    map_allocator::deallocate(map, M);
    map = new_map;
    M = new_count;
    // 新的映射头尾保留一个空映射
    it_bg.set_block(map + 1);
    it_ed.set_block(map + M - 2);
}

/**
 * 返回const队首引用.
 *
 * @return const队首引用
 * @throws std::out_of_range: 双端队列空
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
 * @throws std::out_of_range: 双端队列空
 */
template<typename E>
const E& Deque<E>::back() const
{
    if (empty())
        throw std::out_of_range("Deque::back");
    return *std::prev(end());
}

/**
 * 返回Deque指定位置元素的const引用，并进行越界检查.
 *
 * @return 指定位置元素的const引用
 * @throws std::out_of_range: 索引不合法
 */
template<typename E>
const E& Deque<E>::at(int i) const
{
    if (!valid(i))
        throw std::out_of_range("Deque::at");
    return (*this)[i];
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
    // 头迭代器区块满，则添加新区块到区块映射头部
    if (it_bg.current == it_bg.head)
        insert_block_at_front();
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
    // 尾迭代器区块满，则添加新区块到区块映射尾部
    if (it_ed.current == it_ed.tail)
        insert_block_at_back();
    data_allocator::construct(it_ed.current, std::move(elem));
    ++it_ed.current;
}

/**
 * 添加元素到双端队列迭代器指定的位置.
 *
 * @param pos: 指向添加位置的迭代器
 * @param elem: 要添加的元素
 */
template<typename E>
void Deque<E>::insert(iterator pos, E elem)
{
    if      (pos == it_bg) insert_front(std::move(elem));
    else if (pos == it_ed) insert_back(std::move(elem));
    else
    {
        // 插入位置位于前半部分，则元素前移
        if (pos - it_bg < (size() >> 1))
        {
            // 用头部添加的方式预先安排好区块分配，并前移头部元素
            insert_front(front());
            std::copy(std::next(it_bg, 2), pos, std::next(it_bg));
            // 这时pos指向和原迭代器偏移相同的位置
            pos = std::prev(pos);
        }
        // 插入位置位于后半部分，则元素后移
        else
        {
            // 用尾部添加的方式预先安排好区块分配，并后移尾部元素
            insert_back(back());
            std::copy_backward(pos, std::prev(it_ed, 2), std::prev(it_ed));
        }
        *pos = std::move(elem);
    }
}

/**
 * 移除队首元素.
 * 当双端队列达到1/4容量，缩小双端队列容量.
 *
 * @throws std::out_of_range: 双端队列空
 */
template<typename E>
void Deque<E>::remove_front()
{
    if (empty())
        throw std::out_of_range("Deque::remove_front");
    data_allocator::destroy(it_bg.current);
    it_bg.current++;
    if (it_bg.current == it_bg.tail)
        remove_block_at_front();
}

/**
 * 移除队尾元素.
 * 当双端队列达到1/4容量，缩小双端队列容量.
 *
 * @throws std::out_of_range: 双端队列空
 */
template<typename E>
void Deque<E>::remove_back()
{
    if (empty())
        throw std::out_of_range("Deque::remove_back");
    --it_ed.current;
    data_allocator::destroy(it_end.current);
    if (it_ed.current == it_ed.head)
        remove_block_at_back();
}

/**
 * 移除双端队列迭代器指定位置的元素.
 *
 * @param pos: 指向移除位置的迭代器
 * @throws std::out_of_range: 迭代器非法
 */
template<typename E>
void Deque<E>::remove(iterator pos)
{
    if(!valid(pos))
        throw std::out_of_range("Deque::remove");
    // 移除位置位于前半部分，则元素前移
    if (pos - it_bg < (size() >> 1))
    {
        std::copy_backward(it_bg, pos, std::next(it_bg));
        remove_front();
    }
    // 移除位置位于后半部分，则元素后移
    else
    {
        std::copy(std::next(pos), it_ed, pos);
        remove_back();
    }
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
    swap(M, that.M);
    swap(map, that.map);
    swap(it_bg, that.it_bg);
    swap(it_ed, that.it_ed);
}

/**
 * 清空该双端队列元素.
 */
template<typename E>
void Deque<E>::clear()
{
    E** central_block = map + size() / 2;
    // 析构掉所有区块内的元素
    if (it_bg.block == it_ed.block)
        data_allocator::destroy(it_bg.current, it_ed.current);
    else
    {
        data_allocator::destroy(it_bg.current, it_ed.tail);
        for (E** i = it_bg.block + 1; i < it_ed.block; ++i)
            data_allocator::destroy(*i, *i + BLOCK_SIZE);
        data_allocator::destroy(it_ed.head, it_ed.current);
    }
    // 移除[it_bg.block, it_ed.block)范围的区块空间
    remove_block(it_bg.block, it_ed.block);
    // 最后一个区块映射放到映射中央
    std::swap(central_block, it_ed.block);
    it_bg = it_ed;
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

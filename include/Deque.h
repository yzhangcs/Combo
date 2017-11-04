/*******************************************************************************
 * Deque.h
 *
 * Author: zhangyu
 * Date: 2017.4.23
 ******************************************************************************/

#pragma once
#include <iostream>
#include <iterator>
#include <memory>

namespace cpplib
{

// 根据size值确定区块可存储元素个数
static constexpr size_t block_size(size_t size)
{ return size < 512 ? 512 / size : 1; }

// 双端队列的随机访问迭代器
template<typename E, typename Ptr, typename Ref>
class DequeIterator;

/**
 * 使用模板实现的双端队列.
 * 由动态连续数组存储双端队列.
 */
template<typename E>
class Deque
{
public:
    // 成员类型定义
    using value_type      = E;
    using pointer         = E*;
    using reference       = E&;
    using const_pointer   = const E*;
    using const_reference = const E&;
    using size_type       = std::size_t;
    using difference_type = std::ptrdiff_t;
    using allocator_type  = std::allocator<E>;
    // 迭代器定义
    using iterator               = DequeIterator<E, E*, E&>;
    using const_iterator         = DequeIterator<E, const E*, const E&>;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
private:
    using map_pointer          = pointer*;
    using const_map_pointer    = const pointer*;
    using map_allocator_type   = std::allocator<pointer>;
    using allocator_traits     = typename std::allocator_traits<allocator_type>;
    using map_allocator_traits = typename std::allocator_traits<map_allocator_type>;

    template<bool B, class T = void>
    using enable_if_t = typename std::enable_if<B, T>::type;
    template<typename InputIterator>
    using is_input_iterator = std::is_convertible<
            typename std::iterator_traits<InputIterator>::iterator_category,
            std::input_iterator_tag>;

    static constexpr size_type BLOCK_SIZE = block_size(sizeof(E)); // 区块大小
    static constexpr size_type DEFAULT_MAP_SIZE = 10; // 默认映射大小
public:
    Deque() { initialize_map(0); }
    explicit Deque(size_type count, const E& value = E());
    template<typename InputIterator, typename = enable_if_t<is_input_iterator<InputIterator>::value>>
    Deque(InputIterator first, InputIterator last);
    Deque(std::initializer_list<value_type> ilist);
    Deque(const Deque& that);
    Deque(Deque&& that) noexcept;
    ~Deque();
    Deque& operator=(Deque that);
    Deque& operator=(std::initializer_list<value_type> ilist);
    allocator_type get_allocator() const noexcept { return allocator_type(); }

    iterator begin() noexcept { return it_begin; }
    iterator end()   noexcept { return it_end; }
    const_iterator begin()  const noexcept { return it_begin; }
    const_iterator end()    const noexcept { return it_end; }
    const_iterator cbegin() const noexcept { return it_begin; }
    const_iterator cend()   const noexcept { return it_end; }
    reverse_iterator rbegin() noexcept { return reverse_iterator(it_end); }
    reverse_iterator rend()   noexcept { return reverse_iterator(it_begin); }
    const_reverse_iterator rbegin()  const noexcept { return const_reverse_iterator(it_end); }
    const_reverse_iterator rend()    const noexcept { return const_reverse_iterator(it_begin); }
    const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(it_end); }
    const_reverse_iterator crend()   const noexcept { return const_reverse_iterator(it_begin); }

    // 判断是否为空双端队列
    bool empty() const noexcept { return it_begin == it_end; }
    // 返回双端队列元素的数量
    size_type size() const noexcept { return it_end - it_begin; }
    // 返回双端队列可容纳的最大元素数量
    size_type max_size() const noexcept { return size_type(-1); }
    // 收缩双端队列，移除过剩容量
    void shrink_to_fit();

    // 返回const队首引用
    const E& front() const;
    // 返回const队尾引用
    const E& back() const;
    // 返回指定位置元素的const引用，带边界检查
    const E& at(size_type i) const;
    // 返回指定位置元素的const引用，无边界检查
    const E& operator[](size_type i) const { return it_begin[i]; }
    // 返回队首引用
    E& front() { return const_cast<E&>(static_cast<const Deque&>(*this).front()); }
    // 返回队尾引用
    E& back() { return const_cast<E&>(static_cast<const Deque&>(*this).back()); }
    // 返回指定位置元素的引用，带边界检查
    E& at(size_type i) { return const_cast<E&>(static_cast<const Deque&>(*this).at(i)); }
    // 返回指定位置元素的引用，无边界检查
    E& operator[](size_type i) { return const_cast<E&>(static_cast<const Deque&>(*this)[i]); }

    // 添加元素到队首
    void insert_front(E elem);
    // 添加元素到队尾
    void insert_back(E elem);
    // 添加元素到迭代器指定的位置
    void insert(const_iterator pos, E elem);
    // 添加指定数量的元素到迭代器指定的位置
    void insert(const_iterator pos, size_type count, E elem);
    // 添加初始化列表里的元素到迭代器指定的位置
    void insert(const_iterator pos, std::initializer_list<E> ilist);
    // 队首元素出队
    void remove_front();
    // 队尾元素出队
    void remove_back();
    // 移除迭代器指定位置的元素
    void remove(const_iterator pos);
    // 移除指定迭代器范围内的所有元素
    void remove(const_iterator first, const_iterator last);
    // 内容与另一个Deque对象交换
    void swap(Deque& that);
    // 清空双端队列
    void clear();
private:
    // Note: 将map视为「Vector of blocks」，map的操作类似于Vector
    // 初始化映射
    void initialize_map(size_type count);
    // 重新安排映射的容量
    void reserve_map(size_type new_count, bool at_front);
    // 添加区块到指定区块映射范围
    void insert_block(map_pointer block_begin, map_pointer block_end);
    // 添加区块到区块映射头部
    void insert_block_at_front();
    // 添加区块到区块映射尾部
    void insert_block_at_back();
    // 移除指定区块映射范围的区块
    void remove_block(map_pointer block_begin, map_pointer block_end);
    // 移除区块映射头部的区块
    void remove_block_at_front();
    // 移除区块映射尾部的区块
    void remove_block_at_back();
    // 检查迭代器是否合法
    bool valid(size_type i) const { return i >= 0 && i < size(); }
    // // 得到allocator
    // allocator_type allocator const noexcept { return allocator_type(); }
    // // 得到map_allocator
    // map_allocator_type map_allocator const noexcept { return map_allocator_type(); }
private:
    size_type M;       // 区块个数
    map_pointer map;   // 区块映射
    iterator it_begin; // 队首迭代器
    iterator it_end;   // 队尾迭代器
    allocator_type allocator;
    map_allocator_type map_allocator;
};

/**
 * 双端队列构造函数.
 * 创建并用指定值初始化指定容量的双端队列.
 *
 * @param count: 指定双端队列容量
 * @param value: 用于初始化双端队列的值，不指定时是默认构造的值
 */
template<typename E>
Deque<E>::Deque(size_type count, const E& value)
{
    initialize_map(count);
    uninitialized_fill(it_begin, it_end, value);
}

/**
 * 双端队列构造函数，由初始化列表初始化双端队列.
 *
 * @param ilist: 初始化列表
 */
template<typename E>
Deque<E>::Deque(std::initializer_list<value_type> ilist)
{

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
    uninitialized_copy(that.it_begin, that.it_end, it_begin);
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
    it_begin = that.it_begin;
    it_end = that.it_end;
    that.map = nullptr; // 指向空指针，退出被析构
}

/**
 * 双端队列析构函数函数.
 */
template<typename E>
Deque<E>::~Deque()
{
    // 析构掉所有区块内的元素
    if (it_begin.block == it_end.block)
        for (auto i = it_begin.current; i < it_end.current; ++i)
            allocator_traits::destroy(allocator, i);
    else
    {
        for (auto i = it_begin.current; i < it_begin.tail; ++i)
            allocator_traits::destroy(allocator, i);
        for (map_pointer block = it_begin.block + 1; block < it_end.block; ++block)
            for (auto i = *block; i < *block + BLOCK_SIZE; ++i)
                allocator_traits::destroy(allocator, i);
        for (auto i = it_end.head; i < it_end.current; ++i)
            allocator_traits::destroy(allocator, i);
    }
    // 移除所有的区块
    remove_block(it_begin.block, it_end.block + 1);
    // 释放映射空间
    map_allocator_traits::deallocate(map_allocator, map, M);
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
 * =操作符重载.
 * 让当前Deque对象等于给定初始化列表.
 *
 * @param ilist: 初始化列表
 * @return 当前Deque对象
 */
template<typename E>
Deque<E>& Deque<E>::operator=(std::initializer_list<value_type> ilist)
{
    Deque tmp(ilist);
    // *this与tmp互相交换，退出时tmp被析构
    swap(tmp);
    return *this;
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
    size_type new_count = it_end.block + 1 - it_begin.block + 2;

    map_pointer new_map = map_allocator_traits::allocate(map_allocator, new_count);
    std::copy(it_begin.block, it_end.block + 1, new_map + 1);
    map_allocator_traits::deallocate(map_allocator, map, M);
    map = new_map;
    M = new_count;
    // 新的映射头尾保留一个空映射
    it_begin.set_block(map + 1);
    it_end.set_block(map + M - 2);
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
    return *it_begin;
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
    return *std::prev(it_end);
}

/**
 * 返回Deque指定位置元素的const引用，并进行越界检查.
 *
 * @return 指定位置元素的const引用
 * @throws std::out_of_range: 索引不合法
 */
template<typename E>
const E& Deque<E>::at(size_type i) const
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
    if (it_begin.current == it_begin.head)
        insert_block_at_front();
    --it_begin.current;
    allocator_traits::construct(allocator, it_begin.current, std::move(elem));
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
    allocator_traits::construct(allocator, it_end.current, std::move(elem));
    ++it_end.current;
    // 尾迭代器区块满，则添加新区块到区块映射尾部
    if (it_end.current == it_end.tail)
        insert_block_at_back();
}

/**
 * 添加元素到双端队列迭代器指定的位置.
 *
 * @param pos: 指向添加位置的迭代器
 * @param elem: 要添加的元素
 */
template<typename E>
void Deque<E>::insert(const_iterator pos, E elem)
{
    if      (pos == it_begin) insert_front(std::move(elem));
    else if (pos == it_end)   insert_back(std::move(elem));
    else
    {
        // 插入位置位于前半部分，则元素前移
        if (pos - it_begin < (size() >> 1))
        {
            // 用头部添加的方式预先安排好区块分配，并前移头部元素
            insert_front(front());
            std::copy(std::next(it_begin, 2), pos, std::next(it_begin));
            // 这时pos指向和原迭代器偏移相同的位置
            pos = std::prev(pos);
        }
        // 插入位置位于后半部分，则元素后移
        else
        {
            // 用尾部添加的方式预先安排好区块分配，并后移尾部元素
            insert_back(back());
            std::copy_backward(pos, std::prev(it_end, 2), std::prev(it_end));
        }
        *pos = std::move(elem);
    }
}

/**
 * 添加指定数量的元素到迭代器指定的位置.
 *
 * @param pos: 指向添加位置的迭代器
 * @param count: 添加的元素数量
 * @param elem: 要添加的元素
 */
template<typename E>
void Deque<E>::insert(const_iterator pos, size_type count, E elem)
{
    if (pos == it_begin)
    {

    }
    else if (pos == it_end)
    {

    }
    else
    {

    }
}

/**
 * 添加初始化列表里的元素到迭代器指定的位置.
 *
 * @param pos: 指向添加位置的迭代器
 * @param ilist: 初始化列表
 */
template<typename E>
void Deque<E>::insert(const_iterator pos, std::initializer_list<E> ilist)
{

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
    allocator_traits::destroy(allocator, it_begin.current);
    it_begin.current++;
    if (it_begin.current == it_begin.tail)
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
    if (it_end.current == it_end.head)
        remove_block_at_back();
    --it_end.current;
    allocator_traits::destroy(allocator, it_end.current);
}

/**
 * 移除双端队列迭代器指定位置的元素.
 *
 * @param pos: 指向移除位置的迭代器
 */
template<typename E>
void Deque<E>::remove(const_iterator pos)
{
    // 移除位置位于前半部分，则元素前移
    if (pos - it_begin < difference_type(size() >> 1))
    {
        std::copy_backward(it_begin, pos, std::next(it_begin));
        remove_front();
    }
    // 移除位置位于后半部分，则元素后移
    else
    {
        std::copy(std::next(pos), it_end, pos);
        remove_back();
    }
}

/**
 * 移除双端队列指定迭代器范围内的所有元素.
 *
 * @param first: 指向头部移除位置的迭代器（包含）
 * @param last: 指向尾部移除位置的迭代器（不包含）
 */
template<typename E>
void Deque<E>::remove(const_iterator first, const_iterator last)
{

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
    swap(it_begin, that.it_begin);
    swap(it_end, that.it_end);
}

/**
 * 清空该双端队列元素.
 */
template<typename E>
void Deque<E>::clear()
{
    map_pointer central_block = map + M / 2;
    // 析构掉所有区块内的元素
    if (it_begin.block == it_end.block)
    {
        for (auto i = it_begin.current; i < it_end.current; ++i)
            allocator_traits::destroy(allocator, i);
    }
    else
    {
        for (auto i = it_begin.current; i < it_begin.tail; ++i)
            allocator_traits::destroy(allocator, i);
        for (map_pointer block = it_begin.block + 1; block < it_end.block; ++block)
            for (auto i = *block; i < *block + BLOCK_SIZE; ++i)
                allocator_traits::destroy(allocator, i);
        for (auto i = it_end.head; i < it_end.current; ++i)
            allocator_traits::destroy(allocator, i);
    }
    // 移除[it_begin.block, it_end.block)范围的区块空间
    remove_block(it_begin.block, it_end.block);
    // 最后一个区块映射放到映射中央
    std::swap(central_block, it_end.block);
    it_begin = it_end;
}

/**
 * 初始化映射.
 * 根据指定容量分配相应范围的区块，并初始化迭代器.
 * 初始化后的映射指向的区块是未构造的.
 *
 * @param count: 元素容量
 */
template<typename E>
void Deque<E>::initialize_map(size_type count)
{
    // 满足指定容量所需的最少区块数
    size_type num_blocks = count / BLOCK_SIZE + 1;
    // 映射容量为num_blocks + 2和DEFAULT_MAP_SIZE中的较大值
    M = std::max(num_blocks + 2, size_type(DEFAULT_MAP_SIZE));
    map = map_allocator_traits::allocate(map_allocator, M);
    // 映射两端剩余容量相同
    map_pointer block_begin = map + (M - num_blocks) / 2;
    map_pointer block_end = block_begin + num_blocks;
    // 分配区块，区块映射位于中央位置，便于向两端扩展
    insert_block(block_begin, block_end);
    it_begin = iterator(block_begin, *block_begin);
    it_end = iterator(block_end - 1, *(block_end - 1) + count % BLOCK_SIZE);
}

/**
 * 重新安排映射的容量.
 *
 * @param new_count: 新的映射容量
 * @param at_front: 标识是否将新增的容量安排在映射头部
 */
template<typename E>
void Deque<E>::reserve_map(size_type new_count, bool at_front)
{
    // 如果新的容量小于当前映射容量，则映射不改变
    if (new_count > M)
    {
        map_pointer new_map = map_allocator_traits::allocate(map_allocator, new_count);
        // 如果at_front，则将新增容量安排在映射头部，否则头部剩余容量不变
        map_pointer new_block_begin = new_map + (it_begin.block - map)
                                     + (at_front ? new_count - M : 0);
        // 复制区块映射指针到新的映射，不改变区块
        std::copy(it_begin.block, it_end.block + 1, new_block_begin);
        map_allocator_traits::deallocate(map_allocator, map, M);
        map = new_map;
        M = new_count;
        it_begin.set_block(new_block_begin);
        it_end.set_block(new_block_begin + size() - 1);
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
void Deque<E>::insert_block(map_pointer block_begin, map_pointer block_end)
{
    map_pointer i;
    // Note: commit or rollback
    try
    {
        for (i = block_begin; i < block_end; ++i)
            *i = allocator_traits::allocate(allocator, BLOCK_SIZE);
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
    if (it_begin.block == map)
        reserve_map(2 * M, true);
    *(it_begin.block - 1) = allocator_traits::allocate(allocator, BLOCK_SIZE);
    // 重置头迭代器的指向
    it_begin.set_block(it_begin.block - 1);
    it_begin.current = it_begin.tail;
}

/**
 * 添加区块到区块映射尾部.
 * 添加的区块是未构造的.
 */
template<typename E>
void Deque<E>::insert_block_at_back()
{
    // 尾部映射满，则扩容映射到两倍，新增容量安排在尾部
    if (it_end.block == map + M - 1)
        reserve_map(2 * M, false);
    *(it_end.block + 1) = allocator_traits::allocate(allocator, BLOCK_SIZE);
    // 重置尾迭代器的指向
    it_end.set_block(it_end.block + 1);
    it_end.current = it_end.head;
}

/**
 * 移除[block_begin, block_end)映射范围指向的区块.
 * 移除前的区块已经析构完成.
 *
 * @param block_begin: 区块起始位置（包含）
 * @param block_end: 区块结束位置（不包含）
 */
template<typename E>
void Deque<E>::remove_block(map_pointer block_begin, map_pointer block_end)
{
    for (map_pointer i = block_begin; i < block_end; ++i)
        allocator_traits::deallocate(allocator, *i, BLOCK_SIZE);
}

/**
 * 移除区块映射头部的区块.
 * 移除前的区块已经析构完成.
 */
template<typename E>
void Deque<E>::remove_block_at_front()
{
    // 释放空区块
    allocator_traits::deallocate(allocator, *it_begin.block, BLOCK_SIZE);
    // 重置头迭代器的指向
    it_begin.set_block(it_begin.block + 1);
    it_begin.current = it_begin.head;
}

/**
 * 移除区块映射尾部的区块.
 * 移除前的区块已经析构完成.
 */
template<typename E>
void Deque<E>::remove_block_at_back()
{
    // 释放空区块
    allocator_traits::deallocate(allocator, *it_end.block, BLOCK_SIZE);
    // 重置尾迭代器的指向
    it_end.set_block(it_end.block - 1);
    it_end.current = it_end.tail;
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

template<typename E, typename Ptr, typename Ref>
class DequeIterator
{
public:
    // 成员类型定义
    using iterator_category = std::random_access_iterator_tag;
    using value_type        = E;
    using difference_type   = std::ptrdiff_t;
    using pointer           = Ptr;
    using reference         = Ref;
    // 迭代器定义
    using iterator          = DequeIterator<E, E*, E&>;
    using const_iterator    = DequeIterator<E, const E*, const E&>;
private:
    using map_pointer       = pointer*;
    // 区块大小
    static constexpr size_t BLOCK_SIZE = block_size(sizeof(E));
public:
    DequeIterator() noexcept
    : block(nullptr), current(nullptr), head(nullptr), tail(nullptr) {}
    DequeIterator(map_pointer block, pointer current) noexcept
    : block(block), current(current), head(*block), tail(head + BLOCK_SIZE) {}
    DequeIterator(const iterator& that) noexcept
    : block(that.block), current(that.current), head(that.head), tail(that.tail) {}
    DequeIterator(const const_iterator& that) noexcept
    : block(that.block), current(that.current), head(that.head), tail(that.tail) {}

    reference operator*() const noexcept
    { return *current; }
    pointer operator->() const noexcept
    { return current; }
    reference operator[](difference_type i) const noexcept
    { return *(*this + i); }
    DequeIterator& operator++() noexcept
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
    DequeIterator operator++(int) noexcept
    {
        DequeIterator tmp(*this);
        ++*this;
        return tmp;
    }
    DequeIterator operator+(difference_type n) const noexcept
    {
        DequeIterator tmp(*this);
        return tmp += n;
    }
    DequeIterator& operator+=(difference_type n) noexcept
    {
        // 相对于当前区块头部的偏移
        difference_type offset = n + current - head;

        if (offset >= 0 && offset < difference_type(BLOCK_SIZE))
            current += n;
        else
        {
            difference_type block_offset =
                    offset < 0 ? difference_type((offset + 1) / BLOCK_SIZE) - 1
                               : offset / difference_type(BLOCK_SIZE);
            set_block(block + block_offset);
            current = head + offset - block_offset * difference_type(BLOCK_SIZE);
        }
        return *this;
    }
    DequeIterator& operator--() noexcept
    {
        if (current == head)
            set_block(block - 1);
        --current;
        return *this;
    }
    DequeIterator operator--(int) noexcept
    {
        DequeIterator tmp(*this);
        --*this;
        return tmp;
    }
    DequeIterator operator-(difference_type n) const noexcept
    {
        DequeIterator tmp(*this);
        return tmp += -n;
    }
    DequeIterator& operator-=(difference_type n) noexcept
    { return *this += -n; }
    difference_type operator-(const DequeIterator& that) const noexcept
    {
        return (block - that.block) * BLOCK_SIZE
                + (that.head - that.current)
                + (current - head);
    }
    bool operator==(const DequeIterator& that) const noexcept
    { return current == that.current; }
    bool operator!=(const DequeIterator& that) const noexcept
    { return !(*this == that); }
    bool operator<(const DequeIterator& that) const noexcept
    { return block == that.block ? current < that.current : block < that.block; }
    bool operator>(const DequeIterator& that) const noexcept
    { return that < *this; }
    bool operator<=(const DequeIterator& that) const noexcept
    { return !(that < *this); }
    bool operator>=(const DequeIterator& that) const noexcept
    { return !(*this < that); }
private:
    map_pointer block;  // 映射到当前区块
    pointer current;    // 指向区块当前元素
    pointer head;       // 指向区块头部
    pointer tail;       // 指向区块头部

    // 跳转到指定区块，current的修改交给调用者
    void set_block(map_pointer new_block)
    {
        block = new_block;
        head = *block;
        tail = head + BLOCK_SIZE;
    }

    friend class Deque<E>;
    friend class DequeIterator<E, E*, E&>;
    friend class DequeIterator<E, const E*, const E&>;

    friend void uninitialized_fill(iterator& first, iterator& last, const value_type& value);
    friend void uninitialized_copy(iterator& first, iterator& last, iterator destination);
};

template<typename E, typename Ptr, typename Ref>
void uninitialized_fill(const DequeIterator<E, E*, E&>& first,
                        const DequeIterator<E, E*, E&>& last,
                        const E& value)
{
    using map_pointer = typename DequeIterator<E, E*, E&>::map_pointer;
    using BLOCK_SIZE = DequeIterator<E, E*, E&>::BLOCK_SIZE;

    map_pointer block;

    try
    {
        for (block = first.block + 1; block < last.block; ++block)
            std::uninitialized_fill(*block, *block + BLOCK_SIZE, value);
        if (first.block != last.block)
        {
            std::uninitialized_fill(first.current, first.tail, value);
            std::uninitialized_fill(last.head, last.current, value);
        }
        else
            std::uninitialized_fill(first.current, last.current, value);
    }
    catch(...)
    {
//        destroy(*n, *n + buffer_size());
        throw;
    }

}

template<typename E, typename Ptr, typename Ref>
void uninitialized_copy(const DequeIterator<E, E*, E&>& first,
                        const DequeIterator<E, E*, E&>& last,
                        DequeIterator<E, E*, E&> destination)
{

}

} // namespace cpplib

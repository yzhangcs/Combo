/*******************************************************************************
 * ArrayList.h
 *
 * Author: zhangyu
 * Date: 2017.4.25
 ******************************************************************************/

#pragma once
#include <cassert>
#include <iostream>
#include <iterator>

/**
 * 使用模板实现的数组列表.
 * 由可变长数组存储列表.
 * 实现了数组列表类的前向迭代器.
 */
template<typename E>
class ArrayList
{
private:
    static const int DEFAULT_CAPACITY = 10; // 默认的列表容量

    int n;        // 列表大小
    int capacity; // 列表容量
    E* pl;        // 列表指针
    
    void resize(int size); // 调整列表容量
    bool valid(int i) const { return i >= 0 && i < n; } // 检查索引是否合法
public:
    explicit ArrayList(int cap = DEFAULT_CAPACITY); // 构造函数
    ArrayList(const ArrayList& that); // 复制构造函数
    ArrayList(ArrayList&& that) noexcept; // 移动构造函数
    ~ArrayList() { delete[] pl; } // 析构函数

    int size() const { return n; } // 返回列表当前大小
    int indexOf(const E& elem) const; // 返回第一次出现该元素的位置
    bool isEmpty() const { return n == 0; } // 判断是否为空列表
    bool contains(const E& elem) const // 判断表中是否存在该元素
    { return indexOf(elem) >= 0; }
    void set(int i, E elem); // 设置指定位置的元素值
    void add(int i, E elem); // 添加指定元素到指定位置
    void add(E elem) { add(n, std::move(elem)); } // 添加元素到列表尾部
    void addFirst(E elem) { add(0, std::move(elem)); } // 添加元素到列表头部
    void addLast(E elem) { add(n, std::move(elem)); } // 添加元素到列表尾部
    E remove(int i); // 移除指定位置的元素
    E removeFirst() { return remove(0); } // 移除列表头部元素
    E removeLast() { return remove(n - 1); } // 移除列表尾部元素
    E get(int i) const; // 返回指定位置的元素值
    E front() const { return get(0); } // 返回列表头部元素
    E back() const { return get(n - 1); } // 返回列表尾部元素
    void swap(ArrayList& that); // 内容与另一个ArrayList对象交换
    void clear() { n = 0; } // 清空列表，不释放空间，列表容量不变
    
    E& operator[](int i) const;
    ArrayList& operator=(ArrayList that);
    ArrayList& operator+=(const ArrayList& that);
    template<typename T>
    friend ArrayList<T> operator+(ArrayList<T> lhs, const ArrayList<T>& rhs);
    template <typename T>
    friend bool operator==(const ArrayList<T>& lhs, const ArrayList<T>& rhs);
    template <typename T>
    friend bool operator!=(const ArrayList<T>& lhs, const ArrayList<T>& rhs);
    template<typename T>
    friend std::ostream& operator<<(std::ostream& os, const ArrayList<T>& list);

    class iterator : public std::iterator<std::forward_iterator_tag, E>
    {
    private:
        const ArrayList* list;
        int i;
    public:
        iterator() : list(nullptr), i(0) {}
        iterator(const ArrayList* list, int i) : list(list), i(i) {}
        iterator(const iterator& that) : list(that.list), i(that.i) {}
        ~iterator() {}

        E& operator*() const
        { return list->pl[i]; }
        bool operator==(const iterator& that) const
        { return list->pl == that.list->pl && i == that.i; }
        bool operator!=(const iterator& that) const
        { return list->pl == that.list->pl || i != that.i; }
        iterator& operator++()
        {
            i++;
            return *this;
        }
        iterator operator++(int)
        {
            iterator tmp(*this);
            operator++();
            return tmp;
        }
    };
    iterator begin() { return iterator(this, 0); }
    iterator end() { return iterator(this, n); }
};

/**
 * 变长数组列表构造函数，初始化列表.
 * 列表默认初始容量为10.
 *
 * @param cap: 指定列表容量
 */
template<typename E>
ArrayList<E>::ArrayList(int cap)
{
    n = 0;
    capacity = cap;
    pl = new E[capacity];
}

/**
 * 变长数组列表复制构造函数.
 * 复制另一个列表作为初始化的值.
 *
 * @param that: 被复制的列表
 */
template<typename E>
ArrayList<E>::ArrayList(const ArrayList& that)
{
    n = that.n;
    capacity = that.capacity;
    pl = new E[capacity];
    for (int i = 0; i < n; ++i)
        pl[i] = that.pl[i];
}

/**
 * 变长数组列表移动构造函数.
 * 移动另一个列表，其资源所有权转移到新创建的对象.
 *
 * @param that: 被移动的列表
 */
template<typename E>
ArrayList<E>::ArrayList(ArrayList&& that) noexcept
{
    n = that.n;
    capacity = that.capacity;
    pl = that.pl;
    that.pl = nullptr; // 指向空指针，退出被析构
}

/**
 * 创建指定容量的新数组，并把原数组内的列表元素复制到新数组.
 * 释放原数组内存.
 *
 * @param size: 新的数组大小
 */
template<typename E>
void ArrayList<E>::resize(int size)
{
    assert(size >= n); // 保证新的数组容量不小于列表当前大小
    
    E* pnew = new E[size];

    std::move(pl, pl + n, pnew); // 移动元素到新的数组
    delete[] pl; 
    pl = pnew; // pl指向新数组
    capacity = size;
}

/**
 * 返回第一次出现该元素的位置索引.
 * 当不存在该元素时，返回-1.
 *
 * @param elem: 要查找的元素
 * @return i: 要查找的元素的索引
 *         -1: 找不到该元素
 */
template<typename E>
int ArrayList<E>::indexOf(const E& elem) const
{
    for (int i = 0; i < n; ++i)
        if (pl[i] == elem) return i;
    return -1;
}

/**
 * 设置指定位置的元素值.
 *
 * @param i: 指定元素的索引
 *        elem: 指定的元素值
 * @throws std::out_of_range: 索引不合法
 */
template<typename E>
void ArrayList<E>::set(int i, E elem)
{
    if (!valid(i)) 
        throw std::out_of_range("List index out of range.");
    pl[i] = std::move(elem);
}

/**
 * 添加元素到列表指定位置.
 *
 * @param i: 要添加元素的索引
 *        elem: 要添加的元素
 */
template<typename E>
void ArrayList<E>::add(int i, E elem)
{
    if (!valid(i)) 
        throw std::out_of_range("List index out of range.");
    if (n == capacity)
        resize(capacity * 2);
    // 将pl[i]后面的所有元素向数组后面迁移一个位置
    for (int j = n; j > i; j--)
        pl[j] = std::move(pl[j - 1]);
    pl[i] = std::move(elem);
    n++;
}

/**
 * 移除列表中指定位置的元素.
 *
 * @param i: 要移除元素的索引
 * @return 移除的元素
 * @throws std::out_of_range: 索引不合法
 */
template<typename E>
E ArrayList<E>::remove(int i)
{
    if (!valid(i)) 
        throw std::out_of_range("List index out of range.");

    E tmp = pl[i];
    // 将pl[i]后面的所有元素向前迁移一个位置
    for (int j = i; j < n - 1; j++)
        pl[j] = std::move(pl[j + 1]);
    n--;
    // 保证约为半满状态，保证n>0
    if (n > 0 && n == capacity / 4)
        resize(capacity / 2);
    return tmp; // 发生NRVO
}

/**
 * 返回指定位置的元素.
 *
 * @param i: 指定元素的索引
 * @return 指定位置的元素
 * @throws std::out_of_range: 索引不合法
 */
template<typename E>
E ArrayList<E>::get(int i) const
{
    if (!valid(i)) 
        throw std::out_of_range("List index out of range.");
    return pl[i];
}

/**
 * 交换当前ArrayList对象和另一个ArrayList对象.
 *
 * @param that: ArrayList对象that
 */
template<typename E>
void ArrayList<E>::swap(ArrayList<E>& that)
{
    using std::swap; // 如果没有针对类型的特化swap，则使用std::swap
    swap(n, that.n);
    swap(capacity, that.capacity);
    swap(pl, that.pl);
}

/**
 * []操作符重载.
 *
 * @param i: []操作符内索引
 * @return 索引指向的元素
 * @throws std::out_of_range: 索引不合法
 */
template<typename E>
E& ArrayList<E>::operator[](int i) const
{
    if (!valid(i)) 
        throw std::out_of_range("List index out of range.");
    return pl[i];
}

/**
 * =操作符重载.
 * 让当前ArrayList对象等于给定ArrayList对象that.
 *
 * @param that: ArrayList对象that
 * @return 当前ArrayList对象
 */
template<typename E>
ArrayList<E>& ArrayList<E>::operator=(ArrayList<E> that)
{
    swap(that); // *this与that互相交换，退出时that被析构
    return *this;
}

/**
 * +=操作符重载.
 * 复制另一个对象所有元素,添加到当前对象.
 *
 * @param that: ArrayList对象that
 * @return 当前ArrayList对象
 */
template<typename E>
ArrayList<E>& ArrayList<E>::operator+=(const ArrayList<E>& that)
{
    resize(capacity + that.capacity);
    for (int i = 0; i < that.n; ++i)
        pl[i + n] = that.pl[i];
    n += that.n;
    return *this;
}

/**
 * +操作符重载.
 * 返回一个包含lhs和rhs所有元素的对象.
 *
 * @param lhs: ArrayList对象lhs
 *        rhs: ArrayList对象rhs
 * @return 包含lhs和rhs所有元素的ArrayList对象
 */
template<typename E>
ArrayList<E> operator+(ArrayList<E> lhs, const ArrayList<E>& rhs)
{
    lhs += rhs;
    return lhs;
}

/**
 * ==操作符重载函数，比较两个ArrayList对象是否相等.
 *
 * @param lhs: ArrayList对象lhs
 *        rhs: ArrayList对象rhs
 * @return true: 相等
 *         false: 不等
 */
template<typename E>
std::ostream& operator==(const ArrayList<E>& lhs, const ArrayList<E>& rhs)
{
    if (&lhs == &rhs)             return true;
    if (lhs.size() != rhs.size()) return false;
    return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

/**
 * !=操作符重载函数，比较两个ArrayList对象是否不等.
 *
 * @param lhs: ArrayList对象lhs
 *        rhs: ArrayList对象rhs
 * @return true: 不等
 *         false: 相等
 */
template<typename E>
std::ostream& operator!=(const ArrayList<E>& lhs, const ArrayList<E>& rhs)
{
    return !(lhs == rhs);
}

/**
 * <<操作符重载函数，打印所有列表元素.
 *
 * @param os: 输出流对象
 *        list: 要输出的列表
 * @return 输出流对象
 */
template<typename E>
std::ostream& operator<<(std::ostream& os, const ArrayList<E>& list)
{
    for (int i = 0; i < list.n; ++i)
        os << list.pl[i] << " ";
    return os;
}

/**
 * 交换两个ArrayList对象.
 *
 * @param lhs: ArrayList对象lhs
 *        rhs: ArrayList对象rhs
 */
template<typename E>
void swap(ArrayList<E>& lhs, ArrayList<E>& rhs)
{
    lhs.swap(rhs);
}

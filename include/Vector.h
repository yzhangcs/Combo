/*******************************************************************************
 * Vector.h
 *
 * Author: zhangyu
 * Date: 2017.4.25
 ******************************************************************************/

#pragma once
#include <cassert>
#include <iostream>
#include <iterator>

/**
 * 使用模板实现的Vector.
 * 由动态连续数组存储Vector.
 * 实现了Vector的随机访问迭代器.
 */
template<typename E>
class Vector
{
private:
    static const int DEFAULT_CAPACITY = 10; // 默认的Vector容量

    int n;        // Vector大小
    int capacity; // Vector容量
    E* pl;        // Vector指针
    
    void resize(int size); // 调整Vector容量
    bool valid(int i) const { return i >= 0 && i < n; } // 检查索引是否合法
public:
    explicit Vector(int cap = DEFAULT_CAPACITY); // 构造函数
    Vector(const Vector& that); // 复制构造函数
    Vector(Vector&& that) noexcept; // 移动构造函数
    ~Vector() { delete[] pl; } // 析构函数

    int size() const { return n; } // 返回Vector当前大小
    int indexOf(const E& elem) const; // 返回第一次出现该元素的位置
    bool isEmpty() const { return n == 0; } // 判断是否为空Vector
    bool contains(const E& elem) const // 判断表中是否存在该元素
    { return indexOf(elem) >= 0; }
    void set(int i, E elem); // 设置指定位置的元素值
    void add(int i, E elem); // 添加指定元素到指定位置
    void add(E elem) { add(n, std::move(elem)); } // 添加元素到Vector尾部
    void addFirst(E elem) { add(0, std::move(elem)); } // 添加元素到Vector头部
    void addLast(E elem) { add(n, std::move(elem)); } // 添加元素到Vector尾部
    E remove(int i); // 移除指定位置的元素
    E removeFirst() { return remove(0); } // 移除Vector头部元素
    E removeLast() { return remove(n - 1); } // 移除Vector尾部元素
    E get(int i) const; // 返回指定位置的元素值
    E front() const { return get(0); } // 返回Vector头部元素
    E back() const { return get(n - 1); } // 返回Vector尾部元素
    void swap(Vector& that); // 内容与另一个Vector对象交换
    void clear() { n = 0; } // 清空Vector，不释放空间，Vector容量不变
    
    E& operator[](int i) const;
    Vector& operator=(Vector that);
    Vector& operator+=(const Vector& that);
    template<typename T>
    friend Vector<T> operator+(Vector<T> lhs, const Vector<T>& rhs);
    template <typename T>
    friend bool operator==(const Vector<T>& lhs, const Vector<T>& rhs);
    template <typename T>
    friend bool operator!=(const Vector<T>& lhs, const Vector<T>& rhs);
    template<typename T>
    friend std::ostream& operator<<(std::ostream& os, const Vector<T>& list);

    class iterator : public std::iterator<std::random_access_iterator_tag, E>
    {
    private:
        const Vector* list;
        int i;
    public:
        iterator() : list(nullptr), i(0) {}
        iterator(const Vector* list, int i) : list(list), i(i) {}
        iterator(const iterator& that) : list(that.list), i(that.i) {}
        ~iterator() {}

        E& operator*() const
        { return list->pl[i]; }
        E* operator->() const
        { return &list->pl[i]; }
        E& operator[](int pos) const
        { return list->pl[i + pos]; }
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
        iterator operator+(int pos)
        { return iterator(list, i + pos); }
        iterator operator+=(int pos)
        {
            i += pos;
            return *this;
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
        iterator operator-(int pos) 
        { return iterator(list, i - pos); }
        iterator operator-=(int pos) 
        {
            i -= pos;
            return *this;
        }        
        int operator-(const iterator& that) 
        {
            if (list != that.list)
                throw std::invalid_argument("List invalid iterator.");
            return i - that.i;
        }
        bool operator==(const iterator& that) const
        { return list == that.list && i == that.i; }
        bool operator!=(const iterator& that) const
        { return list != that.list || i != that.i; }
        bool operator<(const iterator& that) const
        {
            if (list != that.list)
                throw std::invalid_argument("List invalid iterator.");
            return i < that.i;
        }
        bool operator<=(const iterator& that) const
        {
            if (list != that.list)
                throw std::invalid_argument("List invalid iterator.");
            return i <= that.i;
        }
        bool operator>(const iterator& that) const
        {
            if (list != that.list)
                throw std::invalid_argument("List invalid iterator.");
            return i > that.i;
        }
        bool operator>=(const iterator& that) const
        {
            if (list != that.list)
                throw std::invalid_argument("List invalid iterator.");
            return i >= that.i;
        }
    };
    iterator begin() const { return iterator(this, 0); }
    iterator end() const { return iterator(this, n); }
};

/**
 * Vector构造函数，初始化Vector.
 * Vector默认初始容量为10.
 *
 * @param cap: 指定Vector容量
 */
template<typename E>
Vector<E>::Vector(int cap)
{
    n = 0;
    capacity = cap;
    pl = new E[capacity];
}

/**
 * Vector复制构造函数.
 * 复制另一个Vector作为初始化的值.
 *
 * @param that: 被复制的Vector
 */
template<typename E>
Vector<E>::Vector(const Vector& that)
{
    n = that.n;
    capacity = that.capacity;
    pl = new E[capacity];
    std::copy(that.begin(), that.end(), begin());
}

/**
 * Vector移动构造函数.
 * 移动另一个Vector，其资源所有权转移到新创建的对象.
 *
 * @param that: 被移动的Vector
 */
template<typename E>
Vector<E>::Vector(Vector&& that) noexcept
{
    n = that.n;
    capacity = that.capacity;
    pl = that.pl;
    that.pl = nullptr; // 指向空指针，退出被析构
}

/**
 * 创建指定容量的新Vector，并移动所有元素到新Vector当中.
 *
 * @param size: 新Vector容量
 */
template<typename E>
void Vector<E>::resize(int size)
{
    // 保证新的容量不小于Vector当前大小
    assert(size >= n); 
    
    Vector tmp(size);
    // 将所有元素移动到临时Vector
    std::move(begin(), end(), tmp.begin());
    tmp.n = n; // 设置临时Vector的大小
    swap(tmp); // *this与tmp互相交换，退出时tmp被析构
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
int Vector<E>::indexOf(const E& elem) const
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
void Vector<E>::set(int i, E elem)
{
    if (!valid(i)) 
        throw std::out_of_range("List i out of range.");
    pl[i] = std::move(elem);
}

/**
 * 添加元素到Vector指定位置.
 *
 * @param i: 要添加元素的索引
 *        elem: 要添加的元素
 */
template<typename E>
void Vector<E>::add(int i, E elem)
{
    if (!valid(i)) 
        throw std::out_of_range("List i out of range.");
    if (n == capacity)
        resize(capacity * 2);
    // 将pl[i]后面的所有元素向数组后面迁移一个位置
    std::move_backward(begin() + i, end(), end() + 1);
    pl[i] = std::move(elem);
    n++;
}

/**
 * 移除Vector中指定位置的元素.
 *
 * @param i: 要移除元素的索引
 * @return 移除的元素
 * @throws std::out_of_range: 索引不合法
 */
template<typename E>
E Vector<E>::remove(int i)
{
    if (!valid(i)) 
        throw std::out_of_range("List i out of range.");

    E tmp = pl[i];
    // 将pl[i]后面的所有元素向前迁移一个位置
    std::move(begin() + i + 1, end(), begin() + i);
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
E Vector<E>::get(int i) const
{
    if (!valid(i)) 
        throw std::out_of_range("List i out of range.");
    return pl[i];
}

/**
 * 交换当前Vector对象和另一个Vector对象.
 *
 * @param that: Vector对象that
 */
template<typename E>
void Vector<E>::swap(Vector<E>& that)
{
    // 如果没有针对类型的特化swap，则使用std::swap
    using std::swap; 
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
E& Vector<E>::operator[](int i) const
{
    if (!valid(i)) 
        throw std::out_of_range("List i out of range.");
    return pl[i];
}

/**
 * =操作符重载.
 * 让当前Vector对象等于给定Vector对象that.
 *
 * @param that: Vector对象that
 * @return 当前Vector对象
 */
template<typename E>
Vector<E>& Vector<E>::operator=(Vector<E> that)
{
    // *this与that互相交换，退出时that被析构
    swap(that);
    return *this;
}

/**
 * +=操作符重载.
 * 复制另一个对象所有元素,添加到当前对象.
 *
 * @param that: Vector对象that
 * @return 当前Vector对象
 */
template<typename E>
Vector<E>& Vector<E>::operator+=(const Vector<E>& that)
{
    resize(capacity + that.capacity);
    std::copy(that.begin(), that.end(), end())
    n += that.n;
    return *this;
}

/**
 * +操作符重载.
 * 返回一个包含lhs和rhs所有元素的对象.
 *
 * @param lhs: Vector对象lhs
 *        rhs: Vector对象rhs
 * @return 包含lhs和rhs所有元素的Vector对象
 */
template<typename E>
Vector<E> operator+(Vector<E> lhs, const Vector<E>& rhs)
{
    lhs += rhs;
    return lhs;
}

/**
 * ==操作符重载函数，比较两个Vector对象是否相等.
 *
 * @param lhs: Vector对象lhs
 *        rhs: Vector对象rhs
 * @return true: 相等
 *         false: 不等
 */
template<typename E>
bool operator==(const Vector<E>& lhs, const Vector<E>& rhs)
{
    if (&lhs == &rhs)             return true;
    if (lhs.size() != rhs.size()) return false;
    return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

/**
 * !=操作符重载函数，比较两个Vector对象是否不等.
 *
 * @param lhs: Vector对象lhs
 *        rhs: Vector对象rhs
 * @return true: 不等
 *         false: 相等
 */
template<typename E>
bool operator!=(const Vector<E>& lhs, const Vector<E>& rhs)
{
    return !(lhs == rhs);
}

/**
 * <<操作符重载函数，打印所有Vector元素.
 *
 * @param os: 输出流对象
 *        list: 要输出的Vector
 * @return 输出流对象
 */
template<typename E>
std::ostream& operator<<(std::ostream& os, const Vector<E>& list)
{
    for (auto i : list)
        os << i << " ";
    return os;
}

/**
 * 交换两个Vector对象.
 *
 * @param lhs: Vector对象lhs
 *        rhs: Vector对象rhs
 */
template<typename E>
void swap(Vector<E>& lhs, Vector<E>& rhs)
{
    lhs.swap(rhs);
}

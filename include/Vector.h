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
    static const int DEFAULT_CAPACITY = 10; // 默认的Vector容量
private:
    int n; // Vector大小
    int N; // Vector容量
    E* pl; // Vector指针
    
    // 调整Vector容量
    void resize(int size); 
    // 检查索引是否合法
    bool valid(int i) const { return i >= 0 && i < n; } 
public:
    explicit Vector(int cap = DEFAULT_CAPACITY); 
    Vector(const Vector& that);
    Vector(Vector&& that) noexcept;
    ~Vector() { delete[] pl; }

    // 返回Vector当前大小
    int size() const { return n; } 
    // 返回Vector容量
    int capacity() const { return N; } 
    // 判断是否为空Vector
    bool empty() const { return n == 0; } 
    // 添加指定元素到指定位置
    void insert(int i, E elem); 
    // 添加元素到Vector尾部
    void insertBack(E elem); 
    // 移除指定位置的元素
    E remove(int i); 
    // 移除Vector尾部元素
    E removeBack();  
    // 返回指定位置元素的引用，带边界检查
    E& at(int i) { return const_cast<E&>(static_cast<const Vector&>(*this).at(i)); }
    // 返回指定位置元素的const引用，带边界检查
    const E& at(int i) const; 
    // 返回Vector头部元素的引用
    E& front() { return const_cast<E&>(static_cast<const Vector&>(*this).front()); }  
    // 返回Vector头部元素的const引用
    const E& front() const; 
    // 返回Vector尾部元素的引用
    E& back() { return const_cast<E&>(static_cast<const Vector&>(*this).back()); }  
    // 返回Vector尾部元素的const引用
    const E& back() const; 
    // 内容与另一个Vector对象交换
    void swap(Vector& that); 
    // 清空Vector，不释放空间，Vector容量不变
    void clear() { n = 0; } 
    
    // 返回指定位置元素的引用，无边界检查
    E& operator[](int i) { return const_cast<E&>(static_cast<const Vector&>(*this)[i]); }
    // 返回指定位置元素的const引用，无边界检查
    const E& operator[](int i) const; 
    Vector& operator=(Vector that);
    Vector& operator+=(const Vector& that);
    template<typename T>
    friend Vector<T> operator+(Vector<T> lhs, const Vector<T>& rhs);
    template <typename T>
    friend bool operator==(const Vector<T>& lhs, const Vector<T>& rhs);
    template <typename T>
    friend bool operator!=(const Vector<T>& lhs, const Vector<T>& rhs);
    template<typename T>
    friend std::ostream& operator<<(std::ostream& os, const Vector<T>& vector);

    class iterator : public std::iterator<std::random_access_iterator_tag, E>
    {
    private:
        const Vector* vector;
        int i;
    public:
        iterator() : vector(nullptr), i(0) {}
        iterator(const Vector* vector, int i) : vector(vector), i(i) {}
        iterator(const iterator& that) : vector(that.vector), i(that.i) {}
        ~iterator() {}

        E& operator*() const
        { return vector->pl[i]; }
        E* operator->() const
        { return &vector->pl[i]; }
        E& operator[](int pos) const
        { return vector->pl[i + pos]; }
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
        { return iterator(vector, i + pos); }
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
        { return iterator(vector, i - pos); }
        iterator operator-=(int pos) 
        {
            i -= pos;
            return *this;
        }        
        int operator-(const iterator& that) 
        {
            if (vector != that.vector)
                throw std::invalid_argument("Vector::iterator::operator-() invalid iterator.");
            return i - that.i;
        }
        bool operator==(const iterator& that) const
        { return vector == that.vector && i == that.i; }
        bool operator!=(const iterator& that) const
        { return vector != that.vector || i != that.i; }
        bool operator<(const iterator& that) const
        {
            if (vector != that.vector)
                throw std::invalid_argument("Vector::iterator::operator<() invalid iterator.");
            return i < that.i;
        }
        bool operator<=(const iterator& that) const
        {
            if (vector != that.vector)
                throw std::invalid_argument("Vector::iterator::operator<=() invalid iterator.");
            return i <= that.i;
        }
        bool operator>(const iterator& that) const
        {
            if (vector != that.vector)
                throw std::invalid_argument("Vector::iterator::operator>() invalid iterator.");
            return i > that.i;
        }
        bool operator>=(const iterator& that) const
        {
            if (vector != that.vector)
                throw std::invalid_argument("Vector::iterator::operator>=() invalid iterator.");
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
    N = cap;
    pl = new E[N];
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
    N = that.N;
    pl = new E[N];
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
    N = that.N;
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
 * 添加元素到Vector指定位置.
 * 当Vector达到最大容量，扩容Vector到两倍容量后，再添加元素.
 *
 * @param i: 要添加元素的索引
 *        elem: 要添加的元素
 * @throws std::out_of_range: 索引不合法
 */
template<typename E>
void Vector<E>::insert(int i, E elem)
{
    if (i == n)
        insertBack(elem);
    else if (!valid(i)) 
        throw std::out_of_range("Vector::insert() i out of range.");
    else
    {
        if (n == N) resize(N * 2);
        // 将pl[i]后面的所有元素向数组后面迁移一个位置
        std::move_backward(std::next(begin(), i), end(), 
                           std::next(begin(), n + 1));
        (*this)[i] = std::move(elem);
        n++;
    }
}

/**
 * 添加元素到Vector尾部.
 * 当Vector达到最大容量，扩容Vector到两倍容量后，再添加元素.
 *
 * @param elem: 要添加的元素
 */
template<typename E>
void Vector<E>::insertBack(E elem)
{
    if (n == N)
        resize(N * 2);
    (*this)[n++] = std::move(elem);
}

/**
 * 移除Vector中指定位置的元素.
 * 当Vector达到1/4容量，缩小Vector容量.
 *
 * @param i: 要移除元素的索引
 * @return 移除的元素
 * @throws std::out_of_range: 索引不合法
 */
template<typename E>
E Vector<E>::remove(int i)
{
    if (i == n - 1)
        return removeBack();
    if (!valid(i)) 
        throw std::out_of_range("Vector::remove() i out of range.");

    E tmp = (*this)[i];
    // 将pl[i]后面的所有元素向前迁移一个位置
    std::move(std::next(begin(), i + 1), end(), 
              std::next(begin(), i));
    n--;
    // 保证约为半满状态，保证n>0
    if (n > 0 && n == N / 4)
        resize(N / 2);
    return tmp; // 发生NRVO
}

/**
 * 移除Vector尾部元素.
 * 当Vector达到1/4容量，缩小Vector容量.
 *
 * @return 移除的元素
 * @throws std::out_of_range: Vector为空
 */
template<typename E>
E Vector<E>::removeBack()
{
    if (empty()) 
        throw std::out_of_range("Vector::removeBack() underflow.");

    E tmp = (*this)[--n];
    // 保证Vector始终约为半满状态，保证n>0
    if (n > 0 && n == N / 4)
        resize(N / 2);
    return tmp; // 发生NRVO
}

/**
 * 返回Vector头部元素的const引用.
 *
 * @return Vector头部元素的const引用
 * @throws std::out_of_range: Vector为空
 */
template<typename E>
const E& Vector<E>::front() const 
{
    if (empty()) 
        throw std::out_of_range("Vector::front() underflow.");
    return *begin();
}

/**
 * 返回Vector尾部元素的引用.
 *
 * @return Vector尾部元素的引用
 * @throws std::out_of_range: Vector为空
 */
template<typename E>
const E& Vector<E>::back() const
{
    if (empty()) 
        throw std::out_of_range("Vector::back() underflow.");
    return *std::prev(end());
}

/**
 * 返回Vector指定位置元素的const引用，并进行越界检查.
 *
 * @return 指定位置元素的const引用
 * @throws std::out_of_range: 索引不合法
 */
template<typename E>
const E& Vector<E>::at(int i) const
{
    if (!valid(i)) 
        throw std::out_of_range("Vector::at() i out of range.");
    return (*this)[i];
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
    swap(N, that.N);
    swap(pl, that.pl);
}

/**
 * []操作符重载.
 *
 * @param i: []操作符内索引
 * @return 索引指向元素的const引用
 */
template<typename E>
const E& Vector<E>::operator[](int i) const
{
    return *std::next(begin(), i);
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
    resize(N + that.N);
    std::copy(that.begin(), that.end(), end());
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
 *        vector: 要输出的Vector
 * @return 输出流对象
 */
template<typename E>
std::ostream& operator<<(std::ostream& os, const Vector<E>& vector)
{
    for (auto i : vector)
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

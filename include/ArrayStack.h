/*******************************************************************************
 * ArrayStack.h
 *
 * Author: zhangyu
 * Date: 2017.4.21
 ******************************************************************************/

#pragma once
#include <cassert>
#include <iostream>
#include <iterator>

/**
 * 使用模板实现的后进先出栈.
 * 由可变长数组存储栈.
 * 实现了数组栈的前向迭代器，方向从栈底开始.
 */
template<typename E>
class ArrayStack
{
private:
    static const int DEFAULT_CAPACITY = 10; // 默认栈容量

    int n; // 栈大小
    int capacity; // 栈容量
    E* ps; // 栈指针

    void resize(int size); // 调整栈容量
public:
    explicit ArrayStack(int cap = DEFAULT_CAPACITY); // 构造函数
    ArrayStack(const ArrayStack& that); // 复制构造函数
    ArrayStack(ArrayStack&& that) noexcept; // 移动构造函数
    ~ArrayStack() { delete[] ps; } // 析构函数  

    int size() const { return n; } // 返回栈当前大小
    bool isEmpty() const { return n == 0; } // 判断是否为空栈
    void push(E elem); // 入栈函数
    E pop(); // 出栈函数
    E top(); // 返回栈顶
    void swap(ArrayStack& that); // 内容与另一个ArrayStack对象交换
    void clear() { n = 0; } // 清空栈，不释放空间，栈容量不变

    ArrayStack& operator=(ArrayStack that);
    template <typename T>
    friend bool operator==(const ArrayStack<T>& lhs, const ArrayStack<T>& rhs);
    template <typename T>
    friend bool operator!=(const ArrayStack<T>& lhs, const ArrayStack<T>& rhs);
    template <typename T>
    friend std::ostream& operator<<(std::ostream& os, const ArrayStack<T>& stack);

    class iterator : public std::iterator<std::forward_iterator_tag, E>
    {
    private:
        const ArrayStack* stack;
        int i;
    public:
        iterator() : stack(nullptr), i(0) {}
        iterator(const ArrayStack* stack, int i) : stack(stack), i(i) {}
        iterator(const iterator& that) : stack(that.stack), i(that.i) {}
        ~iterator() {}

        E& operator*() const
        { return stack->ps[i]; }
        E* operator->() const
        { return &stack->ps[i]; }
        iterator& operator++()
        {
            i++;
            return *this;
        }
        iterator operator++(int)
        {
            iterator tmp(*this);
            i++;
            return tmp;
        }
        bool operator==(const iterator& that) const
        { return stack == that.stack && i == that.i; }
        bool operator!=(const iterator& that) const
        { return stack != that.stack || i != that.i; }
    };
    iterator begin() const { return iterator(this, 0); }
    iterator end() const { return iterator(this, n); }
};

/**
 * 变长数组栈构造函数，初始化栈.
 * 栈初始容量为10.
 *
 * @param cap: 指定栈容量
 */
template<typename E>
ArrayStack<E>::ArrayStack(int cap)
{
    n = 0;
    capacity = cap; // 默认容量为10
    ps = new E[capacity];
}

/**
 * 变长数组栈复制构造函数.
 * 复制另一个栈作为初始化的值.
 *
 * @param that: 被复制的栈
 */
template<typename E>
ArrayStack<E>::ArrayStack(const ArrayStack& that)
{
    n = that.n;
    capacity = that.capacity;
    ps = new E[capacity];
    std::copy(that.begin(), that.end(), begin());
}

/**
 * 变长数组栈移动构造函数.
 * 移动另一个栈，其资源所有权转移到新创建的对象.
 *
 * @param that: 被移动的栈
 */
template<typename E>
ArrayStack<E>::ArrayStack(ArrayStack&& that) noexcept
{
    n = that.n;
    capacity = that.capacity;
    ps = that.ps;
    that.ps = nullptr; // 指向空指针，退出被析构
}

/**
 * 创建指定容量的新栈，并移动所有元素到新栈当中.
 *
 * @param size: 新栈容量
 */
template<typename E>
void ArrayStack<E>::resize(int size)
{
    // 保证新的容量不小于栈当前大小
    assert(size >= n); 
    
    ArrayStack tmp(size);
    // 将所有元素移动到临时栈
    std::move(begin(), end(), tmp.begin());
    tmp.n = n; // 设置临时栈的大小
    swap(tmp); // *this与tmp互相交换，退出时tmp被析构
}

/**
 * 入栈函数，使元素入栈.
 * 当栈达到最大容量，扩容栈到两倍容量后，再将元素入栈.
 *
 * @param elem: 要入栈的元素
 */
template<typename E>
void ArrayStack<E>::push(E elem)
{
    if (n == capacity) 
        resize(capacity * 2);
    // 移动元素入栈
    ps[n++] = std::move(elem); 
}

/**
 * 出栈函数，让最近加入的元素出栈.
 * 当栈达到1/4容量，缩小栈容量.
 *
 * @return 出栈元素
 * @throws std::out_of_range: 栈空
 */
template<typename E>
E ArrayStack<E>::pop()
{
    if (isEmpty()) 
        throw std::out_of_range("Stack underflow.");

    E tmp = ps[--n];
    // 保证栈始终约为半满状态，保证n>0
    if (n > 0 && n == capacity / 4) 
        resize(capacity / 2);
    return tmp; // 发生NRVO
}

/**
 * 返回栈顶元素.
 *
 * @return 栈顶元素
 * @throws std::out_of_range: 栈空
 */
template<typename E>
E ArrayStack<E>::top()
{
    if (isEmpty()) 
        throw std::out_of_range("Stack underflow.");
    return ps[n - 1];
}

/**
 * 交换当前ArrayStack对象和另一个ArrayStack对象.
 *
 * @param that: ArrayStack对象that
 */
template<typename E>
void ArrayStack<E>::swap(ArrayStack<E>& that)
{
    using std::swap; // 如果没有针对类型的特化swap，则使用std::swap
    swap(n, that.n);
    swap(capacity, that.capacity);
    swap(ps, that.ps);
}

/**
 * =操作符重载.
 * 让当前ArrayStack对象等于给定ArrayStack对象that.
 *
 * @param that: ArrayStack对象that
 * @return 当前ArrayStack对象
 */
template<typename E>
ArrayStack<E>& ArrayStack<E>::operator=(ArrayStack that)
{
    // 按值传参，传入右值时会有「复制消除」
    swap(that); // *this与that互相交换，退出时that被析构
    return *this;
}

/**
 * ==操作符重载函数，比较两个ArrayStack对象是否相等.
 *
 * @param lhs: ArrayStack对象lhs
 *        rhs: ArrayStack对象rhs
 * @return true: 相等
 *         false: 不等
 */
template<typename E>
bool operator==(const ArrayStack<E>& lhs, const ArrayStack<E>& rhs)
{
    if (&lhs == &rhs)             return true;
    if (lhs.size() != rhs.size()) return false;
    return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

/**
 * !=操作符重载函数，比较两个ArrayStack对象是否不等.
 *
 * @param lhs: ArrayStack对象lhs
 *        rhs: ArrayStack对象rhs
 * @return true: 不等
 *         false: 相等
 */
template<typename E>
bool operator!=(const ArrayStack<E>& lhs, const ArrayStack<E>& rhs)
{
    return !(lhs == rhs);
}

/**
 * <<操作符重载函数，打印所有栈元素.
 *
 * @param os: 输出流对象
 *        stack: 要输出的栈
 * @return 输出流对象
 */
template<typename E>
std::ostream& operator<<(std::ostream& os, const ArrayStack<E>& stack)
{
    for (auto i : stack)
        os << i << " ";
    return os;
}

/**
 * 交换两个ArrayStack对象.
 *
 * @param lhs: ArrayStack对象lhs
 *        rhs: ArrayStack对象rhs
 */
template<typename E>
void swap(ArrayStack<E>& lhs, ArrayStack<E>& rhs)
{
    lhs.swap(rhs);
}
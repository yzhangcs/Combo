/*******************************************************************************
 * WeightedUnion.h
 *
 * Author: zhangyu
 * Date: 2017.6.8
 ******************************************************************************/

#pragma once

/**
 * 加权快速合并的并查集.
 */
class WeightedUnion
{
private: 
    int n; // 并查集大小
    int components; // 连通分量的数量
    int* id; // id[i]为i的父触点.
    int* sz; // sz[i]指以i为根的连通分量的总触点数

    // 检查触点p是否合法
    bool valid(int p) const { return p >= 0 && p < n; }
public:
    explicit WeightedUnion(int size);
    WeightedUnion(const WeightedUnion& that); // 复制构造函数
    WeightedUnion(WeightedUnion&& that) noexcept; // 移动构造函数
    ~WeightedUnion();

    // 判断p与q是否属于同一个连通分量
    bool isConnected(int p, int q) { return find(p) == find(q); }
    int count() { return components; } // 返回连通分量数
    int find(int p); // 找到p所属连通分量的标识符
    void join(int p, int q); // 合并p与q所属的连通分量
    void swap(WeightedUnion& that); // 内容与另一个WeightedUnion对象交换 

    WeightedUnion& operator=(WeightedUnion that);
};

/**
 * 并查集构造函数，初始化并查集.
 * 将每个触点都初始化为一个单独的连通分量.
 * 
 * @param size: 指定的并查集大小
 */
WeightedUnion::WeightedUnion(int size)
{
    n = size;
    components = n; // n个连通分量
    id = new int[n];
    sz = new int[n];
    // 每个触点都设为一个单独连通分量
    for (int i = 0; i < n; i++)
    {
        id[i] = i; // 设置自身为连通分量的根触点
        sz[i] = 1; // 设置连通分量的大小为1
    }
}

/**
 * 并查集复制构造函数.
 * 复制另一个并查集作为初始化的值.
 *
 * @param that: 被复制的并查集
 */
WeightedUnion::WeightedUnion(const WeightedUnion& that)
{
    n = that.n;
    components = that.components;
    id = new int[n];
    sz = new int[n];
    for (int i = 0; i < n; ++i)
    {
        id[i] = that.id[i];
        sz[i] = that.sz[i];
    }
}

/**
 * 并查集移动构造函数.
 * 移动另一个并查集，其资源所有权转移到新创建的对象.
 *
 * @param that: 被移动的并查集
 */
WeightedUnion::WeightedUnion(WeightedUnion&& that) noexcept
{
    n = that.n;
    components = that.components;
    id = that.id;
    sz = that.sz;
    that.id = nullptr;
    that.sz = nullptr; // 指向空指针，退出被析构
}

/**
 * 并查集析构函数.
 */
WeightedUnion::~WeightedUnion()
{
    delete[] id;
    delete[] sz;
}

/**
 * 找到p所属连通分量的标识符.
 *
 * @param p: 触点p
 * @return p所属连通分量的标识符
 * @throws std::out_of_range: 触点不合法
 */
int WeightedUnion::find(int p)
{
    if (!valid(p)) 
        throw std::out_of_range("WeightedUnion index out of range.");
    // p所属连通分量的根触点的id值作为该连通分量的标识符
    while (p != id[p])
        p = id[p];
    return p;
}

/**
 * 合并p与q所属的连通分量.
 *
 * @param p: 触点p
 *        q: 触点q
 */
void WeightedUnion::join(int p, int q)
{
    int rootP = find(p);
    int rootQ = find(q);

    // 已经属于同一个连通分量中则返回
    if (rootP == rootQ) return;
    // 小的连通分量合并到大的连通分量，使合并后树的高度尽可能小
    if (sz[rootP] < sz[rootQ]) 
    {
        id[rootP] = rootQ;
        sz[rootQ] += sz[rootP];
    }
    else
    {
        id[rootQ] = rootP;
        sz[rootP] += sz[rootQ];
    }
    components--;
}

/**
 * 交换当前WeightedUnion对象和另一个WeightedUnion对象.
 *
 * @param that: WeightedUnion对象that
 */
void WeightedUnion::swap(WeightedUnion& that)
{
    using std::swap; // 如果没有针对类型的特化swap，则使用std::swap
    swap(n, that.n);
    swap(components, that.components);
    swap(id, that.id);
    swap(sz, that.sz);
}

/**
 * =操作符重载.
 * 让当前WeightedUnion对象等于给定WeightedUnion对象that.
 *
 * @param that: WeightedUnion对象that
 * @return 当前WeightedUnion对象
 */
WeightedUnion& WeightedUnion::operator=(WeightedUnion that)
{
    swap(that); // *this与that互相交换，退出时that被析构
    return *this;
}

/**
 * 交换两个WeightedUnion对象.
 *
 * @param lhs: WeightedUnion对象lhs
 *        rhs: WeightedUnion对象rhs
 */
void swap(WeightedUnion& lhs, WeightedUnion& rhs)
{
    lhs.swap(rhs);
}
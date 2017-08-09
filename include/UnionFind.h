/*******************************************************************************
 * UnionFind.h
 *
 * Author: zhangyu
 * Date: 2017.6.8
 ******************************************************************************/

#pragma once

/**
 * 带路径压缩的加权快速合并的并查集.
 */
class UnionFind
{
private: 
    int n; // 并查集大小
    int components; // 连通分量的数量
    int* id; // id[i]为i的父触点.
    int* ht; // ht[i]指以i为根的树的高度

    // 检查触点p是否合法
    bool valid(int p) const { return p >= 0 && p < n; }
public:
    explicit UnionFind(int size);
    UnionFind(const UnionFind& that); // 复制构造函数
    UnionFind(UnionFind&& that) noexcept; // 移动构造函数
    ~UnionFind();

    // 判断p与q是否属于同一个连通分量
    bool isConnected(int p, int q) { return find(p) == find(q); } 
    int count() { return components; } // 返回连通分量数
    int find(int p); // 找到p所属连通分量的标识符
    void join(int p, int q); // 合并p与q所属的连通分量
    void swap(UnionFind& that); // 内容与另一个UnionFind对象交换

    UnionFind& operator=(UnionFind that);
};

/**
 * 并查集构造函数，初始化并查集.
 * 将每个触点都初始化为一个单独的连通分量.
 * 
 * @param size: 指定的并查集大小
 */
UnionFind::UnionFind(int size)
{
    n = size;
    components = n; // n个连通分量
    id = new int[n];
    ht = new int[n];
    // 每个触点都设为一个单独连通分量
    for (int i = 0; i < n; i++)
    {
        id[i] = i; // 设置自身为连通分量的根触点
        ht[i] = 1; // 设置树的高度为1
    }
}

/**
 * 并查集复制构造函数.
 * 复制另一个并查集作为初始化的值.
 *
 * @param that: 被复制的并查集
 */
UnionFind::UnionFind(const UnionFind& that)
{
    n = that.n;
    components = that.components;
    id = new int[n];
    ht = new int[n];
    for (int i = 0; i < n; i++)
    {
        id[i] = that.id[i];
        ht[i] = that.ht[i]; 
    }
}

/**
 * 并查集移动构造函数.
 * 移动另一个并查集，其资源所有权转移到新创建的对象.
 *
 * @param that: 被移动的并查集
 */
UnionFind::UnionFind(UnionFind&& that) noexcept
{
    n = that.n;
    components = that.components;
    id = that.id;
    ht = that.ht;
    that.id = nullptr;
    that.ht = nullptr; // 指向空指针，退出被析构
}

/**
 * 并查集析构函数.
 */
UnionFind::~UnionFind()
{
    delete[] id;
    delete[] ht;
}

/**
 * 找到p所属连通分量的标识符.
 *
 * @param p: 触点p
 * @return p所属连通分量的标识符
 * @throws std::out_of_range: 触点不合法
 */
int UnionFind::find(int p)
{
    if (!valid(p)) 
        throw std::out_of_range("UnionFind index out of range.");
    // p所属连通分量的根触点的id值作为该连通分量的标识符
    while (p != id[p])
    {
        // 每个触点指向其祖父触点
        id[p] = id[id[p]]; // 额外一行代码，有效压缩路径
        p = id[p];
    }
    return p;
}

/**
 * 合并p与q所属的连通分量.
 *
 * @param p: 触点p
 *        q: 触点q
 */
void UnionFind::join(int p, int q)
{
    int rootP = find(p);
    int rootQ = find(q);

    // 已经属于同一个连通分量中则返回
    if (rootP == rootQ) return;
    // 高度较低的树合并到较高的树，使合并后树的高度尽可能小
    if      (ht[rootP] < ht[rootQ]) id[rootP] = rootQ; // p合并到q
    else if (ht[rootP] > ht[rootQ]) id[rootQ] = rootP; // q合并到p
    else 
    {
        // 两棵子树高度相等，合并后树的高度需要增加
        id[rootQ] = rootP; 
        ht[rootP]++;
    }
    components--;
}

/**
 * 交换当前UnionFind对象和另一个UnionFind对象.
 *
 * @param that: UnionFind对象that
 */
void UnionFind::swap(UnionFind& that)
{
    // 如果没有针对类型的特化swap，则使用std::swap
    using std::swap; 
    swap(n, that.n);
    swap(components, that.components);
    swap(id, that.id);
    swap(ht, that.ht);
}

/**
 * =操作符重载.
 * 让当前UnionFind对象等于给定UnionFind对象that.
 *
 * @param that: UnionFind对象that
 * @return 当前UnionFind对象
 */
UnionFind& UnionFind::operator=(UnionFind that)
{
    swap(that); // *this与that互相交换，退出时that被析构
    return *this;
}

/**
 * 交换两个UnionFind对象.
 *
 * @param lhs: UnionFind对象lhs
 *        rhs: UnionFind对象rhs
 */
void swap(UnionFind& lhs, UnionFind& rhs)
{
    lhs.swap(rhs);
}
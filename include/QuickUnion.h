/*******************************************************************************
 * QuickUnion.h
 *
 * Author: zhangyu
 * Date: 2017.6.8
 ******************************************************************************/

#pragma once

namespace combo
{

/**
 * 快速合并的并查集.
 */
class QuickUnion
{
private:
    int n;          // 并查集大小
    int components; // 连通分量的数量
    int* id;        // id[i]为i的父触点.

    // 检查触点p是否合法
    bool valid(int p) const { return p >= 0 && p < n; }
public:
    explicit QuickUnion(int size);
    QuickUnion(const QuickUnion& that);
    QuickUnion(QuickUnion&& that) noexcept;
    ~QuickUnion() { delete[] id; }

    // 判断p与q是否属于同一个连通分量
    bool connected(int p, int q) { return find(p) == find(q); }
    // 返回连通分量数
    int count() { return components; }
    // 找到p所属连通分量的标识符
    int find(int p);
    // 合并p与q所属的连通分量
    void join(int p, int q);
    // 内容与另一个QuickUnion对象交换
    void swap(QuickUnion& that);

    QuickUnion& operator=(QuickUnion that);
};

/**
 * 并查集构造函数，初始化并查集.
 * 将每个触点都初始化为一个单独的连通分量.
 *
 * @param size: 指定的并查集大小
 */
QuickUnion::QuickUnion(int size)
{
    n = size;
    components = n; // n个连通分量
    id = new int[n];
    // 每个触点的id值设置为自身，作为一个单独连通分量的根触点
    for (int i = 0; i < n; i++)
        id[i] = i;
}

/**
 * 并查集复制构造函数.
 * 复制另一个并查集作为初始化的值.
 *
 * @param that: 被复制的并查集
 */
QuickUnion::QuickUnion(const QuickUnion& that)
{
    n = that.n;
    components = that.components;
    id = new int[n];
    for (int i = 0; i < n; ++i)
        id[i] = that.id[i];
}

/**
 * 并查集移动构造函数.
 * 移动另一个并查集，其资源所有权转移到新创建的对象.
 *
 * @param that: 被移动的并查集
 */
QuickUnion::QuickUnion(QuickUnion&& that) noexcept
{
    n = that.n;
    components = that.components;
    id = that.id;
    that.id = nullptr; // 指向空指针，退出被析构
}

/**
 * 找到p所属连通分量的标识符.
 *
 * @param p: 触点p
 * @return p所属连通分量的标识符
 * @throws std::out_of_range: 触点不合法
 */
int QuickUnion::find(int p)
{
    if (!valid(p))
        throw std::out_of_range("QuickUnion::find() index out of range.");
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
void QuickUnion::join(int p, int q)
{
    int rootP = find(p);
    int rootQ = find(q);

    // 已经属于同一个连通分量中则返回
    if (rootP == rootQ) return;
    // p所在连通分量的根触点指向q所在连通分量的根触点，合并为一个连通分量
    id[rootP] = rootQ;
    components--;
}

/**
 * 交换当前QuickUnion对象和另一个QuickUnion对象.
 *
 * @param that: QuickUnion对象that
 */
void QuickUnion::swap(QuickUnion& that)
{
    using std::swap;
    swap(n, that.n);
    swap(components, that.components);
    swap(id, that.id);
}

/**
 * =操作符重载.
 * 让当前QuickUnion对象等于给定QuickUnion对象that.
 *
 * @param that: QuickUnion对象that
 * @return 当前QuickUnion对象
 */
QuickUnion& QuickUnion::operator=(QuickUnion that)
{
    swap(that);
    return *this;
}

/**
 * 交换两个QuickUnion对象.
 *
 * @param lhs: QuickUnion对象lhs
 *        rhs: QuickUnion对象rhs
 */
void swap(QuickUnion& lhs, QuickUnion& rhs)
{
    lhs.swap(rhs);
}

} // namespace combo
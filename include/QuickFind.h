/*******************************************************************************
 * QuickFind.h
 *
 * Author: zhangyu
 * Date: 2017.6.8
 ******************************************************************************/

#pragma once

/**
 * 快速查找的并查集.
 */
class QuickFind
{
private: 
    int n;          // 并查集大小
    int components; // 连通分量的数量
    int* id;        // 指向所有触点的指针

    // 检查触点p是否合法
    bool valid(int p) const { return p >= 0 && p < n; }
public:
    explicit QuickFind(int size);
    QuickFind(const QuickFind& that);
    QuickFind(QuickFind&& that) noexcept;
    ~QuickFind() { delete[] id; }  

    // 判断p与q是否属于同一个连通分量
    bool connected(int p, int q); 
    // 返回连通分量数
    int count() { return components; } 
    // 得到p所属连通分量的标识符
    int find(int p); 
    // 合并p与q所属的连通分量
    void join(int p, int q); 
    // 内容与另一个QuickFind对象交换
    void swap(QuickFind& that); 

    QuickFind& operator=(QuickFind that);
};

/**
 * 并查集构造函数，初始化并查集.
 * 将每个触点都初始化为一个单独的连通分量.
 * 
 * @param size: 指定的并查集大小
 */
QuickFind::QuickFind(int size)
{
    n = size;
    components = n; // n个连通分量
    id = new int[n];
    // id值作为连通分量的标识符，相同id值表示属于同一个连通分量
    for (int i = 0; i < n; i++)
        id[i] = i; // 每个触点的标识符设置为自身
}

/**
 * 并查集复制构造函数.
 * 复制另一个并查集作为初始化的值.
 *
 * @param that: 被复制的并查集
 */
QuickFind::QuickFind(const QuickFind& that)
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
QuickFind::QuickFind(QuickFind&& that) noexcept
{
    n = that.n;
    components = that.components;
    id = that.id;
    that.id = nullptr; // 指向空指针，退出被析构
}

/**
 * 判断p与q是否属于同一个连通分量
 *
 * @param p: 触点p
 *        q: 触点q
 * @return true: 属于同一个连通分量
 *         false: 不属于同一个连通分量
 * @throws std::out_of_range: 触点不合法
 */
bool QuickFind::connected(int p, int q)
{ 
    if (!valid(p) || !valid(q)) 
        throw std::out_of_range("QuickFind::connected() index out of range.");
    // 相同id值表示属于同一个连通分量
    return id[p] == id[q]; 
}

/**
 * 得到p所属连通分量的标识符.
 *
 * @param p: 触点p
 * @return p所属连通分量的标识符
 * @throws std::out_of_range: 触点不合法
 */
int QuickFind::find(int p)
{
    if (!valid(p)) 
        throw std::out_of_range("QuickFind::find() index out of range.");
    return id[p];
}

/**
 * 合并p与q所属的连通分量.
 *
 * @param p: 触点p
 *        q: 触点q
 * @throws std::out_of_range: 触点不合法
 */
void QuickFind::join(int p, int q)
{
    if (!valid(p) || !valid(q)) 
        throw std::out_of_range("QuickFind::join() index out of range.");

    int pid = id[p];
    int qid = id[q];

    // 已经属于同一个连通分量中则返回
    if (pid == qid) return; 
    // 合并p所属连通分量触点到q所属的连通分量
    for (int i = 0; i < n; ++i)
        if (id[i] == pid) id[i] = qid; // 与p在同一分量的触点标识符改成qid
    components--;
}

/**
 * 交换当前QuickFind对象和另一个QuickFind对象.
 *
 * @param that: QuickFind对象that
 */
void QuickFind::swap(QuickFind& that)
{
    // 如果没有针对类型的特化swap，则使用std::swap
    using std::swap; 
    swap(n, that.n);
    swap(components, that.components);
    swap(id, that.id);
}

/**
 * =操作符重载.
 * 让当前QuickFind对象等于给定QuickFind对象that.
 *
 * @param that: QuickFind对象that
 * @return 当前QuickFind对象
 */
QuickFind& QuickFind::operator=(QuickFind that)
{
    // *this与that互相交换，退出时that被析构
    swap(that);
    return *this;
}

/**
 * 交换两个QuickFind对象.
 *
 * @param lhs: QuickFind对象lhs
 *        rhs: QuickFind对象rhs
 */
void swap(QuickFind& lhs, QuickFind& rhs)
{
    lhs.swap(rhs);
}
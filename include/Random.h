/*******************************************************************************
 * Random.h
 *
 * Author: zhangyu
 * Date: 2017.5.9
 ******************************************************************************/

#pragma once
#include <cassert>
#include <cmath>
#include <ctime>
#include <iostream>
#include <random>

using std::mt19937;
using std::uniform_int_distribution;
using std::uniform_real_distribution;

/**
 * 随机类.
 * 提供了生成指定范围随机数的静态方法，其中生成器为mt19937（使用Mersenne
 * 旋转算法生成随机序列），默认使用时间作为种子.
 * 提供了基于常见分布来生成随机数的静态方法，包括均匀分布、bernoulli分布、
 * 正态分布、几何分布、poisson分布和指数分布.
 * 提供了基于Kunth-Shuffle来随机打乱数组的静态方法.
 * 这个类不应该被实例化.
 */
class Random
{
    static size_t seed;
    static mt19937 generator;
public:
    static size_t getSeed() { return seed; }
    static void setSeed(size_t s) { seed = s; }
    // 生成一个在[0, n)范围内呈均匀分布的随机整数
    static int random(int n) { return random(0, n); };
    // 生成一个在[a, b)范围内呈均匀分布的随机整数
    static int random(int a, int b);
    // 生成一个在[0, n)范围内呈均匀分布的随机实数
    static double random(double n = 1.0) { return uniform(0.0, n); };
    // 生成一个在[a, b)范围内呈均匀分布的随机实数
    static double random(double a, double b) { return uniform(a, b); };

    // 生成一个在[0, n)范围内呈均匀分布的随机实数
    static double uniform(double n = 1.0) { return uniform(0.0, n); };
    // 生成一个在[a, b)范围内呈均匀分布的随机实数
    static double uniform(double a, double b);
    // 生成基于bernoulli分布的随机值
    static bool bernoulli(double p = 0.5); 
    // 生成基于标准正态分布的随机值
    static double normal(); 
    // 生成基于正态分布的随机值
    static double normal(double mu, double sigma) 
    { return mu + sigma * normal(); };
    // 生成基于几何分布的随机值
    static int geometric(double p); 
    // 生成基于poisson分布的随机值
    static int poisson(double lambda); 
    // 生成基于指数分布的随机值
    static double exponential(double lambda); 
    
    // 随机打乱数组所有元素
    template<typename E, int n>
    static void shuffle(E (&elems)[n]) { shuffle(elems, 0, n); } 
    // 随机打乱数组指定范围的元素
    template<typename E, int n>
    static void shuffle(E (&elems)[n], int lo, int hi); 
    template<typename RandomAccessIterator>
    static void shuffle(RandomAccessIterator lo, RandomAccessIterator hi); 
};

size_t Random::seed = time(nullptr); // 默认使用时间作为种子
mt19937 Random::generator = mt19937(seed); // Mersenne旋转引擎

/**
 * 生成一个在[a, b)范围内呈均匀分布的随机整数.
 *
 * @param a: 随机值左边界
 *        b: 随机值右边界
 * @return 一个在[a, b)范围内呈均匀分布的随机整数
 */
int Random::random(int a, int b)
{
    assert(a < b); // a必须小于b

    // uniform_int_distribution左右边界都包含，这里去掉右边界
    uniform_int_distribution<int> distribution(a, b - 1);

    return distribution(generator);
}

/**
 * 生成一个在[a, b)范围内呈均匀分布的随机实数.
 *
 * @param a: 随机值左边界
 *        b: 随机值右边界
 * @return 一个在[a, b)范围内呈均匀分布的随机实数
 */
double Random::uniform(double a, double b)
{
    assert(a < b);

    uniform_real_distribution<double> distribution(a, b);

    return distribution(generator);
}

/**
 * 给定一个事件的成功概率，返回一个基于bernoulli分布的随机bool值.
 * 即为该事件的结果.
 *
 * @param p: 事件成功的概率
 * @return true: 成功
 *         false: 不成功
 */
bool Random::bernoulli(double p)
{
    assert(p >= 0.0 && p <= 1.0);
    return uniform() < p;
}

/**
 * 生成基于标准正态分布（期望mu为0，标准差sigma为1）的随机实数.
 *
 * @return 一个基于标准正态分布生成的随机实数
 */
double Random::normal()
{
    double r, x, y;

    do
    {
        x = uniform(-1.0, 1.0);
        y = uniform(-1.0, 1.0);
        r = x * x + y * y;
    }
    while (r >= 1 || r == 0);
    return x * sqrt(-2 * log(r) / r);
}

/**
 * 给定一个事件的成功概率，返回一个基于geometric分布的随机整数.
 *
 * @param p: 事件成功的概率
 * @return 一个基于geometric分布的随机整数
 */

int Random::geometric(double p)
{
    assert(p >= 0.0 && p <= 1.0);
    return (int) ceil(log(uniform()) / log(1.0 - p));
}

/**
 * 给定一个事件单位时间的平均发生次数，返回一个基于poisson分布的随机整数.
 *
 * @param lambda: 事件单位时间的平均发生次数
 * @return 一个基于poisson分布的随机整数
 */

int Random::poisson(double lambda)
{
    assert(lambda > 0.0);

    int k = 0;
    double p = 1.0;
    double el = exp(-lambda);

    do
    {
        k++;
        p *= uniform();
    }
    while (p >= el);
    return k-1;
}

/**
 * 给定一个事件单位时间的平均发生次数，返回一个基于指数分布的随机实数.
 *
 * @param lambda: 事件单位时间的平均发生次数
 * @return 一个基于指数分布的随机实数
 */

double Random::exponential(double lambda)
{
    assert(lambda > 0.0);
    return -log(1 - uniform()) / lambda;
}

/**
 * 使用Kunth-Shuffle随机打乱数组指定范围的元素.
 * 数组元素类型由模板的类型参数指定，数组长度为n，由非类型参数指定.
 *
 * @param elems: 一个长度为n的数组
 *        lo: 左边界（包含）
 *        hi: 右边界（不包含）
 */
template<typename E, int n>
void Random::shuffle(E (&elems)[n], int lo, int hi)
{
    assert(lo >= 0 && hi <= n && lo <= hi);
    // 如果没有针对类型的特化swap，则使用std::swap
    using std::swap; 
    for (int i = lo; i < hi; ++i)
        swap(elems[Random::random(lo, i + 1)], elems[i]);
}

/**
 * 使用Kunth-Shuffle随机打乱由两个迭代器指定的范围的元素.
 * 迭代器满足RandomAccessIterator的要求.
 *
 * @param lo: 左边界迭代器（包含）
 *        hi: 右边界迭代器（不包含）
 */
template<typename RandomAccessIterator>
static void shuffle(RandomAccessIterator lo, RandomAccessIterator hi)
{
    int n = hi - lo;

    using std::swap;
    for (int i = 0; i < n; ++i) 
        swap(lo[Random::random(i + 1)], lo[i]);
}
/*******************************************************************************
 * PriorityQueue.h
 *
 * Author: zhangyu
 * Date: 2017.5.6
 ******************************************************************************/

#pragma once
#include <functional>
#include "BinaryHeap.h"

/**
 * 使用模板实现的基于最大二叉堆的最大优先队列.
 * 最大优先队列每次添加和删除元素后进行上浮和下沉操作，以保持队列的堆有序性.
 */
template<typename K>
class PriorityQueue
{
    using Comparator = std::function<bool(const K& lhs, const K& rhs)>;
private:
    static const int DEFAULT_CAPACITY = 10; // 默认最大优先队列容量

    BinaryHeap<K> heap;
public:
    explicit PriorityQueue(Comparator comp) : heap(DEFAULT_CAPACITY, comp) {}
    explicit PriorityQueue(int cap = DEFAULT_CAPACITY, Comparator comp = std::less<K>()) : 
        heap(cap, comp) {} // 构造函数
    PriorityQueue(const PriorityQueue& that) : 
        heap(that.heap) {} // 复制构造函数
    PriorityQueue(PriorityQueue&& that) noexcept :
        heap(std::move(that.heap)) {} // 移动构造函数
    ~PriorityQueue() {} // 析构函数

    int size() const { return heap.size(); } // 返回优先队列当前大小
    bool isEmpty() const { return heap.isEmpty(); } // 判断是否为空优先队列
    void push(K key) { heap.push(std::move(key)); } // 将元素插入优先队列
    K pop() { return heap.pop(); }; // 删除最大优先队列元素
    K top() { return heap.top(); }; // 获取最大优先队列元素
    void swap(PriorityQueue& that) { heap.swap(that.heap); } // 内容与另一个PriorityQueue对象交换
    void clear() { heap.clear(); } // 清空优先队列
    
    PriorityQueue& operator=(PriorityQueue that)
    {
        swap(that); // *this与that互相交换，退出时that被析构
        return *this;
    }
    template <typename T>
    friend std::ostream& operator<<(std::ostream& os, const PriorityQueue<T>& pq)
    { return os << pq.heap; }
};

/**
 * 交换两个PriorityQueue对象.
 *
 * @param lhs: PriorityQueue对象lhs
 *        rhs: PriorityQueue对象rhs
 */
template<typename K>
void swap(PriorityQueue<K>& lhs, PriorityQueue<K>& rhs)
{
    lhs.swap(rhs);
}

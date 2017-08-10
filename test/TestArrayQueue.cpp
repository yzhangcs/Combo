#include <iostream>
#include <string>
#include "ArrayQueue.h"
#include "gtest/gtest.h"

using std::string;

class TestArrayQueue : public testing::Test 
{
protected:
    ArrayQueue<string> queue;
    ArrayQueue<string> a;
    ArrayQueue<string> b;
    ArrayQueue<string> c;
    string str;    
    int scale;
public:
    virtual void SetUp() { scale = 32; }
    virtual void TearDown() {}

    void enqueue_n(ArrayQueue<string>& s, int n)
    {
        for (int i = 0; i < n; ++i)
            s.enqueue(std::to_string(i));
    }
    void dequeue_n(ArrayQueue<string>& s, int n)
    {
        for (int i = 0; i < n; ++i)
            s.dequeue();
    }
};

TEST_F(TestArrayQueue, Basic)
{
    EXPECT_NO_THROW({
        ArrayQueue<string> s1;
        ArrayQueue<string> s2(s1);
        ArrayQueue<string> s3(30);
        ArrayQueue<string> s4(ArrayQueue<string>());

        s1 = s2;
        s2 = ArrayQueue<string>(15);
    });
}

TEST_F(TestArrayQueue, ElementAccess)
{
    EXPECT_THROW(queue.front(), std::out_of_range);
    EXPECT_THROW(queue.back(), std::out_of_range);
    EXPECT_NO_THROW({
        for (int i = 0; i < scale; ++i)
        {
            str = std::to_string(i);
            queue.enqueue(str);
            EXPECT_EQ(str, queue.back());
        }
        for (int i = 0; i < scale; ++i)
        {
            str = queue.front();
            EXPECT_EQ(str, queue.dequeue());
        }
    });
    EXPECT_THROW(queue.front(), std::out_of_range);
    EXPECT_THROW(queue.back(), std::out_of_range);
}

TEST_F(TestArrayQueue, Iterators)
{
    EXPECT_EQ(queue.begin(), queue.end());
    enqueue_n(queue, scale);
    EXPECT_NE(queue.begin(), queue.end());  
    
    auto it = queue.begin();

    for (int i = 0; i < scale; ++i)
        EXPECT_EQ(std::to_string(i), *(it++));
    EXPECT_EQ(it, queue.end());
}

TEST_F(TestArrayQueue, Capacity)
{
    EXPECT_TRUE(queue.isEmpty());
    EXPECT_EQ(0, queue.size());
    enqueue_n(queue, scale);
    EXPECT_EQ(scale, queue.size());
    dequeue_n(queue, scale);
    EXPECT_TRUE(queue.isEmpty());
}

TEST_F(TestArrayQueue, Modifiers)
{
    EXPECT_THROW(queue.dequeue(), std::out_of_range);
    EXPECT_NO_THROW({
        enqueue_n(queue, scale);
        dequeue_n(queue, scale);
    });
    EXPECT_THROW(queue.dequeue(), std::out_of_range);

    enqueue_n(queue, scale);
    queue.clear();
    EXPECT_TRUE(queue.isEmpty());
    EXPECT_EQ(0, queue.size());
    EXPECT_THROW(queue.dequeue(), std::out_of_range);

    enqueue_n(a, scale);
    b.swap(a);
    EXPECT_EQ(scale, b.size());
    for (int i = 0; i < scale; ++i)
        EXPECT_EQ(std::to_string(i), b.dequeue());
}

TEST_F(TestArrayQueue, Other)
{
    using std::swap;
    enqueue_n(a, scale);
    c = a;
    EXPECT_TRUE(c == a && c != b);
    b.swap(a);
    EXPECT_TRUE(c != a && c == b);
    swap(a, b);
    EXPECT_TRUE(c == a && c != b);
}
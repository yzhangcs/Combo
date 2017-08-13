#include <iostream>
#include <string>
#include "Queue.h"
#include "gtest/gtest.h"

using std::string;

class TestQueue : public testing::Test 
{
protected:
    Queue<string> queue;
    Queue<string> a;
    Queue<string> b;
    Queue<string> c;
    string str;    
    int scale;
public:
    virtual void SetUp() { scale = 32; }
    virtual void TearDown() {}

    void enqueue_n(Queue<string>& s, int n)
    {
        for (int i = 0; i < n; ++i)
            s.enqueue(std::to_string(i));
    }
    void dequeue_n(Queue<string>& s, int n)
    {
        for (int i = 0; i < n; ++i)
            s.dequeue();
    }
};

TEST_F(TestQueue, Basic)
{
    EXPECT_NO_THROW({
        Queue<string> s1;
        Queue<string> s2(s1);
        Queue<string> s3(Queue<string>());

        s1 = s2;
        s2 = Queue<string>();
    });
}

TEST_F(TestQueue, ElementAccess)
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

TEST_F(TestQueue, Capacity)
{
    EXPECT_TRUE(queue.empty());
    EXPECT_EQ(0, queue.size());
    
    enqueue_n(queue, scale);
    EXPECT_EQ(scale, queue.size());
    dequeue_n(queue, scale);
    EXPECT_TRUE(queue.empty());
}

TEST_F(TestQueue, Modifiers)
{
    EXPECT_THROW(queue.dequeue(), std::out_of_range);
    EXPECT_NO_THROW({
        enqueue_n(queue, scale);
        for (int i = 0; i < scale; ++i)
            EXPECT_EQ(std::to_string(i), queue.dequeue());
    });
    EXPECT_THROW(queue.dequeue(), std::out_of_range);

    enqueue_n(queue, scale);
    queue.clear();
    EXPECT_TRUE(queue.empty());
    EXPECT_EQ(0, queue.size());
    EXPECT_THROW(queue.dequeue(), std::out_of_range);

    enqueue_n(a, scale);
    b.swap(a);
    EXPECT_EQ(scale, b.size());
    for (int i = 0; i < scale; ++i)
        EXPECT_EQ(std::to_string(i), b.dequeue());
}

TEST_F(TestQueue, Other)
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
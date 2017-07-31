#include <iostream>
#include <string>
#include "LinkedQueue.h"
#include "gtest/gtest.h"

using std::string;

TEST(TestLinkedQueue, Enqueue)
{
    LinkedQueue<string> queue;

    EXPECT_TRUE(queue.isEmpty());
    for (int i = 0; i < 32; ++i)
    {
        queue.enqueue(std::to_string(i));
        EXPECT_EQ(std::to_string(i), queue.back());
    }
    EXPECT_EQ(32, queue.size());
}

TEST(TestLinkedQueue, Dequeue)
{
    LinkedQueue<string> queue;
    string str;

    for (int i = 0; i < 32; ++i)
        queue.enqueue(std::to_string(i));
    EXPECT_EQ(32, queue.size());
    EXPECT_NO_THROW({
        for (int i = 0; i < 32; ++i)
        {
            str = queue.front();
            EXPECT_EQ(str, queue.dequeue());
        }
    });
    EXPECT_TRUE(queue.isEmpty());
    EXPECT_THROW(queue.dequeue(), std::out_of_range);
    EXPECT_THROW(queue.back(), std::out_of_range);
}

TEST(TestLinkedQueue, Other)
{
    LinkedQueue<string> a;
    LinkedQueue<string> b;
    LinkedQueue<string> c;
    string str;

    for (int i = 0; i < 10; ++i)
        a.enqueue(std::to_string(i));
    b = a;
    EXPECT_TRUE(a == b);
    a.clear();
    EXPECT_TRUE(a.isEmpty());
    EXPECT_THROW(a.dequeue(), std::out_of_range);
    c = b;
    b.dequeue();
    a = b;
    EXPECT_TRUE(b != c);
    b.swap(c);
    EXPECT_TRUE(a == c);
    std::cout << "queue a: " << a << std::endl;    
    std::cout << "queue b: ";
    for (auto i : b) std::cout << i << " ";
    std::cout << std::endl;
    std::cout << "queue c: ";
    for (auto i : c) std::cout << i << " ";
    std::cout << std::endl;
}
#include <iostream>
#include <string>
#include "LinkedDeque.h"
#include "gtest/gtest.h"

using std::string;

class TestLinkedDeque : public testing::Test 
{
protected:
    LinkedDeque<string> deque;
    LinkedDeque<string> a;
    LinkedDeque<string> b;
    LinkedDeque<string> c;
    string str;    
    int scale;
public:
    virtual void SetUp() { scale = 32; }
    virtual void TearDown() {}

    void add_n(LinkedDeque<string>& s, int n, bool isForward)
    {
        if (isForward)
        {
            for (int i = 0; i < n; ++i) 
                s.addLast(std::to_string(i));
        }
        else
        {
            for (int i = 0; i < n; ++i) 
                s.addFirst(std::to_string(i));
        }
    }
    void remove_n(LinkedDeque<string>& s, int n, bool isForward)
    {
        if (isForward)
        {
            for (int i = 0; i < n; ++i) 
                s.removeLast();
        }
        else
        {
            for (int i = 0; i < n; ++i) 
                s.removeFirst();
        }
    }
};

TEST_F(TestLinkedDeque, Basic)
{
    EXPECT_NO_THROW({
        LinkedDeque<string> s1;
        LinkedDeque<string> s2(s1);
        LinkedDeque<string> s3(LinkedDeque<string>());

        // s1 = s2;
        // s2 = LinkedDeque<string>();
    });
}

TEST_F(TestLinkedDeque, ElementAccess)
{
    EXPECT_THROW(deque.front(), std::out_of_range);
    EXPECT_THROW(deque.back(), std::out_of_range);
    EXPECT_NO_THROW({
        for (int i = 0; i < scale; ++i)
        {
            str = std::to_string(i);
            deque.addLast(str);
            EXPECT_EQ(str, deque.back());
        }
        for (int i = 0; i < scale; ++i)
        {
            str = std::to_string(i);
            deque.addFirst(str);
            EXPECT_EQ(str, deque.front());
        }
        for (int i = 0; i < scale; ++i)
        {
            str = deque.front();
            EXPECT_EQ(str, deque.removeFirst());
        }
        for (int i = 0; i < scale; ++i)
        {
            str = deque.back();
            EXPECT_EQ(str, deque.removeLast());
        }
    });
    EXPECT_THROW(deque.front(), std::out_of_range);
    EXPECT_THROW(deque.back(), std::out_of_range);
}

TEST_F(TestLinkedDeque, Iterators)
{
    EXPECT_EQ(deque.begin(), deque.end());
    add_n(deque, scale, true);
    EXPECT_NE(deque.begin(), deque.end());
    
    auto bg = deque.begin();
    auto ed = deque.end();

    for (int i = 0; i < scale; ++i)
        EXPECT_EQ(std::to_string(i), *(bg++));
    EXPECT_EQ(bg, deque.end());
    for (int i = scale; i > 0; --i)
        EXPECT_EQ(std::to_string(i - 1), *(--ed));
    EXPECT_EQ(ed, deque.begin());
}

TEST_F(TestLinkedDeque, Capacity)
{
    EXPECT_TRUE(deque.isEmpty());
    EXPECT_EQ(0, deque.size());
    add_n(deque, scale, true);
    EXPECT_EQ(scale, deque.size());
    remove_n(deque, scale, true);
    EXPECT_TRUE(deque.isEmpty());
    add_n(deque, scale, false);
    EXPECT_EQ(scale, deque.size());
    remove_n(deque, scale, false);
    EXPECT_TRUE(deque.isEmpty());
}

TEST_F(TestLinkedDeque, Modifiers)
{
    EXPECT_THROW(deque.dequeue(), std::out_of_range);
    EXPECT_NO_THROW({
        add_n(deque, scale, true);
        remove_n(deque, scale, true);
        add_n(deque, scale, false);
        remove_n(deque, scale, false);
    });
    EXPECT_THROW(deque.dequeue(), std::out_of_range);

    add_n(deque, scale, true);
    deque.clear();
    EXPECT_TRUE(deque.isEmpty());
    EXPECT_EQ(0, deque.size());
    EXPECT_THROW(deque.dequeue(), std::out_of_range);

    add_n(a, scale, true);
    b.swap(a);
    EXPECT_EQ(scale, b.size());
    for (int i = 0; i < scale; ++i)
        EXPECT_EQ(std::to_string(i), b.dequeue());
}

TEST_F(TestLinkedDeque, Other)
{
    using std::swap;
    add_n(a, scale, true);
    c = a;
    EXPECT_TRUE(c == a && c != b);
    b.swap(a);
    EXPECT_TRUE(c != a && c == b);
    swap(a, b);
    EXPECT_TRUE(c == a && c != b);
}
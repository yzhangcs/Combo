#include <iostream>
#include <string>
#include "ArrayDeque.h"
#include "gtest/gtest.h"

using std::string;

class TestArrayDeque : public testing::Test 
{
protected:
    ArrayDeque<string> deque;
    ArrayDeque<string> a;
    ArrayDeque<string> b;
    ArrayDeque<string> c;
    string str;    
    int scale;
public:
    virtual void SetUp() { scale = 32; }
    virtual void TearDown() {}

    void add_n(ArrayDeque<string>& s, int n, bool isForward)
    {
        if (isForward)
        {
            for (int i = 0; i < n; ++i) 
                s.insertBack(std::to_string(i));
        }
        else
        {
            for (int i = 0; i < n; ++i) 
                s.insertFront(std::to_string(i));
        }
    }
    void remove_n(ArrayDeque<string>& s, int n, bool isForward)
    {
        if (isForward)
        {
            for (int i = 0; i < n; ++i) 
                s.removeBack();
        }
        else
        {
            for (int i = 0; i < n; ++i) 
                s.removeFront();
        }
    }
};

TEST_F(TestArrayDeque, Basic)
{
    EXPECT_NO_THROW({
        ArrayDeque<string> s1;
        ArrayDeque<string> s2(s1);
        ArrayDeque<string> s3(30);
        ArrayDeque<string> s4(ArrayDeque<string>());

        s1 = s2;
        s2 = ArrayDeque<string>(15);
    });
}

TEST_F(TestArrayDeque, ElementAccess)
{
    EXPECT_THROW(deque.front(), std::out_of_range);
    EXPECT_THROW(deque.back(), std::out_of_range);
    EXPECT_NO_THROW({
        for (int i = 0; i < scale; ++i)
        {
            str = std::to_string(i);
            deque.insertBack(str);
            EXPECT_EQ(str, deque.back());
        }
        for (int i = 0; i < scale; ++i)
        {
            str = std::to_string(i);
            deque.insertFront(str);
            EXPECT_EQ(str, deque.front());
        }
        for (int i = 0; i < scale; ++i)
        {
            str = deque.front();
            EXPECT_EQ(str, deque.removeFront());
        }
        for (int i = 0; i < scale; ++i)
        {
            str = deque.back();
            EXPECT_EQ(str, deque.removeBack());
        }
    });
    EXPECT_THROW(deque.front(), std::out_of_range);
    EXPECT_THROW(deque.back(), std::out_of_range);
}

TEST_F(TestArrayDeque, Iterators)
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

TEST_F(TestArrayDeque, Capacity)
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

TEST_F(TestArrayDeque, Modifiers)
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

TEST_F(TestArrayDeque, Other)
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
#include <iostream>
#include <string>
#include "Deque.h"
#include "gtest/gtest.h"

using std::string;

class TestDeque : public testing::Test 
{
protected:
    Deque<string> deque;
    Deque<string> a;
    Deque<string> b;
    Deque<string> c;
    string str;    
    int scale;
public:
    virtual void SetUp() { scale = 32; }
    virtual void TearDown() {}

    void insert_n(Deque<string>& s, int n, bool from_back = true)
    {
        if (from_back)
        {
            for (int i = 0; i < n; ++i) 
                s.insert_back(std::to_string(i));
        }
        else
        {
            for (int i = 0; i < n; ++i) 
                s.insert_front(std::to_string(i));
        }
    }
    void remove_n(Deque<string>& s, int n, bool from_back = true)
    {
        if (from_back)
        {
            for (int i = 0; i < n; ++i) 
                s.remove_back();
        }
        else
        {
            for (int i = 0; i < n; ++i) 
                s.remove_front();
        }
    }
};

TEST_F(TestDeque, Basic)
{
    EXPECT_NO_THROW({
        Deque<string> s1;
        Deque<string> s2(s1);
        Deque<string> s3(30);
        Deque<string> s4(Deque<string>());

        s1 = s2;
        s2 = Deque<string>(15);
    });
}

TEST_F(TestDeque, ElementAccess)
{
    EXPECT_THROW(deque.front(), std::out_of_range);
    EXPECT_THROW(deque.back(), std::out_of_range);
    EXPECT_NO_THROW({
        for (int i = 0; i < scale; ++i)
        {
            str = std::to_string(i);
            deque.insert_back(str);
            EXPECT_EQ(str, deque.back());
        }
        for (int i = 0; i < scale; ++i)
        {
            str = std::to_string(i);
            deque.insert_front(str);
            EXPECT_EQ(str, deque.front());
        }
        for (int i = 0; i < scale; ++i)
        {
            str = deque.front();
            EXPECT_EQ(str, deque.remove_front());
        }
        for (int i = 0; i < scale; ++i)
        {
            str = deque.back();
            EXPECT_EQ(str, deque.remove_back());
        }
    });
    EXPECT_THROW(deque.front(), std::out_of_range);
    EXPECT_THROW(deque.back(), std::out_of_range);
}

TEST_F(TestDeque, Iterators)
{
    EXPECT_EQ(deque.begin(), deque.end());
    insert_n(deque, scale);
    EXPECT_NE(deque.begin(), deque.end());
    
    auto bg = deque.begin();
    auto ed = deque.end();

    for (int i = 0; i < scale; ++i)
        EXPECT_EQ(std::to_string(i), *bg++);
    EXPECT_EQ(bg, deque.end());
    for (int i = scale - 1; i >= 0; --i)
        EXPECT_EQ(std::to_string(i), *--ed);
    EXPECT_EQ(ed, deque.begin());
}

TEST_F(TestDeque, Capacity)
{
    EXPECT_TRUE(deque.empty());
    EXPECT_EQ(0, deque.size());
    EXPECT_EQ(10, deque.capacity());

    insert_n(deque, scale, true);
    EXPECT_EQ(scale, deque.size());
    EXPECT_LE(scale, deque.capacity());
    EXPECT_GE(scale, deque.capacity() / 4);
    remove_n(deque, scale, true);
    EXPECT_TRUE(deque.empty());
    
    insert_n(deque, scale, false);
    EXPECT_EQ(scale, deque.size());
    EXPECT_LE(scale, deque.capacity());
    EXPECT_GE(scale, deque.capacity() / 4);
    remove_n(deque, scale, false);
    EXPECT_TRUE(deque.empty());
}

TEST_F(TestDeque, Modifiers)
{
    EXPECT_THROW(deque.remove_back(), std::out_of_range);
    EXPECT_THROW(deque.remove_front(), std::out_of_range);
    EXPECT_NO_THROW({
        insert_n(deque, scale, true);
        for (int i = scale - 1; i >= 0; --i)
            EXPECT_EQ(std::to_string(i), deque.remove_back());
        insert_n(deque, scale, false);
        for (int i = scale - 1; i >= 0; --i)
            EXPECT_EQ(std::to_string(i), deque.remove_front());
    });
    EXPECT_THROW(deque.remove_back(), std::out_of_range);
    EXPECT_THROW(deque.remove_front(), std::out_of_range);

    insert_n(deque, scale);
    deque.clear();
    EXPECT_TRUE(deque.empty());
    EXPECT_EQ(0, deque.size());
    EXPECT_THROW(deque.remove_back(), std::out_of_range);

    insert_n(a, scale);
    b.swap(a);
    EXPECT_EQ(scale, b.size());
    for (int i = 0; i < scale; ++i)
        EXPECT_EQ(std::to_string(i), b.remove_front());
}

TEST_F(TestDeque, Other)
{
    using std::swap;
    insert_n(a, scale);
    c = a;
    EXPECT_TRUE(c == a && c != b);
    b.swap(a);
    EXPECT_TRUE(c != a && c == b);
    swap(a, b);
    EXPECT_TRUE(c == a && c != b);
}
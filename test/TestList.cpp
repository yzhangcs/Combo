#include <iostream>
#include <string>
#include "List.h"
#include "gtest/gtest.h"

using std::string;

class TestList : public testing::Test 
{
protected:
    List<string> list;
    List<string> a;
    List<string> b;
    List<string> c;
    string str;    
    int scale;
public:
    virtual void SetUp() { scale = 32; }
    virtual void TearDown() {}

    void insert_n(List<string>& s, int n, bool from_back = true)
    {
        if (from_back)
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
    void remove_n(List<string>& s, int n, bool from_back = true)
    {
        if (from_back)
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

TEST_F(TestList, Basic)
{
    EXPECT_NO_THROW({
        List<string> s1;
        List<string> s2(s1);
        List<string> s3(List<string>());

        s1 = s2;
        s2 = List<string>();
    });
}

TEST_F(TestList, ElementAccess)
{
    EXPECT_THROW(list.front(), std::out_of_range);
    EXPECT_THROW(list.back(), std::out_of_range);
    EXPECT_NO_THROW({
        for (int i = 0; i < scale; ++i)
        {
            str = std::to_string(i);
            list.insertBack(str);
            EXPECT_EQ(str, list.back());
        }
        for (int i = 0; i < scale; ++i)
        {
            str = std::to_string(i);
            list.insertFront(str);
            EXPECT_EQ(str, list.front());
        }
        for (int i = 0; i < scale; ++i)
        {
            str = list.front();
            EXPECT_EQ(str, list.removeFront());
        }
        for (int i = 0; i < scale; ++i)
        {
            str = list.back();
            EXPECT_EQ(str, list.removeBack());
        }
    });
    EXPECT_THROW(list.front(), std::out_of_range);
    EXPECT_THROW(list.back(), std::out_of_range);
}

TEST_F(TestList, Iterators)
{
    EXPECT_EQ(list.begin(), list.end());
    insert_n(list, scale);
    EXPECT_NE(list.begin(), list.end());
    
    auto bg = list.begin();
    auto ed = list.end();

    for (int i = 0; i < scale; ++i)
        EXPECT_EQ(std::to_string(i), *bg++);
    EXPECT_EQ(bg, list.end());
    for (int i = scale - 1; i >= 0; --i)
        EXPECT_EQ(std::to_string(i), *--ed);
    EXPECT_EQ(ed, list.begin());
}

TEST_F(TestList, Capacity)
{
    EXPECT_TRUE(list.empty());
    EXPECT_EQ(0, list.size());

    insert_n(list, scale, true);
    EXPECT_EQ(scale, list.size());
    remove_n(list, scale, true);
    EXPECT_TRUE(list.empty());
    
    insert_n(list, scale, false);
    EXPECT_EQ(scale, list.size());
    remove_n(list, scale, false);
    EXPECT_TRUE(list.empty());
}

TEST_F(TestList, Modifiers)
{
    EXPECT_THROW(list.removeBack(), std::out_of_range);
    EXPECT_THROW(list.removeFront(), std::out_of_range);
    EXPECT_NO_THROW({
        insert_n(list, scale, true);
        for (int i = scale - 1; i >= 0; --i)
            EXPECT_EQ(std::to_string(i), list.removeBack());
        insert_n(list, scale, false);
        for (int i = scale - 1; i >= 0; --i)
            EXPECT_EQ(std::to_string(i), list.removeFront());

        for (int i = 0; i < scale; ++i)
            list.insert(0, std::to_string(i));
        for (int i = scale - 1; i >= 0; --i)
            EXPECT_EQ(std::to_string(i), list.remove(0));
        for (int i = 0; i < scale; ++i)
            list.insert(i, std::to_string(i));
        for (int i = scale - 1; i >= 0; --i)
            EXPECT_EQ(std::to_string(i), list.remove(i));
    });
    EXPECT_THROW(list.removeBack(), std::out_of_range);
    EXPECT_THROW(list.removeFront(), std::out_of_range);

    insert_n(list, scale);
    list.clear();
    EXPECT_TRUE(list.empty());
    EXPECT_EQ(0, list.size());
    EXPECT_THROW(list.removeBack(), std::out_of_range);

    insert_n(list, scale);
    a = a + list;
    b += list;
    for (int i = 0; i < scale; ++i)
        EXPECT_EQ(a.removeBack(), b.removeBack());
    
    c.swap(list);
    EXPECT_EQ(scale, c.size());
    for (int i = scale - 1; i >= 0; --i)
        EXPECT_EQ(std::to_string(i), c.removeBack());
}

TEST_F(TestList, Other)
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
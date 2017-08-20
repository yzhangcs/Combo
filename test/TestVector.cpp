#include <iostream>
#include <string>
#include "Vector.h"
#include "gtest/gtest.h"

using std::string;

class TestVector : public testing::Test 
{
protected:
    Vector<string> vector;
    Vector<string> a;
    Vector<string> b;
    Vector<string> c;
    string str;    
    int scale;
public:
    virtual void SetUp() { scale = 32; }
    virtual void TearDown() {}

    void insert_n(Vector<string>& s, int n)
    {
        for (int i = 0; i < n; ++i) 
            s.insert_back(std::to_string(i));
    }
    void remove_n(Vector<string>& s, int n)
    {
        for (int i = 0; i < n; ++i) 
            s.remove_back();
    }
};

TEST_F(TestVector, Basic)
{
    EXPECT_NO_THROW({
        Vector<string> s1;
        Vector<string> s2(s1);
        Vector<string> s3(30);
        Vector<string> s4(Vector<string>());

        s1 = s2;
        s2 = Vector<string>(15);
    });
}

TEST_F(TestVector, ElementAccess)
{
    EXPECT_THROW(vector.front(), std::out_of_range);
    EXPECT_THROW(vector.back(), std::out_of_range);
    EXPECT_NO_THROW({
        for (int i = 0; i < scale; ++i)
        {
            str = std::to_string(i);
            vector.insert_back(str);
            EXPECT_EQ(str, vector.back());
        }
        EXPECT_EQ(std::to_string(0), vector.front());
        for (int i = 0; i < scale; ++i)
        {
            str = vector.back();
            EXPECT_EQ(str, vector.remove_back());
        }
    });
    EXPECT_THROW(vector.front(), std::out_of_range);
    EXPECT_THROW(vector.back(), std::out_of_range);

    insert_n(vector, scale);
    for (int i = 0; i < scale; ++i)
        EXPECT_EQ(std::to_string(i), vector.at(i)); 
    for (int i = 0; i < scale; ++i)
        EXPECT_EQ(std::to_string(i), vector[i]);
    EXPECT_THROW(vector.at(scale), std::out_of_range);
}

TEST_F(TestVector, Iterators)
{
    EXPECT_EQ(vector.begin(), vector.end());
    insert_n(vector, scale);
    EXPECT_LT(vector.begin(), vector.end());
    
    auto bg = vector.begin();
    auto ed = vector.end();

    for (int i = 0; i < scale; ++i)
        EXPECT_EQ(std::to_string(i), bg[i]);
    for (int i = 0; i < scale; ++i)
        EXPECT_EQ(std::to_string(i), *(bg + i));
    for (int i = 0; i < scale; ++i)
        EXPECT_EQ(std::to_string(i), *(ed - scale + i));  
    
    for (int i = 0; i < scale; ++i)
    {
        auto it = vector.begin();
        it += i;
        EXPECT_EQ(std::to_string(i), *it);
    }
    for (int i = 0; i < scale; ++i)
    {
        auto it = vector.end();
        it -= (scale - i);
        EXPECT_EQ(std::to_string(i), *it);
    }
    for (int i = 0; i < scale; ++i)
    {
        auto it = bg + i;
        EXPECT_EQ(i, it - bg);
    }        

    for (int i = 0; i < scale; ++i)
        EXPECT_EQ(std::to_string(i), *bg++);
    EXPECT_EQ(bg, vector.end());
    for (int i = scale - 1; i >= 0; --i)
        EXPECT_EQ(std::to_string(i), *--ed);
    EXPECT_EQ(ed, vector.begin());

}

TEST_F(TestVector, Capacity)
{
    EXPECT_TRUE(vector.empty());
    EXPECT_EQ(0, vector.size());
    EXPECT_EQ(10, vector.capacity());

    insert_n(vector, scale);
    EXPECT_EQ(scale, vector.size());
    EXPECT_LE(scale, vector.capacity());
    EXPECT_GE(scale, vector.capacity() / 4);

    remove_n(vector, scale);
    EXPECT_TRUE(vector.empty());
}

TEST_F(TestVector, Modifiers)
{
    EXPECT_THROW(vector.remove_back(), std::out_of_range);
    EXPECT_NO_THROW({
        insert_n(vector, scale);
        for (int i = scale - 1; i >= 0; --i)
            EXPECT_EQ(std::to_string(i), vector.remove_back());
        
        for (int i = 0; i < scale; ++i)
            vector.insert(0, std::to_string(i));
        for (int i = scale - 1; i >= 0; --i)
            EXPECT_EQ(std::to_string(i), vector.remove(0));
        for (int i = 0; i < scale; ++i)
            vector.insert(i, std::to_string(i));
        for (int i = scale - 1; i >= 0; --i)
            EXPECT_EQ(std::to_string(i), vector.remove(i));
    });
    EXPECT_THROW(vector.remove_back(), std::out_of_range);

    insert_n(a, scale);
    b = a + c;
    c += b;
    for (int i = 0; i < scale; ++i)
        EXPECT_EQ(b.remove_back(), c.remove_back());

    insert_n(vector, scale);
    vector.clear();
    EXPECT_TRUE(vector.empty());
    EXPECT_EQ(0, vector.size());
    EXPECT_THROW(vector.remove_back(), std::out_of_range);

    insert_n(vector, scale);
    a = a + vector;
    b += vector;
    for (int i = 0; i < scale; ++i)
        EXPECT_EQ(a.remove_back(), b.remove_back());
    
    c.swap(vector);
    EXPECT_EQ(scale, c.size());
    for (int i = scale - 1; i >= 0; --i)
        EXPECT_EQ(std::to_string(i), c.remove_back());
}

TEST_F(TestVector, Other)
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
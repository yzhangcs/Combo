#include <iostream>
#include <string>
#include "ArrayStack.h"
#include "gtest/gtest.h"

using std::string;

class TestArrayStack : public testing::Test 
{
protected:
    ArrayStack<string> stack;
    ArrayStack<string> a;
    ArrayStack<string> b;
    ArrayStack<string> c;
    string str;    
public:
    virtual void SetUp() {}
    virtual void TearDown() {}
};

TEST_F(TestArrayStack, Basic)
{
    EXPECT_NO_THROW({
        ArrayStack<string> s1;
        ArrayStack<string> s2(30);
        ArrayStack<string> s3(a);
        ArrayStack<string> s4(ArrayStack<string>());

        s1 = s2;
        s2 = ArrayStack<string>(15);
    });
}

TEST_F(TestArrayStack, ElementAccess)
{
    EXPECT_THROW(stack.top(), std::out_of_range);
    EXPECT_NO_THROW({
        for (int i = 0; i < 32; ++i)
        {
            str = std::to_string(i);
            stack.push(str);
            EXPECT_EQ(str, stack.top());
        }
        for (int i = 0; i < 32; ++i)
        {
            str = stack.top();
            EXPECT_EQ(str, stack.pop());
        }
    });
    EXPECT_THROW(stack.top(), std::out_of_range);
}

TEST_F(TestArrayStack, Iterators)
{
    auto it = stack.begin();

    EXPECT_EQ(it, stack.end());
    for (int i = 0; i < 32; ++i)
        stack.push(std::to_string(i));  
    EXPECT_NE(it, stack.end());  
    for (int i = 0; i < 32; ++i)
        EXPECT_EQ(std::to_string(i), *(it++));
    EXPECT_EQ(it, stack.end());
}

TEST_F(TestArrayStack, Capacity)
{
    EXPECT_TRUE(stack.isEmpty());
    EXPECT_EQ(0, stack.size());
    for (int i = 0; i < 32; ++i)
    { 
        EXPECT_EQ(i, stack.size());
        stack.push(std::to_string(i)); 
    }
    for (int i = 0; i < 32; ++i)
        stack.pop();
    EXPECT_TRUE(stack.isEmpty());
}

TEST_F(TestArrayStack, Modifiers)
{
    EXPECT_THROW(stack.pop(), std::out_of_range);
    EXPECT_NO_THROW({
        for (int i = 0; i < 32; ++i)
            stack.push(std::to_string(i));
        for (int i = 0; i < 32; ++i)
            stack.pop();
    });
    EXPECT_THROW(stack.pop(), std::out_of_range);
    stack.push(std::to_string(0));
    stack.clear();
    EXPECT_TRUE(stack.isEmpty());
    EXPECT_EQ(0, stack.size());
    EXPECT_THROW(stack.pop(), std::out_of_range);
    
    auto it = b.begin();

    for (int i = 0; i < 32; ++i)
        a.push(std::to_string(i));
    b.swap(a);
    EXPECT_EQ(32, b.size());
    for (int i = 0; i < 32; ++i)
        EXPECT_EQ(std::to_string(i), *(it++));
}

TEST_F(TestArrayStack, Other)
{
    using std::swap;
    for (int i = 0; i < 32; ++i)
        a.push(std::to_string(i));
    c = a;
    EXPECT_TRUE(c == a && c != b);
    b.swap(a);
    EXPECT_TRUE(c != a && c == b);
    swap(a, b);
    EXPECT_TRUE(c == a && c != b);
}
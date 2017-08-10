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
    int scale;
public:
    virtual void SetUp() { scale = 32; }
    virtual void TearDown() {}

    void push_n(ArrayStack<string>& s, int n)
    {
        for (int i = 0; i < n; ++i)
            s.push(std::to_string(i));
    }
    void pop_n(ArrayStack<string>& s, int n)
    {
        for (int i = 0; i < n; ++i)
            s.pop();
    }
};

TEST_F(TestArrayStack, Basic)
{
    EXPECT_NO_THROW({
        ArrayStack<string> s1;
        ArrayStack<string> s2(s1);
        ArrayStack<string> s3(30);
        ArrayStack<string> s4(ArrayStack<string>());

        s1 = s2;
        s2 = ArrayStack<string>(15);
    });
}

TEST_F(TestArrayStack, ElementAccess)
{
    EXPECT_THROW(stack.top(), std::out_of_range);
    EXPECT_NO_THROW({
        for (int i = 0; i < scale; ++i)
        {
            str = std::to_string(i);
            stack.push(str);
            EXPECT_EQ(str, stack.top());
        }
        for (int i = 0; i < scale; ++i)
        {
            str = stack.top();
            EXPECT_EQ(str, stack.pop());
        }
    });
    EXPECT_THROW(stack.top(), std::out_of_range);
}

TEST_F(TestArrayStack, Iterators)
{
    EXPECT_EQ(stack.begin(), stack.end());
    push_n(stack, scale);
    EXPECT_NE(stack.begin(), stack.end());  

    auto it = stack.begin();

    for (int i = 0; i < scale; ++i)
        EXPECT_EQ(std::to_string(i), *(it++));
    EXPECT_EQ(it, stack.end());
}

TEST_F(TestArrayStack, Capacity)
{
    EXPECT_TRUE(stack.isEmpty());
    EXPECT_EQ(0, stack.size());
    push_n(stack, scale);
    EXPECT_EQ(scale, stack.size());
    pop_n(stack, scale);
    EXPECT_TRUE(stack.isEmpty());
}

TEST_F(TestArrayStack, Modifiers)
{
    EXPECT_THROW(stack.pop(), std::out_of_range);
    EXPECT_NO_THROW({
        push_n(stack, scale);
        pop_n(stack, scale);
    });
    EXPECT_THROW(stack.pop(), std::out_of_range);

    push_n(stack, scale);
    stack.clear();
    EXPECT_TRUE(stack.isEmpty());
    EXPECT_EQ(0, stack.size());
    EXPECT_THROW(stack.pop(), std::out_of_range);

    push_n(a, scale);
    b.swap(a);
    EXPECT_EQ(scale, b.size());
    for (int i = scale; i > 0; --i)
        EXPECT_EQ(std::to_string(i - 1), b.pop());
}

TEST_F(TestArrayStack, Other)
{
    using std::swap;
    push_n(a, scale);
    c = a;
    EXPECT_TRUE(c == a && c != b);
    b.swap(a);
    EXPECT_TRUE(c != a && c == b);
    swap(a, b);
    EXPECT_TRUE(c == a && c != b);
}
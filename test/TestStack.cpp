#include <iostream>
#include <string>
#include "Stack.h"
#include "gtest/gtest.h"

using std::string;

class TestStack : public testing::Test 
{
protected:
    Stack<string> stack;
    Stack<string> a;
    Stack<string> b;
    Stack<string> c;
    string str;    
    int scale;
public:
    virtual void SetUp() { scale = 32; }
    virtual void TearDown() {}

    void push_n(Stack<string>& s, int n)
    {
        for (int i = 0; i < n; ++i)
            s.push(std::to_string(i));
    }
    void pop_n(Stack<string>& s, int n)
    {
        for (int i = 0; i < n; ++i)
            s.pop();
    }
};

TEST_F(TestStack, Basic)
{
    EXPECT_NO_THROW({
        Stack<string> s1;
        Stack<string> s2(s1);
        Stack<string> s3(Stack<string>());

        s1 = s2;
        s2 = Stack<string>();
    });
}

TEST_F(TestStack, ElementAccess)
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

TEST_F(TestStack, Capacity)
{
    EXPECT_TRUE(stack.empty());
    EXPECT_EQ(0, stack.size());
    
    push_n(stack, scale);
    EXPECT_EQ(scale, stack.size());
    pop_n(stack, scale);
    EXPECT_TRUE(stack.empty());
}

TEST_F(TestStack, Modifiers)
{
    EXPECT_THROW(stack.pop(), std::out_of_range);
    EXPECT_NO_THROW({
        push_n(stack, scale);
        for (int i = scale - 1; i >= 0; --i)
            EXPECT_EQ(std::to_string(i), stack.pop());
    });
    EXPECT_THROW(stack.pop(), std::out_of_range);

    push_n(stack, scale);
    stack.clear();
    EXPECT_TRUE(stack.empty());
    EXPECT_EQ(0, stack.size());
    EXPECT_THROW(stack.pop(), std::out_of_range);

    push_n(a, scale);
    b.swap(a);
    EXPECT_EQ(scale, b.size());
    for (int i = scale - 1; i >= 0; --i)
        EXPECT_EQ(std::to_string(i), b.pop());
}

TEST_F(TestStack, Other)
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
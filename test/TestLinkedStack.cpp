#include <iostream>
#include <string>
#include "LinkedStack.h"
#include "gtest/gtest.h"

using std::string;

class TestLinkedStack : public testing::Test 
{
protected:
    LinkedStack<string> stack;
    LinkedStack<string> a;
    LinkedStack<string> b;
    LinkedStack<string> c;
    string str;    
    int scale;
public:
    virtual void SetUp() { scale = 32; }
    virtual void TearDown() {}

    void push_n(LinkedStack<string>& s, int n)
    {
        for (int i = 0; i < n; ++i)
            s.push(std::to_string(i));
    }
    void pop_n(LinkedStack<string>& s, int n)
    {
        for (int i = 0; i < n; ++i)
            s.pop();
    }
};

TEST_F(TestLinkedStack, Basic)
{
    EXPECT_NO_THROW({
        LinkedStack<string> s1;
        LinkedStack<string> s2(s1);
        LinkedStack<string> s3(LinkedStack<string>());

        s1 = s2;
        s2 = LinkedStack<string>();
    });
}

TEST_F(TestLinkedStack, ElementAccess)
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

TEST_F(TestLinkedStack, Iterators)
{
    EXPECT_EQ(stack.begin(), stack.end());
    push_n(stack, scale);
    EXPECT_NE(stack.begin(), stack.end());  
    
    auto it = stack.begin();

    for (int i = 0; i < scale; ++i)
        EXPECT_EQ(std::to_string(i), *(it++));
    EXPECT_EQ(it, stack.end());
}

TEST_F(TestLinkedStack, Capacity)
{
    EXPECT_TRUE(stack.isEmpty());
    EXPECT_EQ(0, stack.size());
    push_n(stack, scale);
    EXPECT_EQ(scale, stack.size());
    pop_n(stack, scale);
    EXPECT_TRUE(stack.isEmpty());
}

TEST_F(TestLinkedStack, Modifiers)
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

TEST_F(TestLinkedStack, Other)
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
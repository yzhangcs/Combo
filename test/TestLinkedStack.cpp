#include <iostream>
#include <string>
#include "LinkedStack.h"
#include "gtest/gtest.h"

using std::string;

TEST(TestLinkedStack, Push)
{
    LinkedStack<string> stack;

    EXPECT_TRUE(stack.isEmpty());
    for (int i = 0; i < 32; ++i)
    {
        stack.push(std::to_string(i));
        EXPECT_EQ(std::to_string(i), stack.top());
    }
    EXPECT_EQ(32, stack.size());
}

TEST(TestLinkedStack, Pop)
{
    LinkedStack<string> stack;
    string str;

    for (int i = 0; i < 32; ++i)
        stack.push(std::to_string(i));
    EXPECT_EQ(32, stack.size());
    EXPECT_NO_THROW({
        for (int i = 0; i < 32; ++i)
        {
            str = stack.top();
            EXPECT_EQ(str, stack.pop());
        }
    });
    EXPECT_TRUE(stack.isEmpty());
    EXPECT_THROW(stack.pop(), std::out_of_range);
    EXPECT_THROW(stack.top(), std::out_of_range);
}

TEST(TestLinkedStack, Other)
{
    LinkedStack<string> a;
    LinkedStack<string> b;
    LinkedStack<string> c;
    string str;

    for (int i = 0; i < 10; ++i)
        a.push(std::to_string(i));
    b = a;
    EXPECT_TRUE(a == b);
    a.clear();
    EXPECT_TRUE(a.isEmpty());
    EXPECT_THROW(a.pop(), std::out_of_range);
    c = b;
    b.pop();
    a = b;
    EXPECT_TRUE(b != c);
    b.swap(c);
    EXPECT_TRUE(a == c);
    std::cout << "stack a: " << a << std::endl;  
    std::cout << "stack b: ";
    for (auto i : b) std::cout << i << " ";
    std::cout << std::endl;
    std::cout << "stack c: ";
    for (auto i : c) std::cout << i << " ";
    std::cout << std::endl;
}
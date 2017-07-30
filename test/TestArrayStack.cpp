#include <string>
#include "ArrayStack.h"
#include "gtest/gtest.h"

using std::string;

// To use a test fixture, derive a class from testing::Test.
class TestArrayStack : public testing::Test 
{
protected:  
    ArrayStack<string> stack;

    virtual void SetUp() {}
    virtual void TearDown() {}
};

TEST_F(TestArrayStack, Push)
{
    ASSERT_TRUE(stack.isEmpty());
    for (int i = 0; i < 32; ++i)
    {
        stack.push(std::to_string(i + 100));
        ASSERT_EQ(std::to_string(i + 100), stack.top());
    }
    ASSERT_EQ(32, stack.size());
}

TEST_F(TestArrayStack, Pop)
{
    ASSERT_NO_THROW({
        string str;

        for (int i = 0; i < stack.size(); ++i)
        {
            str = stack.top();
            ASSERT_EQ(str, stack.pop());
        }
    });
    ASSERT_TRUE(stack.isEmpty());
    ASSERT_THROW(stack.pop(), std::out_of_range);
}
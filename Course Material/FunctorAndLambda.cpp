#include "pch.h"

#include <algorithm>
#include <vector>

// Functor for sorting strings in reverse order
struct ReverseStringSort 
{
    bool operator()(const std::string& a, const std::string& b) const {
        return a > b;
    }
};

// Test sorting a vector of strings using the ReverseStringSort functor
TEST(sort_test, reverse_string_sort_functor)
{
    std::vector<std::string> v = { "apple", "banana", "cherry", "date" };
    std::sort(v.begin(), v.end(), ReverseStringSort());

    // Check that the vector is sorted in reverse alphabetical order
    EXPECT_EQ(v, (std::vector<std::string>{"date", "cherry", "banana", "apple"}));
}

// Test sorting a vector of strings using a lambda to sort in reverse order
TEST(sort_test, reverse_string_sort_lambda) 
{
    std::vector<std::string> v = { "apple", "banana", "cherry", "date" };
    std::sort(v.begin(), v.end(), [](const std::string& a, const std::string& b)
        {        
            return a > b;
        }
    );

    // Check that the vector is sorted in reverse alphabetical order
    EXPECT_EQ(v, (std::vector<std::string>{"date", "cherry", "banana", "apple"}));
}

TEST(closure, capture_by_value)
{
    int x = 5;
    auto f = [x]() {
        return x;
    };
    x = 10;
    int result = f();

    // The value of x is captured by value, so the value of x in the closure
    // is 5, even though x is changed to 10 after the closure is created.
    EXPECT_EQ(result, 5);
}
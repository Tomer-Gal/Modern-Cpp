#include "pch.h"

#include <algorithm>
#include <numeric>

TEST(stl_algorithms, sort_test)
{
	std::vector<int> v = { 5, 4, 3, 2, 1 };
	std::sort(v.begin(), v.end());

	// Check that the vector is sorted in ascending order
	EXPECT_EQ(v, (std::vector<int>{1, 2, 3, 4, 5}));
}

// Test transforming a vector of integers using std::transform
TEST(stl_algorithms, transform_test) 
{
    std::vector<int> v = { 1, 2, 3, 4, 5 };
    std::vector<int> v2(v.size());
    std::transform(v.begin(), v.end(), v2.begin(), [](int x) {
        return x * x;
        });

    // Check that the transformed vector has the correct values
    EXPECT_EQ(v2, (std::vector<int>{1, 4, 9, 16, 25}));
}

// Test applying a function to each element of a vector using std::for_each
TEST(stl_algorithms, for_each_test)
{
    std::vector<int> v = { 1, 2, 3, 4, 5 };
    int sum = 0;
    std::for_each(v.begin(), v.end(), [&sum](int x) {
        sum += x;
        });

    // Check that the sum of the vector is correct
    EXPECT_EQ(sum, 15);
}

// Test computing the sum of a vector of integers using std::accumulate
TEST(stl_algorithms, accumulate_test) 
{
    std::vector<int> v = { 1, 2, 3, 4, 5 };
    int sum = std::accumulate(v.begin(), v.end(), 0);

    // Check that the sum of the vector is correct
    EXPECT_EQ(sum, 15);
}

// Test finding an element in a vector of integers using std::find
TEST(stl_algorithms, find_test)
{
    std::vector<int> v = { 1, 2, 9, 4, 5 };
    auto it = std::find(v.begin(), v.end(), 9);

    // Check that the iterator points to the correct element
    EXPECT_EQ(*it, 9);
}

// Test copying elements from one vector to another using std::copy
TEST(stl_algorithms, copy_test)
{
    std::vector<int> v = { 1, 2, 3, 4, 5 };
    std::vector<int> v2(v.size());
    std::copy(v.begin(), v.end(), v2.begin());

    // Check that the copied vector has the correct values
    EXPECT_EQ(v2, v);
}

// Test removing consecutive duplicates from a vector of integers using std::unique
TEST(stl_algorithms, unique_test)
{
    // Create a vector with consecutive duplicates
    std::vector<int> v = { 1, 2, 2, 3, 3, 3, 4, 5, 5 };
    
    // Remove the duplicates
    auto it = std::unique(v.begin(), v.end());

    // Erase the remaining elements
    v.erase(it, v.end());

    // Check that the vector has no consecutive duplicates
    EXPECT_EQ(v, (std::vector<int>{1, 2, 3, 4, 5}));
}

// Test counting the occurrences of an element in a vector of integers using std::count
TEST(stl_algorithms, count_test)
{
    std::vector<int> v = { 1, 2, 2, 3, 3, 3, 4, 5, 5 };
    int count = std::count(v.begin(), v.end(), 3);

    // Check that the count of the element is correct
    EXPECT_EQ(count, 3);
}

// Test reversing the order of elements in a vector of integers using std::reverse
TEST(stl_algorithms, reverse_test)
{
    std::vector<int> v = { 1, 2, 3, 4, 5 };
    std::reverse(v.begin(), v.end());

    // Check that the vector is reversed
    EXPECT_EQ(v, (std::vector<int>{5, 4, 3, 2, 1}));
}

// Test partitioning a vector of integers using a predicate with std::partition
TEST(stl_algorithms, partition_test)
{
    std::vector<int> v = { 1, 2, 3, 4, 5 };
    auto it = std::partition(v.begin(), v.end(), [](int x) {
        return x % 2 == 0;
        });

    // Check that all even elements come before odd elements
    ASSERT_TRUE(std::all_of(v.begin(), it, [](int x) { return x % 2 == 0; }));
    ASSERT_TRUE(std::all_of(it, v.end(), [](int x) { return x % 2 != 0; }));

    // Check that the iterator points to the correct element (the first odd element)
    ASSERT_EQ(std::distance(v.begin(), it), 2);
}

// Test finding the first mismatched pair of elements in two vectors using std::mismatch
TEST(stl_algorithms, mismatch_test) 
{
    std::vector<int> v1 = { 1, 2, 3, 4, 5 };
    std::vector<int> v2 = { 1, 2, 4, 4, 5 };
    auto it = std::mismatch(v1.begin(), v1.end(), v2.begin());

    // Check that the mismatched pair of elements is correct
    EXPECT_EQ(*it.first, 3);
    EXPECT_EQ(*it.second, 4);
}

// Test checking whether all elements in a vector of integers satisfy a predicate using std::all_of
TEST(stl_algorithms, all_of_test)
{
    std::vector<int> v = { 2, 4, 6, 8, 10 };
    bool result = std::all_of(v.begin(), v.end(), [](int x) {
        return x % 2 == 0;
        });

    // Check that all elements satisfy the predicate
    EXPECT_TRUE(result);
}

// Test checking whether any elements in a vector of integers satisfy a predicate using std::any_of
TEST(stl_algorithms, any_of_test)
{
    std::vector<int> v = { 1, 2, 3, 4, 5 };
    bool result = std::any_of(v.begin(), v.end(), [](int x) 
        {
            return x % 2 == 0;
        });

    // Check that at least one element satisfies the predicate
    EXPECT_TRUE(result);
}

// Test checking whether no elements in a vector of integers satisfy a predicate using std::none_of
TEST(stl_algorithms, none_of_test)
{
    std::vector<int> v = { 1, 3, 5, 7, 9 };
    bool result = std::none_of(v.begin(), v.end(), [](int x) 
        {
            return x % 2 == 0;
        });

    // Check that no elements satisfy the predicate
    EXPECT_TRUE(result);
}
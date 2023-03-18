#include "pch.h"
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <functional>
#include <numeric>
#include <memory>
#include <queue>
#include <condition_variable>
#include <future>
#include <Windows.h>

TEST(async, launch_policy)
{
    std::cout << "This thread id: " << std::this_thread::get_id() << std::endl;

    auto func = []()->std::thread::id 
    {
        return std::this_thread::get_id(); 
    };

    std::future<std::thread::id> policy_async = std::async(std::launch::async, func);
    std::cout << "std::launch::async id: " << policy_async.get() << std::endl;

    std::future<std::thread::id> policy_deferred = std::async(std::launch::deferred, func);
    std::cout << "std::launch::deferred id: " << policy_deferred.get() << std::endl;

    constexpr int num_threads{ 3 };
    std::vector<std::future<std::thread::id>> policy_deferred_vector(num_threads);
    for (auto& future : policy_deferred_vector)
    {
        future = std::async(std::launch::deferred, func);
    }
    for (auto& future : policy_deferred_vector)
    {
        std::cout << "multiple calls - std::launch::deferred id: " << future.get() << std::endl;
    }
}

#include <string>
#include <future>

struct X
{
    void foo(int add) 
    {
        int init_value = value;
        value += add;
        std::cout << "value was updated from " << init_value << " to " << value << std::endl;
    }

    int value{};
};

TEST(async, passing_arguments)
{
    X x;
    auto future = std::async(&X::foo, x, 37);
    future.get();
    ASSERT_EQ(0, x.value);
}

TEST(async, passing_arguments_by_reference)
{
    X x;
    auto future = std::async(&X::foo, &x, 37);
    future.get();
    ASSERT_EQ(37, x.value);
}
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

void fetch_data_from_database(std::promise<std::string>& promise)
{
    // Simulate some work to fetch data from a database
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::string data = "Some important data";

    // Set the result on the promise
    promise.set_value(data);
}

TEST(promise, fetch_data_from_database)
{
    // Create a promise and a future that will retrieve the result
    std::promise<std::string> promise;
    std::future<std::string> future = promise.get_future();

    // Launch a new thread to fetch data from a database and set the result on the promise
    std::thread worker(fetch_data_from_database, std::ref(promise));

    // Do some other work while waiting for the result
    std::cout << "Fetching data from database..." << std::endl;

    // Wait for the result to be ready and retrieve it from the future
    std::string data = future.get();

    // Assert that the data retrieved from the database is correct
    ASSERT_EQ(data, "Some important data");

    // Wait for the worker thread to finish and join it
    worker.join();
}

TEST(promise, work_throws_exception)
{
    auto do_some_work = [](std::promise<int>& p) -> int
    {
        try {
            // ... do some work that throws an exception ...
            throw std::runtime_error("An error occurred");
        }
        catch (...) {
            // If an exception is thrown, set it on the promise
            p.set_exception(std::current_exception());
        }
    };
    // Create a promise and a future that will retrieve the result
    std::promise<int> p;
    std::future<int> f = p.get_future();

    // Launch a new thread to perform the work and set the result or exception on the promise
    std::thread worker(do_some_work, std::ref(p));

    // Do some other work while waiting for the result
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    // Wait for the result to be ready and retrieve it from the future
    ASSERT_THROW(f.get(), std::runtime_error);

    // Wait for the worker thread to finish and join it
    worker.join();
}

TEST(threads, show_that_no_busy_wait_occurs)
{
    std::mutex m;
    auto acquire_lock_func = [&]()->void
    {
        {
            std::cout << std::this_thread::get_id() << " try to get the lock\n";
            std::lock_guard<std::mutex> guard(m);
            std::cout << std::this_thread::get_id() << " acquires the lock\n";
            std::this_thread::sleep_for(std::chrono::seconds(10));
        }
        std::cout << std::this_thread::get_id() << " released the lock\n";
    };
    std::thread first_acquire_thread(acquire_lock_func);

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    constexpr int NUM_THREADS = 20;
    std::vector<std::thread> threads(NUM_THREADS);
    for (auto& t : threads)
    {
        t = std::thread(acquire_lock_func);
    }
    first_acquire_thread.join();
    for (auto& t : threads)
    {
        t.join();
    }
}

TEST(threads, dead_lock)
{
    std::mutex mutex1;
    std::mutex mutex2;

    std::thread a([&]()->void
        {
            std::cout << std::this_thread::get_id() << " of a\n";
            while (true)
            {
                {
                    std::lock_guard<std::mutex> guard1(mutex1);
                    std::lock_guard<std::mutex> guard2(mutex2);
                    std::cout << "a";
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
        }
    );
    std::thread b([&]()->void
        {
            std::cout << std::this_thread::get_id() << " of b\n";
            while (true)
            {
                {
                    std::lock_guard<std::mutex> guard2(mutex2);
                    std::lock_guard<std::mutex> guard1(mutex1);
                    std::cout << "b";
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
        }
    );
    a.join();
    b.join();
}

//std::mutex mutex;
//void increment(int& value)
//{
//    for (int i = 0; i < 1'000'000; i++)
//    {
//        std::lock_guard<std::mutex> guard(mutex);
//    }
//};
//
//TEST(threads, pass_by_reference)
//{
//    int counter{};
//    std::thread t(increment, std::ref(counter));
//    t.join();
//    std::cout << "Counter value is: " << counter << std::endl;
//}

double square_root(double x)
{
    if (x < 0)
    {
        std::cout << "Will throw an exception\n";
        throw std::out_of_range("x < 0");
    }
    return sqrt(x);
}

TEST(future, throw_exception)
{
    auto future_result = std::async(square_root, -1.0);
    Sleep(1000);
    std::cout << "Will now read the future_result\n";
    future_result.get();
}

TEST(future, shared_future)
{
    auto func = []()->int {return 5; };
    std::shared_future<int> future_result = std::async(func);
    std::cout << __LINE__ << std::endl;
    future_result.get();
    std::cout << __LINE__ << std::endl;
    future_result.get();
    std::cout << __LINE__ << std::endl;
}
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
TEST(locks, raii_acquire_release_acquire)
{
    std::mutex my_mutex;

    // Create a unique_lock object and acquire the lock
    std::unique_lock<std::mutex> lock(my_mutex);

    // Do some work that requires the lock
    std::cout << "Thread " << std::this_thread::get_id() << " is working." << std::endl;

    // Release the lock
    lock.unlock();

    // Do some work that does not require the lock
    std::cout << "Thread " << std::this_thread::get_id() << " is resting." << std::endl;

    // Re-acquire the lock
    lock.lock();

    // Do some more work that requires the lock
    std::cout << "Thread " << std::this_thread::get_id() << " is working again." << std::endl;

}
TEST(locks, defer_lock)
{
	std::mutex my_mutex;
	// Create a unique_lock with std::defer_lock parameter
	std::unique_lock<std::mutex> lock(my_mutex, std::defer_lock);

	// Do some work that does not require the lock

	// Acquire the lock when needed
	lock.lock();

	// Do some work that requires the lock

	// Release the lock
	lock.unlock();

	// Do some more work that does not require the lock
}

struct Example
{
    Example()
    {
        //std::cout << __FUNCTION__ << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        is_init = true;
    }
    bool is_init = false;
};

std::unique_ptr<Example> resource_ptr;
std::mutex resource_mutex;

void unsafe_double_checked_locking()
{
    if (!resource_ptr) // First check without lock
    {
        std::lock_guard<std::mutex> lk(resource_mutex); // Lock held for a short time
        if (!resource_ptr) // Second check with lock, but a race condition can occur here
        {
            resource_ptr.reset(new Example());
        }
    }
    if (!resource_ptr->is_init)
    {
        std::cout << "BUG!\n";
        throw "Bug";
    }
    //std::cout << "Thread " << std::this_thread::get_id() <<
    //    ", resource_ptr::is_init=" << resource_ptr->is_init << std::endl;
}

TEST(locks, double_checked_locking_pattern_test)
{
    // Tried to create an error of the double checked locking pattern
    // Didn't didn't manage to make it happen

    std::thread t1(unsafe_double_checked_locking);
    std::thread t2(unsafe_double_checked_locking);

    t1.join();
    t2.join();

    resource_ptr.reset();

    std::cout << std::endl;
}

TEST(locks, condition_variable)
{
    std::mutex mutex;
    std::condition_variable cv;
    bool is_ready = false;

    auto producer = [&]() -> void 
    {
        // Do some work...
        std::cout << "Producer is working\n";
        std::this_thread::sleep_for(std::chrono::seconds(2));

        {
            std::lock_guard<std::mutex> lock(mutex);
            std::cout << "Producer is_ready=true\n";
            is_ready = true;
        }

        cv.notify_one();
    };

    auto consumer = [&]() -> void
    {
        {
            std::unique_lock<std::mutex> lock(mutex);
            cv.wait(lock, [&] { std::cout << "Consumer is checking the condition\n"; return is_ready; });
        }

        std::cout << "Consumer: The resource is ready." << std::endl;
    };

    std::thread t1(producer);
    std::thread t2(consumer);

    t1.join();
    t2.join();

}

template<typename T>
class threadsafe_queue
{
private:
    mutable std::mutex mut;
    std::queue<T> data_queue;
    std::condition_variable data_cond;
public:
    threadsafe_queue()
    {}
    threadsafe_queue(threadsafe_queue const& other)
    {
        std::lock_guard<std::mutex> lk(other.mut);
        data_queue = other.data_queue;
    }
    void push(T new_value)
    {
        std::lock_guard<std::mutex> lk(mut);
        data_queue.push(new_value);
        data_cond.notify_one();
    }
    void wait_and_pop(T& value)
    {
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk, [this] {return !data_queue.empty(); });
        value = data_queue.front();
        data_queue.pop();
    }
    std::shared_ptr<T> wait_and_pop()
    {
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk, [this] {return !data_queue.empty(); });
        std::shared_ptr<T> res(std::make_shared<T>(data_queue.front()));
        data_queue.pop();
        return res;
    }

    bool try_pop(T & value)
    {
        std::lock_guard<std::mutex> lk(mut);
        if (data_queue.empty())
            return false;
        value = data_queue.front();
        data_queue.pop();
        return true;
    }

    std::shared_ptr<T> try_pop()
    {
        std::lock_guard<std::mutex> lk(mut);
        if (data_queue.empty())
            return std::shared_ptr<T>();
        std::shared_ptr<T> res(std::make_shared<T>(data_queue.front()));
        data_queue.pop();
        return res;
    }
    bool empty() const
    {
        std::lock_guard<std::mutex> lk(mut);
        return data_queue.empty();
    }
};

TEST(locks, implementation_of_threadsafe_queue_using_conditional_variable)
{
    threadsafe_queue<int> q;
}

// A function that represents a long-running computation
int find_the_answer_to_ltuae()
{
    std::this_thread::sleep_for(std::chrono::seconds(1));
    return 42;
}

// A function that represents other work that can be done while waiting for the computation
void do_other_stuff()
{
}
TEST(locks, future)
{
    // Launch the computation asynchronously and get a future that represents the result
    std::future<int> the_answer = std::async(find_the_answer_to_ltuae);

    // Do other work while waiting for the computation to complete
    do_other_stuff();

    // Get the result of the computation from the future and print it out
    std::cout << "The answer is " << the_answer.get() << std::endl;
}

std::mutex m;
std::deque<std::packaged_task<void()> > tasks;

// Checks if a shutdown message has been received.
bool is_exit_gui_thread = {};
bool gui_shutdown_message_received() { return is_exit_gui_thread; }

// Processes the next GUI message.
void get_and_process_gui_message() {}

// The GUI thread function that processes tasks.
void gui_thread()
{
    std::cout << "GUI thread: " << std::this_thread::get_id() << std::endl;
    while (!gui_shutdown_message_received())
    {
        get_and_process_gui_message();
        std::packaged_task<void()> task;
        {
            std::lock_guard<std::mutex> lk(m);
            // Continue if the task queue is empty.
            if (tasks.empty())
                continue;
            task = std::move(tasks.front());
            tasks.pop_front();
        }
        // Execute the task.
        task();
    }
}

// Posts a task to the GUI thread and returns a future for its result.
template<typename Func>
std::future<void> post_task_for_gui_thread(Func f)
{
    std::packaged_task<void()> task(f);
    std::future<void> res = task.get_future();
    // Lock the mutex and add the task to the queue.
    std::lock_guard<std::mutex> lk(m);
    tasks.push_back(std::move(task));
    return res;
}

void do_something()
{
    // Do some time-consuming task.
    std::cout << "Starting a task in thread: " << std::this_thread::get_id() << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(3));
    std::cout << "Task completed.\n";
}

TEST(locks, packaged_task)
{
    std::cout << "Unit test thread: " << std::this_thread::get_id() << std::endl;
    // Start the GUI thread.
    std::thread gui_bg_thread(gui_thread);

    // Post a task to the GUI thread and get a future for the task's result.
    auto task = []() { do_something(); };
    auto fut = post_task_for_gui_thread(task);

    // Wait for the task to complete.
    fut.wait();

    std::cout << "Done.\n";

    is_exit_gui_thread = true;

    if (gui_bg_thread.joinable())
        gui_bg_thread.join();
}
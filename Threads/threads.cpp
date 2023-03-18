#include "pch.h"
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <functional>
#include <numeric>
#include <atomic>

void hello()
{
	std::cout << "Hello thread\n";
}

std::atomic<int> g_counter;
int t_start_counter[2] = {};
int t_end_counter[2] = {};

std::mutex g_mutex;

void foo_counter(int id)
{
	t_start_counter[id] = g_counter;
	for (int i = 0; i < 1000000; i++)
	{
		// std::lock_guard<std::mutex> lock(g_mutex);
		g_counter++;
		//g_counter--;
		//g_counter+=10;
	}
	t_end_counter[id] = g_counter;
}

TEST(threads, throw_exception)
{
	std::thread t1(&foo_counter, 0);
	std::thread t2(&foo_counter, 0);
	// t2 = t1;

	//t2 = std::move(t1);
	//std::cout << "Didn't arrive here\n";
	//t2.join();
}

TEST(threads, does_stl_vector_crash)
{
	const int NUM_THREADS = 10;
	std::vector<std::thread> threads(NUM_THREADS);

	std::vector<int> counters(NUM_THREADS,0);

	auto func = [&](int i) -> void
	{
		while (true)
		{
			// std::cout << i << ": I'm alive\n";
			counters[i]++;
		}
	};
	int i;
	for (i = 0; i < threads.size(); i++)
	{
		threads[i] = std::thread(func, i);
	}
	while (true)
	{
		//std::this_thread::sleep_for(std::chrono::milliseconds(1));
		std::cout << i << " Main thread about to increment the vector\n";
		threads.push_back(std::thread(func, i));
		counters.push_back(0);
		i++;
	}

	for (auto& t : threads)
	{
		if (t.joinable())
			t.join();
	}

}

TEST(threads, incrementer)
{
	for (int j = 0; j < 1000; j++)
	{
		g_counter = 0;
		std::vector<std::thread> threads;
		for (int i = 0; i < 2; i++)
		{
			threads.push_back(std::thread(&foo_counter,i));
		}

		for (int i = 0; i < 2; i++)
		{
			if (threads[i].joinable())
				threads[i].join();
			std::cout << "t_start_counter[" << i << "] = " << t_start_counter[i] << std::endl;
			std::cout << "t_end_counter[" << i << "] = " << t_end_counter[i] << std::endl;
		}

		std::cout << "The counter value is: " << g_counter << std::endl;
		std::cout << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}
}


TEST(threads, hello_world) 
{
	std::thread t(&hello);
	t.join();
	std::cout << "Bye main\n";
}

void hello_t()
{
	for (int i=0;i<100;i++)
		std::cout << "t";
}

TEST(threads, two_threads_print)
{
	std::thread t(&hello_t);
	for (int i=0;i<100;i++)
		std::cout << "m";
	t.join();
	std::cout << std::endl;
}

void self_join(std::thread* t)
{
	try
	{
		t->join();
	}
	catch (std::system_error e)
	{		
		std::cout << "Exception: " << e.code() << ", " << e.what() << std::endl;
	}

}

TEST(threads, join_on_itself)
{
	std::thread t;
	t = std::thread(&self_join, &t);
	std::this_thread::sleep_for(std::chrono::milliseconds(500));	
	t.join();
}

TEST(threads, join_twice)
{
	std::thread t;
	t = std::thread(&self_join, &t);
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	t.join();
	EXPECT_THROW(t.join(), std::system_error);
}

TEST(threads, get_id)
{
	std::cout << "Running thread ID: " << std::this_thread::get_id() << std::endl;
	// std::cout << "Running thread ID using Windows: " << ::GetCurrentThreadId() << std::endl;
}

void pause_thread(int n)
{
	std::this_thread::sleep_for(std::chrono::seconds(2 * n));
	std::cout << "thread " << n << " is here\n";
}

TEST(threads, sleep_for)
{
	std::cout << "Spawning 2 threads\n";
	std::thread t1(pause_thread, 1);
	std::thread t2(pause_thread, 2);
	std::cout << "Done spawning threads\n";
	std::cout << "Now waiting for them to join:\n";
	t1.join();
	t2.join();
	std::cout << "Threads joined\n";
}

TEST(threads, sleep_until)
{
	// Get the current time point
	auto now = std::chrono::system_clock::now();

	// Calculate the time point to wake up at (1 second from now)
	auto wakeup_time = now + std::chrono::seconds(1);

	// Sleep until the specified time point
	std::this_thread::sleep_until(wakeup_time);

	// Print a message after waking up
	std::time_t wakeup_time_t = std::chrono::system_clock::to_time_t(wakeup_time);
	std::cout << "Woke up at " << std::ctime(&wakeup_time_t) << std::endl;
}

void increment(std::atomic<int>& value)
{
	for (int i = 0; i < 1000000; i++)
	{
		//std::lock_guard<std::mutex> lock(g_mutex);
		value++;
	
		//::_InterlockedIncrement(&value);
	}
}

// No longer compiles in VS2022
//TEST(threads, passing_reference_parameters_to_threads)
//{
//	int counter = 0;
//	std::thread t(&increment, counter);
//	t.join();
//	std::cout << counter << std::endl;
//}

TEST(threads, passing_reference_parameters_to_threads)
{
	std::atomic<int> counter = 0;
	std::thread t(&increment, std::ref(counter));
	t.join();
	std::cout << counter << std::endl;
}

struct ClassWithStatic {
	struct Impl {
		Impl() { std::cout << "Hello from class"; }
	};
	static Impl impl;
};
typename ClassWithStatic::Impl ClassWithStatic::impl;


TEST(threads, vector_of_references)
{
	int a = 1;
	int b = 2;
	int c = 3;

	// Create a vector of reference wrappers
	std::vector<std::reference_wrapper<int>> vec{ a, b, c };

	// Increment each element of the vector
	for (int& i : vec)
	{
		i++;
	}

	// Print the values of the original variables, which have been modified
	std::cout << "a = " << a << ", b = " << b << ", c = " << c << std::endl;

	ASSERT_EQ(a, 2);
	ASSERT_EQ(b, 3);
	ASSERT_EQ(c, 4);	
}

void do_work(unsigned id) {}

TEST(threads, spawn_some_threads_and_wait_to_finish)
{
	std::vector<int> v{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	int sum = std::accumulate(v.begin(), v.end(), 0);

	std::vector<std::thread> threads;
	for (unsigned i = 0; i < 20; ++i)
	{
		threads.push_back(std::thread(do_work, i));
	}
	for (auto& t : threads)
	{
		if (t.joinable())
			t.join();
	}
	// An alternative syntax:
	// std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));
}

template<typename Iterator, typename T>
struct accumulate_block
{
	void operator()(Iterator first, Iterator last, T& result)
	{
		result = std::accumulate(first, last, result);
	}
};

template<typename Iterator, typename T>
T parallel_accumulate(Iterator first, Iterator last, T init)
{
	// Calculate the length of the range between first and last iterators
	unsigned long const length = std::distance(first, last);

	// If the length is 0, return the initial value
	if (!length)
		return init;

	// Define the minimum number of items per thread
	unsigned long const min_per_thread = 25;

	// Calculate the maximum number of threads based on the range length
	unsigned long const max_threads = (length + min_per_thread - 1) / min_per_thread;

	// Determine the number of hardware threads available, or use a default of 2
	unsigned long const hardware_threads = std::thread::hardware_concurrency();
	unsigned long const num_threads =
		std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads);

	// Calculate the block size for each thread
	unsigned long const block_size = length / num_threads;

	// Create a vector to hold the results for each thread
	std::vector<T> results(num_threads);

	// Create a vector to hold the threads
	std::vector<std::thread> threads(num_threads - 1);

	// Split the range into blocks for each thread
	Iterator block_start = first;
	for (unsigned long i = 0; i < (num_threads - 1); ++i)
	{
		Iterator block_end = block_start;
		std::advance(block_end, block_size);

		// Create a thread to accumulate the values in the current block
		threads[i] = std::thread(
			accumulate_block<Iterator, T>(),
			block_start, block_end, std::ref(results[i]));

		// Move the start of the next block to the end of the current block
		block_start = block_end;
	}

	// Accumulate the remaining values in the final block
	accumulate_block<Iterator, T>()(block_start, last, results[num_threads - 1]);

	// Wait for all threads to finish before continuing
	std::for_each(threads.begin(), threads.end(),
		std::mem_fn(&std::thread::join));

	// Accumulate the results from each thread and return the total sum
	return std::accumulate(results.begin(), results.end(), init);
}

TEST(threads, parallel_accumulate)
{
	std::vector<int> v(1000);
	std::iota(v.begin(), v.end(), 0);

	int sum = std::accumulate(v.begin(), v.end(), 0);
	int sum_parallel = parallel_accumulate(v.begin(), v.end(), 0);

	std::cout << "sum: " << sum << ", sum_parallel: " << sum_parallel << std::endl;

	ASSERT_EQ(((v.size() - 1) * v.size()) / 2, sum);
	ASSERT_EQ(sum, sum_parallel);
}

class StoppableThread
{
public:
	StoppableThread() : m_isStopRequested(false) {}

	void start(std::function<void()> func)
	{
		m_thread = std::thread([func]() { func(); });
	}

	void stop()
	{
		m_isStopRequested = true;
		if (m_thread.joinable())
		{
			m_thread.join();
		}
	}

	bool isStopRequested() const
	{
		return m_isStopRequested;
	}

private:
	std::thread m_thread;
	std::atomic_bool m_isStopRequested;
};

void long_running_operation(StoppableThread& thread)
{
	std::cout << "long_running_operation started" << std::endl;

	auto start = std::chrono::high_resolution_clock::now();
	int i{ 0 };
	while (!thread.isStopRequested())
	{
		++i;
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}

	auto end = std::chrono::high_resolution_clock::now();

	std::cout << "long_running_operation stopped after " << i << " iterations and " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms" << std::endl;
}


TEST(threads, stoppable_thread)
{
	StoppableThread thread;
	thread.start(std::bind(long_running_operation, std::ref(thread)));

	std::this_thread::sleep_for(std::chrono::seconds(1));

	thread.stop();
}


#include "pch.h"
#include <latch>
#include <thread>

void long_running_operation(std::latch& latch)
{
	std::cout << "long_running_operation started" << std::endl;
	std::this_thread::sleep_for(std::chrono::seconds(1));

	std::cout << "long_running_operation completed" << std::endl;

	latch.count_down();
}

constexpr int factorial(int x)
{
	if (x <= 1)
		return 1;
	else
		return x * factorial(x - 1);
}

TEST(latch, wait)
{
	constexpr int result_of_add = factorial(3);
	constexpr int NUM_THREADS{ 3 };
	std::latch latch(NUM_THREADS);
	std::vector<std::thread> threads(NUM_THREADS);

	for (int i = 0; i < threads.size(); ++i)
	{
		threads[i] = std::thread(long_running_operation, std::ref(latch));
	}

	latch.wait();

	for (auto& thread : threads)
	{
		thread.join();
	}
}
#include "pch.h"
#include <thread>

void long_running_operation(std::stop_token stop_token)
{
	std::cout << "long_running_operation started" << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    int i{ 0 };
    while (!stop_token.stop_requested())
    {
        ++i;
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    auto end = std::chrono::high_resolution_clock::now();

    std::cout << "long_running_operation stopped after " << i << " iterations and " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms" << std::endl;

}

TEST(jthread, stop)
{
    std::jthread thread(long_running_operation);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    thread.request_stop();
}
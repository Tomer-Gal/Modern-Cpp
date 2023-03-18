#include "pch.h"

#include <thread>
#include <iostream>
#include <chrono>
#include <semaphore>

TEST(semaphore, acquire_release)
{
	auto worker = [](int id, std::counting_semaphore<2>& sem)
	{
		sem.acquire();
		std::cout << "Worker " << id << " starting work" << std::endl;

		std::this_thread::sleep_for(std::chrono::seconds(1));

		std::cout << "Worker " << id << " finished work" << std::endl;

		sem.release();
	};

	std::counting_semaphore<2> sem(2);
	std::thread t1(worker, 1, std::ref(sem));
	std::thread t2(worker, 2, std::ref(sem));
	std::thread t3(worker, 3, std::ref(sem));

	t1.join();
	t2.join();
	t3.join();
}
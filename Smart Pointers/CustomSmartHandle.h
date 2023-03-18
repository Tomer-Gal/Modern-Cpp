#pragma once
#include <Windows.h>
#include <memory>
#include <iostream>

struct Student
{
	const char* name;
	int age;
};

struct HandleDeleter
{
	void operator()(HANDLE h)
	{
		if (h != INVALID_HANDLE_VALUE)
		{
			std::cout << "Closing the handle: " << h << std::endl;
			// CloseHandle(h);
		}
	}

	void operator()(Student* h)
	{
		if (h)
		{
			delete h;
		}
	}

};

using UniqueHandle = std::unique_ptr<void, HandleDeleter>;

using UniqueStudent = std::unique_ptr<Student, HandleDeleter>;
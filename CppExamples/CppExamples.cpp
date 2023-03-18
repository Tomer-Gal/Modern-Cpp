#include <vector>
#include <cstdlib>
#include <iostream>
#include <mutex>

void something(int& x)
{
	int& y = (int&) x;
	//int& y = const_cast<int&>(x);
	y++;

	std::cout << (void*)&x << std::endl;
	std::cout << (void*)&y << std::endl;

	std::cout << x << std::endl;
	std::cout << y << std::endl;
}

int main()
{
	int x = 5;
	something(x);
	int y = x + 1;
	std::cout << "Value of x: " << x << std::endl;
	std::cout << (void*)&x << std::endl;
	return EXIT_SUCCESS;
}
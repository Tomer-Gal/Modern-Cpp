#include <iostream>
#include <exception>
#include <chrono>

using namespace std::chrono;

float div_2(float val)
{
	return val / 2.0F;
}

float mul_2(float val)
{
	return val * 2;
}

int main()
{
	float i;
	//mul_2(div_2(i)) == i;
	float val = 1.0F;
	while (true)
	{
		std::cout << val << std::endl;
		if (val == 0.0F)
		{
			std::cout << "0 encountered" << std::endl;
			break;
		}
		val = val / 2.0F;
	}
}
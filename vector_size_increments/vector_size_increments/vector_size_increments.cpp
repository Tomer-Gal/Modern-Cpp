#include <vector>
#include <iostream>
#include <iomanip>

void riddle()
{
	const int GB = 1000 * 1000 * 1000;
	std::vector<unsigned char> v(GB, '0');
	std::cout << v.size() << std::endl;		// Size 1GB
	std::cout << v.capacity() << std::endl; // Capacity 1GB

	v.push_back({});

	std::cout << v.size() << std::endl;		// Size 1GB + 1 byte
	std::cout << v.capacity() << std::endl;	// Capacity: ???

}

int main()
{
	std::cout << "Given a vector, add to it elements using push_back while printing its size and capacity\n";

	std::vector<unsigned char> v;

	size_t max_capacity{};

	const int GB = 1000 * 1000 * 1000;

	std::cout << std::left << std::setw(20) << "vector size" << std::setw(20) << "capacity" << std::setw(20) << "ratio" << std::endl;
	for (int i = 1; i < GB; i++)
	{
		v.push_back('0');
		if (max_capacity < v.capacity())
		{
			
			max_capacity = v.capacity();			
			std::cout << std::setw(20) << v.size() <<
				std::setw(20) << max_capacity <<
				std::setw(20) << static_cast<float>(max_capacity) / v.size() << std::endl;
		}
	}

	return 0;
}

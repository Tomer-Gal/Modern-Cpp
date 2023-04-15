#include "pch.h"
#include <type_traits>
#include <any>

template <typename T>
void print_type(T&& arg)
{
	if (std::is_pointer_v<std::remove_reference_t<T>>)
	{
		std::cout << "pointer" << std::endl;
	}
	else if (std::is_reference_v<std::remove_reference_t<T>>)
	{
		std::cout << "reference" << std::endl;
	}
	else if (std::is_arithmetic_v<std::remove_reference_t<T>>)
	{
		std::cout << "arithmetic" << std::endl;
	}
	else
	{
		std::cout << "other" << std::endl;
	}
}

TEST(type_traits, is_pointer)
{
	int* ptr = nullptr;
	print_type(ptr);
}

TEST(type_traits, is_reference)
{
	int value = 42;
	int& ref = value;
	print_type(ref);
}

TEST(type_traits, is_arithmetic)
{
	int value = 42;	
	print_type(value);
	print_type(3.14);
}

TEST(type_traits, is_other)
{
	std::string str = "hello";
	print_type(str);
}

TEST(type_traits, any)
{
	int b = 3;
	std::any a = &b;
	print_type(a);

}

std::ostream& operator<<(std::ostream& os, const std::any& a)
{
	os << __FUNCTION__;
	if (a.has_value())
	{
		os << "Has value";
	}
	else
	{
		os << "No value";
	}
	return os;
}

TEST(type_traits, print_any)
{
	std::any a = 5;
	std::cout << a << std::endl;
}
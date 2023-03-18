#include "pch.h"
#include <iostream>
#include <tuple>
#include <array>
#include <optional>
#include <string>
#include <optional>

#define PRINT_VAR(x) std::cout << #x << " = " << x << std::endl

TEST(structured_binding, binding_a_tuple)
{
	//auto& [x, y, z] = std::tuple<int, double, const char*>(1, 3.14, "hello");
	//std::cout << x << " " << y << " " << z << std::endl;
}

TEST(structured_binding, binding_an_array)
{
	// Binding an array
	int bbox[4] = { 150, 170, 200,200 };
	auto& [x, y, width, height ] = bbox;
	PRINT_VAR(x);
	PRINT_VAR(y);
	PRINT_VAR(width);
	PRINT_VAR(height);
	std::cout << x << " " << y << " " << width << " " << height << std::endl;
}

TEST(structured_binding, binding_an_STL_array)
{
	// Binding an array
	std::array<int, 4> bbox = { 150, 170, 200,200 };
	auto& [x, y, width, height] = bbox;
	PRINT_VAR(x);
	PRINT_VAR(y);
	PRINT_VAR(width);
	PRINT_VAR(height);
	std::cout << x << " " << y << " " << width << " " << height << std::endl;
}

struct Student
{
	std::string name;
	std::string state;

	int age{};
	Student() { std::cout << __FUNCSIG__ << std::endl; }
	Student(const Student& other) { std::cout << __FUNCSIG__ << std::endl; }
	Student(const Student&& other) { std::cout << __FUNCSIG__ << std::endl; }
	Student& operator=(const Student& other) { std::cout << __FUNCSIG__ << std::endl; return *this;}
	Student& operator=(const Student&& other) { std::cout << __FUNCSIG__ << std::endl; return *this; }
	~Student() { std::cout << __FUNCSIG__ << std::endl;	}
};

struct non_copyable
{
	non_copyable() = default;
	non_copyable& operator=(const non_copyable& other) = delete;
	non_copyable(const non_copyable& other) = delete;
};

struct Point : public non_copyable
{
	// Please do not copy this struct!
	int x, y, z;
	int* ptr = new int[1000];

	~Point()
	{
		delete[] ptr;
	}

};

double our_sqrt1(double x)
{
	if (x < 0)
	{
		return -1.0;
	}
	return sqrt(x);
}

std::optional<double> our_sqrt2(double x)
{
	if (x < 0)
	{
		return {};
	}
	return sqrt(x);
}

std::optional<Student> lookupStudent(std::string name)
{
	return {};
}

TEST(move_semantics, copy_constructor)
{
	std::optional<Student> output = lookupStudent("Keith");
	auto func = [](std::optional<Student> stu)
	{
		return stu ? (stu.value().name + "is from") : "";
	};
	
	std::string s1 = func(output);
	std::cout << s1 << std::endl;
	//std::cout << output.and_then(func).value_or(“No student found”);

	Point p1;
	Point p2;
	// p2 = p1; // Won't compile
	Student a1;
	Student a2 = std::move(a1);
	Student a3;
	a2 = std::move(a3);

	//std::optional<unsigned int> x = {};
	//std::cout << *x << std::endl;
}
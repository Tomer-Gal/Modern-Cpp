#include "pch.h"
#include <any>
#include <ostream>

TEST(any, any_type)
{
	std::any a = 5;
	std::any b = std::string("hello");
	int value = std::any_cast<int>(a);
	std::string str = std::any_cast<std::string>(b);

	EXPECT_EQ(value, 5);
	EXPECT_EQ(str, "hello");
}

TEST(any, bad_any_cast)
{
	std::any a = 5;
	// This should throw a std::bad_any_cast exception
	EXPECT_THROW(std::any_cast<std::string>(a), std::bad_any_cast);
}

TEST(any, any_has_value)
{
	std::any a = 5;
	EXPECT_TRUE(a.has_value());

	a.reset();

	EXPECT_FALSE(a.has_value());
}

struct Student
{
	std::string name;
	int age;
};

std::ostream& operator<<(std::ostream& os, const Student& s)
{
	os << s.name << " " << s.age;
	return os;
}

TEST(any, custom_type)
{
	Student s = { "Moshe",30 };
	std::any a = s;
	Student s2 = std::any_cast<Student>(a);

	std::cout << s << std::endl;
	std::cout << s2 << std::endl;

	EXPECT_STREQ(a.type().name(), "struct Student");
}
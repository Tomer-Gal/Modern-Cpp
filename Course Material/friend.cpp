#include "pch.h"

class MyClass {
public:
    MyClass(int value) : m_value(value) {}

    friend void print_value(const MyClass& obj);

private:
    int m_value;
};

void print_value(const MyClass& obj) {
    std::cout << "The value of MyClass is: " << obj.m_value << std::endl;
}

TEST(friend_tester, friend_test)
{
    MyClass obj(123);
    testing::internal::CaptureStdout();
    print_value(obj);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "The value of MyClass is: 123\n");
}
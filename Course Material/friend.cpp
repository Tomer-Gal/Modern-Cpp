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

class One 
{
private:
    int private_variable{};

protected:
    int protected_variable{};

public:
    One()
    {
        private_variable = 10;
        protected_variable = 99;
    }

    // friend class declaration
    friend class Two;
};

class Two 
{
public:
    void display(One& t)
    {
        std::cout << "The value of Private Variable = " << t.private_variable << std::endl;
        std::cout << "The value of Protected Variable = " << t.protected_variable;
    }
};

TEST(friend_tester, friend_class_test)
{
    One t1;
	Two t2;
	t2.display(t1);

}
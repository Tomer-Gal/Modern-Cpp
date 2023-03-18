#include <iostream>
#include <string>
#include <tuple>
#include <map>
#include <vector>

class Foo
{
public:
    Foo() { something(); }
    void something() { Func(); }
    virtual void Func() = 0;
};

class Bar : public Foo
{
public:
    void Func() override { std::cout << __FUNCTION__ << std::endl; }
};

struct Student
{
    Student(const std::string& _state, const std::string _name, int _age)
    {

    }
    std::string state;
    std::string name;
    int age{};
};

int add(const int& x, const int& y)
{
    std::cout << "x:" << x << std::endl;
    std::cout << "y:" << y << std::endl;
    int& modify = (int&)(x);
    modify++;
    std::cout << (void*)&modify << std::endl;
    return x + y;
}
void f(const std::vector<std::pair<bool, Student>>& input)
{
    const std::vector<std::pair<bool, Student>>& something1 = input;
    auto something2 = input;
    const int a = 3;
    auto b = a;
    b = 7;
}
void Printer(const Student& sasjfoijojowge)
{
    //
    //
    // Some long code
    //
    //
    auto s = sasjfoijojowge;
}

struct Point
{
    float distance(int _x, int _y)
    {
    }
    int x;
    int y;
};

void someFunc(std::vector<int>& s)
{
}

int main()
{
    std::vector<int> v;
    someFunc(v);
    int result1 = add(3, 5);
    int result2 = add(3, 5);
    int result3 = add(3, 5);
    Student s = { "Moshe","Israel",35 };
    std::cout << s.name << std::endl;
    Printer(s);
    std::cout << s.name << std::endl;
    //Bar bar;
    //Student student1 = { "Frankie", "MN", 21 };
    //Student student2;
    // auto type = std::make_pair(3, "Hello");
}
#include "pch.h"

class MyObject 
{
public:
    MyObject()
    {
        // Simulate a failure that can occur during construction
        m_ptr = new int[5];
        throw std::runtime_error("Error during construction");        
    }

    ~MyObject()
    {
        std::cout << __FUNCTION__ << " called" << std::endl;
		delete m_ptr;
	}

private:
    int* m_ptr;
};

TEST(pitfalls, exception_thrown_in_constructor)
{
    //ASSERT_THROW(MyObject obj, std::runtime_error);

    try 
    {
        MyObject obj;
    }
    catch (const std::runtime_error& e) {
        std::cout << "Exception caught: " << e.what() << std::endl;
    }
}

class Base
{
public:
    Base() 
    { 
        bar(); 
    }
	virtual ~Base() { std::cout << __FUNCTION__ << " called" << std::endl; }
    void bar() { foo(); }
    virtual void foo() = 0;
};

class Derived : public Base
{
public:
    void foo() override { std::cout << __FUNCTION__ << " called" << std::endl; }
    virtual ~Derived() { std::cout << __FUNCTION__ << " called" << std::endl; }
};



TEST(DISABLED_pitfalls, calling_virtual_method_from_constructor)
{
    // Pure virtual call from constructor
    std::unique_ptr<Base> base = std::make_unique<Derived>();
}

class LeakyBase
{
public:    
    LeakyBase() { std::cout << __FUNCTION__ << " called" << std::endl; }
	~LeakyBase() { std::cout << __FUNCTION__ << " called" << std::endl; }
};;

class LeakyDerived : public LeakyBase
{
public:
    LeakyDerived() { std::cout << __FUNCTION__ << " called" << std::endl; }
    ~LeakyDerived() { std::cout << __FUNCTION__ << " called" << std::endl; }
};

TEST(pitfalls, non_virtual_destructor_causes_leaks)
{
	std::unique_ptr<LeakyBase> base = std::make_unique<LeakyDerived>();
}
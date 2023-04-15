#include "pch.h"
#include <typeinfo>

class Animal 
{
public:
    virtual void speak() { std::cout << "I am an animal." << std::endl; }
};

class Dog : public Animal 
{
public:
    void speak() { std::cout << "I am a dog." << std::endl; }
};

class Cat : public Animal 
{
public:
    void speak() { std::cout << "I am a cat." << std::endl; }
};

TEST(rtti_test, type_check) 
{
    std::unique_ptr<Animal> animal = std::make_unique<Dog>();
    Dog* dog = dynamic_cast<Dog*>(animal.get());
    Cat* cat = dynamic_cast<Cat*>(animal.get());

    ASSERT_NE(dog, nullptr);
    ASSERT_EQ(cat, nullptr);
}

TEST(rtti_test, type_identification) 
{
    std::unique_ptr<Animal> animal = std::make_unique<Cat>();
    const std::type_info& type = typeid(*animal);

    EXPECT_EQ(type.name(), typeid(Cat).name());
}

// The RTTITest, VirtualFunction test is related to RTTI in C++ because it uses virtual functions to demonstrate the polymorphic behavior of objects
// and the role of RTTI in determining the type of an object at runtime.
TEST(rtti_test, virtual_function) 
{
    std::unique_ptr<Animal> animal = std::make_unique<Dog>();
    testing::internal::CaptureStdout();
    animal->speak();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "I am a dog.\n");
}
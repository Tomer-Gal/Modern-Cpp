#include "pch.h"
#include "Singleton.h"

std::unique_ptr<Singleton, Singleton::Deleter> Singleton::instance;
std::once_flag Singleton::flag;

Singleton& Singleton::getInstance()
{
    static std::once_flag flag;
    std::call_once(flag, []() { instance.reset(new Singleton()); });
    return *instance;
}

void Singleton::releaseInstance()
{
    instance.reset();
}

Singleton::Singleton() { std::cout << "Singleton created." << std::endl; }

Singleton::~Singleton() { std::cout << "Singleton destroyed." << std::endl; }

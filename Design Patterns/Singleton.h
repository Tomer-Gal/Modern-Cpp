#pragma once

#include <iostream>
#include <memory>
#include <mutex>

class Singleton 
{
public:
    static Singleton& getInstance();
    static void releaseInstance();

    Singleton(const Singleton&) = delete;
    Singleton(Singleton&&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    Singleton& operator=(Singleton&&) = delete;

private:
    Singleton();
    ~Singleton();

    struct Deleter 
    {
        void operator()(Singleton* p) const 
        {
            delete p;
        }
    };

    static std::unique_ptr<Singleton, Deleter> instance;
    static std::once_flag flag;
};


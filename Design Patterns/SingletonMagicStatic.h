#pragma once
class SingletonMagicStatic
{
public:
    static SingletonMagicStatic& getInstance()
    {
        static SingletonMagicStatic instance;
        return instance;
    }

    SingletonMagicStatic(const SingletonMagicStatic&) = delete;
    SingletonMagicStatic(SingletonMagicStatic&&) = delete;
    SingletonMagicStatic& operator=(const SingletonMagicStatic&) = delete;
    SingletonMagicStatic& operator=(SingletonMagicStatic&&) = delete;

private:
    SingletonMagicStatic() { std::cout << "SingletonMagicStatic created." << std::endl; }
    ~SingletonMagicStatic() { std::cout << "SingletonMagicStatic destroyed." << std::endl; }
};


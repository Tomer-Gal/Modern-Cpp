#include "pch.h"
#include "Singleton.h"
#include "SingletonMagicStatic.h"

TEST(Singleton, instance_is_unique)
{
    Singleton& s1 = Singleton::getInstance();
    Singleton& s2 = Singleton::getInstance();
    ASSERT_EQ(&s1, &s2);
}

TEST(SingletonTest, instance_releases_resources) {
    {
        Singleton& s1 = Singleton::getInstance();
    }
    Singleton::releaseInstance();
    Singleton& s1 = Singleton::getInstance();
    Singleton& s2 = Singleton::getInstance();
    ASSERT_EQ(&s1, &s2);
}

TEST(SingletonMagicStatic, instance_is_unique)
{
    SingletonMagicStatic& s1 = SingletonMagicStatic::getInstance();
    SingletonMagicStatic& s2 = SingletonMagicStatic::getInstance();
    ASSERT_EQ(&s1, &s2);
}
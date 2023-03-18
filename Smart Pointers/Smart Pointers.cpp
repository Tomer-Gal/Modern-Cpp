#include <iostream>
#include "CustomSmartHandle.h"
#include "ForwardDeclaration.h"

void foo()
{
    std::shared_ptr<int> sp = std::make_shared<int>(5);
    std::weak_ptr<int> wp = sp;
    if (!wp.expired())
    {
        std::shared_ptr<int> sp2 = wp.lock();
        if (sp2)
        {
            std::cout << *sp2 << std::endl;
        }
    }
}

int main()
{
	ForwardDeclaration f;
	{
		UniqueHandle h1(INVALID_HANDLE_VALUE);
		UniqueHandle h2((HANDLE)1);

		UniqueStudent s1(new Student());
	}
}

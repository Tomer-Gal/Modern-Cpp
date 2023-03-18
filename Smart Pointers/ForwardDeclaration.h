#pragma once
#include <memory>

class Resource; //Forward declaration

class ForwardDeclaration
{
public:
	ForwardDeclaration();
	~ForwardDeclaration();

private:
	std::unique_ptr<Resource> m_ptr;
};


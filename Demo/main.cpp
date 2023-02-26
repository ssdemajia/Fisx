#include "FISX.h"
#include <iostream>

int main(int argv, char** argc)
{
	Fisx::Vector3 v{ 0.1, 0.2, 0.3 };
	std::cout << v.x << std::endl;
	std::cout << v[2] << std::endl;
}
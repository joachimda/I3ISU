// main.cpp

#include "SmartString.hpp"
#include <iostream>
#include <string>
using namespace std;

int main(int argc, char* argv[])
{
	SmartString ss (new std::string("Hello world"));
	std::cout << "\nString lenght: " << ss->length() << std::endl;

	if(ss->find("world") != std::string::npos)
		std::cout << "The 'ss' string does contain 'world'" << std::endl;
	else
		std::cout << "The 'ss' string does not contain 'world'" << std::endl;

	//SmartString ss2 = ss;
}
// main.cpp

#include "SmartString.hpp"
using namespace std;

int main(int argc, char* argv[])
{
	SmartString ss (new std::string("Hello world"));

	SmartString ss2 = ss;
}

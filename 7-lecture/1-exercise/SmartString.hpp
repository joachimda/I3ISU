// SmartString.hpp
#ifndef SMARTSTRING_HPP_
#define SMARTSTRING_HPP_

#include <string>
using namespace std;

class SmartString
{
public:
	SmartString(string* str)
	{
		str_ = str;	
	}

	~SmartString()
	{
		delete str_;
	}

	string* get()
	{
		return str_;
	}

	string* operator->()
	{
		return str_;
	}

	string& operator*()
	{
		return *str_;
	}

private:
	string* str_;
	SmartString(const SmartString&);
	SmartString& operator=(const SmartString& other);
};

#endif
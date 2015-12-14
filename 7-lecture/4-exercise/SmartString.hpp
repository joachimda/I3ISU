// SmartString.hpp
#ifndef SMARTSTRING_HPP_
#define SMARTSTRING_HPP_

#include <string>
#include <iostream>
using namespace std;

class SmartString
{
public:
	SmartString(string* str)
	{

		str_ = str;
		counter_ = new unsigned int(1);

		cout << endl << __PRETTY_FUNCTION__ << ", counter is: " << *counter_ << "." << endl;
	}

	~SmartString()
	{
		(*counter_)--;
		cout << __PRETTY_FUNCTION__ << ", dtor invoked, counter is: " << *counter_ << "." << endl;

		if(*counter_ == 0)
		{
			cout << __PRETTY_FUNCTION__ << ", counter is zero, deleting object and data." << endl;
			delete str_;
			delete counter_;
		}
	}

	string* get()
	{
		cout << __PRETTY_FUNCTION__ << endl;
		return str_;
	}

	string* operator->()
	{
		cout << __PRETTY_FUNCTION__ << endl;
		return get();
	}

	string& operator*()
	{
		cout << __PRETTY_FUNCTION__ << endl;
		return *str_;
	}

	SmartString(const SmartString& other)
	{
		*this = other;

		cout << __PRETTY_FUNCTION__ << ", counter is: " << *counter_ << "." << endl;
	}

	SmartString& operator=(const SmartString& other)
	{
		str_ = other.str_;
		counter_ = other.counter_;
		(*counter_)++;

		cout << __PRETTY_FUNCTION__ << ", counter is: " << *counter_ << "." << endl;
	}

private:
	string* str_;
	unsigned int* counter_;
};

#endif
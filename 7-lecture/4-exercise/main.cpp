// main.cpp

#include <boost/shared_ptr.hpp>
using namespace std;

int main(int argc, char* argv[])
{
	// ctor
	cout << "\nTest of " << "Constructor." << endl;
	boost::shared_ptr<string> sp1(new string("Hello world"));
	cout << "sp1: " << *sp1 << endl;
	cout << "Use count: " << sp1.use_count() << endl;

	// copy ctor
	{
		cout << "\nTest of " << "Copy constructor and Destructor." << endl;
		boost::shared_ptr<string> sp2(sp1);
		cout << "sp2: " << *sp2 << endl;
		cout << "Use count: " << sp2.use_count() << endl;
	}
	
	// assign operator
	cout << "\nTest of " << "Assignment operator." << endl;
	boost::shared_ptr<string> sp3(new string("Derp"));
	cout << "sp3: " << *sp3 << endl;
	cout << "Use count: " << sp3.use_count() << endl;

	// dtor
	//cout << "sp2: " << *sp2 << endl;
	//cout << "Use count: " << sp2.use_count() << endl;
}

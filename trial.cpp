#include <iostream>
#include <sstream>
using namespace std;
int main()
{
	char buffer[]="huzi      123";
	stringstream ss;
	ss<<buffer;
	string method;
	int id;
	ss>>method>>id;
	// cout<<method<<id<<endl;
	printf("%s %d",ss.str().c_str(),ss.str().size());
}
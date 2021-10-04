// CPP concatenate two char numerals to digit i.e. '1','2' -> 12
#include <iostream> 
#include <string> 

using namespace std; 

int main() 
{ 
	char c1 = '1';
	char c2 = '2';
	char c3 = '3';

	std::string s; // an empty string

	s += c1; // append the first character
	s += c2; // append the second character
	s += c3;
	
	int x = stoi(s);

	cout << x;
	return 0; 
} 

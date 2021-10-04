// CPP program to demonstrate working of string 
// find to search a string 
#include <iostream> 
#include <string> 

using namespace std; 

int main() 
{ 
	string str = "set 1, 2 * D[0]"; 
	string str1 = "2 *"; 

	// Find first occurrence of "geeks" 
	size_t found = str.find(str1); 
	if (found != string::npos) 
		cout << "found" ;
	else
		cout << "not found";

	return 0; 
} 


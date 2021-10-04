// C++ program to demonstrate working of regex_match() 
#include <iostream> 
#include <regex> 

using namespace std; 
int main() 
{ 
	char a[] = "set 0, read"; 

	// Here b is an object of regex (regular expression) 
	regex b("(set)(.*)"); // set followed by any character 

	// regex_match function matches string a against regex b 
	if ( regex_match(a, b) ) 
		cout << "String 'a' matches regular expression 'b' \n"; 
	else
		cout<<"does not match";


	return 0; 
} 


// C++ program to demonstrate working of regex_search() 
#include <iostream> 
#include <regex> 
#include<string.h> 
using namespace std; 

int main() 
{ 
	string output;
	// Target strings 
	string s1 = "D[0+0+0+0]+(D[(D[1+(D[2]+3)*4]-5)/6]+7)/8+9";
	string s2 = "jump 0*D[(0+(1*(2-(3*4*5)+(6*7-8)*(9-10*11))-(12*(13+14)+(15/16)*(17+18))/19)+20)%21]%22+23/24*25/26";
	string s3 = "jump 5, D[0] == 0"; 

	// An object of regex for pattern to be searched
	regex jump("(jump )(.*)");
	//<Term> Regex 
	regex t1("((\\d*)[*|//|%]*)*D\\[.+?]([*|//|%]*\\d*)*");
	regex t2("\\(+D\\[(.*)\\].*\\)+([*|//|%]*\\d*)*");
	regex t3("[+|-]*(\\d)*(([*|//|%]*\\d*))*[+|-]*");
	//<Factor> Regex
	regex f1("\\d*");
	regex f2("D\\[.+?]");
	// flag type for determining the matching behavior 
	// here it is for matches on 'string' objects 
	smatch m1;
	smatch m2;
	smatch m3;
	smatch m4;
	
	// regex_search() for searching the regex pattern 
	// 'r' in the string 's'. 'm' is flag for determining 
	// matching behavior. 
	// STATEMENT 1 
		
	regex_search(s1, m1, t1);
	output = m1.str(0);	
	cout << output << endl;
	s1.erase(0,output.length()+1); 
	if ( regex_match(output, f2) ){
		cout << "d[expr]" << endl;
		output.erase(0,2);
		output.pop_back();
		cout << output << endl;
	}
	
	regex_search(s1, m2, t2);
	output = m2.str(0);	
	cout << output << endl;
	s1.erase(0,output.length()+1);
	
	regex_search(s1, m3, t3);
	output = m3.str(0);
	cout << output << endl;
	/*
	// STATEMENT 2 
	if(regex_match(s2,jump))
		cout << "jump" << endl;
		s2.erase(0,5);	
	regex_search(s2, m1, t1);
	output = m1.str(0);	
	cout << output << endl;
	if(!output.empty())
		s2.erase(0,output.length()+1); 
	regex_search(s2, m2, t2);
	output = m2.str(0);	
	cout << output << endl;
	if(!output.empty())
		s2.erase(0,output.length()+1);
	regex_search(s2, m3, t3);
	output = m3.str(0);
	cout << output << endl;
	

	//STATEMENT 3 
	if(regex_match(s3,jump))
		cout << "jump" << endl;
		s3.erase(0,5);	
	regex_search(s3, m1, t1);
	output = m1.str(0);	
	cout << output << endl;
	if(!output.empty())
		s3.erase(0,output.length()+1); 
	regex_search(s3, m2, t2);
	output = m2.str(0);	
	cout << output << endl;
	if(!output.empty())
		s3.erase(0,output.length()+1);
	regex_search(s3, m3, t3);
	output = m3.str(0);
	cout << output << endl;*/
	
	//S1 T1 - FACTOR BREAKDOWN
	if ( regex_match(output, f1) ){
		cout << "number";
	}
	else if ( regex_match(output, f2) ){
		cout << "d[expr]" << endl;
		output.erase(0,2);
		output.pop_back();
		cout << output;
	}
	else{
		cout << "expr";
	}
	

} 


/****** SAMPLE OUTPUT ********
s1,t1: D[0+0+0+0]
s1,t2:(D[(D[1+(D[2]+3)*4]-5)/6]+7)/8
s1,t3: 9
s2,t1: 0*D[(0+(1*(2-(3*4*5)+(6*7-8)*(9-10*11))-(12*(13+14)+(15/16)*(17+18))/19)+20)%21]%22 
s2,t2: x
*/

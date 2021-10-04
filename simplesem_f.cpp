/***************************************
 * SIMPLESEM.cpp
 *
 * Student Name: ADARSH B SHANKAR
 * Student ID: 11972805
 * UCI Net ID: shankar1@uci.edu
 **************************************/

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <regex>
using namespace std;

class SIMPLESEM {
public:
  SIMPLESEM(string sourceFile);
  void runProgram();
  
  string inCode;
  char* text;
  ifstream codeIn;
  ifstream inputIn;
  ofstream outFile;
  static int nextChar;
  const int eof = -1;

  void printRule(const string);

  void parseProgram();
  void parseStatement();
  void parseSet();
  void parseJump();
  void parseJumpt();
  void parseExpr(string expr = "");
  void parseTerm(string t = "");
  void parseFactor(string f = "");
  void parseNumber();
};
SIMPLESEM::SIMPLESEM(string sourceFile) {
	this->codeIn.open(sourceFile);
	/*
	while (getline (codeIn, myText)) {
	this->parseProgram();
    // Output the text from the file
    cout << myText << endl;}
    codeIn.close();*/
    
    outFile.open(sourceFile + ".out");
}
void SIMPLESEM::printRule(string rule)
{
	outFile << rule << endl;
}

void SIMPLESEM::parseProgram()
{
	this->printRule("Program");
}
void SIMPLESEM::parseStatement()
{
	regex set("(set)(.*)");
	regex jump("(jump )(.*)");
	regex jumpt("(jumpt)(.*)");
	regex halt("(halt)");
	
	this->printRule("Statement");
	
	if(regex_match(inCode,set))
		this->parseSet();
	else if(regex_match(inCode,jump))
		this->parseJump();
	else if(regex_match(inCode,jumpt))
		this->printRule("Jumpt");
		//this->parseJumpt();
	else if(regex_match(inCode,halt))
		this->printRule("Statement");
	
}
void SIMPLESEM::parseJump()
{
	this->printRule("Jump");
	this->inCode.erase(0,5);
	parseExpr();
}
void SIMPLESEM::parseSet()
{
	this->printRule("Set");
	this->inCode.erase(0,4);
	parseExpr();
}

void SIMPLESEM::parseExpr(string expr)
{
	this->printRule("Expr");
	
	vector<string> terms;
	smatch m1,m2,m3;
	string output;
	
	regex t1("((\\d*)[*|//|%]*)*D\\[.+?]([*|//|%]*\\d*)*");
	regex t2("\\(+D\\[(.*)\\].*\\)+([*|//|%]*\\d*)*");
	regex t3("[+|-]*(\\d)*(([*|//|%]*\\d*))*[+|-]*");
	
	regex_search(this->inCode, m1, t1);
	output = m1.str();
	if(!output.empty())
		terms.push_back(output);
		this->inCode.erase(0,output.length()+1);
	
	regex_search(this->inCode, m2, t2);
	output = m2.str();
	if(!output.empty())
		terms.push_back(output);
		this->inCode.erase(0,output.length()+1);
	
	regex_search(this->inCode, m3, t3);
	output = m3.str();
	if(!output.empty())
		terms.push_back(output);
		this->inCode.erase(0,output.length()+1);
	
	if(!expr.empty()) {
		output = expr;
		for(int i = 0; i <output.length(); i++){
			if(output.at(i) == '+' || output.at(i) == '-')
			{
				continue;
			}
			else
			{
				string s(1,output.at(i));
				parseTerm(s);
			}
		}
	}
	
	for(auto v : terms)
		parseTerm(v);
}
void SIMPLESEM::parseTerm(string t)
{
	this->printRule("Term");
	parseFactor(t);
}
void SIMPLESEM::parseFactor(string f)
{
	vector<string> factors;
	string output = f;
	this->printRule("Factor");
	
	regex f1("\\d*");
	regex f2("D\\[.+?]");
	
	if ( regex_match(output, f2) ){
		output.erase(0,2);
		output.pop_back();
	}else if ( regex_match(output, f1) ){
		parseNumber();
	}
	else{
		printRule(output);
		parseExpr();
	}
	
	if(!f.empty())
	{
		for(int i = 0; i < f.length(); i++){
			if(f.at(i) == 'D'){
				
			}
		}
	}
}
void SIMPLESEM::parseNumber()
{
	this->printRule("Number");
}

/**********MAIN DRIVER CODE************/
int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cerr << "init: This program requires an input "
              << "filename as a parameter " << std::endl;
    exit(-2);
  }
  SIMPLESEM SS(argv[1]);
  SS.parseProgram();
  
  while (getline (SS.codeIn, SS.inCode)) {
	SS.parseStatement();
    // Output the text from the file
    cout << SS.inCode << endl;}
  SS.codeIn.close();
}

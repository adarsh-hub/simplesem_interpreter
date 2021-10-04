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
  
  //HELPER FUNCTIONS
  void match();
  void till();
};
SIMPLESEM::SIMPLESEM(string sourceFile) {
	this->codeIn.open(sourceFile);
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
		this->parseJumpt();
	else if(regex_match(inCode,halt))
		this->printRule("Statement");
	
}
void SIMPLESEM::parseJump()
{
	this->printRule("Jump");
	parseExpr();
}
void SIMPLESEM::parseJumpt()
{
	this->printRule("Jumpt");
	parseExpr();
}
void SIMPLESEM::parseSet()
{
	this->printRule("Set");
	parseExpr();
}

void SIMPLESEM::parseExpr(string expr)
{
	this->printRule("Expr");
}
void SIMPLESEM::parseTerm(string t)
{
	this->printRule("Term");
	parseFactor(t);
}
void SIMPLESEM::parseFactor(string f)
{
	this->printRule("Factor");
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

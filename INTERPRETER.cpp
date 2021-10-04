/***************************************
 * INTERPRETER.cpp
 *
 * Student Name: Adarsh B Shankar
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

#define DATA_SEG_SIZE 100

class INTERPRETER {
public:
  INTERPRETER(char *sourceFile);
  void runProgram();
  std::ifstream codeIn;
  std::ifstream inputIn;
  std::ofstream outFile;

  std::vector<std::string> C;
	int D[DATA_SEG_SIZE];
	int PC;
  std::string IR;
	bool run_bit;

  unsigned curIRIndex;

  void printDataSeg();

  void fetch();
  void incrementPC();
  void execute();

  // Output: used in the case of: set write, source
  void write(int source);

	// Input: used in the case of: set destination, read
  int read();

  /**
   * Checks and returns if the character c is found at the current
   * position in IR. If c is found, advance to the next
   * (non-whitespace) character.
   */
  bool accept(char c);

  /**
   * Checks and returns if the string s is found at the current
   * position in IR. If s is found, advance to the next
   * (non-whitespace) character.
   */
  bool accept(const char *s);

  /**
   * Checks if the character c is found at the current position in
   * IR. Throws a syntax error if c is not found at the current
   * position.
   */
  void expect(char c);

  /**
   * Checks if the string s is found at the current position in
   * IR. Throws a syntax error if s is not found at the current
   * position.
   */
  void expect(const char *s);

  void skipWhitespace();

  void parseStatement();
  void parseSet();
  void parseJump();
  void parseJumpt();
  int parseExpr();
  int parseTerm();
  int parseFactor();
  int parseNumber();

  void syntaxError();
};


INTERPRETER::INTERPRETER(char *sourceFile) {
  codeIn.open(sourceFile, std::fstream::in);
  if (codeIn.fail()) {
    std::cerr << "init: Errors accessing source file "
              << sourceFile << std::endl;
    exit(-2);
  }

  inputIn.open("input.txt", std::fstream::in);
  if (inputIn.fail()) {
    std::cerr << "init: Errors accessing input file input.txt" << std::endl;
    exit(-2);
  }

  outFile.open((std::string(sourceFile) + ".out").c_str(), std::fstream::out);
  if (outFile.fail()) {
    std::cerr << "init: Errors accessing output file "
              << std::string(sourceFile) + ".out" << std::endl;
    exit(-2);
  }

  // Initialize the SIMPLESEM processor state
  // Initialize the Code segment
  while (codeIn.good()) {
    std::string line;
    std::getline(codeIn, line);
    C.push_back(line);
  }

  // Initialize the Data segment
  for (int i=0; i<DATA_SEG_SIZE ;i++) {
    D[i]=0;
  }
  PC = 0; // Every SIMPLESEM program begins at instruction 0
  run_bit = true; // Enable the processor
}

void INTERPRETER::runProgram() {
	while(this->run_bit)
	{
		this->fetch();
		this->incrementPC();
		this->execute();
    }
		
  //}
  printDataSeg();
}

void INTERPRETER::printDataSeg() {
  outFile << "Data Segment Contents" << std::endl;
  for(int i=0; i < DATA_SEG_SIZE; i++)
  {
    outFile << i << ": " << D[i] << std::endl;
  }
}

void INTERPRETER::fetch() {
  //std::cerr << "Fetch" << std::endl;
  this->IR = this->C[this->PC];
  this->curIRIndex = 0;
  //std::cerr << this->IR << std::endl;
}

void INTERPRETER::incrementPC() {
  //std::cerr << "Increment" << std::endl;
  this->PC++;
}

void INTERPRETER::execute() {
  //std::cerr << "Execute" << std::endl;
  this->parseStatement();
}

//Output: used in the case of: set write, source
void INTERPRETER::write(int source){
  outFile << source << std::endl;
}

//Input: used in the case of: set destination, read
int INTERPRETER::read() {
  int value;
  inputIn >> value;
  return value;
}

/**
 * Checks and returns if the character c is found at the current
 * position in IR. If c is found, advance to the next
 * (non-whitespace) character.
 */
bool INTERPRETER::accept(char c) {
  if (curIRIndex >= IR.length())
    return false;

  if (IR[curIRIndex] == c) {
    curIRIndex++;
    skipWhitespace();
    return true;
  } else {
    return false;
  }
}

/**
 * Checks and returns if the string s is found at the current
 * position in IR. If s is found, advance to the next
 * (non-whitespace) character.
 */
bool INTERPRETER::accept(const char *s) {
  unsigned s_len = strlen(s);

  if (curIRIndex+s_len > IR.length())
    return false;

  for (unsigned i = 0; i < s_len; ++i) {
    if (IR[curIRIndex+i] != s[i]) {
      return false;
    }
  }

  curIRIndex += s_len;
  skipWhitespace();
  return true;
}

/**
 * Checks if the character c is found at the current position in
 * IR. Throws a syntax error if c is not found at the current
 * position.
 */
void INTERPRETER::expect(char c) {
  if (!accept(c))
    syntaxError();
}

/**
 * Checks if the string s is found at the current position in
 * IR. Throws a syntax error if s is not found at the current
 * position.
 */
void INTERPRETER::expect(const char *s) {
  if (!accept(s))
    syntaxError();
}

void INTERPRETER::skipWhitespace() {
  while (curIRIndex < IR.length() && isspace(IR[curIRIndex]))
    curIRIndex++;
}

void INTERPRETER::parseStatement() {
  std::cerr << "Statement" << std::endl;

  if (accept("halt"))
  	this->run_bit = false;
  else if(accept("set"))
    parseSet();
  else if(accept("jumpt"))
    parseJumpt();
  else if(accept("jump"))
    parseJump();
}

void INTERPRETER::parseSet() {
  std::cerr << "Set" << std::endl;
  std::cerr << this->IR << std::endl;
  std::cerr << this->curIRIndex << std::endl;
  int exp;
  int exp_source;
  int exp_dest;
  
  size_t found_w = this->IR.find("write");
  size_t found_r = this->IR.find("read");
  
  bool is_write = false;
  
  if((found_w == std::string::npos) && (found_r == std::string::npos)){
  	exp_dest = parseExpr();
  	expect(',');
  	exp_source = parseExpr();
  	this->D[exp_dest] = exp_source;
  	std::cerr << "<SET> D[" << exp_dest << "]=" << this->D[exp_dest] << std::endl;
  	return;
  }
  	
  if (!accept("write") && this->curIRIndex <= IR.length())
    exp = parseExpr();
  if (found_w != std::string::npos){
  	std::cerr << "WRITE? :" << this->IR << std::endl;
  	is_write = true;
  	std::cerr << "write accepted" << std::endl;}
  	
    
  expect(',');

  if (!accept("read") && this->curIRIndex <= IR.length())
    exp = parseExpr();
  if (found_r != std::string::npos){
  	std::cerr << "READ? :" << this->IR << std::endl;
    this->D[exp] = read();
  	std::cerr << "D[" << exp << "]=" << this->D[exp] << std::endl;
  	std::cerr << "read accepted" << std::endl;}
  
  	
  if(is_write){
  	write(exp);
  }
}

int INTERPRETER::parseExpr() {
  std::cerr << "Expr" << std::endl;
  std::cerr << this->IR << std::endl;
  std::cerr << "Index is: "<< this->curIRIndex << std::endl;
  std::cerr << "<Exp> : " << this->IR.at(this->curIRIndex) << std::endl;
  int term1, term2;
  
  if(accept("2 *")){
    std::cerr << "Yes 2 * occurs" << std::endl;
  	term1 = 2 * parseTerm();
  }
  else{
  	term1 = parseTerm();
  }
  
  
  if (accept('+')){
    	term2 = parseTerm();
  		std::cerr << "<Term 1> : " << term1 << std::endl;
  		std::cerr << "<Term 2> : " << term2 << std::endl;
    	term1 += term2;
  		std::cerr << "<Term 1> + <Term2>: " << term1 << std::endl;
  		std::cerr << "<FINAL Expr> : " << term1 << std::endl;
		return term1;
  }
  else if (accept('-')){
    	term2 = parseTerm();
  		std::cerr << "<Term 1> : " << term1 << std::endl;
  		std::cerr << "<Term 2> : " << term2 << std::endl;
    	term1 -= term2;
  		std::cerr << "<Term 1> - <Term2>: " << term1 << std::endl;
  		std::cerr << "<FINAL Expr> : " << term1 << std::endl;
		return term1;
  }
  
    std::cerr << "<FINAL Expr> : " << term1 << std::endl;
	return term1;
}

int INTERPRETER::parseTerm() {
  std::cerr << "Term" << std::endl;
  std::cerr << this->IR << std::endl;
  std::cerr << "Index is: "<< this->curIRIndex << std::endl;
  if(this->curIRIndex <= 12){
  std::cerr << "<Term> : " << this->IR.at(this->curIRIndex) << std::endl;
  }

  int f1 = parseFactor();
  int f2;

  if (accept('*')){
  		f2 = parseFactor();
  		std::cerr << "<Factor 1> : " << f1 << std::endl;
  		std::cerr << "<Factor 2> : " << f2 << std::endl;
    	f1 = f1*f2;
  		std::cerr << "<Factor 1> * <Factor>: " << f1 << std::endl;
  }
  else if (accept('/')){
  		f2 = parseFactor();
  		std::cerr << "<Factor 1> : " << f1 << std::endl;
  		std::cerr << "<Factor 2> : " << f2 << std::endl;
    	f1 = f1/f2;
  		std::cerr << "<Factor 1> / <Factor>: " << f1 << std::endl;
  }
  else if (accept('%')){
  		f2 = parseFactor();
  		std::cerr << "<Factor 1> : " << f1 << std::endl;
  		std::cerr << "<Factor 2> : " << f2 << std::endl;
    	f1 = f1%f2;
  		std::cerr << "<Factor 1> % <Factor>: " << f1 << std::endl;
	}
	
	std::cerr << "<FINAL Term> : " << f1 << std::endl;
    return f1;
}

int INTERPRETER::parseFactor() {
  std::cerr << "Factor" << std::endl;
  std::cerr << this->IR << std::endl;
  std::cerr << "Index is: "<< this->curIRIndex << std::endl;
  if(this->curIRIndex <= 12){
  std::cerr << "<Factor> : " << this->IR.at(this->curIRIndex) << std::endl;
  }
  int value;
  bool data_seg = false;

  if (accept("D[")) {
    value = this->D[parseExpr()];

    expect(']');
  } else if (accept('(')) {
    value = parseExpr();

    expect(')');
  } else {
    value = parseNumber();
  }
  
  std::cerr << "<FINAL Factor> : " << value << std::endl;
  return value;
}

int INTERPRETER::parseNumber() {
  std::cerr << "Number" << std::endl;
  std::cerr << this->IR << std::endl;
  std::cerr << "Index is: "<< this->curIRIndex << std::endl;
  std::cerr << "Length is: "<< IR.length() << std::endl;
  int number;
  std::string double_digit;
  
  std::string triple_digit;
  //Check for triple digits
  if(curIRIndex == IR.length()-3 && (isdigit(IR[curIRIndex+1])) && (isdigit(IR[curIRIndex+2]))){
  		char d1 = IR.at(curIRIndex);
  		curIRIndex = curIRIndex + 1;
  		char d2 = IR.at(curIRIndex);
  		curIRIndex = curIRIndex + 1;
  		char d3 = IR.at(curIRIndex);
  		triple_digit += d1;
		triple_digit += d2;
		triple_digit += d3;
		number = stoi(triple_digit);
  		std::cerr << "<FINAL NumberT> : " << number << std::endl;
		return number; 
  	//std::cerr << "<NumberT> : " << this->IR.at(this->curIRIndex)<< this->IR.at(this->curIRIndex + 1) << this->IR.at(this->curIRIndex + 2)<< std::endl;
  }
  
  //Check for double digits
  if((curIRIndex < IR.length()-1 && IR[curIRIndex] != '0') && (isdigit(IR[curIRIndex])) ){
  		char d1 = IR.at(curIRIndex);
  		curIRIndex = curIRIndex + 1;
  		char d2 = IR.at(curIRIndex);
  		double_digit += d1;
		double_digit += d2;
		number = stoi(double_digit);
  		std::cerr << "<FINAL NumberD> : " << number << std::endl;
		return number; 
  }
  
  
  if (curIRIndex >= IR.length())
    syntaxError();

  if (IR[curIRIndex] == '0') {
  	number = this->IR.at(this->curIRIndex) - 48 ;
    curIRIndex++;
    skipWhitespace();
    return number;
  } else if (isdigit(IR[curIRIndex])) {
    while (curIRIndex < IR.length() &&
           isdigit(IR[curIRIndex])) {
  	  number = this->IR.at(this->curIRIndex) - 48;
      curIRIndex++;
    }
    skipWhitespace();
  	std::cerr << "<FINAL NumberS> : " << number << std::endl;
    return number;
  } else {
    syntaxError();
  }
}

void INTERPRETER::parseJump() {
  std::cerr << "Jump" << std::endl;

  this->PC = parseExpr();
  std::cerr << "<Jump Index> : " << this->PC << std::endl;
}

//<Jumpt>-> jumpt <Expr>, <Expr> ( != | == | > | < | >= | <= ) <Expr>
void INTERPRETER::parseJumpt() {
  std::cerr << "Jumpt" << std::endl;

  int new_pc = parseExpr();
  expect(',');

  int exp1 = parseExpr();
  std::cerr << "<Exp1>: " << exp1 << std::endl;
	
  if (accept("!=")) {
    int exp2 = parseExpr();
    std::cerr << "<Exp2>: " << exp2 << std::endl;
    if(exp1 != exp2){
    	this->PC = new_pc;
	}
  }
  else if(accept("==")){
    int exp2 = parseExpr();
    std::cerr << "<Exp2>: " << exp2 << std::endl;
    if(exp1 == exp2){
    	this->PC = new_pc;
	}
  }
  else if(accept(">=")){
    int exp2 = parseExpr();
    std::cerr << "<Exp2>: " << exp2 << std::endl;
    if(exp1 >= exp2){
    	this->PC = new_pc;
	}
  }
  else if(accept("<=")){
    int exp2 = parseExpr();
    std::cerr << "<Exp2>: " << exp2 << std::endl;
    if(exp1 <= exp2){
    	this->PC = new_pc;
	}
  }
  else if(accept('>')){
    int exp2 = parseExpr();
    std::cerr << "<Exp2>: " << exp2 << std::endl;
    if(exp1 > exp2){
    	this->PC = new_pc;
	}
  }
  else if(accept('<')){
    int exp2 = parseExpr();
    std::cerr << "<Exp2>: " << exp2 << std::endl;
    if(exp1 < exp2){
    	this->PC = new_pc;
	}
  }  
  else {
    syntaxError();
  }
}

void INTERPRETER::syntaxError() {
  std::cerr << "Syntax Error!" << std::endl;
  exit(-1);
}

int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cerr << "init: This program requires an input "
              << "filename as a parameter " << std::endl;
    exit(-2);
  }
  INTERPRETER i(argv[1]);
  i.runProgram();
}

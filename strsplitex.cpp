#include <iostream>
#include <regex>
#include <string>
using namespace std;

int main()
{
	vector<string> terms;
	string s = "jump D[0+0+0+0]+(D[(D[1+(D[2]+3)*4]-5)/6]+7)/8+9";
	regex r("\\+");

	vector<string> out(
					sregex_token_iterator(s.begin(), s.end(), r, -1),
					sregex_token_iterator()
					);

	for (auto s: out) {
		cout << s << '\n';
	}

	return 0;
}

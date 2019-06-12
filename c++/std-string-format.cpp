#include <iostream>
#include <string>
#include <vector>
using namespace std;

static std::string __format(const char *pszFmt, ...)
{
	std::string str;
	va_list args;
	va_start(args, pszFmt);
	{
		int nLength = _vscprintf(pszFmt, args);
		nLength += 1; //add again
		std::vector<char> vectorChars(nLength);
		_vsnprintf(vectorChars.data(), nLength, pszFmt, args);
		str.assign(vectorChars.data());
	}
	va_end(args);
	return str;
}
int main(int argc, char *argv[]){

	std::string a = __format("this %s"," a test");
	return 0;
}


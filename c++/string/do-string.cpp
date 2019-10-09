#include <iostream>
#include <string>

using namespace std;

string trim(string src) {
	unsigned int sp = src.find_first_not_of(" ");//在字符串中查找第一个与str中的字符都不匹配的字符，返回它的位置。搜索从index开始。如果没找到就返回string::nops
	unsigned int ep = src.find_last_not_of(" ");//反向找
	if (sp == string::npos || ep == string::npos) {
		return "";
	}
	else {
		return src.substr(sp, ep - sp + 1);
	}
}
void split(pair<string, string>& hp, string src, char delim) {
	hp.first = "";
	hp.second = "";
	unsigned int p = src.find(delim);
	if (p == string::npos) {
		hp.first = src;
		hp.second = "";
	}
	else {
		hp.first = trim(src.substr(0, p));
		hp.second = trim(src.substr(p + 1));
	}
}

void testStr(string s) {
	printf("%s", s.c_str());
}


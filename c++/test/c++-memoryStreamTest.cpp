#include <locale>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <string.h>
#include <sstream>
using namespace std;
 
void TestMemoryStream()
{
	unsigned char buf[] = {0xf1,0x0,0xfe,0x00};
	string str((char*)buf,4);
	size_t str_size = str.size();
	cout << "str_size: " << str_size << endl;
	cout << (int)str[1] << endl;
 
	istringstream iss(str,ios_base::in);
	iss.seekg(0,ios_base::beg);
	char buffer[4];
	memset(buffer,0,sizeof(buffer));
	iss.read(buffer,1);
	cout << (int)(buffer[0] & 0xff) << endl;
 
	iss.seekg(0,ios_base::end);
	size_t last_pos = iss.tellg();
 
	cout << "stream size: " << last_pos  << endl;
 
}
 
//http://www.cplusplus.com/reference/ios/
int main(int argc, char const *argv[])
{
	//1.测试内存流.
	TestMemoryStream();
 
	//2.文件流->内存流->文件流.
	//ifstream->ostringstream->istringstream->ofstream
	cout << argv[0] << endl;
	ifstream is(argv[0],ios_base::binary);
	ostringstream oss(ios_base::binary);
 
	streamsize ss_size = 0;
	char buf[65];
	cout << "begin read is" << endl;
	while(!is.eof())
	{
		is.read(buf,64);
		ss_size = is.gcount();
		if(ss_size)
		{
			oss.write(buf,ss_size);
		}
	}
 
	cout << "end read is" << endl;
	istringstream iss(oss.str(),ios_base::binary);
	iss.seekg(0,ios_base::beg);
 
	string str_path(argv[0]);
	str_path.append(".bak");
	ofstream ioo(str_path.c_str(),ios_base::binary);
	
	cout << "start read iss" << endl;
	while(!iss.eof())
	{
		iss.read(buf,64);
		ss_size = iss.gcount();
		if(ss_size)
		{
			ioo.write(buf,ss_size);
		}
	}
	cout << "end read iss" << endl;
 
	return 0;
}


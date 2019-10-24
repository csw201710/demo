
#ifndef _BASE64_H_
#define _BASE64_H_
#include <string>
using namespace std;

class Base64
{
private:
  static string part_encode(const string& subplain);
  static string part_decode(const string& subCrypted);
  static char getValue(char ch);
public:
  static string encode(string plain);
  static string decode(string crypted);
};

#endif // _BASE64_H_

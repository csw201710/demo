#ifndef _D_EXCEPTION_H_
#define _D_EXCEPTION_H_

#include <string>
#include <stdarg.h>

using namespace std;

class Exception {
private:
	string msg;
protected:
	void setMsg(string msgsrc, ...) {
		va_list ap;
		va_start(ap, msgsrc);
		char buf[256];
		vsnprintf(buf, sizeof(buf), msgsrc.c_str(), ap);
		msg = buf;
		va_end(ap);
	}
public:
	Exception() {}
	virtual ~Exception() {}
	string getMsg() { return msg; }
};

class DlAbortEx :public Exception {
public:
	DlAbortEx() :Exception() {}
	DlAbortEx(string msg, ...) :Exception() {
		va_list ap;
		va_start(ap, msg);
		setMsg(msg, ap);
		va_end(ap);
	}
};
void testExcept() {
	try {
		throw  DlAbortEx("this  is a test");
	}
	catch (Exception e) {
		printf("%s", e.getMsg().c_str());

	}
}
#endif // _D_EXCEPTION_H_


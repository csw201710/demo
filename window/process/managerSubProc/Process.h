#ifndef __PROCESS_H_00A__
#define __PROCESS_H_00A__

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

class CProcess
{
public:
	CProcess(const char* fileName, const char * args = 0, int isHidden = 0);
	
	int start();
	int stop();
	void restart();

	virtual ~CProcess();
private:
	const char *m_fileName;
	const char* m_args;
	int m_isHidden;
	

	void * m_hProcess;
};


void testProc();

#endif

#pragma once
class SM4Handler
{
public:
	SM4Handler();
	virtual ~SM4Handler();

	int do_encrypt(unsigned char * inBuf, int inBufLen,
		unsigned char *outBuf, int *outBufLen, int do_encrypt);


};


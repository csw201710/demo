#pragma once
class SM3Handler
{
public:
	SM3Handler();
	~SM3Handler();
	int calcHash(char* inData, int inDataLen, unsigned char* outData, int * poutDataLen);
};


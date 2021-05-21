#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef ERROR
#define ERROR(...) \
	do{\
	  printf("[ERROR  ] ");\
		printf(__VA_ARGS__);\
		printf("\r\n");\
	}while(0);
#endif




static int _hexchar2int(unsigned char c)
{
	switch (c) {
	case '0':
		return 0;
	case '1':
		return 1;
	case '2':
		return 2;
	case '3':
		return 3;
	case '4':
		return 4;
	case '5':
		return 5;
	case '6':
		return 6;
	case '7':
		return 7;
	case '8':
		return 8;
	case '9':
		return 9;
	case 'a': case 'A':
		return 0x0A;
	case 'b': case 'B':
		return 0x0B;
	case 'c': case 'C':
		return 0x0C;
	case 'd': case 'D':
		return 0x0D;
	case 'e': case 'E':
		return 0x0E;
	case 'f': case 'F':
		return 0x0F;
	}
	return -1;
}

unsigned char *hexstr2buf(const char *str, int *len)
{
	unsigned char *hexbuf, *q;
	unsigned char ch, cl;
	int chi, cli;
	const unsigned char *p;
	size_t s;

	s = strlen(str);
	if ((hexbuf = (unsigned char*)calloc(s >> 1, sizeof(char))) == NULL) {
		ERROR("calloc failed");
		return NULL;
	}
	for (p = (const unsigned char *)str, q = hexbuf; *p; ) {
		ch = *p++;
		cl = *p++;
		if (!cl) {
			ERROR("CRYPTO_R_ODD_NUMBER_OF_DIGITS");
			free(hexbuf);
			return NULL;
		}
		cli = _hexchar2int(cl);
		chi = _hexchar2int(ch);
		if (cli < 0 || chi < 0) {
			free(hexbuf);
			ERROR("CRYPTO_R_ILLEGAL_HEX_DIGIT");
			return NULL;
		}
		*q++ = (unsigned char)((chi << 4) | cli);
	}

	if (len)
		*len = q - hexbuf;
	return hexbuf;
}

char *buf2hexstr(const unsigned char *buffer, long len)
{
	const static char hexdig[] = "0123456789ABCDEF";
	char *tmp, *q;
	const unsigned char *p;
	int i;

	if (len == 0)
	{
		return (char*)calloc(1, sizeof(char));
	}

	if ((tmp = (char*)calloc(len * 3, sizeof(char))) == NULL) {
		ERROR("calloc failed");
		return NULL;
	}
	q = tmp;
	for (i = 0, p = buffer; i < len; i++, p++) {
		*q++ = hexdig[(*p >> 4) & 0xf];
		*q++ = hexdig[*p & 0xf];
	}



	return tmp;
}


void testHEX() {
	void * aabb = (void*)0x11223344;
	printf("aabb1:[%x]\r\n", aabb);
	char* hex1 = buf2hexstr((unsigned char*)&aabb, sizeof(void*));
	printf("hex1:[%s]\r\n", hex1);
	int len = 0;
	unsigned char* d = hexstr2buf(hex1, &len);
	void* aabb2 = (void*)&d;
	printf("aabb2:[%x]\r\n", aabb2);

}





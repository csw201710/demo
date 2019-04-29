#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <memory.h>
#include <string>
#include <memory>

#include <iostream>
#include <fstream>
#include <sstream>

#include "ZlibDecompressionStream.h"

#include <memory>
using namespace std;

static void print_arr(const char *string, unsigned char*data, int size)
{
	int i;

	printf("---------------------------------------------\n");
	printf("%s len=%d\n", string, size);
	for (i = 0; i<size; i++)
	{
		printf("%02x ", data[i]);
		if ((i % 16) == 15)
			printf("\n");
	}
	if (size % 16)
		printf("\n");
	printf("---------------------------------------------\n");
}
typedef unsigned char byte;
byte* textFileRead(char* filename, long* size)
{
	byte* text;
	FILE *pf = fopen(filename, "rb");
	fseek(pf, 0, SEEK_END);
	long lSize = ftell(pf);
	*size = lSize;
	// 用完后需要将内存free掉
	text = (byte*)calloc(lSize + 1, sizeof(byte));
	rewind(pf);
	fread(text, 1, lSize, pf);
	text[lSize] = '\0';
	return text;
}

void getmem(byte ** temp, unsigned int size) {
	*temp = (byte *)calloc(size, sizeof(byte));
}


int parse_BRICKED(char * CET_ArchivePath) {
	std::ifstream  in(CET_ArchivePath, ios::in | ios::binary);
	//std::ifstream  in("d:\\test002-ARCHIVE.dat", ios::in | ios::binary);

	int filecount = 0, filenumber = 0;
	int hasRead = in.read((char*)&filecount, sizeof(filecount)).gcount();



	CZlibDecompressionStream z(in, true);

	while (filenumber++ < filecount) {
		Bytef * temp = 0;
		unsigned int size,rawSize;
		char fileName[_MAX_PATH] = { 0 };
		char folderName[_MAX_PATH] = { 0 };
		//get the filename

		z.Read((Bytef*)&size, sizeof(size));
		getmem(&temp, size + 1);
		z.Read((Bytef*)temp, size);
		temp[size] = '\0';
		memcpy(fileName, temp, size);
		printf("filename : %s\n", temp);
		free(temp);

		//get the folder
		z.Read((Bytef*)&size, sizeof(size));
		getmem(&temp, size + 1);
		z.Read((Bytef*)temp, size);
		temp[size] = '\0';
		memcpy(folderName, temp, size);
		printf("folder : %s\n", temp);
		free(temp);
		//fileSize
		z.Read((Bytef*)&rawSize, sizeof(rawSize));
		std::shared_ptr<Bytef> sp_rawBuf(new Bytef[rawSize]);
		
		Bytef *rawBuf = sp_rawBuf.get();
		z.Read(rawBuf, rawSize);
		//free(buf);

		if (_strcmpi(fileName, "CET_TRAINER.CETRAINER") == 0) {
			printf("[+] decode %s\n", fileName);
			int err;

			long filesize = rawSize;
			char * m = (char*)rawBuf;

			if (_strnicmp("<?XML", (char*)m, 5) == 0) {
				printf("find xml,break \n");
				break;
			}

			if (m == NULL || filesize <= 0) {
				printf("textFileRead failed");
				break;
			}
			for (int i = 2; i <= filesize - 1; i++) {
				m[i] ^= m[i - 2];
			}
			for (int i = filesize - 2; i >= 0; i--) {
				m[i] ^= m[i + 1];
			}
			//byte k = 0xce; //标准数据可以解密 ok
			byte k = 0xca;
			for (int i = 0; i <= filesize - 1; i++) {
				m[i] ^= k;
				k = k + 5;
				//k++;
			}
			byte *p = (byte*)m;

			string str((char*)m, filesize);
			istringstream in2(str, ios_base::in | ios_base::binary);
			in2.seekg(0, ios_base::beg);

			std::fstream out("BRICKED_out.CT", ios::out | ios::binary | ios::trunc);
			//const char *tag = "CHEAT";//"BRICKED"
			Bytef* buf;
			/*
			if (memcmp(p, "CHEAT", strlen("CHEAT")) == 0) {
				CZlibDecompressionStream z_dec(in2, true);
				char p0[5];
				in2.read(p0, 5);
				size = 0;
				err = z_dec.Read((Bytef*)&size, sizeof(size));
				if (err > 0) {
					getmem(&buf, size);
					if (z_dec.Read(buf, size) > 0) {
						out.write((char*)buf, size);
					}

				}

			}*/
			if (memcmp(p, "BRICKED", strlen("BRICKED")) == 0) {
				char p0[16] = { 0 };
				in2.read(p0, 10);
				memset(p0, 0x00, sizeof(p0));

				CZlibDecompressionStream z_dec(in2, true);
				size = 1024;

				getmem(&buf, size);
				int ActuallyRead = 1024;

				while (ActuallyRead > 0) {
					ActuallyRead = z_dec.Read(buf, size);

					if (ActuallyRead > 0) {
						out.write((char*)buf, ActuallyRead);
					}

				}
				free(buf);
			}
			else {
				CZlibDecompressionStream z_dec(in2, true);
				size = 1024;

				getmem(&buf, size);
				int ActuallyRead = 1024;

				while (ActuallyRead > 0) {
					ActuallyRead = z_dec.Read(buf, size);

					if (ActuallyRead > 0) {
						out.write((char*)buf, ActuallyRead);
					}

				}
				free(buf);
			}
			out.close();


			break;
		}
		
	}
	return 0;
}

int main() {
	parse_BRICKED("d:\\CET_Archive.dat");
}


/*
int main_ok1() {
	std::ifstream  in("d:\\CET_Archive.dat", ios::in | ios::binary);
	//std::ifstream  in("d:\\test002-ARCHIVE.dat", ios::in | ios::binary);

	int filecount = 0, filenumber = 0;
	int hasRead = in.read((char*)&filecount, sizeof(filecount)).gcount();



	CZlibDecompressionStream z(in, true);

	while (filenumber++ < filecount) {
		Bytef * temp = 0;
		unsigned int size;
		char fileName[_MAX_PATH] = { 0 };
		char folderName[_MAX_PATH] = { 0 };
		//get the filename

		z.Read((Bytef*)&size, sizeof(size));
		getmem(&temp, size + 1);
		z.Read((Bytef*)temp, size);
		temp[size] = '\0';
		memcpy(fileName, temp, size);
		printf("filename : %s\n", temp);
		free(temp);

		//get the folder
		z.Read((Bytef*)&size, sizeof(size));
		getmem(&temp, size + 1);
		z.Read((Bytef*)temp, size);
		temp[size] = '\0';
		memcpy(folderName, temp, size);
		printf("folder : %s\n", temp);
		free(temp);
		//fileSize

		z.Read((Bytef*)&size, sizeof(size));
		{
			fstream f1;
			Bytef *buf = new Bytef[size];
			f1.open(fileName, ios::out | ios::binary);
			z.Read(buf, size);
			f1.write((char*)buf, size);
			f1.close();
			free(buf);
		}
		if (strcmp(fileName, "CET_TRAINER.CETRAINER") == 0) {
			char * decFileName = "decCt_001.tmp";
			char * decFileName2 = "decCt_out.txt";
			int err;
			long filesize = 0;
			//byte * m = textFileRead("d:\\CET_TRAINER.CETRAINER", &filesize);
			char * m = (char*)textFileRead(fileName, &filesize);

			if (_strnicmp("<?XML", (char*)m, 5) == 0) {
				printf("find xml,break \n");
				break;
			}

			if (m == NULL || filesize <= 0) {
				printf("textFileRead failed");
				break;
			}
			for (int i = 2; i <= filesize - 1; i++) {
				m[i] ^= m[i - 2];
			}
			for (int i = filesize - 2; i >= 0; i--) {
				m[i] ^= m[i + 1];
			}
			//byte k = 0xce; //标准数据可以解密 ok
			byte k = 0xca;
			for (int i = 0; i <= filesize - 1; i++) {
				m[i] ^= k;
				k = k + 5;
				//k++;
			}
			byte *p = (byte*)m;

			std::fstream f1(decFileName, ios::out | ios::binary | ios::trunc);
			f1.write((char*)m, filesize);
			f1.close();


			std::ifstream  in2(decFileName, ios::in | ios::binary);
			std::fstream out(decFileName2, ios::out | ios::binary | ios::trunc);
			//const char *tag = "CHEAT";//"BRICKED"
			Bytef* buf;
			if (memcmp(p, "CHEAT", strlen("CHEAT")) == 0) {
				CZlibDecompressionStream z_dec(in2, true);
				char p0[5];
				in2.read(p0, 5);
				size = 0;
				err = z_dec.Read((Bytef*)&size, sizeof(size));
				if (err > 0) {
					getmem(&buf, size);
					if (z_dec.Read(buf, size) > 0) {
						out.write((char*)buf, size);
					}

				}

			}
			else if (memcmp(p, "BRICKED", strlen("BRICKED")) == 0) {
				char p0[16] = { 0 };
				in2.read(p0, 10);
				memset(p0, 0x00, sizeof(p0));

				CZlibDecompressionStream z_dec(in2, true);
				size = 1024;

				getmem(&buf, size);
				int ActuallyRead = 1024;

				while (ActuallyRead > 0) {
					ActuallyRead = z_dec.Read(buf, size);

					if (ActuallyRead > 0) {
						out.write((char*)buf, ActuallyRead);
					}

				}
				free(buf);
			}
			else {
				CZlibDecompressionStream z_dec(in2, true);
				size = 1024;

				getmem(&buf, size);
				int ActuallyRead = 1024;

				while (ActuallyRead > 0) {
					ActuallyRead = z_dec.Read(buf, size);

					if (ActuallyRead > 0) {
						out.write((char*)buf, ActuallyRead);
					}

				}
				free(buf);
			}
			out.close();
			f1.close();
			in2.close();

			free(m);

			break;
		}

	}
	return 0;
}
*/



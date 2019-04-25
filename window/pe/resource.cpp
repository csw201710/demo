#include "log.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <memory.h>
#include <iostream>
#include <string>
#include <memory>

using namespace std;

#if 0


#include "ZlibDecompressionStream.h"

#include "zlib.h"

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
	fread(text, sizeof(byte), lSize, pf);
	text[lSize] = '\0';
	return text;
}

int Zip(void *out,unsigned int *outLen ,void * in,unsigned int inLen) {
	int ret = 0;
	z_stream strm;
	uInt allOutLen = *outLen;
	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;
	ret = deflateInit(&strm, -1);//Initializes the internal stream state for compression. 
	if (ret != Z_OK)
		return ret;

	strm.avail_in = inLen;
	strm.next_in = (Bytef*)in;
	do {
		strm.avail_out = *outLen;
		strm.next_out = (Bytef*)out;
		ret = deflate(&strm, Z_FINISH);    /* no bad return value */// Z_FINISH
		assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
		uInt have = allOutLen - strm.avail_out;
		*outLen = have;
	} while (false);
	(void)deflateEnd(&strm);
	return 0;
}

int unZip(void *out, unsigned int *outLen, void * in, unsigned int inLen) {
	int ret = 0;
	z_stream strm;
	uInt allOutLen = *outLen;

	/* allocate inflate state */
	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;
	strm.avail_in = 0;
	strm.next_in = Z_NULL;
	ret = inflateInit(&strm);
	if (ret != Z_OK)
		return ret;

	/* decompress until deflate stream ends or end of file */
	do {
		strm.avail_in = inLen;

		if (strm.avail_in == 0)
			break;
		strm.next_in = (Bytef*)in;

		/* run inflate() on input until output buffer not full */
		do {
			strm.next_out = (Bytef*)out;
			strm.avail_out = *outLen;
			
			ret = inflate(&strm, 0);//Z_FINISH
			assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
			switch (ret) {
			case Z_NEED_DICT:
				ret = Z_DATA_ERROR;     /* and fall through */
			case Z_DATA_ERROR:
			case Z_MEM_ERROR:
				(void)inflateEnd(&strm);
				return ret;
			}
			uInt have = allOutLen - strm.avail_out;
			*outLen = have;
		} while (false);

		/* done when inflate() says it's done */
	} while (ret != Z_STREAM_END);

	/* clean up and return */
	(void)inflateEnd(&strm);
	return ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR;
}


void getmem(byte ** temp, unsigned int size) {
	*temp = (byte *)calloc(size, sizeof(byte));
}

int maintesst() {
	int err;
	int filenumber = 0;
	long size = 0;
	byte * m = textFileRead("d:\\xxx.dat", &size);
	int filecount = *(int *)m;
	m += sizeof(int);

	byte *uncompr = new byte[size];
	uInt uncomprLen = size;
	if(0)
	{
		byte txt[] = "abcd";
		byte enc[1024] ;
		byte dec[1024] = {0};
		uInt encLen = sizeof(enc);
		uInt decLen = sizeof(dec);
		err = Zip(enc, &encLen, txt, strlen((char*)txt));
		err = unZip(dec,&decLen,enc, encLen);
		dec[decLen] = 0;
		printf("err:%d ,%s\n", err,dec);

	}
	CZlibDecompressionStream obj;
	obj.Create(m, size);
	int fileNameSize = 0;
	obj.Read((Bytef*)&fileNameSize, sizeof(fileNameSize));
	
	//err = uncompress(uncompr, &uncomprLen, m, 4);
	for (int i = 1;i <= 1024*10;i++) {
		uncomprLen = 100;
		err = unZip(uncompr, &uncomprLen, m, i);
		if (err == -3) {
			continue;
		}
		printf("i :%d\n", i);
		exit(0);
	}
	err = unZip(uncompr, &uncomprLen, m, 9);
	if (err != Z_OK)
	{
		cerr << "解压错误: " << err << '\n ';
		return 0;
	}

	while (filenumber++ < filecount) {
		byte *temp = 0;
		byte * folder = 0;
		byte * filename = 0;

		//get the filename
		unsigned int size = *(unsigned int *)m;
		m += sizeof(unsigned int);
		getmem(&temp, size + 1);
		memcpy(temp, m, size);
		m += size;
		filename = temp;
		free(temp);
		//get the folder
		size = *(unsigned int *)m;
		m += sizeof(unsigned int);
		getmem(&temp, size + 1);
		memcpy(temp, m, size);
		m += size;
		folder = temp;
		free(temp);
		if (folder != 0) {

		}
		//get file size
		int filesize = *(unsigned int *)m;
		m += sizeof(int);
		FILE *f = fopen((char*)filename, "wb");
		fwrite(m, filesize, 1, f);
		fclose(f);
	}
	return 0;
}

int main11() {
	int err;
	long size = 0;
	byte * m = textFileRead("d:\\xx.xxxx", &size);
	for (int i = 2; i <= size - 1; i++) {
		m[i] ^= m[i - 2];
	}
	for (int i = size - 2; i >= 0; i--) {
		m[i] ^= m[i + 1];
	}
	byte k = 0xce;
	for (int i = 0; i <= size - 1; i++) {
		m[i] ^= k;
		k++;
	}
	byte *p = m;



	if (memcmp(p, "CHEAT", 5) == 0) {
		m += 5;
		uLong comprLen = *(int*)m;
		m += sizeof(int);
		uLong destLen = comprLen * 3;
		std::shared_ptr<byte> out_ptr = std::make_shared<byte>(destLen);
		Bytef * out = (Bytef*)out_ptr.get();

		err = uncompress((Bytef*)out, &destLen, (Bytef*)m, comprLen);
		if (err != Z_OK)
		{
			cerr << "解压错误: " << err << '\n ';
			return 1;
		}
	}
	else {
		uLong comprLen = size;
		uLong destLen = comprLen * 3;
		std::shared_ptr<byte> out_ptr = std::make_shared<byte>(destLen);
		Bytef * out = (Bytef*)out_ptr.get();
		//Z_DATA_ERROR
		err = unZip((Bytef*)out, (uInt*)&destLen, (Bytef*)m, comprLen);
		//err = uncompress((Bytef*)out, &destLen, (Bytef*)m, comprLen);
		if (err != Z_OK)
		{
			cerr << "解压错误: " << err << '\n ';
			return 1;
		}
	}
}

int case002() {
	int err;
	Byte compr[200], uncompr[200]; //足够大
	uLong comprLen = 0, uncomprLen = 0;
	const char* hello = "1234567890"; //原文

	uLong len = strlen(hello);// + 1;
	comprLen = sizeof(compr) / sizeof(compr[0]);

	err = compress(compr, &comprLen, (const Bytef*)hello, len);

	if (err != Z_OK)
	{
		cerr << "压缩错误: " << err << '\n ';
	}

	cout << "原文长度: " << len << "压缩后长度:" << comprLen << std::endl;
	printf("原文长度: %d: 压缩后长度: %ld\n", len, comprLen);
	strcpy((char*)uncompr, "garbage ");
	uncomprLen = sizeof(uncompr) / sizeof(uncompr[0]);
	err = uncompress(uncompr, &uncomprLen, compr, comprLen);

	if (err != Z_OK)
	{
		cerr << "解压错误: " << err << '\n ';
	}

	printf("原文长度: %d: 解压后长度: %ld\n" , len,uncomprLen );

	cout << "原文长度:" << len << "解压后长度:" << uncomprLen << endl;

	if (strncmp((char*)uncompr, hello,len))
	{
		cerr << "\n解压失败!!!\n ";
	}
	else
	{
		cout << "\n解压成功:" << (char*)uncompr;
	}
	/*
	FILE *f = fopen("d:\\b1.bin", "wb");
	fwrite(compr, comprLen, 1, f);
	fclose(f);
	*/
	getchar();
	return 0;
	return 0;
}



#endif
#include <Windows.h>
void FormatError(DWORD nErrno) {
	LPTSTR lpMsgBuf;
	//DWORD nErrno = GetLastError();
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		nErrno,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0,
		NULL);

	MessageBox(NULL, (LPCTSTR)lpMsgBuf, TEXT("NamedPipe Error"), MB_OK | MB_ICONINFORMATION);
	LocalFree(lpMsgBuf);
}

void showResources(char * path) {
	/*

	HMODULE hFile = LoadLibrary(path);
	if (hFile == NULL) {
	FormatError(GetLastError());
	return;
	}

	HANDLE hFile = BeginUpdateResource(path, FALSE);
	*/
	
	
	HANDLE hFile = CreateFile(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		FormatError(GetLastError());
		return;
	}

	HRSRC   hResource = FindResource((HMODULE)hFile, TEXT("ARCHIVE"), RT_RCDATA);
	if (hResource)
	{
		HGLOBAL   hg = LoadResource((HMODULE)hFile, hResource);
		if (hg)
		{

			DWORD   dwSize = SizeofResource(GetModuleHandle(NULL), hResource); //This function returns the size, in bytes, of the specified resource. 
			HANDLE h = CreateFile("out001.bin", GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
			if (h)
			{
				DWORD bw;
				WriteFile(h, hg, dwSize, &bw, NULL);
				CloseHandle(h);
			}
		}
	}
	else {
		FormatError(GetLastError());
		return;
	}
}


//计算Offset
ULONG RvaToOffset(IMAGE_NT_HEADERS32 * pNtHeader, ULONG Rva)
{
	//PE节
	IMAGE_SECTION_HEADER *p_section_header;
	ULONG sNum, i;
	//取得节表项数目
	sNum = pNtHeader->FileHeader.NumberOfSections;
	//取得第一个节表项
	p_section_header = (IMAGE_SECTION_HEADER *)
		((BYTE *)pNtHeader + sizeof(IMAGE_NT_HEADERS32));
	for (i = 0;i<sNum;i++)
	{
		//printf("PE 节名称: %s\n",p_section_header->Name);
		if ((p_section_header->VirtualAddress <= Rva) && Rva<(p_section_header->VirtualAddress + p_section_header->SizeOfRawData))
		{
			return Rva - p_section_header->VirtualAddress + p_section_header->PointerToRawData;
		}
		p_section_header++;
	}
	return 0;
}


int showResources32(char * file) {
	//DOS头
	PIMAGE_DOS_HEADER pImageDosHeader;
	//NT头(包括PE标识+Image_File_Header+OptionHeader)
	PIMAGE_NT_HEADERS32 pImageNtHeaders;
	//标准PE头、
	PIMAGE_FILE_HEADER pImageFileHeader;

	//扩展PE头
	IMAGE_OPTIONAL_HEADER32 pImageOptionHeaders;

	//资源类型名称
	static char* szResName[0x11] = {
		0,
		"Corsor",
		"Bitmap",
		"Icon",
		"Menu",
		"Dialog",
		"StringTable",
		"FontDir",
		"Font",
		"Accelerator",
		"RCDATA",
		"MessageTable",
		"GroupCursor",
		"zz",
		"GroupIcon",
		"xx",
		"Version"
	};


	HANDLE hFile;
	HANDLE hMapObject;
	//DOS头
	PUCHAR uFileMap;

	hFile = CreateFile(file, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, 0);
	if (hFile == NULL)
	{
		printf("打开文件失败\n");
		system("pause");
		return 0;
	}

	hMapObject = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
	if (hMapObject == NULL)
	{
		printf("创建文件映射内核对对象失败\n");
		system("pause");
		return 0;
	}

	//PE基址
	uFileMap = (PUCHAR)MapViewOfFile(hMapObject, FILE_MAP_READ, 0, 0, 0);
	if (uFileMap == NULL)
	{
		printf("映射到进程地址空间失败\n");
		system("pause");
		return 0;
	}

	pImageDosHeader = (PIMAGE_DOS_HEADER)uFileMap;
	if (pImageDosHeader->e_magic != IMAGE_DOS_SIGNATURE)
	{
		printf("不是PE结构\n");
		system("pause");
		return 0;
	}

	//定位到NT PE头
	pImageNtHeaders = (PIMAGE_NT_HEADERS32)((PUCHAR)uFileMap + pImageDosHeader->e_lfanew);

	//资源表
	if(1)
	{
		ULONG rva_offtable = pImageNtHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].VirtualAddress;
		ULONG offset_importtable = RvaToOffset(pImageNtHeaders, rva_offtable);
		if (!offset_importtable)
		{
			printf("获取资源表偏移地址失败\n");
			return 0;
		}
		//获取资源表地址
		/*
			typedef struct _IMAGE_RESOURCE_DIRECTORY {
				DWORD   Characteristics;　　　　　　　　　　资源属性
				DWORD   TimeDateStamp;　　　　　　　　　　　时间戳
				WORD    MajorVersion;　　　　　　　　　　　　资源大版本号
				WORD    MinorVersion;　　　　　　　　　　　　资源小版本号
				WORD    NumberOfNamedEntries;　　　　　　　按照名称命名的数量
				WORD    NumberOfIdEntries;　　　　　　　　　按照ID命名的数量
			//  IMAGE_RESOURCE_DIRECTORY_ENTRY DirectoryEntries[];
			} IMAGE_RESOURCE_DIRECTORY, *PIMAGE_RESOURCE_DIRECTORY;
			
			typedef struct _IMAGE_RESOURCE_DIRECTORY_ENTRY {
				union {
					struct {
						DWORD NameOffset:31;　　　　　　　　　　位段: 低31位偏移 定义了目录项的名称或者ID
						DWORD NameIsString:1;　　　　　　　　　 位段: 高位, 如果这位为1,则表示31位的偏移指向的是一个Unicode字符串的指针偏移
					};　　　　　　　　　　　　　　　　　　　　　　　
					DWORD   Name;　　　　　　　　　　　　　　　　　
					WORD    Id;
				};
				union {
					DWORD   OffsetToData;　　　　　　　　　　　　偏移RVA因为是联合体,所以有不同的解释
					struct {
						DWORD   OffsetToDirectory:31;　　　　看高位,如果高位是1,那么RVA偏移指向的是新的(根目录)
						DWORD   DataIsDirectory:1;　　　　　　
					};
				};
			} IMAGE_RESOURCE_DIRECTORY_ENTRY, *PIMAGE_RESOURCE_DIRECTORY_ENTRY;
			
			typedef struct _IMAGE_RESOURCE_DATA_ENTRY {
				DWORD   OffsetToData;　　　　　　　　　　资源数据的偏移RVA
				DWORD   Size;　　　　　　　　　　　　　　　大小
				DWORD   CodePage;　　　　　　　　　　　　 代码页缓冲(CMD设置窗口的时候就是这个,没用)
				DWORD   Reserved;　　　　　　　　　　　　 保留
			} IMAGE_RESOURCE_DATA_ENTRY, *PIMAGE_RESOURCE_DATA_ENTRY;
			
			typedef struct _IMAGE_RESOURCE_DIR_STRING_U {
				WORD    Length;//字符串长度
				WCHAR   NameString[1];//字符串数组
			} IMAGE_RESOURCE_DIR_STRING_U, *PIMAGE_RESOURCE_DIR_STRING_U;
		*/
		PIMAGE_RESOURCE_DIRECTORY  pResourceTable = (PIMAGE_RESOURCE_DIRECTORY )((char*)uFileMap + offset_importtable);
		
		//DWORD dwTypeSize = pResourceTable->NumberOfIdEntries + pResourceTable->NumberOfNamedEntries;//数量
		
		PIMAGE_RESOURCE_DIRECTORY_ENTRY pResourceEntry = (PIMAGE_RESOURCE_DIRECTORY_ENTRY)((DWORD)pResourceTable + sizeof(IMAGE_RESOURCE_DIRECTORY));
		//遍历第一层：资源类型
		for (int i = 0; i<(pResourceTable->NumberOfNamedEntries + pResourceTable->NumberOfIdEntries); i++)
		{
			//最高位为0
			if (!pResourceEntry[i].NameIsString)
			{
				if (pResourceEntry[i].Id < 0x11)
				{
					printf("资源类型ID：%p %s\n", pResourceEntry[i].Id, szResName[pResourceEntry[i].Id]);
				}
				else
				{
					char type[20];
					sprintf(type, "%d", pResourceEntry[i].Id);
					printf("资源类型ID：%p %s\n", pResourceEntry[i].Id, type);
				}
			}
			else //最高位为1
			{
				PIMAGE_RESOURCE_DIR_STRING_U pstcStr = (PIMAGE_RESOURCE_DIR_STRING_U)((DWORD)pResourceTable + pResourceEntry[i].NameOffset);
				WCHAR szStr[MAX_PATH] = {0};
				memcpy(szStr, pstcStr->NameString, pstcStr->Length * sizeof(WCHAR));
				printf("-资源字符串：%ls\n", szStr);
			}
			
			//解析第二层目录
			if (pResourceEntry[i].DataIsDirectory) //1
			{
				printf("资源第二层目录偏移：%p\n", pResourceEntry[i].OffsetToDirectory);
				PIMAGE_RESOURCE_DIRECTORY pRes2 = (PIMAGE_RESOURCE_DIRECTORY)((DWORD)pResourceTable + pResourceEntry[i].OffsetToDirectory);
				DWORD dwCount = pRes2->NumberOfIdEntries + pRes2->NumberOfNamedEntries;
				PIMAGE_RESOURCE_DIRECTORY_ENTRY pResEntry2 = (PIMAGE_RESOURCE_DIRECTORY_ENTRY)(pRes2 + 1);
				for (DWORD i=0; i<dwCount; i++)
				{
					WCHAR szStr[MAX_PATH] = { 0 };
					if (!pResEntry2[i].NameIsString)
					{
						printf(" ->资源标识ID：%d\n", pResEntry2[i].Id);
					}
					else
					{
						//显示资源字符串
						//NameOffset为相对资源文件的偏移
						//字符串偏移为：资源基地址 + NameOffset
						PIMAGE_RESOURCE_DIR_STRING_U pstcStr = (PIMAGE_RESOURCE_DIR_STRING_U)((DWORD)pResourceTable + pResEntry2[i].NameOffset);
						
						memcpy(szStr, pstcStr->NameString, pstcStr->Length * sizeof(WCHAR));
						printf(" -->资源字符串：%ls\n", szStr);
					}

					//解析第三层目录
					PIMAGE_RESOURCE_DIRECTORY pRes3 = (PIMAGE_RESOURCE_DIRECTORY)((DWORD)pResourceTable + pResEntry2[i].OffsetToDirectory);
					printf("  -->第三层目录：%d\n", pRes3->NumberOfIdEntries);
					PIMAGE_RESOURCE_DIRECTORY_ENTRY pResEntry3 = (PIMAGE_RESOURCE_DIRECTORY_ENTRY)(pRes3+1);
					if (!pResEntry3[i].DataIsDirectory)
					{
						//取数据偏移，显示数据
						PIMAGE_RESOURCE_DATA_ENTRY pResData = (PIMAGE_RESOURCE_DATA_ENTRY)((DWORD)pResourceTable + pResEntry3->OffsetToData);
						printf("  -->数据 RVA：%p\n", pResData->OffsetToData);
						
						printf("  -->数据偏移：%p\n", RvaToOffset(pImageNtHeaders, pResData->OffsetToData));
						//printf("  -->数据偏移：%p\n", RVA2FOA(pResData->OffsetToData, pDosHeader));
						printf("  -->数据大小：%p\n", pResData->Size);
						if (lstrcmpW(szStr, L"ARCHIVE") == 0) {
							FILE * fp = fopen("123.bin", "wb");
							fwrite((void*)(uFileMap + RvaToOffset(pImageNtHeaders, pResData->OffsetToData)), 1, pResData->Size, fp);
							fclose(fp);
						}
					}
					printf("\n");
				}
				printf("\n\n");
			}
			else
			{
				//取数据偏移，显示数据
				PIMAGE_RESOURCE_DATA_ENTRY pResData = (PIMAGE_RESOURCE_DATA_ENTRY)((DWORD)pResourceTable + pResourceEntry[i].OffsetToData);
				printf("  -->数据 RVA：%p\n", pResData->OffsetToData);
				printf("  -->数据偏移：%p\n", RvaToOffset(pImageNtHeaders, pResData->OffsetToData));
				//printf("  -->数据偏移：%p\n", RVA2FOA(pResData->OffsetToData, pDosHeader));
				printf("  -->数据大小：%p\n", pResData->Size);
			}
		}



	}

	if(0)
	{
		//导入表的相对虚拟地址(RVA)
		ULONG rva_ofimporttable = pImageNtHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;
		//根据相对虚拟(rva)地址计算偏移地址(offset)
		ULONG offset_importtable = RvaToOffset(pImageNtHeaders, rva_ofimporttable);
		if (!offset_importtable)
		{
			printf("获取导入表偏移地址失败\n");
			return 0;
		}
		PIMAGE_THUNK_DATA32 s;

		//取得导入表的地址
		IMAGE_IMPORT_DESCRIPTOR *pImportTable = (IMAGE_IMPORT_DESCRIPTOR *)((char*)uFileMap + offset_importtable);


		IMAGE_IMPORT_DESCRIPTOR null_iid;
		IMAGE_THUNK_DATA32 null_thunk;
		memset(&null_iid, 0, sizeof(null_iid));
		memset(&null_thunk, 0, sizeof(null_thunk));

		//每个元素代表了一个引入的DLL。
		for (int i = 0; memcmp(pImportTable + i, &null_iid, sizeof(null_iid)) != 0; i++)
		{
			char *dllName = (char*)(uFileMap + RvaToOffset(pImageNtHeaders, pImportTable[i].Name));

			//拿到了DLL的名字

			printf("模块[%d]: %s\n", i, (char*)dllName);
			PIMAGE_THUNK_DATA32 pThunk = (PIMAGE_THUNK_DATA32)(uFileMap + RvaToOffset(pImageNtHeaders, pImportTable[i].FirstThunk));

			while (pThunk->u1.Ordinal != NULL)
			{
				PIMAGE_IMPORT_BY_NAME pname = (PIMAGE_IMPORT_BY_NAME)(uFileMap + RvaToOffset(pImageNtHeaders, pThunk->u1.AddressOfData));
				printf("函数编号: %d 名称: %s\n", pname->Hint, pname->Name);
				pThunk++;
			}
		}
	}
	

}
int main() {
	showResources32("D:\\xxx\\xx\\1.exe.bak");
	return 0;
}


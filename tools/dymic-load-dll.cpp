#include <iostream>
#include <Windows.h>
typedef struct BASE_RELOCATION_BLOCK {
       DWORD PageAddress;
       DWORD BlockSize;
} BASE_RELOCATION_BLOCK, *PBASE_RELOCATION_BLOCK;
typedef struct BASE_RELOCATION_ENTRY {
       USHORT Offset : 12;
       USHORT Type : 4;
} BASE_RELOCATION_ENTRY, *PBASE_RELOCATION_ENTRY;
using DLLEntry = BOOL(WINAPI *)(HINSTANCE dll, DWORD reason, LPVOID reserved);
int main()
{
       //得到当前模块的基址
       PVOID imageBase = GetModuleHandleA(NULL);
       //本地加载dll内容至内存中
       HANDLE dll =  CreateFileA("dll.dll", GENERIC_READ,  NULL, NULL, OPEN_EXISTING, NULL, NULL);
       DWORD64 dllSize = GetFileSize(dll, NULL);
       LPVOID dllBytes = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dllSize);
       DWORD outSize = 0;
       ReadFile(dll, dllBytes, dllSize, &outSize, NULL);
       //获取已加载至内存中的dll的头部数据
       PIMAGE_DOS_HEADER dosHeaders = (PIMAGE_DOS_HEADER)dllBytes;
       PIMAGE_NT_HEADERS ntHeaders = (PIMAGE_NT_HEADERS)((DWORD_PTR)dllBytes +  dosHeaders->e_lfanew);
       SIZE_T dllImageSize = ntHeaders->OptionalHeader.SizeOfImage;
       //分配dll加载时所需的内存空间
       LPVOID dllBase = VirtualAlloc((LPVOID)ntHeaders->OptionalHeader.ImageBase,  dllImageSize, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
       //得到实际分配的内存基址与预期的基址差值,便于后续进行重定向
       DWORD_PTR deltaImageBase = (DWORD_PTR)dllBase -  (DWORD_PTR)ntHeaders->OptionalHeader.ImageBase;
       //将dll头部数据复制到分配的内存空间
       std::memcpy(dllBase, dllBytes, ntHeaders->OptionalHeader.SizeOfHeaders);
       //加载节区数据至新的内存空间
       PIMAGE_SECTION_HEADER section = IMAGE_FIRST_SECTION(ntHeaders);
       for (size_t i = 0; i < ntHeaders->FileHeader.NumberOfSections; i++)
       {
              LPVOID sectionDestination = (LPVOID)((DWORD_PTR)dllBase +  (DWORD_PTR)section->VirtualAddress);
              LPVOID sectionBytes = (LPVOID)((DWORD_PTR)dllBytes +  (DWORD_PTR)section->PointerToRawData);
              std::memcpy(sectionDestination, sectionBytes,  section->SizeOfRawData);
              section++;
       }
       // 开始dll加载实现重定位
       IMAGE_DATA_DIRECTORY relocations =  ntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC];
       DWORD_PTR relocationTable = relocations.VirtualAddress +  (DWORD_PTR)dllBase;
       DWORD relocationsProcessed = 0;
       while (relocationsProcessed < relocations.Size)
       {
              PBASE_RELOCATION_BLOCK relocationBlock =  (PBASE_RELOCATION_BLOCK)(relocationTable + relocationsProcessed);
              relocationsProcessed += sizeof(BASE_RELOCATION_BLOCK);
              DWORD relocationsCount = (relocationBlock->BlockSize -  sizeof(BASE_RELOCATION_BLOCK)) / sizeof(BASE_RELOCATION_ENTRY);
              PBASE_RELOCATION_ENTRY relocationEntries =  (PBASE_RELOCATION_ENTRY)(relocationTable + relocationsProcessed);
              for (DWORD i = 0; i < relocationsCount; i++)
              {
                     relocationsProcessed += sizeof(BASE_RELOCATION_ENTRY);
                     if (relocationEntries[i].Type == 0)
                     {
                           continue;
                     }
                     DWORD_PTR relocationRVA = relocationBlock->PageAddress +  relocationEntries[i].Offset;
                     DWORD_PTR addressToPatch = 0;
                     ReadProcessMemory(GetCurrentProcess(),  (LPCVOID)((DWORD_PTR)dllBase + relocationRVA), &addressToPatch, sizeof(DWORD_PTR),  NULL);
                     addressToPatch += deltaImageBase;
                     std::memcpy((PVOID)((DWORD_PTR)dllBase + relocationRVA),  &addressToPatch, sizeof(DWORD_PTR));
              }
       }
       //解析导入表
       PIMAGE_IMPORT_DESCRIPTOR importDescriptor = NULL;
       IMAGE_DATA_DIRECTORY importsDirectory =  ntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
       importDescriptor =  (PIMAGE_IMPORT_DESCRIPTOR)(importsDirectory.VirtualAddress + (DWORD_PTR)dllBase);
       LPCSTR libraryName = "";
       HMODULE library = NULL;
       while (importDescriptor->Name != NULL)
       {
              libraryName = (LPCSTR)importDescriptor->Name + (DWORD_PTR)dllBase;
              library = LoadLibraryA(libraryName);
              if (library)
              {
                     PIMAGE_THUNK_DATA thunk = NULL;
                     thunk = (PIMAGE_THUNK_DATA)((DWORD_PTR)dllBase +  importDescriptor->FirstThunk);
                     while (thunk->u1.AddressOfData != NULL)
                     {
                           if (IMAGE_SNAP_BY_ORDINAL(thunk->u1.Ordinal))
                           {
                                  LPCSTR functionOrdinal =  (LPCSTR)IMAGE_ORDINAL(thunk->u1.Ordinal);
                                  thunk->u1.Function =  (DWORD_PTR)GetProcAddress(library, functionOrdinal);
                           }
                           else
                           {
                                  PIMAGE_IMPORT_BY_NAME functionName =  (PIMAGE_IMPORT_BY_NAME)((DWORD_PTR)dllBase + thunk->u1.AddressOfData);
                                  DWORD_PTR functionAddress =  (DWORD_PTR)GetProcAddress(library, functionName->Name);
                                  thunk->u1.Function = functionAddress;
                           }
                           ++thunk;
                     }
              }
              importDescriptor++;
       }
       //执行加载的dll
       DLLEntry DllEntry = (DLLEntry)((DWORD_PTR)dllBase +  ntHeaders->OptionalHeader.AddressOfEntryPoint);
       (*DllEntry)((HINSTANCE)dllBase, DLL_PROCESS_ATTACH, 0);
       CloseHandle(dll);
       HeapFree(GetProcessHeap(), 0, dllBytes);
       return 0;
}


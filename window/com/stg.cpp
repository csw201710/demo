#include <objbase.h.>

int main(int argc, char* argv[])
{
	CoInitialize(NULL);
	IStorage* pStr;
	HRESULT hr  = StgCreateDocfile( L"test.mcb", STGM_DIRECT|STGM_CREATE|STGM_READWRITE|STGM_SHARE_EXCLUSIVE, NULL, &pStr);
	IStream* pStream;
	hr = pStr->CreateStream( L"mcbStream", STGM_DIRECT|STGM_CREATE|STGM_READWRITE|STGM_SHARE_EXCLUSIVE, 0, 0, &pStream );
	ULONG name;
	char data[] = "Mahesh testing " ;
	pStream->Write( data, strlen(data), &name ) ;
	CoUninitialize();
	return 0;
}


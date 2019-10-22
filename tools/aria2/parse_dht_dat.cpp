
#pragma pack(1)
typedef struct {
	char header[8];//8
	uint64_t time;//16
	char localnode[8];//24
	char localnodeId[20];//44
	char reserved1[4];//48
	uint32_t numNodes;
	char reserved2[4];
} ARIA2_V3;

#pragma pack()


inline uint64_t byteswap64(uint64_t x)
{
	uint64_t v1 = ntohl(x & 0x00000000ffffffffllu);
	uint64_t v2 = ntohl(x >> 32);
	return (v1 << 32) | v2;
}
// 32bit swap
UINT Swap32(UINT value)
{
	UINT r;
	((BYTE *)&r)[0] = ((BYTE *)&value)[3];
	((BYTE *)&r)[1] = ((BYTE *)&value)[2];
	((BYTE *)&r)[2] = ((BYTE *)&value)[1];
	((BYTE *)&r)[3] = ((BYTE *)&value)[0];
	return r;
}

// 64-bit swap
UINT64 Swap64(UINT64 value)
{
	UINT64 r;
	((BYTE *)&r)[0] = ((BYTE *)&value)[7];
	((BYTE *)&r)[1] = ((BYTE *)&value)[6];
	((BYTE *)&r)[2] = ((BYTE *)&value)[5];
	((BYTE *)&r)[3] = ((BYTE *)&value)[4];
	((BYTE *)&r)[4] = ((BYTE *)&value)[3];
	((BYTE *)&r)[5] = ((BYTE *)&value)[2];
	((BYTE *)&r)[6] = ((BYTE *)&value)[1];
	((BYTE *)&r)[7] = ((BYTE *)&value)[0];
	return r;
}

//#include <winsock2.h>
#pragma comment(lib,"Ws2_32.lib")
class ARIA_DHT {
private:
	ARIA2_V3 m_header;
	std::unique_ptr<char[]> m_buf;
	int m_pos;
public:
	ARIA_DHT() {
		memset(&m_header, 0x00, sizeof(m_header));
		m_pos = 0;
	}
	virtual ~ARIA_DHT() {
	}
	int loadFile(const char * path) {
		FILE *fp = fopen(path, "rb");
		if (fp == 0) {
			throw "open file failed";
		}
		fseek(fp, 0, SEEK_END);
		long size = ftell(fp);
		if (size == 0) {
			fclose(fp);
			throw "file  size invalid";

		}
		m_buf = std::unique_ptr<char[]>(new char[size]);
		fseek(fp, 0, SEEK_SET);
		if (fread(m_buf.get(), sizeof(char), size, fp) != size) {
			fclose(fp);
			throw "file  size invalid";
		}
		
		fclose(fp);
		return 0;
	}
	int readBytes(void *buf, int len) {
		if (len > 0) {
			memcpy(buf, m_buf.get() + m_pos, len);
			m_pos += len;
		}
		else {
			throw "read len is invalid";
		}
	}
	int parse() {
		char header[8] = {0};
		// magic
		header[0] = 0xa1u;
		header[1] = 0xa2u;
		// format ID
		header[2] = 0x02u;
		// version
		header[6] = 0;
		header[7] = 0x03u;

		if (m_buf.get() == 0) {
			throw "please call loadFile first ";
		}
		readBytes((char*)&m_header, sizeof(m_header));
		if (memcmp(m_header.header, header, 8) != 0) {
			throw "invalid header magic";
		}
		m_header.time = Swap64(m_header.time);
		m_header.numNodes = Swap32(m_header.numNodes);

		int compactlen = 6;//IPV4
		char zero[18] = {0};
		printf("m_header.numNodes: %d\n\n", m_header.numNodes);
		for (int i = 0;i < m_header.numNodes;i++) {
			char buf[255] = {0};
			// 1byte compact peer info length
			uint8_t peerInfoLen;
			readBytes( &peerInfoLen, sizeof(peerInfoLen));
			if (peerInfoLen != compactlen) {
				// skip this entry
				readBytes(buf, 7 + 48);
				continue;
			}
			// 7bytes reserved
			readBytes(buf, 7);
			// compactlen bytes compact peer info
			readBytes(buf, compactlen);// ip + port

			//无效ip和端口跳过
			if (memcmp(zero, buf, compactlen) == 0) {
				// skip this entry
				readBytes(buf, 48 - compactlen);
				continue;
			}
			unpackcompact(buf, 6);


			// 24-compactlen bytes reserved
			readBytes(buf, 24 - compactlen);
			// node ID
			readBytes(buf, 20);
			// 4bytes reserved
			readBytes(buf, 4);


		}

	}
	void unpackcompact(const  char* compact,
		int f) {
		int portOffset = f == 6 ? 4 : 16;
		char buf[30];

		{
			in_addr inaddr;
			memcpy(&inaddr, compact, 4);
			//inaddr.S_un.S_addr = Swap32(*(UINT*)&inaddr);
			
			char *ipaddr = inet_ntoa(inaddr);
			printf("%20s ", ipaddr);
		}
		{
			uint16_t portN;
			memcpy(&portN, compact + portOffset, sizeof(portN));
			int port = ntohs(portN);
			printf("%d\n", port);
		}
	}

};


int main(int argc, char **argv)
{
	ARIA_DHT obj;
	try {
		obj.loadFile("aria2-1.35.0-win-64bit-build1\\dht.dat");
		obj.parse();
	}
	catch (char *s) {
		std::cout << "Exception:"<< s << std::endl;
	}
	catch (...) {
		std::cout << "unkonw error" << std::endl;
	}

	


	return 0;
}



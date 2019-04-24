#pragma once
#include <iostream>
#include <memory>

#include "zlib.h"

class TCustomZlibStream {
public:
	TCustomZlibStream() {
		memset(m_FBuffer, 0x00, sizeof(m_FBuffer));
		m_FStrm = 0;
		m_dataLen = 0;
		m_FStrmPos = 0;
	}
	virtual ~TCustomZlibStream() {}
	
protected:
	uInt read(Bytef *data, uInt len) {
		if (m_FStrmPos < m_dataLen) {
			if (m_FStrmPos + len < m_dataLen) {
				memcpy(data, m_FStrm + m_FStrmPos, len);
				m_FStrmPos += len;
				return len;
			}
			else {
				memcpy(data, m_FStrm + m_FStrmPos, len);
				m_FStrmPos += len;
				return m_dataLen - m_FStrmPos;

			}
		}
		return 0;

	}
	Bytef * m_FStrm;
	uLong m_FStrmPos;


	Bytef m_FBuffer[65536 ];
	uLong m_dataLen;
	//zlib struct
	z_stream m_strm;
};



class CZlibDecompressionStream:public TCustomZlibStream
{
public:
	CZlibDecompressionStream() {
		
	}
	virtual ~CZlibDecompressionStream() {
		inflateEnd(&m_strm);
		delete[]m_FStrm;
	};

	int Create(Bytef *data, uLong dataLen) {
		if (dataLen <= 0) {
			return -1;
		}
		m_FStrm = new Bytef[ dataLen];
		m_dataLen = dataLen;
		

		m_strm.zalloc = Z_NULL;
		m_strm.zfree = Z_NULL;
		m_strm.opaque = Z_NULL;
		m_strm.next_in = m_FBuffer;
		m_strm.avail_in = 0;
		

		DCheck(inflateInit(&m_strm));
		return 0;
	}
	int Read(Bytef *Buffer, uInt Count) {
		m_strm.next_out = Buffer;
		m_strm.avail_out = Count;
		while (m_strm.avail_out > 0) {
			if (m_strm.avail_in == 0) {
				m_strm.avail_in = read(m_FBuffer, sizeof(m_FBuffer));
				//read finished
				if (m_strm.avail_in == 0) {
					return Count - m_strm.avail_out;
				}
				m_strm.next_in = m_FBuffer;
			}
			
			DCheck(inflate(&m_strm, 0));
		}
		return Count;

	}
private:
	void DCheck(int ret) {
		assert(ret == 0);
		
	}

};




#pragma once
#include <iostream>
#include <memory>
#include <fstream>
using namespace std;

#include "zlib.h"

class TCustomZlibStream {
public:
	TCustomZlibStream(istream& fstream):m_fstream(fstream){

	}
	virtual ~TCustomZlibStream() {}
	
protected:
	uInt read(Bytef *data, uInt len) {
		

	}


	Bytef m_FBuffer[16384];
	//zlib struct
	z_stream m_strm;

	istream & m_fstream;
};



class CZlibDecompressionStream:public TCustomZlibStream
{
public:
	CZlibDecompressionStream(std::istream& fstream, bool Askipheader): TCustomZlibStream(fstream){
		raw_read = 0;
		compressed_read = 0;


		m_strm.zalloc = Z_NULL;
		m_strm.zfree = Z_NULL;
		m_strm.opaque = Z_NULL;
		m_strm.next_in = m_FBuffer;
		m_strm.avail_in = 0;
		if (Askipheader) {
			err = inflateInit2(&m_strm, -MAX_WBITS);
			if (err != Z_OK) {
				printf("inflateInit2 failed ret: %d\n", err);
			}
		}
		else {
			err = inflateInit(&m_strm);
			if (err != Z_OK) {
				printf("inflateInit failed ret :%d\n", err);
			}
		}
	}
	virtual ~CZlibDecompressionStream() {
		inflateEnd(&m_strm);
	};

	int Read(Bytef *Buffer, uInt Count) {
		uLong lastavail;
		m_strm.next_out = Buffer;
		m_strm.avail_out = Count;
		lastavail = Count;
		while (m_strm.avail_out != 0) {
			if (m_strm.avail_in == 0) {
				m_strm.next_in = m_FBuffer;
				m_strm.avail_in = m_fstream.read((char*)m_FBuffer, sizeof(m_FBuffer)).gcount(); //m_fsread(m_FBuffer, sizeof(m_FBuffer));
				compressed_read += m_strm.avail_in;
				raw_read += (lastavail - m_strm.avail_out);
				lastavail = m_strm.avail_out;
				
			}
			err = inflate(&m_strm, Z_NO_FLUSH);
			if (Z_STREAM_END == err) {
				break;
			}
			if (err != Z_OK) {
				printf("inflate failed ret%d", err);
				return err;
			}

		}
		if (Z_STREAM_END == err) {
			compressed_read-= m_strm.avail_in;
		}
		raw_read+=lastavail - m_strm.avail_out;
		//read = Count - m_strm.avail_out;
		return Count - m_strm.avail_out;

	}
private:
	unsigned long raw_read, compressed_read;
	int err;
};


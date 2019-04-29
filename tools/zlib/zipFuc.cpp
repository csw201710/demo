#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "zlib.h"
int Zip(void *out, unsigned int *outLen, void * in, unsigned int inLen) {
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


int unZip2(void *out, unsigned int *outLen, void * in, unsigned int inLen) {
	int ret = 0;
	z_stream strm;
	uInt allOutLen = *outLen;

	/* allocate inflate state */
	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;
	strm.avail_in = 0;
	strm.next_in = Z_NULL;
	ret = inflateInit2(&strm, -MAX_WBITS);
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


int test001() {
	int err;
	Bytef txt[] = "abcd";
	Bytef enc[1024];
	Bytef dec[1024] = { 0 };
	uInt encLen = sizeof(enc);
	uInt decLen = sizeof(dec);
	err = Zip(enc, &encLen, txt, strlen((char*)txt));
	err = unZip(dec, &decLen, enc, encLen);
	dec[decLen] = 0;
	printf("err:%d ,%s\n", err, dec);
	return 0;
}

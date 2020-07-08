
std::string calcMd5Hex(std::string data)
{
	std::string md5_value;
	unsigned char result[MD5_DIGEST_LENGTH * 5] = { 0 };
	MD5_CTX md5Context;
	char hex[35];

	unsigned char* buf = (unsigned char*)data.data();
	int bufLen = data.length();

	MD5_Init(&md5Context);
	MD5_Update(&md5Context, buf, bufLen);

	MD5_Final(result, &md5Context);


	memset(hex, 0, sizeof(hex));
	for (int i = 0; i < MD5_DIGEST_LENGTH; ++i)
	{
		sprintf(hex + i * 2, "%02x", result[i]);
	}
	md5_value = (char*)hex;
	return md5_value;
}



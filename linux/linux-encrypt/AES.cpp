#define AES_BLOCK_SIZE 16
int aes_crypt(char* indata,int inlen,char* outdata,int *outlen,int flag) //flag:1=>encrypt 0=>decrypt
{
	int plain_data_len,i;
	char *plain_data;
    unsigned char iv[AES_BLOCK_SIZE]={0x01,0x02,0x03,0x04,0x04,0x03,0x02,0x01,0x01,0x02,0x03,0x04,0x04,0x03,0x02,0x01};
	unsigned char aesKey[AES_BLOCK_SIZE ] = {0x01, 0x02, 0x03, 0x04, 0x30, 0x47, 0x71, 0xb3, 0x9d, 0x30, 0x21, 0xc4, 0xe1, 0x71, 0x10, 0x11};
	int * headLen = 0;
	AES_KEY	key;

	if(flag == 1){
		
		AES_set_encrypt_key(aesKey, 128, &key);

		int remain = (inlen+4) % AES_BLOCK_SIZE;
		plain_data_len = (inlen+4) + (AES_BLOCK_SIZE - remain);
		plain_data = (char*)malloc(plain_data_len);
		headLen = (int *) plain_data;
		
		*headLen = inlen;

		memcpy(plain_data + 4, indata, inlen);
		memset(plain_data+ (inlen+4), AES_BLOCK_SIZE - remain, AES_BLOCK_SIZE - remain);

		
	}
	else{
		AES_set_decrypt_key(aesKey, 128, &key);

		int remain = inlen%AES_BLOCK_SIZE;
		plain_data_len = inlen + (AES_BLOCK_SIZE - remain);
		plain_data = (char*)malloc(plain_data_len);
		memcpy(plain_data, indata, inlen);
		memset(plain_data+inlen, AES_BLOCK_SIZE - remain, AES_BLOCK_SIZE - remain);		
	}




	int count = plain_data_len/AES_BLOCK_SIZE;

	for(i=0; i<count; i++)
	{
		AES_ecb_encrypt((unsigned char*)plain_data + i*AES_BLOCK_SIZE, (unsigned char*)outdata  + i*AES_BLOCK_SIZE, &key, flag);
		AES_cbc_encrypt((unsigned char*)plain_data + i*AES_BLOCK_SIZE, (unsigned char*)outdata  + i*AES_BLOCK_SIZE,AES_BLOCK_SIZE, &key,iv, flag);
	}
	
	if(flag == 1){
		*outlen = plain_data_len;
		
	}else{
		headLen = (int*) outdata;
		*outlen = *headLen;
		memcpy(outdata, outdata + 4, *outlen);
	}
	free(plain_data);
		
	return 0;
}


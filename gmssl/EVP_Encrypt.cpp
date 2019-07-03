int SM4_encrypt(unsigned char * inBuf, int inBufLen ,
	unsigned char *outBuf,int *outBufLen,int do_encrypt)
{
	EVP_CIPHER_CTX *ctx = 0;
	int tmplen = 0;
	int encLen = 0;
	int ret = 0;
	unsigned char iv[] = "1234567887654321"; 
	unsigned char key[16]={0x11,0x13,0xdd,0x13,0x11,0x13,0x11,0x13,
	                       0xff,0x13,0x11,0xcc,0xee,0x13,0x11,0xaa};
	ctx = EVP_CIPHER_CTX_new();  //creates a cipher context.
		   
	if(!EVP_CipherInit_ex(ctx, EVP_sms4_cbc(), NULL, key, iv, do_encrypt)){
		ret = -1;goto err;
	}
	//int ret = EVP_Cipher(ctx,outBuf,inBuf,inBufLen);
	if(!EVP_CipherUpdate(ctx, outBuf, &tmplen, inBuf, inBufLen)){
		ret = -1;goto err;
	}
    encLen = tmplen;
	if(!EVP_CipherFinal(ctx, outBuf+tmplen, &tmplen)){
		ret = -1;goto err;
	}
	encLen += tmplen;
	*outBufLen = encLen;
	ret = 0;
err:
	EVP_CIPHER_CTX_free(ctx);
	return ret;
	
}

int main(int argc,char *argv[]){
	
	unsigned char inbuf[1024]={0};
	int inbufLen = sizeof(inbuf);

	for(int i=100;i< sizeof(inbuf);i++){
		unsigned char outbuf[1024]={0};
		int outbufLen = sizeof(outbuf);

		unsigned char decoutbuf[1024]={0};
		int decoutbufLen = sizeof(outbuf);

		for(int j=0;j< i;j++){
			inbuf[j] = 0x11;
		}
		inbufLen = i;
		SM4_encrypt(inbuf,inbufLen,outbuf,&outbufLen,1);
		SM4_encrypt(outbuf,outbufLen,decoutbuf,&decoutbufLen,0);
		if(memcmp(inbuf,decoutbuf,inbufLen) != 0 || decoutbufLen != inbufLen){
			printf("error\n");
			break;
		}
	}

	return 0;
}

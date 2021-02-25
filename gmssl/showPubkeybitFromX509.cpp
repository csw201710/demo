void showPubKeyFromX509(){
	BIO *b = BIO_new_file("D:\\xxxx\\xxx\\xxx.crt", "r");
	if (b == NULL) { printf("%s\n", ERR_error_string(ERR_get_error(), NULL));  return; }
	X509 *x = PEM_read_bio_X509(b, NULL, NULL, NULL);
	BIO_free(b);

	//获取公钥数据
	ASN1_BIT_STRING *public_bit=NULL;
	public_bit = X509_get0_pubkey_bitstr(x);
	if(public_bit == NULL)
	{
		goto err;
	}
	int bit_len = ASN1_STRING_length(public_bit);
	if(bit_len == 0){
		goto err;
	}
	unsigned char *p = ASN1_STRING_data(public_bit);
	if(p == 0){
		goto err;	
	}
	for (int i = 0; i < bit_len; i++) {
		printf("%02X ", p[i]);
		if(i % 16 == 15){
			printf("\n");
		}
	}
	printf("\n");
err:
	X509_free(x);
	return ;
}


// 获取证书查看是的公钥完整数据
int GetCertPubKey(unsigned char* s_cert, int dwCertLen, unsigned char *pubkey, int *dwKeyLen)
{
	const unsigned char * pbcert = NULL;
	X509 *x = NULL;
	ASN1_BIT_STRING *ec_public_key = NULL;
	const unsigned char *p;
	int len;

	pbcert = s_cert;
	x = d2i_X509(NULL, &pbcert, dwCertLen);
	if (x == NULL)
	{
        printf("d2i_X509 failed!, %s", ERR_error_string(ERR_get_error(), NULL));
		return 1;
	}

	ec_public_key = X509_get0_pubkey_bitstr(x);
	if (ec_public_key == NULL)
	{
        free(x);
        printf("X509_get0_pubkey_bitstr failed!, %s", ERR_error_string(ERR_get_error(), NULL));
		return 2;
	}

	//p=(BYTE*)malloc(128);
	len = ASN1_STRING_length(ec_public_key);

	p = ASN1_STRING_get0_data(ec_public_key);
	if (p == NULL)
	{
		X509_free(x);
		return 4;
	}
	memcpy(pubkey, p, len);
	*dwKeyLen = len;

	X509_free(x);

	return 0;
}



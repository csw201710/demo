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


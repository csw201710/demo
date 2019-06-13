X509* load_certfile(const char* certFile,int type)  
{  
    X509* x509 = NULL;  
    BIO* in = NULL;  
  
    if(certFile==NULL)  
        goto end;  
    in = BIO_new_file(certFile,"rb");  
    if(in==NULL)  
        goto end;  
    //将IO中数据以PEM格式读入到X509对象  
	if(type == 0){
		x509 = PEM_read_bio_X509(in,NULL,NULL,NULL);  
	}else{
		x509 = d2i_X509_bio(in, NULL);
	}
	
    if(x509== NULL)  
        goto end;  
end:  
    if(in)BIO_free(in);  
    return x509;  
}  
static X509_NAME_ENTRY * _getX509_NAME_ENTRY(X509_NAME *pX509Name, int nid){
	if(pX509Name == 0) return 0;
	for (int i = 0; i < X509_NAME_entry_count(pX509Name); i++) {
		X509_NAME_ENTRY *ne = X509_NAME_get_entry(pX509Name, i);
		ASN1_STRING* str = X509_NAME_ENTRY_get_data(ne);
		ASN1_OBJECT* obj = X509_NAME_ENTRY_get_object(ne);
		int s_nid = OBJ_obj2nid(obj);
		if(s_nid ==  nid) return  ne;
	}
	return 0;
}
static X509_NAME_ENTRY * _delX509_NAME_ENTRY(X509_NAME *pX509Name, int nid){
	if(pX509Name == 0) return 0;
	for (int i = 0; i < X509_NAME_entry_count(pX509Name); i++) {
		X509_NAME_ENTRY *ne = X509_NAME_get_entry(pX509Name, i);
		ASN1_STRING* str = X509_NAME_ENTRY_get_data(ne);
		ASN1_OBJECT* obj = X509_NAME_ENTRY_get_object(ne);
		int s_nid = OBJ_obj2nid(obj);
		if(s_nid ==  nid) 
			return  X509_NAME_delete_entry(pX509Name,i);
		
	}
	return 0;
}

void testPrint(){
	const char* cert ="xxx.cer";
    BIO *bio_err = BIO_new_fp(stderr,BIO_NOCLOSE);
	X509* x509 = load_certfile(cert, 1);
	
	
	//修改时间
	X509_gmtime_adj(X509_get_notBefore(x509), (long) 0);
	X509_gmtime_adj(X509_get_notAfter(x509), 2 * 24 * 60 * 60);

	{
		X509_NAME *pX509Name = NULL;
		pX509Name = X509_get_issuer_name(x509);
		
		//X509_NAME_add_entry_by_txt(pX509Name, "emailAddress", V_ASN1_UTF8STRING, (const unsigned char*)"aa@qq.com", -1, -1, 0);
		
		
		//修改commonName
		X509_NAME_ENTRY * entry_name = _getX509_NAME_ENTRY(pX509Name, NID_commonName);
		if(entry_name == 0) return;
		_delX509_NAME_ENTRY(pX509Name, NID_commonName);
		X509_NAME_add_entry_by_txt(pX509Name, "CN", V_ASN1_UTF8STRING, (const unsigned char*)"this is CN", -1, -1, 0);
        //X509_NAME_ENTRY_set_data(entry_name,V_ASN1_UNDEF,(const unsigned char*)"this is CN",-1); //unuseful
		
		
	}
	{
		unsigned char md[EVP_MAX_MD_SIZE];
		unsigned int mdLen;
		const EVP_MD * pmd = EVP_get_default_digest();

		EVP_PKEY* pkey = EVP_PKEY_new();
		RSA* pRSA = RSA_generate_key(1024, RSA_3, NULL, NULL);
		EVP_PKEY_assign_RSA(pkey, pRSA);

		if (!X509_digest(x509, pmd, md, &mdLen)) {
			printf("%s\n", ERR_error_string(ERR_get_error(), NULL));
			return;
		}
		if(!X509_sign(x509, pkey, pmd))
		{
			printf("%s\n", ERR_error_string(ERR_get_error(), NULL));
			return;
		}
		if(!X509_verify(x509, pkey))
		{
			printf("%s\n", ERR_error_string(ERR_get_error(), NULL));
			return;
		}
		
		if (1) {
			BIO *bp = BIO_new_file("aa.cer", "wb");	 
			(void)i2d_X509_bio(bp, x509);
			BIO_free(bp);
		}else{
			BIO *bp = BIO_new_file("aa.crt", "wb");
			PEM_write_bio_X509(bp, x509);
			BIO_free(bp);
		}
		
	}



	X509_print(bio_err, x509);
	X509_free(x509);
}



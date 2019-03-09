/*
	描述: 加载crt证书或者cer证书的公钥数据
	
*/
static int load_public_cert(const char *certfile, EVP_PKEY ** pub_key,int type)
{
    X509 *x  = 0;
    BIO *b = 0 ;

    if(type == 1){
        b = BIO_new_file(certfile, "rb");
        if (b == NULL) {
            qDebug()<< ERR_error_string(ERR_get_error(), NULL);
            return 0;
        }
        x = d2i_X509_bio(b,NULL);
    }else if (type == 0){
        b = BIO_new_file(certfile, "r");
        if (b == NULL) {
            qDebug()<< ERR_error_string(ERR_get_error(), NULL);
            return 0;
        }
        x = PEM_read_bio_X509(b, NULL, NULL, NULL);
    }


    if (x == NULL) {
        qDebug()<< ERR_error_string(ERR_get_error(), NULL);
        BIO_free(b);
        return 0;
    }
    *pub_key = X509_get_pubkey(x);
    if (*pub_key == NULL) { qDebug()<< ERR_error_string(ERR_get_error(), NULL); BIO_free(b);  return 0; }

    BIO_free(b);

  return 1;
}




static ASN1_OBJECT *txt2obj(const char *oid)
{
	ASN1_OBJECT *oid_obj = NULL;

	if ((oid_obj = OBJ_txt2obj(oid, 0)) == NULL)
		ERROR("cannot convert %s to OID\n", oid);

	return oid_obj;
}

static int create_digest(BIO *input, const char *digest, const EVP_MD *md,
	unsigned char *out, int* outLen)
{
	int md_value_len;
	int rv = 0;
	EVP_MD_CTX *md_ctx = NULL;
	unsigned char md_value[1024] = {0};


	md_value_len = EVP_MD_size(md);
	if (md_value_len < 0)
		return 0;
	if (outLen == 0 || (*outLen) < md_value_len) {
		return 0;
	}
	if (input) {
		unsigned char buffer[4096];
		int length;

		md_ctx = EVP_MD_CTX_new();
		if (md_ctx == NULL)
			return 0;
		if (!EVP_DigestInit(md_ctx, md))
			goto err;
		while ((length = BIO_read(input, buffer, sizeof(buffer))) > 0) {
			if (!EVP_DigestUpdate(md_ctx, buffer, length))
				goto err;
		}
		if (!EVP_DigestFinal(md_ctx, md_value, NULL))
			goto err;
		md_value_len = EVP_MD_size(md);
		memcpy(out, md_value, md_value_len);
		(*outLen) = md_value_len;
	}
	else {
		long digest_len;
		unsigned char* tmp = OPENSSL_hexstr2buf(digest, &digest_len);
		if (!tmp || md_value_len != digest_len) {
			ERROR("bad digest, %d bytes " "must be specified\n", md_value_len);
			return 0;
		}
		memcpy(out, tmp, digest_len);
		(*outLen) = digest_len;
	}
	rv = md_value_len;
err:
	EVP_MD_CTX_free(md_ctx);
	return rv;
}


static ASN1_INTEGER *create_nonce(int bits)
{
	unsigned char buf[20];
	ASN1_INTEGER *nonce = NULL;
	int len = (bits - 1) / 8 + 1;
	int i;

	if (len > (int)sizeof(buf))
		goto err;
	if (RAND_bytes(buf, len) <= 0)
		goto err;

	// Find the first non-zero byte and creating ASN1_INTEGER object.
	for (i = 0; i < len && !buf[i]; ++i)
		continue;
	if ((nonce = ASN1_INTEGER_new()) == NULL)
		goto err;
	OPENSSL_free(nonce->data);
	nonce->length = len - i;
	nonce->data = (unsigned char*)calloc(nonce->length + 1, sizeof(char));
	memcpy(nonce->data, buf + i, nonce->length);
	return nonce;

err:
	printf("could not create nonce\n");
	ASN1_INTEGER_free(nonce);
	return NULL;
}

static TS_REQ *  genTSReq(const std::string& inData,int certReq, int mdType,const std::string& ext) {
	int ret = 0;
	TS_REQ *ts_req = NULL;
	TS_MSG_IMPRINT *msg_imprint = NULL;

	X509_ALGOR *algo = NULL;
	int len;
	BIO * data_bio = 0;

	unsigned char hash[512] = { 0 };
	int hashLen = sizeof(hash);

	//policy
	const char *policy = 0;
	ASN1_OBJECT *policy_obj = NULL;
	ASN1_INTEGER *nonce_asn1 = NULL;

	//no_nonce
	int no_nonce = 0;


	const EVP_MD *md = 0;
    if(mdType == SGD_SM3){
        md = EVP_sm3();
    }else if(mdType == SGD_SHA1){
        md = EVP_sha1();
    }else if(mdType == SGD_SHA256){
        md = EVP_sha256();
    }else{
        ret = 0; goto err;
        ERROR("invalid md type");
    }


	ts_req = TS_REQ_new();
	if (!TS_REQ_set_version(ts_req, 1)) {
		ERROR("TS_REQ_set_version failed");
		ret = 0; goto err;
	}

	//设置msg_imprint
	msg_imprint = TS_MSG_IMPRINT_new();
	if ((algo = X509_ALGOR_new()) == NULL) {
        ERROR("X509_ALGOR_new failed");
		ret = 0; goto err;
	}
	if ((algo->algorithm = OBJ_nid2obj(EVP_MD_type(md))) == NULL) {
        ERROR("OBJ_nid2obj failed");
		ret = 0; goto err;
	}
	if ((algo->parameter = ASN1_TYPE_new()) == NULL) {
        ERROR("ASN1_TYPE_new failed");
		ret = 0; goto err;
	}
	algo->parameter->type = V_ASN1_NULL;
	if (!TS_MSG_IMPRINT_set_algo(msg_imprint, algo)) {
        ERROR("TS_MSG_IMPRINT_set_algo failed");
		ret = 0; goto err;
	}

    data_bio = BIO_new_mem_buf(inData.data(), inData.length());

	if ((len = create_digest(data_bio, 0, md, (unsigned char*)hash, &hashLen)) == 0) {
        ERROR("create_digest failed");
		ret = 0; goto err;
	}
	TS_MSG_IMPRINT_set_msg(msg_imprint, hash, hashLen);
	if (!TS_REQ_set_msg_imprint(ts_req, msg_imprint)) {
        ERROR("TS_REQ_set_msg_imprint failed");
        ret = 0; goto err;
	}

	//policy
	if (policy && (policy_obj = txt2obj(policy)) == NULL) {
        ERROR("txt2obj failed");
        ret = 0; goto err;
	}
	if (policy_obj && !TS_REQ_set_policy_id(ts_req, policy_obj)) {
        ERROR("TS_REQ_set_policy_id failed");
        ret = 0; goto err;
	}

	// Setting nonce if requested.
	if (!no_nonce && (nonce_asn1 = create_nonce(64)) == NULL) {
        ERROR("create_nonce failed");
        ret = 0; goto err;
	}

	if (nonce_asn1 && !TS_REQ_set_nonce(ts_req, nonce_asn1)) {
        ERROR("TS_REQ_set_nonce failed");
        ret = 0; goto err;
	}

	if (!TS_REQ_set_cert_req(ts_req, certReq)) {
        ERROR("TS_REQ_set_cert_req failed");
        ret = 0; goto err;
	}

    //TODO
    if(ext.length() > 0){
        unsigned char* pp = (unsigned char*)ext.data();
        int ppLen = ext.length();
        X509_EXTENSION *xx = d2i_X509_EXTENSION(0, (const unsigned char**)&pp, ppLen);
        if(xx != 0){
            TS_REQ_add_ext(ts_req,xx, -1);
            X509_EXTENSION_free(xx);
        }
    }


	ret = 1;
err:
	if (ret != 1) {
		TS_REQ_free(ts_req);
		ts_req = NULL;
	}
	TS_MSG_IMPRINT_free(msg_imprint);
	X509_ALGOR_free(algo);
	ASN1_OBJECT_free(policy_obj);
	ASN1_INTEGER_free(nonce_asn1);

	return ts_req;
}





int gm_getTSRq(const std::string& inData,int certReq, int mdType,const std::string& ext, std::string &out) {

    int ret;
	TS_REQ * req = 0;
    unsigned char* derData = 0;
    unsigned char* p;
    int len = 0;
    req = genTSReq(inData, certReq, mdType, ext);
    if(req == 0){
        ERROR("genTSReq failed");
        ret = -1; goto err;
    }
	len = i2d_TS_REQ(req, 0);
	derData = (unsigned char*) calloc(len + 1, sizeof(char));
    if(derData == 0){
        ERROR("calloc failed");
        ret = -1; goto err;
    }

    p = derData;
	len = i2d_TS_REQ(req, &p);
    if(len <= 0){
        ERROR("i2d_TS_REQ failed");
        ret = -1; goto err;
    }

    //test
    {
        BIO* out = BIO_new_file("ts_req.bin", "wb");
        BIO_write(out, derData, len);
        BIO_free(out);
    }

    out.clear();
    out.append((char*)derData, len);
    ret = 0;
err:
    if(req) TS_REQ_free(req);
    if(derData) free(derData);
    return ret;
}



static int _getTS(TS_TST_INFO * tst_info, std::map<int, std::string>& map){
    const ASN1_GENERALIZEDTIME *time = 0;
    BIO* bio = 0;
    char buf[4096] = {0};
    bio = BIO_new(BIO_s_mem());
    time = TS_TST_INFO_get_time(tst_info);
    ASN1_GENERALIZEDTIME_print(bio, time);
    BIO_read(bio, buf, sizeof(buf));
    map[STF_TIME_OF_STAMP] = std::string(buf);
    BIO_free(bio);
    return 0;
}

static int _getTSA(TS_TST_INFO * tst_info, std::map<int, std::string>& map){
   GENERAL_NAME *tsa = 0;
   STACK_OF(CONF_VALUE) *nval;

   tsa = TS_TST_INFO_get_tsa(tst_info);
   if(tsa != 0){
       if ((nval = i2v_GENERAL_NAME(NULL, tsa, NULL))){
           char buf[4096] = {0};
           BIO* bio = 0;
           bio = BIO_new(BIO_s_mem());
           X509V3_EXT_val_prn(bio, nval, 0, 0);
           BIO_read(bio, buf, sizeof(buf));
           map[STF_CN_OF_TSSIGNER] = std::string(buf);
           BIO_free(bio);
       }
       sk_CONF_VALUE_pop_free(nval, X509V3_conf_free);
   }
   return 0;
}

static int _getCert(PKCS7 * token, std::map<int, std::string>& map){
    STACK_OF(X509) *signers = NULL;
    X509 * signer = 0;
    signers = PKCS7_get0_signers(token, 0, 0);
    if (!signers || sk_X509_num(signers) != 1){
        return STF_TS_MALFORMAT;
    }

    signer = sk_X509_value(signers, 0);
    if(signer != 0){
        BIO* bio = 0;
        X509_NAME *pSubName = NULL;
        char buf[40960] = {0};
        int iLen;

        bio = BIO_new(BIO_s_mem());
        PEM_write_bio_X509(bio, signer);
        BIO_read(bio, buf, sizeof(buf));
        BIO_free(bio);

        map[STF_CERT_OF_TSSERVER] = std::string(buf);


        pSubName = X509_get_subject_name(signer);
        //签发者国家
        iLen = X509_NAME_get_text_by_NID(pSubName, NID_countryName, buf, sizeof(buf));
        if (iLen > 0){
            buf[iLen] = '\0';
            map[STF_SUBJECT_COUNTRY_OF_TSSIGNER] = std::string(buf);
        }
        //签发者组织
        iLen = X509_NAME_get_text_by_NID(pSubName, NID_organizationName, buf, sizeof(buf));
        if (iLen > 0){
            buf[iLen] = '\0';
            map[STF_SUBJECT_ORGNIZATION_OF_TSSIGNER] = std::string(buf);
        }
        //签发者城市
        iLen = X509_NAME_get_text_by_NID(pSubName, NID_localityName, buf, sizeof(buf));
        if (iLen > 0){
            buf[iLen] = '\0';
            map[STF_SUBJECT_ORGNIZATION_OF_TSSIGNER] = std::string(buf);
        }
        //签发者联系用电子信箱
        iLen = X509_NAME_get_text_by_NID(pSubName, NID_pkcs9_emailAddress, buf, sizeof(buf));
        if (iLen > 0){
            buf[iLen] = '\0';
            map[STF_SUBJECT_ORGNIZATION_OF_TSSIGNER] = std::string(buf);
        }
    }
    return 0;
}

static int _getCertChain(PKCS7 * token, std::map<int, std::string>& map){
    STACK_OF(X509) *signers = NULL;
    std::string cert_chain;
    signers = PKCS7_get0_signers(token, 0, 0);
    if (!signers || sk_X509_num(signers) == 0){
        return STF_TS_MALFORMAT;
    }
    for(int i=0;i< sk_X509_num(signers);i++){
        X509* x509 = 0;
        BIO* bio = 0;
        X509_NAME *pSubName = NULL;
        char buf[40960] = {0};
        int iLen;
        x509 = sk_X509_value(signers, 0);

        bio = BIO_new(BIO_s_mem());
        PEM_write_bio_X509(bio, x509);
        BIO_read(bio, buf, sizeof(buf));
        BIO_free(bio);

        cert_chain.append((char*)buf, strlen(buf));

    }
    map[STF_CERTCHAIN_OF_TSSERVER] = cert_chain;
    return 0;
}

static int _getAccuracy(TS_TST_INFO * tst_info, std::map<int, std::string>& map){
    const TS_ACCURACY* accuracy = 0;
    accuracy = TS_TST_INFO_get_accuracy(tst_info);
    if (accuracy != 0){
        BIO* bio = 0;
        const ASN1_INTEGER* seconds = 0;
        const ASN1_INTEGER* millis = 0;
        const ASN1_INTEGER* micros = 0;
        char buf[40960] = {0};

        seconds = TS_ACCURACY_get_seconds(accuracy);
        millis = TS_ACCURACY_get_millis(accuracy);
        micros = TS_ACCURACY_get_micros(accuracy);

        bio = BIO_new(BIO_s_mem());
        if (seconds != NULL)
            TS_ASN1_INTEGER_print_bio(bio, seconds);
        else
            BIO_printf(bio, "unspecified");
        BIO_printf(bio, " seconds, ");
        if (millis != NULL)
            TS_ASN1_INTEGER_print_bio(bio, millis);
        else
            BIO_printf(bio, "unspecified");
        BIO_printf(bio, " millis, ");
        if (micros != NULL)
            TS_ASN1_INTEGER_print_bio(bio, micros);
        else
            BIO_printf(bio, "unspecified");
        BIO_printf(bio, " micros");

        BIO_read(bio, buf, sizeof(buf));
        BIO_free(bio);

        map[STF_TIME_PRECISION] = std::string(buf);
    }
    return 0;
}

int gm_parseTS(const std::string& resp, std::map<int, std::string>& map){
    int ret;
    BIO* in_bio = 0;
    TS_RESP *response = NULL;
    TS_TST_INFO * tst_info = 0;

    PKCS7 * token = 0;

    in_bio = BIO_new_mem_buf((void*)resp.data(), resp.length());
    if(in_bio == 0){
        ERROR("BIO_new_mem_buf failed");
        ret = -1; goto err;
    }
    if ((response = d2i_TS_RESP_bio(in_bio, NULL)) == NULL) {
        ERROR("d2i_TS_RESP_bio failed");
        ret = STF_TS_MALFORMAT; goto err;
    }
    tst_info = TS_RESP_get_tst_info(response);
    if(tst_info == 0){
        ERROR("TS_RESP_get_tst_info failed");
        ret = STF_TS_MALFORMAT; goto err;
    }
    token = TS_RESP_get_token(response);
    if(token == 0){
        ERROR("TS_RESP_get_token failed");
        ret = STF_TS_MALFORMAT; goto err;
    }
    //time
    ret = _getTS(tst_info, map);
    if(ret != 0){
        ERROR("_getTS failed");
        goto err;
    }
    //tsa
    ret = _getTSA(tst_info, map);
    if(ret != 0){
        ERROR("_getTSA failed");
        goto err;
    }

    //data
    map[STF_ORINGINAL_DATA] = std::string("");

    //cert
    ret = _getCert(token, map);
    if(ret != 0){
        ERROR("_getCert failed");
        goto err;
    }

    //cert chain
    ret = _getCertChain(token, map);
    if(ret != 0){
        ERROR("_getCertChain failed");
        goto err;
    }

    //时间来源
    map[STF_SOURCE_OF_TIME] = std::string("");

    //时间精度
    ret = _getAccuracy(tst_info, map);
    if(ret != 0){
        ERROR("_getAccuracy failed");
        goto err;
    }

    //响应方式
    map[STF_RESPONSE_TYPE] = std::string("0");


err:
    if(in_bio) BIO_free(in_bio);
    if(response) TS_RESP_free(response);
    return ret;
}










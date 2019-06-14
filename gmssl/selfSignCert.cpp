include <openssl/conf.h>
#include <openssl/pem.h>
#include <openssl/rand.h>
#include <openssl/bn.h>
#include <openssl/ec.h>
#include <openssl/ebcdic.h>
#include <openssl/ecdsa.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/bio.h>
#include <openssl/aes.h>
#include <openssl/engine.h>
#include <../rsa/rsa_locl.h>
#include <../evp/evp_locl.h>
#include <internal/evp_int.h>
#include <../crypto/ec/ec_lcl.h>

int pkey_ctrl_string(EVP_PKEY_CTX *ctx, const char *value)
{
    int rv;
    char *stmp, *vtmp = NULL;
    stmp = OPENSSL_strdup(value);
    if (!stmp)
        return -1;
    vtmp = strchr(stmp, ':');
    if (vtmp) {
        *vtmp = 0;
        vtmp++;
    }
    rv = EVP_PKEY_CTX_ctrl_str(ctx, stmp, vtmp);
    OPENSSL_free(stmp);
    return rv;
}


static int do_sign_init(EVP_MD_CTX *ctx, EVP_PKEY *pkey,
                        const EVP_MD *md, STACK_OF(OPENSSL_STRING) *sigopts)
{
    EVP_PKEY_CTX *pkctx = NULL;
    int i;

    if (ctx == NULL)
        return 0;
    if (!EVP_DigestSignInit(ctx, &pkctx, md, NULL, pkey))
        return 0;
    for (i = 0; i < sk_OPENSSL_STRING_num(sigopts); i++) {
        char *sigopt = sk_OPENSSL_STRING_value(sigopts, i);
        if (pkey_ctrl_string(pkctx, sigopt) <= 0) {
			printf("%s\n", ERR_error_string(ERR_get_error(), NULL));
            return 0;
        }
    }
    return 1;
}

static int do_X509_sign(X509 *x, EVP_PKEY *pkey, const EVP_MD *md,
                 STACK_OF(OPENSSL_STRING) *sigopts)
{
    int rv;
    EVP_MD_CTX *mctx = EVP_MD_CTX_new();

    rv = do_sign_init(mctx, pkey, md, sigopts);
    if (rv > 0)
        rv = X509_sign_ctx(x, mctx);
    EVP_MD_CTX_free(mctx);
    return rv > 0 ? 1 : 0;
}


static X509_NAME *CreateDN(char *pbEmail, char *pbCN, char *pbOU, char *pbO, char *pbL, char *pbST, char *pbC)
{
    X509_NAME *pX509Name = NULL;
    if(pbCN == NULL)
    {
        return NULL;
    }
    
    if (!(pX509Name = X509_NAME_new()))
    {
        return NULL;
    }
    X509_NAME_add_entry_by_txt(pX509Name, "emailAddress", V_ASN1_UTF8STRING, (const unsigned char *)pbEmail, -1, -1, 0);
    X509_NAME_add_entry_by_txt(pX509Name, "CN", V_ASN1_UTF8STRING, (const unsigned char *)pbCN, -1, -1, 0);
    X509_NAME_add_entry_by_txt(pX509Name, "OU", V_ASN1_UTF8STRING, (const unsigned char *)pbOU, -1, -1, 0);
    X509_NAME_add_entry_by_txt(pX509Name, "O", V_ASN1_UTF8STRING, (const unsigned char *)pbO, -1, -1, 0);
    X509_NAME_add_entry_by_txt(pX509Name, "L", V_ASN1_UTF8STRING, (const unsigned char *)pbL, -1, -1, 0);
    X509_NAME_add_entry_by_txt(pX509Name, "ST", V_ASN1_UTF8STRING, (const unsigned char *)pbST, -1, -1, 0);
    X509_NAME_add_entry_by_txt(pX509Name, "C", V_ASN1_UTF8STRING, (const unsigned char *)pbC, -1, -1, 0);
    return pX509Name;
}

static X509_NAME* genX509_NAME(){
	char *pbEmail = "abc001@qq.com";
	char *pbCN = "abc001";
	char *pbOU = "cc";
	char *pbO = "bb";
	char *pbL = "aa";
	char *pbST = "js";
	char *pbC = "CN";


	X509_NAME * xname = CreateDN(pbEmail, pbCN, pbOU, pbO, pbL, pbST, pbC);
	return xname;
}

void testSelfSign(){
	BIO *bio_err = BIO_new_fp(stderr,BIO_NOCLOSE);
	int do_param = 0;
	BIO *out = 0;
	
	EVP_PKEY *pkey = NULL;
	
    EVP_PKEY_CTX *ctx = NULL;
    const EVP_PKEY_ASN1_METHOD *ameth;
    ENGINE *tmpeng = NULL;
    int pkey_id;


	const char *algname ="rsa";
    ameth = EVP_PKEY_asn1_find_str(&tmpeng, algname, -1);


    if (!ameth) {
		printf("%s\n", ERR_error_string(ERR_get_error(), NULL));
		goto err;
    }

    ERR_clear_error();

    EVP_PKEY_asn1_get0_info(&pkey_id, NULL, NULL, NULL, NULL, ameth);

    ENGINE_finish(tmpeng);

    ctx = EVP_PKEY_CTX_new_id(pkey_id, 0);

    if (!ctx){
		printf("%s\n", ERR_error_string(ERR_get_error(), NULL));
		goto err;
	}
   
    
    if (EVP_PKEY_keygen_init(ctx) <= 0){
		printf("%s\n", ERR_error_string(ERR_get_error(), NULL));
		goto err;

    }
	EVP_PKEY_CTX_ctrl_str(ctx,"rsa_keygen_bits","1024");//rctx->nbits = p1;

	if (EVP_PKEY_keygen(ctx, &pkey) <= 0) {
		printf("%s\n", ERR_error_string(ERR_get_error(), NULL));
		goto err;
	}
	
	/*
	out = BIO_new_file("pri.pem", "wb");	 
	PEM_write_bio_PrivateKey(out, pkey, 0, NULL, 0, NULL, 0);
	//i2d_PrivateKey_bio(out, pkey);
	BIO_free(out);
	*/
	/*
	out = BIO_new_file("pri.pem", "rb");	 
    pkey = PEM_read_bio_PrivateKey(out, NULL,0,0);
	BIO_free(out);
	out = BIO_new_file("pub.pem", "wb");
	PEM_write_bio_PUBKEY(out, pkey);
	*/
	X509_REQ* req = X509_REQ_new();
	//版本
    if (!X509_REQ_set_version(req, 0L))
        goto err;    

	//req subjectName
	X509_NAME* n = genX509_NAME();  //个人信息设置
   if (!X509_REQ_set_subject_name(req, n)) {
        X509_NAME_free(n);
        goto err;
    }
   X509_NAME_free(n);

	if (!X509_REQ_set_pubkey(req, pkey))
		goto err;

    //随机序列号
	X509 *x509ss = X509_new();
    //if ("v3_va" && !X509_set_version(x509ss, 2))
        //goto err;
    BIGNUM *btmp;
    btmp = BN_new();

    if (btmp == NULL)
        goto err;

	if (!BN_pseudo_rand(btmp, 64, 0, 0)){
		BN_free(btmp);
		goto err;
	}
        
	if (!BN_to_ASN1_INTEGER(btmp, X509_get_serialNumber(x509ss))){
		BN_free(btmp);
		goto err;
	}
        
     BN_free(btmp);
    
	 //x509 issuerName
    if (!X509_set_issuer_name(x509ss, X509_REQ_get_subject_name(req)))
        goto err;
    if (X509_gmtime_adj(X509_getm_notBefore(x509ss), 0) == NULL)
        goto err;
	//X509_gmtime_adj(X509_get_notAfter(x509), 2 * 24 * 60 * 60);
    if (X509_time_adj_ex(X509_getm_notAfter(x509ss), 365, 0, NULL)
        == NULL)
        goto err;
    //x509 subjectName
    if (!X509_set_subject_name
        (x509ss, X509_REQ_get_subject_name(req)))
        goto err;
    EVP_PKEY* tmppkey = X509_REQ_get0_pubkey(req);
    if (!tmppkey || !X509_set_pubkey(x509ss, tmppkey))
        goto err;
    //int i = do_X509_sign(x509ss, pkey, 0, 0);

	EVP_MD_CTX *mctx = EVP_MD_CTX_new();
    int i = X509_sign_ctx(x509ss, mctx);
    EVP_MD_CTX_free(mctx);
    if (!i) {
        goto err;
    }
	out = BIO_new_file("a1.cer", "wb");	
	i = i2d_X509_bio(out, x509ss);
	//i = PEM_write_bio_X509(out, x509ss);
err:
    EVP_PKEY_free(pkey);
    EVP_PKEY_CTX_free(ctx);
    BIO_free_all(out);
	return;
}



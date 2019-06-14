typedef struct asn1_type_st {
    int type;
    union {
        char *ptr;
        ASN1_BOOLEAN boolean;
        ASN1_STRING *asn1_string;
        ASN1_OBJECT *object;
        ASN1_INTEGER *integer;
        ASN1_ENUMERATED *enumerated;
        ASN1_BIT_STRING *bit_string;
        ASN1_OCTET_STRING *octet_string;
        ASN1_PRINTABLESTRING *printablestring;
        ASN1_T61STRING *t61string;
        ASN1_IA5STRING *ia5string;
        ASN1_GENERALSTRING *generalstring;
        ASN1_BMPSTRING *bmpstring;
        ASN1_UNIVERSALSTRING *universalstring;
        ASN1_UTCTIME *utctime;
        ASN1_GENERALIZEDTIME *generalizedtime;
        ASN1_VISIBLESTRING *visiblestring;
        ASN1_UTF8STRING *utf8string;
        /*
         * set and sequence are left complete and still contain the set or
         * sequence bytes
         */
        ASN1_STRING *set;
        ASN1_STRING *sequence;
        ASN1_VALUE *asn1_value;
    } value;
} ASN1_TYPE;   //类型



struct asn1_object_st {
    const char *sn, *ln;
    int nid;
    int length;
    const unsigned char *data;  /* data remains const after init */
    int flags;                  /* Should we free this one */
};  //OID 类型

struct x509_st {
    X509_CINF cert_info; //证书信息
		/*
			struct x509_cinf_st {
				ASN1_INTEGER *version;      // [ 0 ] default of v1 版本
				ASN1_INTEGER serialNumber; //序列号
				X509_ALGOR signature;  //签名类型
				X509_NAME *issuer;   //颁发者
							//struct X509_name_st {
							//	STACK_OF(X509_NAME_ENTRY) *entries; // DN components 
													//struct X509_name_entry_st {
													//	ASN1_OBJECT *object; // AttributeType 
													//	ASN1_STRING *value;  // AttributeValue 
													//	int set;   // index of RDNSequence for this entry 
													//	int size;     // temp variable 
													//};//X509_NAME_ENTRY
							//	int modified;   //true if 'bytes' needs to be built 
							//	BUF_MEM *bytes;  //cached encoding: cannot be NULL 
							//	canonical encoding used for rapid Name comparison 
							//	unsigned char *canon_enc;
							//	int canon_enclen;
							}; // X509_NAME  
				X509_VAL validity;   //使用者
							//typedef struct X509_val_st {
							//	ASN1_TIME *notBefore;
							//	ASN1_TIME *notAfter;
							//} X509_VAL;
				X509_NAME *subject; //使用者
				X509_PUBKEY *key;  //公钥
							//struct X509_pubkey_st {
							//	X509_ALGOR *algor;
							//	ASN1_BIT_STRING *public_key;
							//	EVP_PKEY *pkey;
							//};
				ASN1_BIT_STRING *issuerUID; //颁发者UID [ 1 ] optional in v2 
				ASN1_BIT_STRING *subjectUID; //使用者UID [ 2 ] optional in v2 
				STACK_OF(X509_EXTENSION) *extensions; //扩展项 [ 3 ] optional in v3 
							//struct X509_extension_st {
							//	ASN1_OBJECT *object;
							//	ASN1_BOOLEAN critical;
							//	ASN1_OCTET_STRING value;
							//}; //X509_EXTENSION
				ASN1_ENCODING enc;
							//typedef struct ASN1_ENCODING_st {
							//	unsigned char *enc;    // DER encoding 
							//	long len;              // Length of encoding 
							//	int modified;      // set to 1 if 'enc' is invalid 
							//} ASN1_ENCODING;
			};
		*/
	
    X509_ALGOR sig_alg; //签名类型
			
		//struct X509_algor_st {
		//	ASN1_OBJECT *algorithm; //sn RSA-SHA256
		//	ASN1_TYPE *parameter;
		//} /* X509_ALGOR */ ;
		
    ASN1_BIT_STRING signature; //签名值
    int references;
    CRYPTO_EX_DATA ex_data;
    /* These contain copies of various extension values */
    long ex_pathlen;
    long ex_pcpathlen;
    uint32_t ex_flags;
    uint32_t ex_kusage;
    uint32_t ex_xkusage;
    uint32_t ex_nscert;
    ASN1_OCTET_STRING *skid;
    AUTHORITY_KEYID *akid;
    X509_POLICY_CACHE *policy_cache;
    STACK_OF(DIST_POINT) *crldp;
    STACK_OF(GENERAL_NAME) *altname;
    NAME_CONSTRAINTS *nc;
#ifndef OPENSSL_NO_RFC3779
    STACK_OF(IPAddressFamily) *rfc3779_addr;
    struct ASIdentifiers_st *rfc3779_asid;
# endif
    unsigned char sha1_hash[32 /*SHA_DIGEST_LENGTH*/];
    X509_CERT_AUX *aux;
    CRYPTO_RWLOCK *lock;
} /* X509 */ ;




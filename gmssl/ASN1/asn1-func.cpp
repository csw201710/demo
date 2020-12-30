ASN1_OBJECT *ASN1_OBJECT_new(void)
{
    ASN1_OBJECT *ret;

    ret = OPENSSL_zalloc(sizeof(*ret));
    if (ret == NULL) {
        ASN1err(ASN1_F_ASN1_OBJECT_NEW, ERR_R_MALLOC_FAILURE);
        return (NULL);
    }
    ret->flags = ASN1_OBJECT_FLAG_DYNAMIC;
    return (ret);
}

void ASN1_OBJECT_free(ASN1_OBJECT *a)
{
    if (a == NULL)
        return;
    if (a->flags & ASN1_OBJECT_FLAG_DYNAMIC_STRINGS) {
#ifndef CONST_STRICT            /* disable purely for compile-time strict
                                 * const checking. Doing this on a "real"
                                 * compile will cause memory leaks */
        OPENSSL_free((void*)a->sn);
        OPENSSL_free((void*)a->ln);
#endif
        a->sn = a->ln = NULL;
    }
    if (a->flags & ASN1_OBJECT_FLAG_DYNAMIC_DATA) {
        OPENSSL_free((void*)a->data);
        a->data = NULL;
        a->length = 0;
    }
    if (a->flags & ASN1_OBJECT_FLAG_DYNAMIC)
        OPENSSL_free(a);
}

ASN1_OBJECT *ASN1_OBJECT_create(int nid, unsigned char *data, int len,
                                const char *sn, const char *ln)
{
    ASN1_OBJECT o;

    o.sn = sn;
    o.ln = ln;
    o.data = data;
    o.nid = nid;
    o.length = len;
    o.flags = ASN1_OBJECT_FLAG_DYNAMIC | ASN1_OBJECT_FLAG_DYNAMIC_STRINGS |
        ASN1_OBJECT_FLAG_DYNAMIC_DATA;
    return (OBJ_dup(&o));
}






/*
asn1 第一个tag字节
	BER_TYPE_BOOLEAN	0x01
	BER_TYPE_INTEGER	0x02
	BER_TYPE_BIT_STRING	0x03
	BER_TYPE_OCTET_STRING	0x04
	BER_TYPE_NULL	0x05
	BER_TYPE_OID	0x06
	BER_TYPE_SEQUENCE	0x30
	BER_TYPE_SNMP_SET	0xA3
*/
// 根据asn1编码，解析处实际证书长度
int calc_cert_length(uint8 cert_header[4])
{
	uint16 cert_len = -1;
	//cert_header[0] == 0x30
	if ((cert_header[1] & 0x80) != 0) //case more than 127 bytes
	{
		if ((cert_header[1] & 0x7f) != 0x02)//check the length word whether correct
		{
			return -1;
		}
		//calculate the actual length of certificate
		cert_len = ((int)cert_header[2] << 8) + cert_header[3] + 4;
	}
	else    //case <=127 bytes,in fact,this will never act
	{
		cert_len = (cert_header[1] & 0x7f) + 2;
	}

	return cert_len;
}



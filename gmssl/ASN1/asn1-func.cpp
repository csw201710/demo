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


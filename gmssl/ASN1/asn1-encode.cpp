/*
struct asn1_object_st {
    const char *sn, *ln;
    int nid;
    int length;
    const unsigned char *data;  /* data remains const after init */
    int flags;                  /* Should we free this one */
};
*/

//NID 自动递增
//Serialized OID's
//tet_a2d_ASN1_OBJECT(data,dataLen,"1.2.156.10197.1.301.1",-1);//    0x2A,0x81,0x1C,0xCF,0x55,0x01,0x82,0x2D,0x01,  /* [ 7144] OBJ_sm2sign */
int tet_a2d_ASN1_OBJECT(unsigned char *out, int olen, const char *buf, int num)
{
    int i, first, len = 0, c, use_bn;
    char ftmp[24], *tmp = ftmp;
    int tmpsize = sizeof ftmp;
    const char *p;
    unsigned long l;
    BIGNUM *bl = NULL;

    if (num == 0)
        return (0);
    else if (num == -1)
        num = strlen(buf);

    p = buf;
    c = *(p++);
    num--;
    if ((c >= '0') && (c <= '2')) {
        first = c - '0';
    } else {
        printf("ASN1err(ASN1_F_A2D_ASN1_OBJECT, ASN1_R_FIRST_NUM_TOO_LARGE)\n");
        goto err;
    }

    if (num <= 0) {
		printf("ASN1err(ASN1_F_A2D_ASN1_OBJECT, ASN1_R_MISSING_SECOND_NUMBER)\n");
        goto err;
    }
    c = *(p++);
    num--;
    for (;;) {
        if (num <= 0)
            break;
        if ((c != '.') && (c != ' ')) {
			printf("ASN1err(ASN1_F_A2D_ASN1_OBJECT, ASN1_R_INVALID_SEPARATOR)\n");
            goto err;
        }
        l = 0;
        use_bn = 0;
        for (;;) {
            if (num <= 0)
                break;
            num--;
            c = *(p++);
            if ((c == ' ') || (c == '.'))
                break;
            if ((c < '0') || (c > '9')) {
				printf("ASN1err(ASN1_F_A2D_ASN1_OBJECT, ASN1_R_INVALID_DIGIT)\n");
                goto err;
            }
            if (!use_bn && l >= ((ULONG_MAX - 80) / 10L)) {
                use_bn = 1;
                if (bl == NULL)
                    bl = BN_new();
                if (bl == NULL || !BN_set_word(bl, l))
                    goto err;
            }
            if (use_bn) {
                if (!BN_mul_word(bl, 10L)
                    || !BN_add_word(bl, c - '0'))
                    goto err;
            } else
                l = l * 10L + (long)(c - '0');
        }
        if (len == 0) {
            if ((first < 2) && (l >= 40)) {
				printf("ASN1err(ASN1_F_A2D_ASN1_OBJECT,ASN1_R_SECOND_NUMBER_TOO_LARGE)\n");
                goto err;
            }
            if (use_bn) {
                if (!BN_add_word(bl, first * 40))
                    goto err;
            } else
                l += (long)first *40;
        }
        i = 0;
        if (use_bn) {
            int blsize;
            blsize = BN_num_bits(bl);
            blsize = (blsize + 6) / 7;
            if (blsize > tmpsize) {
                if (tmp != ftmp)
                    OPENSSL_free(tmp);
                tmpsize = blsize + 32;
                tmp = (char*)OPENSSL_malloc(tmpsize);
                if (tmp == NULL)
                    goto err;
            }
            while (blsize--) {
                BN_ULONG t = BN_div_word(bl, 0x80L);
                if (t == (BN_ULONG)-1)
                    goto err;
                tmp[i++] = (unsigned char)t;
            }
        } else {

            for (;;) {
                tmp[i++] = (unsigned char)l & 0x7f;
                l >>= 7L;
                if (l == 0L)
                    break;
            }

        }
        if (out != NULL) {
            if (len + i > olen) {
				printf("ASN1err(ASN1_F_A2D_ASN1_OBJECT, ASN1_R_BUFFER_TOO_SMALL)\n");
                goto err;
            }
            while (--i > 0)
                out[len++] = tmp[i] | 0x80;
            out[len++] = tmp[0];
        } else
            len += i;
    }
    if (tmp != ftmp)
        OPENSSL_free(tmp);
    BN_free(bl);
    return (len);
 err:
    if (tmp != ftmp)
        OPENSSL_free(tmp);
    BN_free(bl);
    return (0);
}


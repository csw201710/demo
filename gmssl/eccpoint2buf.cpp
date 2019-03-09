/*
*/
static int sm2_point2buf(const EC_GROUP *group, const EC_POINT *point, char  *pubEcc)
{
    int rv = 0;
    BIGNUM *x = NULL;
    BIGNUM *y = NULL;

    x = BN_new();
    y = BN_new();

    /*	从曲线坐标系反投影			*/
	//获取素数域上椭圆曲线上某个点的x和y的几何坐标
    if (!EC_POINT_get_affine_coordinates_GFp(group, point, x, y, NULL)) {
        printf("sm2_point2buf failed\n");
        rv = 0;
        goto err;
    }

    int bitLen = ((256+7) / 8);


    BN_bn2bin(x, pubEcc);
    BN_bn2bin(y, &pubEcc[32]);

    rv = 1;

err:
    if (x) {
        BN_free(x);
        x = NULL;
    }

    if (y) {
        BN_free(y);
        y = NULL;
    }

    return rv;
}


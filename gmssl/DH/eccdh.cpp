
#include <openssl/evp.h>
#include <openssl/ec.h>




int testECDH()
{
	int i;
	EC_KEY *key1,*key2;
	EC_KEY *key3;
		const EC_POINT *pubkey1,*pubkey2;
	// EC_POINT *pubkey3,*pubkey4;
	EC_GROUP *group1,*group2;

	int ret,nid,size;
	EC_builtin_curve *curves = 0;
	int crv_len;
	char shareKey1[128],shareKey2[128];
	int len1,len2;

	unsigned char *out;

#if 0 
	/* 构造EC_KEY 数据结构 */
	key1=EC_KEY_new();
	if(key1==NULL)
	{
		printf("EC_KEY_new err!\n");
		return -1;
	}
	key2=EC_KEY_new();
	if(key2==NULL)
	{
		printf("EC_KEY_new err!\n");
		return -1;
	}


	/* 获取实现的椭圆曲线个数 */
	crv_len = EC_get_builtin_curves(NULL, 0);
	curves = (EC_builtin_curve *)malloc(sizeof(EC_builtin_curve) * crv_len);
	/* 获取椭圆曲线列表 */
	EC_get_builtin_curves(curves, crv_len);
	/*
	nid=curves[0].nid;会有错误，原因是密钥太短
	*/
	/* 选取一种椭圆曲线 */
	nid=curves[25].nid;

	/* 根据选择的椭圆曲线生成密钥参数group */
	group1=EC_GROUP_new_by_curve_name(nid);
	if(group1==NULL)
	{
		printf("EC_GROUP_new_by_curve_name err!\n");
		return -1;
	}
	group2=EC_GROUP_new_by_curve_name(nid);
	if(group2==NULL)
	{
		printf("EC_GROUP_new_by_curve_name err!\n");
		return -1;
	}

	/* 设置密钥参数 */
	ret=EC_KEY_set_group(key1,group1);
	if(ret!=1)
	{
		printf("EC_KEY_set_group err.\n");
		return -1;
	}
	ret=EC_KEY_set_group(key2,group2);
	if(ret!=1)
	{
		printf("EC_KEY_set_group err.\n");
		return -1;
	}
#else
	key1 = EC_KEY_new_by_curve_name(NID_sm2p256v1);
	if(key1==NULL)
	{
		printf("EC_KEY_new_by_curve_name err!\n");
		return -1;
	}
	key2 = EC_KEY_new_by_curve_name(NID_sm2p256v1);
	if(key2==NULL)
	{
		printf("EC_KEY_new_by_curve_name err!\n");
		return -1;
	}
#endif

	/* 生成密钥 */
	ret=EC_KEY_generate_key(key1);
	if(ret!=1)
	{
		printf("EC_KEY_generate_key err.\n");
		return -1;
	}
	ret=EC_KEY_generate_key(key2);
	if(ret!=1)
	{
		printf("EC_KEY_generate_key err.\n");
		return -1;
	}
	/* 检查密钥 */
	ret=EC_KEY_check_key(key1);
	if(ret!=1)
	{
		printf("check key err.\n");
		return -1;
	}
	/* 获取密钥大小 */
	size=ECDSA_size(key2);
	printf("size: %d \n",size);

	/* 获取对方公钥，不能直接引用 */
	key3=EC_KEY_new();
	//key3 = EC_KEY_new_by_curve_name(NID_sm2p256v1);
	//ret=EC_KEY_set_group(key3, EC_GROUP_new_by_curve_name(NID_sm2p256v1));
	ret=EC_KEY_set_group(key3, EC_GROUP_dup(EC_KEY_get0_group(key2)));
	if(ret!=1)
	{
		printf("[key3] EC_KEY_set_group err.\n");
		return -1;
	}


	out=(unsigned char *)malloc(size*3);
	unsigned char *pszTmp = out;
	ret=i2o_ECPublicKey(key2,&pszTmp);

	printf("%d",ret);
	for(int i=0;i<ret;i++)
		printf("%02x",*(out+i));
	
	o2i_ECPublicKey(&key3,(const unsigned char **)&out,ret);
	pubkey2=EC_KEY_get0_public_key(key3);
	if(pubkey2 == 0)
	{
		printf("[key3] EC_KEY_get0_public_key err.\n");
		return -1;
	}

	/* 生成一方的共享密钥 */
	len1=ECDH_compute_key(shareKey1, 128, pubkey2, key1, NULL);

	pubkey1=EC_KEY_get0_public_key(key1);
	/* 生成另一方共享密钥 这里没有转化格式，直接用了*/
	len2=ECDH_compute_key(shareKey2, 128, pubkey1, key2, NULL);
	printf("\n");
	for(i=0;i<len1;i++)
		printf("%x",(unsigned char)shareKey1[i]);
	printf("\n");
	for(i=0;i<len2;i++)
		printf("%x",(unsigned char)shareKey2[i]);
	printf("\n");

	if(len1!=len2)
	{
		printf("shareKey err\n");
	}
	else
	{
		ret=memcmp(shareKey1,shareKey2,len1);
		if(ret==0)
			printf("生成共享密钥成功\ntest ok!\n");
		else
			printf("生成共享密钥失败\n");
	}
	EC_KEY_free(key1);
	EC_KEY_free(key2);
	free(curves);
	return 0;
}



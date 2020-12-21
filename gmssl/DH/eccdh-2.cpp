#include <openssl/ssl.h>
#define ECDH_SIZE 33
 
void handleErrors()
{
	printf("Error occurred.\n");
}
static void disp(const char *str, const void *pbuf, const int size)
{
	int i=0;
	if(str != NULL){
		printf("%s:\n", str);
	}
	if(pbuf != NULL && size > 0){
		for(i=0;i<size;i++)
			printf("%02x ", *((unsigned char *)pbuf+i));
		putchar('\n');
	}
	putchar('\n');
}
 
static EC_KEY *genECDHtemppubkey(unsigned char *pubkey)
{
	int len;
	EC_KEY *ecdh = EC_KEY_new();
 
	//Generate Public
	ecdh = EC_KEY_new_by_curve_name(NID_X9_62_prime256v1);//NID_secp521r1
	EC_KEY_generate_key(ecdh);
	const EC_POINT *point = EC_KEY_get0_public_key(ecdh);
	const EC_GROUP *group = EC_KEY_get0_group(ecdh);
 
	//unsigned char *pubkey = malloc(ECDH_SIZE);
	if(0 == (len = EC_POINT_point2oct(group, point, POINT_CONVERSION_COMPRESSED, pubkey, ECDH_SIZE, NULL))) handleErrors();
	printf("len=%d\n",len);
 
	//return pubkey;
	return ecdh;
}
 
static unsigned char *genECDHsharedsecret(EC_KEY *ecdh, unsigned char *peerkey, size_t secret_len)
{
	int len;
	unsigned char *shared = malloc(ECDH_SIZE);
	const EC_GROUP *group = EC_KEY_get0_group(ecdh);
 
	//ComputeKey
	EC_POINT *point_peer = EC_POINT_new(group);
	EC_POINT_oct2point(group, point_peer, peerkey, ECDH_SIZE, NULL);
 
	//if (0 != EC_POINT_cmp(group, point2, point2c, NULL)) handleErrors();
	if(0 == (len = ECDH_compute_key(shared, secret_len, point_peer, ecdh, NULL))) handleErrors();
	printf("len=%d\n",len);
	disp("shared", shared, secret_len);
 
	return shared;
}
 
int main() {
	unsigned char *keydata = malloc(ECDH_SIZE);
	unsigned char *keydata2 = malloc(ECDH_SIZE);
 
	EC_KEY *ecdh = genECDHtemppubkey(keydata);
	EC_KEY *ecdh2 = genECDHtemppubkey(keydata2);
	unsigned char *ECDH_keydata = genECDHsharedsecret(ecdh2, keydata, ECDH_SIZE-1);
	unsigned char *ECDH_keydata2 = genECDHsharedsecret(ecdh, keydata2, ECDH_SIZE-1);
 
	printf("To the end\n");
	free(keydata);
	free(keydata2);
	EC_KEY_free(ecdh);
	EC_KEY_free(ecdh2);
	free(ECDH_keydata);
	free(ECDH_keydata2);
	return 0;
}


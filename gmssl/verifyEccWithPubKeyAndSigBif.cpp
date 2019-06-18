static EC_KEY* pubkeyToEC_KEY(unsigned char * pubkey, int len){

	EC_KEY *ret;
	BIGNUM *x = NULL;
	BIGNUM *y = NULL;
	if (!(ret = EC_KEY_new_by_curve_name(NID_sm2p256v1))) {
		printf("EC_KEY_new_by_curve_name failed \n");
		return NULL;
	}
	if (!(x = BN_bin2bn(pubkey, 0x20, NULL))) {
		printf("BN_bin2bn failed \n");
		goto end;
	}
	if (!(y = BN_bin2bn(pubkey + 0x20, 0x20, NULL))) {
		printf("BN_bin2bn failed \n");
		goto end;
	}
	if (!EC_KEY_set_public_key_affine_coordinates(ret, x, y)) {
		printf("EC_KEY_set_public_key_affine_coordinates failed \n");
		goto end;
	}
end:
  BN_free(x);
  BN_free(y);
	return ret;

}


int My_SM2_do_verify(unsigned char * pubkey,int pubkeyLen, unsigned char * dgst,int dgstLen, unsigned char* sig,int sigLen)
{
  EC_KEY *ec_key = NULL;
  int ret = 0;
  ECDSA_SIG *ecdSig = 0;
  BIGNUM* r = 0, *s = 0;
	unsigned char *der = NULL;
	int derlen = -1;
  
  ec_key = pubkeyToEC_KEY(pubkey, pubkeyLen);
  if(ec_key == 0){
    ERROR("pubkeyToEC_KEY failed");
    ret = -1;
    goto err;
  }
  if(sigLen < 64){
    ERROR("sigature data sigLen < 64");
    ret = -1;
    goto err;    
  }
  ecdSig = ECDSA_SIG_new();
		if(sig == NULL)
		{
				ERROR("ECDSA_SIG_new error.");
				ret = -1;
				goto err;
		}
  r = BN_new();
		s = BN_new();
		
		ECDSA_SIG_set0(ecdSig, r, s);
		
		if (!BN_bin2bn(sig, 32, r)) {
				ERROR("BN_bin2bn r error.");
				ret = -1;
				goto err;
		}
		if (!BN_bin2bn(sig + 32, 32, s)) {
				ERROR("BN_bin2bn s error.");
				ret = -1;
				goto err;
		}
		/*
  	derlen = i2d_ECDSA_SIG(ecdSig, &der);
  	if (derlen != sigLen || memcmp(sig, der, derlen)) {
  	  ERROR("sigautre data is not correct");
  	  ret = -1;
  		goto err;
  	}
  	*/
  	
  	ret = SM2_do_verify(dgst, dgstLen, ecdSig, ec_key);
		if(ret != 1){
		  ERROR("SM2_do_verify failed");
  	  ret = -1;
  		goto err;
		}
				
  ret = 0;
err:
	if (derlen > 0) {
		OPENSSL_cleanse(der, derlen);
		OPENSSL_free(der);
	}
  ECDSA_SIG_free(ecdSig);
  EC_KEY_free(ec_key);
  
  return ret;
}




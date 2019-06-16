// create by cswhello

//EVP_PKEY_set1_EC_KEY(evpkey,ec_key);
static EC_KEY* pubkeyToEC_KEY(unsigned char * pubkey, int len){
	/*
	EC_KEY *ec_key = EC_KEY_new();
	ec_key = EC_KEY_new_by_curve_name(NID_sm2p256v1);
	const EC_GROUP *ec_group = EC_KEY_get0_group(ec_key);
	BN_CTX *ctx = BN_CTX_new();
	EC_POINT * point1 = EC_POINT_hex2point(ec_group, pubkey, NULL, ctx);
	ec_key -> pub_key = point1;
	BN_CTX_free(ctx);
	return ec_key;
	*/
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


int getSm3DigestWithPubKey(unsigned char * pubkey, int pubkeyLen,unsigned char * inData,int inDataLen, unsigned char* outData, int *poutDataLen){
  EC_KEY *ec_key = NULL;
  int ret = 0;
  
  EVP_MD_CTX *md_ctx = NULL;
  unsigned char za[64];
  size_t zalen = sizeof(za);
  unsigned int outlen;
  
  
  ec_key = pubkeyToEC_KEY(pubkey, pubkeyLen);
  if(ec_key == 0){
    ERROR("pubkeyToEC_KEY failed");
    ret = -1;
    goto err;
  }

  /*
  if(!SM2_compute_message_digest(EVP_sm3(), EVP_sm3(), inData,inDataLen, "1234567812345678", 16, outData, poutDataLen,ec_key)){
    ERROR("SM2_compute_message_digest failed");
    ret = -1;
    goto err;
  }
  */
  
	if (!SM2_compute_id_digest(EVP_sm3(), "1234567812345678", 16, za, &zalen, ec_key)) {
		  ERROR("SM2_compute_id_digest failed");
		  goto err;
	}

  if (!(md_ctx = EVP_MD_CTX_new())){
    ERROR("EVP_MD_CTX_new failed");
    goto err;
  }
  if (!EVP_DigestInit_ex(md_ctx,EVP_sm3(), NULL)){
    ERROR("EVP_DigestInit_ex failed");
    goto err;
  }
  if (!EVP_DigestInit_ex(md_ctx,EVP_sm3(), NULL)){
  		  ERROR("EVP_DigestInit_ex failed");
  		  goto err;
  	}
  if (!EVP_DigestUpdate(md_ctx, za, zalen)){
  		  ERROR("EVP_DigestUpdate failed");
  		  goto err;
  	}
  	
  	
  	
  if (!EVP_DigestUpdate(md_ctx, inData, inDataLen)){
    ERROR("EVP_DigestUpdate failed");
    goto err;
  }
	
  if (!EVP_DigestFinal_ex(md_ctx, outData, (unsigned int *)poutDataLen)){
    ERROR("EVP_DigestFinal_ex failed");
    goto err;
  }

	
  
err:
  EVP_MD_CTX_free(md_ctx);
  EC_KEY_free(ec_key);
  return ret;
}


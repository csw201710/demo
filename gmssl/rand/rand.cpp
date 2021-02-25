// 使用BN_rand_range 生成随机数
	{
		char filePath [256] = {0};
		sprintf(filePath, "D:\\random_data\\%d.bin",i);
		FILE* fp = fopen(filePath,"wb+");
		unsigned char buf[128 * 1024 + 1]={0};
		BIGNUM*  ex = BN_new();
		BIGNUM* order = BN_new();
		BN_init(ex);
		BN_init(order);
		BN_set_bit(order, 8 * 128 * 1024 + 1);
		int num_r = BN_num_bytes(order);
		BN_rand_range(ex,order);
		int num_e = BN_num_bytes(ex);
		BN_bn2bin(ex, buf);
		fwrite(buf,sizeof(char),128 * 1024,fp);
		BN_free(ex);
		BN_free(order);
		fclose(fp);
	}







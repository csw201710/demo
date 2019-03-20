static void hexDump(const char* buf, int len)
{
	int i = 0;
	int leftSize = 0;
	int j = 0;
	const unsigned char *c = (const unsigned  char*)buf;
	if (len < 1 || buf == NULL) return;

	
	
	
 
	for (i = 0; i < (len / 16) * 16; i += 16)
	{
		
		printf("%08x  ",i);
		for (j = 0; j < 16; j++)
		{
			
			printf("%02x", c[i+j]);
			if((j+1)%8 == 0) printf("  "); 
			else printf(" "); 
			
		}
		printf("|");
		for (j = 0; j < 16; j++)
		{
			printf("%c", c[i+j] >= 32 && c[i+j] < 127 ? c[i+j] : '.');
		}
 		printf("|\n");
	}
 
	leftSize = len % 16;
	if (leftSize < 1) return;
		
	printf("%08x  ",i);
	for(j = 0; j< 16; j++){
		
		if(j < leftSize){
			printf("%02x", c[i+j]);
		}else{
			printf("  ");	
		}
		
		if((j+1)%8 == 0) printf("  ");
		else printf(" "); 
	}
	printf("|");	
	for(j = 0; j< 16; j++){
		if(j < leftSize){
			printf("%c", c[i+j] >= 32 && c[i+j] < 127 ? c[i+j] : '.');
		}else{
			printf(" ");	
		}
	}
	printf("|\n");
}


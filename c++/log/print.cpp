#include <stdio.h>
#include <string.h>

void print_data(const char *string, unsigned char*data, int size)
{
	int i;

	printf("---------------------------------------------\n");
	printf("%s:\n", string);
	for(i=0; i<size; i++)
	{
		printf("%02x ", data[i]);
		if((i%16) == 15)
			printf("\n");
	}
	if(size%16)
		printf("\n");
	printf("---------------------------------------------\n");
}


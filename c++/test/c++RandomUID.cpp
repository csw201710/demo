#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <time.h>

#include <iostream>
#include <string>
using namespace std;


std::string RandomUID() {
	string uuid = "";
	const char* c = "0123456789qwertyuiopasdfghjklzxcvbnm";
	srand(time(0));
	for (int n = 0; n < 16; n++) {
		char uuid_temp[32] = {0};
		int b = rand() % 255;
		switch (n) {
		case 6:
			sprintf(uuid_temp ,"4%x", b % 15);
			break;
		case 8:
			sprintf(uuid_temp, "%c%x", c[rand() % strlen(c)], b % 15);
			break;
		default:
			sprintf(uuid_temp, "%02x", b);
			break;
		}
		uuid += uuid_temp;

		switch (n) {
		case 3:
		case 5:
		case 7:
		case 9:
			uuid += '-';
			break;
		}
	}
	return uuid;
}

int main(){
	printf("RandomUID: %s \n", RandomUID().c_str());
	return 0;
}


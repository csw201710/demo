static void getTimeBuf(char * buf,size_t len){

	struct tm* timenow;
	time_t now;
	
	time(&now);	
	timenow = localtime(&now);
	snprintf(buf, len, "%04d-%02d-%02d-%02d-%02d-%02d",
				   timenow->tm_year + 1900, timenow->tm_mon + 1, timenow->tm_mday,
				   timenow->tm_hour, timenow->tm_min, timenow->tm_sec);
	
	return ;
}

int  getTimeBuf(char* szTime){

	time_t now;
	time(&now);
		
	struct tm* timenow;
	timenow = localtime(&now);
	int nLen = sprintf(szTime, "[%04d-%02d-%02d %02d:%02d:%02d] ",
				   timenow->tm_year + 1900, timenow->tm_mon + 1, timenow->tm_mday,
				   timenow->tm_hour, timenow->tm_min, timenow->tm_sec);
	return (int)nLen;
}

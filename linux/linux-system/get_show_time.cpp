static const char* showTime(time_t t){
  tm  sttm;
  static char szBuf[64] = {0};
  sttm = *localtime(&t);
  strftime(szBuf, 64, "%Y-%m-%d %H:%M:%S", &sttm);
  return (const char*) szBuf;
}

std::string getCurFormatTime()
{
   char buffer[32] = {0};
   std::string ret = "";
   time_t rawtime;
   struct tm *info;
   time( &rawtime );
   info = localtime( &rawtime );
   strftime(buffer, sizeof(buffer), "%Y%m%d%H%M%SZ+0800", info);
   ret = buffer;
   return ret;
}


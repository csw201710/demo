//for linux 
static std::string StrFormat(const char *pszFmt, ...)
{
  char *buf = 0;
	std::string str;
	va_list args;
	va_start(args, pszFmt);
	vasprintf(&buf, pszFmt, args);
	if(buf != 0){
	  str = buf;
	  free(buf);
	}else{
	  ERROR("vasprintf failed");
	}

	va_end(args);
	return str;
}



static std::string fmt(const char *pszFmt, ...)
{
#ifdef _WIN32
	std::string str;
	va_list args;
	va_start(args, pszFmt);
	{
		int nLength = _vscprintf(pszFmt, args);
		nLength += 1; //add again
		std::vector<char> vectorChars(nLength);
		_vsnprintf(vectorChars.data(), nLength, pszFmt, args);
		str.assign(vectorChars.data());
	}
	va_end(args);
	return str;
#else
  va_list ap;
  va_start(ap, pszFmt);
  char buf[2048];
  int rv;
  rv = vsnprintf(buf, sizeof(buf), pszFmt, ap);

  if (rv < 0) {
    buf[0] = '\0';
  }
  
  va_end(ap);
  return buf;
#endif
}

//vs 不定参printf
static int default_error_callback(const char *format, ...)
{
  int ret;
  va_list argptr;
  va_start(argptr, format);
  ret = vfprintf(stderr, format, argptr);
  va_end(argptr);
  return ret;
}





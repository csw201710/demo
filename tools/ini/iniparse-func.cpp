static char* _load_key(const char* iniPath,const char* key, const char* defaultValue);
static int _load_key(const char* iniPath,const char* key, int defaultValue);

static char* _load_key(const char* iniPath,const char* key, const char* defaultValue){
  static char *ret=0;
  dictionary *ini=0;
  ini = iniparser_load(iniPath);
  if(ini == NULL){
    ERROR("load file:%s failed!",iniPath);
    return 0;
  }
  const char *iniValue = iniparser_getstring(ini,key ,defaultValue);

  ret = (char *) calloc(strlen(iniValue) + 1, sizeof(char));
  
  memcpy(ret,iniValue, strlen(iniValue));
  iniparser_freedict(ini);
  INFO("load key [%s] value:[%s]", key, ret);
  return ret;
  
}
static int _load_key(const char* iniPath,const char* key, int defaultValue){
  int ret=0;
  dictionary *ini=0;
  ini = iniparser_load(iniPath);
  if(ini == NULL){
    ERROR("load file:%s failed!",iniPath);
    return 0;
  }
  ret = iniparser_getint(ini,key ,defaultValue);
  iniparser_freedict(ini);
  INFO("load key [%s] value:[%d]", key, ret);
  return ret;
  
}

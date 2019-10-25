#include<libintl.h>
#include<locale.h>
#include<stdio.h>

#define _(String) gettext (String)
#define _t(String1,String2,n) ngettext (String1,String2,n)

int main()
{
    int i = 10;
    setlocale(LC_ALL,"zh_CN");
    bindtextdomain("hellomain","/usr/share/locale");
    textdomain("hellomain");
    printf(_("Hello World\n"));
    //printf(_t("test001 %d a","test002 %d  aa",i),i);
    printf(_("The number is %d\n"),i);
}//main

/*

xgettext -d hellomain -o hellomain.pot --keyword=_t:1,2 -k_ -s main.c
  -d, --default-domain=NAME   use NAME.po for output (instead of messages.po)
  -o, --output=FILE           write output to specified file
  -k, --keyword               do not to use default keywords
  -s, --sort-output           generate sorted output


xgettext -k_ -o hellomain.pot  main.cpp 
	-a, --extract-all           extract all strings
	-k, --keyword               do not to use default keywords

	
msgfmt hellomain.po -o hellomain.mo

	
//export LANG=zh_CN.UTF-8 //忽略

/usr/share/locale/zh_CN.GB2312/LC_MESSAGES/hellomain.mo
/usr/share/locale/zh_CN/LC_MESSAGES/hellomain.mo

*/



  
  

#include <stdio.h>


#include "LOGC.h"

using namespace LOG;
int test_logc()
{
    char    buf[ 64 + 1 ] ;
    long    buflen ;
    
    SetLogFile("test_logc.log" );
    SetLogLevel(LOGLEVEL_FATAL );
	   SwitchStdOut(1);
	    
    DEBUGLOG( "call DebugLog" );
    INFOLOG(  "call InfoLog" );
    WARNLOG( "call WarnLog" );
    ERRORLOG("call ErrorLog" );
    FATALLOG("call FatalLog" );
    
    memset( buf , 0x00 , sizeof(buf) );
    buflen = sizeof(buf) - 1 ;
    DebugHexLog( __FILE__ , __LINE__ , buf , buflen , "call DebugHexLog" );
    InfoHexLog( __FILE__ , __LINE__ , buf , buflen , "call InfoHexLog" );
    WarnHexLog( __FILE__ , __LINE__ , buf , buflen , "call WarnHexLog" );
    ErrorHexLog( __FILE__ , __LINE__ , buf , buflen , "call ErrorHexLog" );
    FatalHexLog( __FILE__ , __LINE__ , buf , buflen , "call FatalHexLog" );
    
    return 0;
}

int main()
{
	test_logc();

	return 0;
}


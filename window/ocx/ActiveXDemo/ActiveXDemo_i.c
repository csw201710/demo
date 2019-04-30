

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Tue Apr 30 10:58:20 2019
 */
/* Compiler settings for ActiveXDemo.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, LIBID_ActiveXDemoLib,0x6C8A24C1,0x5F75,0x4555,0xB8,0x93,0x05,0x1B,0x3A,0x7D,0x25,0x5A);


MIDL_DEFINE_GUID(IID, DIID__DActiveXDemo,0x7FA6A843,0x6D85,0x4825,0xBB,0x00,0xE0,0xEF,0x3F,0x48,0xBE,0xFB);


MIDL_DEFINE_GUID(IID, DIID__DActiveXDemoEvents,0x2B04342B,0xBC5E,0x442B,0x9E,0xC2,0x84,0x07,0x6F,0x8A,0x2C,0xE5);


MIDL_DEFINE_GUID(CLSID, CLSID_ActiveXDemo,0x0153809B,0x60ED,0x402B,0x9E,0xF2,0x8F,0x46,0xE2,0x17,0x6C,0x6D);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif




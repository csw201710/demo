

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


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


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __ActiveXDemoidl_h__
#define __ActiveXDemoidl_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef ___DActiveXDemo_FWD_DEFINED__
#define ___DActiveXDemo_FWD_DEFINED__
typedef interface _DActiveXDemo _DActiveXDemo;
#endif 	/* ___DActiveXDemo_FWD_DEFINED__ */


#ifndef ___DActiveXDemoEvents_FWD_DEFINED__
#define ___DActiveXDemoEvents_FWD_DEFINED__
typedef interface _DActiveXDemoEvents _DActiveXDemoEvents;
#endif 	/* ___DActiveXDemoEvents_FWD_DEFINED__ */


#ifndef __ActiveXDemo_FWD_DEFINED__
#define __ActiveXDemo_FWD_DEFINED__

#ifdef __cplusplus
typedef class ActiveXDemo ActiveXDemo;
#else
typedef struct ActiveXDemo ActiveXDemo;
#endif /* __cplusplus */

#endif 	/* __ActiveXDemo_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __ActiveXDemoLib_LIBRARY_DEFINED__
#define __ActiveXDemoLib_LIBRARY_DEFINED__

/* library ActiveXDemoLib */
/* [control][version][uuid] */ 


EXTERN_C const IID LIBID_ActiveXDemoLib;

#ifndef ___DActiveXDemo_DISPINTERFACE_DEFINED__
#define ___DActiveXDemo_DISPINTERFACE_DEFINED__

/* dispinterface _DActiveXDemo */
/* [uuid] */ 


EXTERN_C const IID DIID__DActiveXDemo;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("7FA6A843-6D85-4825-BB00-E0EF3F48BEFB")
    _DActiveXDemo : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DActiveXDemoVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DActiveXDemo * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DActiveXDemo * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DActiveXDemo * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DActiveXDemo * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DActiveXDemo * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DActiveXDemo * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DActiveXDemo * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DActiveXDemoVtbl;

    interface _DActiveXDemo
    {
        CONST_VTBL struct _DActiveXDemoVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DActiveXDemo_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DActiveXDemo_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DActiveXDemo_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DActiveXDemo_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DActiveXDemo_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DActiveXDemo_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DActiveXDemo_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DActiveXDemo_DISPINTERFACE_DEFINED__ */


#ifndef ___DActiveXDemoEvents_DISPINTERFACE_DEFINED__
#define ___DActiveXDemoEvents_DISPINTERFACE_DEFINED__

/* dispinterface _DActiveXDemoEvents */
/* [uuid] */ 


EXTERN_C const IID DIID__DActiveXDemoEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("2B04342B-BC5E-442B-9EC2-84076F8A2CE5")
    _DActiveXDemoEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DActiveXDemoEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DActiveXDemoEvents * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DActiveXDemoEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DActiveXDemoEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DActiveXDemoEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DActiveXDemoEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DActiveXDemoEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DActiveXDemoEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DActiveXDemoEventsVtbl;

    interface _DActiveXDemoEvents
    {
        CONST_VTBL struct _DActiveXDemoEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DActiveXDemoEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DActiveXDemoEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DActiveXDemoEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DActiveXDemoEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DActiveXDemoEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DActiveXDemoEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DActiveXDemoEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DActiveXDemoEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_ActiveXDemo;

#ifdef __cplusplus

class DECLSPEC_UUID("0153809B-60ED-402B-9EF2-8F46E2176C6D")
ActiveXDemo;
#endif
#endif /* __ActiveXDemoLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif



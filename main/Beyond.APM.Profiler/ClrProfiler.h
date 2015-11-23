// ClrProfiler.h : CClrProfiler 的声明

#pragma once
#include "resource.h"       // 主符号



#include "BeyondAPMProfiler_i.h"
#include "ProfileBase.h"
#include "ProfilerInfo.h"
#include <unordered_map>
#include "ReleaseTrace.h"
#include "Method.h"
#include "TracerCfg.h"
#include "SignatureBlob.h"




#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE 平台(如不提供完全 DCOM 支持的 Windows Mobile 平台)上无法正确支持单线程 COM 对象。定义 _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA 可强制 ATL 支持创建单线程 COM 对象实现并允许使用其单线程 COM 对象实现。rgs 文件中的线程模型已被设置为“Free”，原因是该模型是非 DCOM Windows CE 平台支持的唯一线程模型。"
#endif

using namespace ATL;


#define COM_FAIL_MSG_RETURN_ERROR(hr, msg) if (!SUCCEEDED(hr)) { RELTRACE(msg, hr); return (hr); }

//#define COM_FAILMSG(hr, msg) if (!SUCCEEDED(hr)) { RELTRACE(msg, hr); return; }

#define COM_FAIL_MSG_RETURN_OTHER(hr, ret, msg) if (!SUCCEEDED(hr)) { RELTRACE(msg, hr); return (ret); }

typedef void(__fastcall *ipv)(ULONG);

#define BUFFER_SIZE 16384
/// <summary>The main profiler COM object</summary>
class ATL_NO_VTABLE CClrProfiler :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CClrProfiler, &CLSID_ClrProfiler>,
	//public IDispatchImpl<IClrProfiler, &IID_IClrProfiler, &LIBID_BeyondAPMProfilerLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public CProfilerBase
{
public:
	CClrProfiler()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_CLRPROFILER)


BEGIN_COM_MAP(CClrProfiler)
	COM_INTERFACE_ENTRY(ICorProfilerCallback)
    COM_INTERFACE_ENTRY(ICorProfilerCallback2)
    COM_INTERFACE_ENTRY(ICorProfilerCallback3)
#ifndef _TOOLSETV71
    COM_INTERFACE_ENTRY(ICorProfilerCallback4)
    COM_INTERFACE_ENTRY(ICorProfilerCallback5)
#endif
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
		 if (m_profilerInfo!=NULL) m_profilerInfo.Release();
        if (m_profilerInfo2!=NULL) m_profilerInfo2.Release();
        if (m_profilerInfo3!=NULL) m_profilerInfo3.Release();
#ifndef _TOOLSETV71
        if (m_profilerInfo4!=NULL) m_profilerInfo4.Release();
#endif
	}

public:
	CComQIPtr<ICorProfilerInfo> m_profilerInfo;
    CComQIPtr<ICorProfilerInfo2> m_profilerInfo2;
    CComQIPtr<ICorProfilerInfo3> m_profilerInfo3;
#ifndef _TOOLSETV71
    CComQIPtr<ICorProfilerInfo4> m_profilerInfo4;
#endif

    std::wstring GetModulePath(ModuleID moduleId);
    std::wstring GetModulePath(ModuleID moduleId,LPCBYTE &pModuleBaseLoadAddress, AssemblyID *pAssemblyId);
    std::wstring GetAssemblyName(AssemblyID assemblyId);
    BOOL GetTokenAndModule(FunctionID funcId, mdToken& functionToken, ModuleID& moduleId, std::wstring &modulePath,LPCBYTE &pModuleBaseLoadAddress, AssemblyID *pAssemblyId);
	std::wstring GetTypeAndMethodName(FunctionID functionId,PCCOR_SIGNATURE &ppvSig,ULONG &pcbSig,DWORD &wdMethodAttr,DWORD &wdMethodImplFlags,mdTypeDef &classId);
    void __fastcall AddVisitPoint(ULONG uniqueId);

private:

	HRESULT ProfilerInitialise(IUnknown *pICorProfilerInfoUnk);
	DWORD AppendProfilerEventMask(DWORD currentEventMask);

	ipv static GetInstrumentPointVisit();
private:
    static UINT_PTR _stdcall FunctionMapper2(FunctionID functionId, void* clientData, BOOL* pbHookFunction);
    static UINT_PTR _stdcall FunctionMapper(FunctionID functionId, BOOL* pbHookFunction);
public:
    void FunctionEnter2(
    /*[in]*/FunctionID                          funcID, 
    /*[in]*/UINT_PTR                            clientData, 
    /*[in]*/COR_PRF_FRAME_INFO                  func, 
    /*[in]*/COR_PRF_FUNCTION_ARGUMENT_INFO      *argumentInfo);

    void FunctionLeave2(
    /*[in]*/FunctionID                          funcID, 
    /*[in]*/UINT_PTR                            clientData, 
    /*[in]*/COR_PRF_FRAME_INFO                  func, 
    /*[in]*/COR_PRF_FUNCTION_ARGUMENT_RANGE     *retvalRange);

    void FunctionTailcall2(
    /*[in]*/FunctionID                          funcID, 
    /*[in]*/UINT_PTR                            clientData, 
    /*[in]*/COR_PRF_FRAME_INFO                  func);
private:
    std::unordered_map<std::wstring, bool> m_allowModules;
    std::unordered_map<std::wstring, std::wstring> m_allowModulesAssemblyMap;
	std::vector<TracerNode*> m_traceList;
	std::unordered_map<std::wstring,TracerNode*> m_tracerMap;

    COR_PRF_RUNTIME_TYPE m_runtimeType;
    ASSEMBLYMETADATA m_runtimeVersion;

    bool m_useOldStyle;
	ULONG m_threshold;
	bool m_tracingEnabled;
	FILE *stream;//log file
	//Logger logger;
private://CClrProfiler_ProfilerInfo.cpp
    mdSignature GetMethodSignatureToken_I4(ModuleID moduleID); 
    HRESULT GetModuleRef(ModuleID moduleId, WCHAR*moduleName, mdModuleRef &mscorlibRef);

    HRESULT GetModuleRef4000(IMetaDataAssemblyEmit *metaDataAssemblyEmit, WCHAR*moduleName, mdModuleRef &mscorlibRef);
    HRESULT GetModuleRef2000(IMetaDataAssemblyEmit *metaDataAssemblyEmit, WCHAR*moduleName, mdModuleRef &mscorlibRef);
    HRESULT GetModuleRef2050(IMetaDataAssemblyEmit *metaDataAssemblyEmit, WCHAR*moduleName, mdModuleRef &mscorlibRef);
	void ReadTracerConfig(const char* configXML,std::unordered_map<std::wstring,TracerNode*> &tracerList);
	ULONG GetParamCountFromMethodSig(PCCOR_SIGNATURE &pSignature) const;
	CorElementType BypassOptCustomMod(PCCOR_SIGNATURE& pSignature) const;
	//LPCWSTR C2W(const char* _src);
	//std::wstring C2W(const std::string & s);
private:

    CComObject<CProfilerInfo> *m_infoHook;
    HRESULT OpenCoverSupportInitialize(IUnknown *pICorProfilerInfoUnk);
	HRESULT InstrumentMethodWith(ModuleID moduleId,mdToken functionToken,Method &method, mdSignature newLocalVarSig,InstructionList &instructions);
	HRESULT InstrumentMethodWith2(ModuleID moduleId,mdToken functionToken,Method &method, InstructionList &instructionsBefroe,InstructionList &instructionsEnd,ULONG newLocalVarStart);
	WORD EmitNewLocalVarToken(ModuleID moduleId,mdSignature tkOldLocalVarToken, mdSignature &tkNewLocalVarToken);
	friend class CProfilerInfo;
public:
    virtual HRESULT STDMETHODCALLTYPE Initialize( 
        /* [in] */ IUnknown *pICorProfilerInfoUnk);
    virtual HRESULT STDMETHODCALLTYPE Shutdown( void);

    //virtual HRESULT STDMETHODCALLTYPE ModuleAttachedToAssembly( 
    //    /* [in] */ ModuleID moduleId,
    //    /* [in] */ AssemblyID assemblyId);
    //
     virtual HRESULT STDMETHODCALLTYPE ModuleLoadFinished( 
        /* [in] */ ModuleID moduleId,
        /* [in] */ HRESULT hrStatus);

    virtual HRESULT STDMETHODCALLTYPE JITCompilationStarted( 
        /* [in] */ FunctionID functionId,
        /* [in] */ BOOL fIsSafeToBlock);
};

OBJECT_ENTRY_AUTO(__uuidof(ClrProfiler), CClrProfiler)

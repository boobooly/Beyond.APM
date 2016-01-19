// ClrProfiler.cpp : CClrProfiler 的实现

#include "stdafx.h"
#include "ClrProfiler.h"

#include "dllmain.h"
//#include "ATLTRACE_HELPER.h"
//
//#include <log4cplus/logger.h>  
//#include <log4cplus/configurator.h>  
//#include <log4cplus/loggingmacros.h>
//#include <log4cplus/layout.h>
//#include <log4cplus/fileappender.h>
//#include <log4cplus/helpers/loglog.h>
//#include <log4cplus/ndc.h>
//#include <iomanip>  

// CClrProfiler
//using namespace std;
//using namespace log4cplus;
//using namespace log4cplus::helpers;
//using namespace tinyxml2;

/// <summary>Handle <c>ICorProfilerCallback::Initialize</c></summary>
/// <remarks>Initialize the profiling environment and establish connection to the host</remarks>
HRESULT STDMETHODCALLTYPE CClrProfiler::Initialize(
	/* [in] */ IUnknown *pICorProfilerInfoUnk)
{
	return ProfilerInitialise(pICorProfilerInfoUnk);
}

HRESULT CClrProfiler::ProfilerInitialise(IUnknown *pICorProfilerInfoUnk){
	//1.initialize log4cplus
	/*log4cplus::initialize ();
    helpers::LogLog::getLogLog()->setInternalDebugging(true);
    log4cplus::SharedAppenderPtr append_1(
        new log4cplus::RollingFileAppender(LOG4CPLUS_TEXT("BeyondAPM.log"), 5*1024, 5));
    append_1->setName(LOG4CPLUS_TEXT("First"));
    append_1->setLayout( std::auto_ptr<log4cplus::Layout>(new log4cplus::TTCCLayout()) );
    log4cplus::Logger::getRoot().addAppender(append_1);

	log4cplus::Logger logger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("Profiler"));*/
	
	//1.ATLTRACE to file initialize

	//TRACEHELPEXW(MY_CATEGORY,0,L"::BeyondAPM Initialise");
	//FILE *stream;
	//freopen_s(&stream,"c:\\beyondAPM.txt","a",stdout);
	_wfreopen_s(&stream,L"c:\\beyondAPM.txt",L"a",stdout);
	fwprintf(stream,L"haha\n");
	//fclose(stream);

	//read CoreInstrument.xml
	ReadTracerConfig("c:\\CoreInstrumentation.xml",m_tracerMap);
	/*ATLTRACE(_T("tinyxml2 ok"));*/
	//ATLTRACE(_T("doc.ErrorID is :%d",doc.ErrorID));

	ATLTRACE(_T("::BeyondAPM Initialise"));

    OLECHAR szGuid[40]={0};
	int nCount = ::StringFromGUID2(CLSID_ClrProfiler, szGuid, 40);
    RELTRACE(L"    ::Initialize(...) => CLSID == %s", szGuid);
    //::OutputDebugStringW(szGuid);

    WCHAR szExeName[MAX_PATH];
    GetModuleFileNameW(NULL, szExeName, MAX_PATH);
    RELTRACE(L"    ::Initialize(...) => EXE = %s", szExeName);

    WCHAR szModuleName[MAX_PATH];
    GetModuleFileNameW(_AtlModule.m_hModule, szModuleName, MAX_PATH);
    RELTRACE(L"    ::Initialize(...) => PROFILER = %s", szModuleName);
    //::OutputDebugStringW(szModuleName);

    //if (g_pProfiler!=NULL) 
    //    RELTRACE(_T("Another instance of the profiler is running under this process..."));

    m_profilerInfo = pICorProfilerInfoUnk;
    if (m_profilerInfo != NULL) ATLTRACE(_T("    ::Initialize (m_profilerInfo OK)"));
    if (m_profilerInfo == NULL) return E_FAIL;
    m_profilerInfo2 = pICorProfilerInfoUnk;
    if (m_profilerInfo2 != NULL) ATLTRACE(_T("    ::Initialize (m_profilerInfo2 OK)"));
    if (m_profilerInfo2 == NULL) return E_FAIL;
    m_profilerInfo3 = pICorProfilerInfoUnk;
#ifndef _TOOLSETV71
    m_profilerInfo4 = pICorProfilerInfoUnk;
#endif

    ZeroMemory(&m_runtimeVersion, sizeof(m_runtimeVersion));
    if (m_profilerInfo3 != NULL) 
    {
        ATLTRACE(_T("    ::Initialize (m_profilerInfo3 OK)"));
        
        ZeroMemory(&m_runtimeVersion, sizeof(m_runtimeVersion));
        m_profilerInfo3->GetRuntimeInformation(NULL, &m_runtimeType, 
            &m_runtimeVersion.usMajorVersion, 
            &m_runtimeVersion.usMinorVersion, 
            &m_runtimeVersion.usBuildNumber, 
            &m_runtimeVersion.usRevisionNumber, 0, NULL, NULL); 

        ATLTRACE(_T("    ::Initialize (Runtime %d)"), m_runtimeType);
    }

  /*  TCHAR key[1024] = {0};
    ::GetEnvironmentVariable(_T("OpenCover_Profiler_Key"), key, 1024);
    RELTRACE(_T("    ::Initialize(...) => key = %s"), key);

    TCHAR ns[1024] = {0};
    ::GetEnvironmentVariable(_T("OpenCover_Profiler_Namespace"), ns, 1024);
    ATLTRACE(_T("    ::Initialize(...) => ns = %s"), ns);

    TCHAR instrumentation[1024] = {0};
    ::GetEnvironmentVariable(_T("OpenCover_Profiler_Instrumentation"), instrumentation, 1024);
    ATLTRACE(_T("    ::Initialize(...) => instrumentation = %s"), instrumentation);

    TCHAR threshold[1024] = {0};
    ::GetEnvironmentVariable(_T("OpenCover_Profiler_Threshold"), threshold, 1024);
    m_threshold = _tcstoul(threshold, NULL, 10);
    ATLTRACE(_T("    ::Initialize(...) => threshold = %ul"), m_threshold);

    TCHAR tracebyTest[1024] = {0};
    ::GetEnvironmentVariable(_T("OpenCover_Profiler_TraceByTest"), tracebyTest, 1024);
    m_tracingEnabled = _tcslen(tracebyTest) != 0;
	ATLTRACE(_T("    ::Initialize(...) => tracingEnabled = %s (%s)"), m_tracingEnabled ? _T("true") : _T("false"), tracebyTest);*/


    //m_useOldStyle = (tstring(instrumentation) == _T("oldSchool"));

	////communication host initialise
 //   if (!m_host.Initialise(key, ns, szExeName))
 //   {
 //       RELTRACE(_T("    ::Initialize => Profiler will not run for this process."));
 //       return E_FAIL;
 //   }

 //   OpenCoverSupportInitialize(pICorProfilerInfoUnk);

	//if (m_chainedProfiler == NULL){
		DWORD dwMask = AppendProfilerEventMask(0); 

		COM_FAIL_MSG_RETURN_ERROR(m_profilerInfo2->SetEventMask(dwMask),
			_T("    ::Initialize(...) => SetEventMask => 0x%X"));
	//}

 //   if(m_profilerInfo3 != NULL)
 //   {
 //       COM_FAIL_MSG_RETURN_ERROR(m_profilerInfo3->SetFunctionIDMapper2(FunctionMapper2, this), 
 //           _T("    ::Initialize(...) => SetFunctionIDMapper2 => 0x%X"));
 //   }
 //   else
 //   {
 //       COM_FAIL_MSG_RETURN_ERROR(m_profilerInfo2->SetFunctionIDMapper(FunctionMapper), 
 //           _T("    ::Initialize(...) => SetFunctionIDMapper => 0x%X"));
 //   }

 //   //g_pProfiler = this;

//#ifndef _TOOLSETV71
//    COM_FAIL_MSG_RETURN_ERROR(m_profilerInfo2->SetEnterLeaveFunctionHooks2(
//        _FunctionEnter2, _FunctionLeave2, _FunctionTailcall2), 
//        _T("    ::Initialize(...) => SetEnterLeaveFunctionHooks2 => 0x%X"));
//#endif
//    RELTRACE(_T("::Initialize - Done!"));
    
    return S_OK; 
}

HRESULT CClrProfiler::OpenCoverSupportInitialize(
	/* [in] */ IUnknown *pICorProfilerInfoUnk)
{
	    HRESULT hr2 = CComObject<CProfilerInfo>::CreateInstance(&m_infoHook);
		ULONG count = m_infoHook->AddRef();

		m_infoHook->m_pProfilerHook = this;

		m_infoHook->SetProfilerInfo(pICorProfilerInfoUnk);

		return S_OK;
}
DWORD CClrProfiler::AppendProfilerEventMask(DWORD currentEventMask)
{
	DWORD dwMask = currentEventMask;
	dwMask |= COR_PRF_MONITOR_MODULE_LOADS;			// Controls the ModuleLoad, ModuleUnload, and ModuleAttachedToAssembly callbacks.
	dwMask |= COR_PRF_MONITOR_JIT_COMPILATION;	    // Controls the JITCompilation, JITFunctionPitched, and JITInlining callbacks.
	dwMask |= COR_PRF_DISABLE_INLINING;				// Disables all inlining.
	dwMask |= COR_PRF_DISABLE_OPTIMIZATIONS;		// Disables all code optimizations.
	dwMask |= COR_PRF_USE_PROFILE_IMAGES;           // Causes the native image search to look for profiler-enhanced images

	//if (m_tracingEnabled)
	//	dwMask |= COR_PRF_MONITOR_ENTERLEAVE;       // Controls the FunctionEnter, FunctionLeave, and FunctionTailcall callbacks.

	//if (m_useOldStyle)
	//	dwMask |= COR_PRF_DISABLE_TRANSPARENCY_CHECKS_UNDER_FULL_TRUST;      // Disables security transparency checks that are normally done during just-in-time (JIT) compilation and class loading for full-trust assemblies. This can make some instrumentation easier to perform.

#ifndef _TOOLSETV71
	if (m_profilerInfo4 != NULL)
	{
		ATLTRACE(_T("    ::Initialize (m_profilerInfo4 OK)"));
		dwMask |= COR_PRF_DISABLE_ALL_NGEN_IMAGES;
	}
#endif

    dwMask |= COR_PRF_MONITOR_THREADS;

	return dwMask;
}

/// <summary>Handle <c>ICorProfilerCallback::Shutdown</c></summary>
HRESULT STDMETHODCALLTYPE CClrProfiler::Shutdown( void) 
{ 
    RELTRACE(_T("::Shutdown - Starting"));

  //  if (m_chainedProfiler != NULL)
		//m_chainedProfiler->Shutdown();

  //  m_host.CloseChannel(m_tracingEnabled);

    WCHAR szExeName[MAX_PATH];
    GetModuleFileNameW(NULL, szExeName, MAX_PATH);
    RELTRACE(_T("::Shutdown - Nothing left to do but return S_OK(%s)"), szExeName);
    //g_pProfiler = NULL;
	fwprintf(stream,L"Profiler ShutDown()!\n");
	 fclose(stream);
    return S_OK; 
}

HRESULT STDMETHODCALLTYPE CClrProfiler::ModuleLoadFinished(
	/* [in] */ ModuleID moduleId,
	/* [in] */ HRESULT hrStatus)
{
	/*if (m_chainedProfiler != NULL)
		m_chainedProfiler->ModuleLoadFinished(moduleId, hrStatus);

	return RegisterCuckoos(moduleId);*/
	std::wstring szModulePath = GetModulePath(moduleId);
	/*fwprintf(stream,(std::wstring(L"[ModuleLoadFinished]:") + szModulePath).c_str());
	fwprintf(stream,L"\n");*/
	ATLTRACE(szModulePath.c_str());
	return S_OK;
}

/// <summary>Handle <c>ICorProfilerCallback::JITCompilationStarted</c></summary>
/// <remarks>The 'workhorse' </remarks>
HRESULT STDMETHODCALLTYPE CClrProfiler::JITCompilationStarted( 
        /* [in] */ FunctionID functionId,
        /* [in] */ BOOL fIsSafeToBlock)
{

    //if (GetTokenAndModule(functionId, functionToken, moduleId, modulePath, &assemblyId))
    //{
    //    if (OpenCoverSupportRequired(assemblyId, functionId))
    //        OpenCoverSupportCompilation(functionId, functionToken, moduleId, assemblyId, modulePath);

    //    CuckooSupportCompilation(assemblyId, functionToken, moduleId);

    //    if (m_allowModules[modulePath])
    //    {
    //        ATLTRACE(_T("::JITCompilationStarted(%X, ...) => %d, %X => %s"), functionId, functionToken, moduleId, W2CT(modulePath.c_str()));

    //        std::vector<SequencePoint> seqPoints;
    //        std::vector<BranchPoint> brPoints;

    //        if (m_host.GetPoints(functionToken, (LPWSTR)modulePath.c_str(),
    //            (LPWSTR)m_allowModulesAssemblyMap[modulePath].c_str(), seqPoints, brPoints))
    //        {
    //            if (seqPoints.size() != 0)
    //            {
    //                LPCBYTE pMethodHeader = NULL;
    //                ULONG iMethodSize = 0;
    //                COM_FAIL_MSG_RETURN_ERROR(m_profilerInfo2->GetILFunctionBody(moduleId, functionToken, &pMethodHeader, &iMethodSize),
    //                    _T("    ::JITCompilationStarted(...) => GetILFunctionBody => 0x%X"));

    //                IMAGE_COR_ILMETHOD* pMethod = (IMAGE_COR_ILMETHOD*)pMethodHeader;

    //                Method instumentedMethod(pMethod);
    //                instumentedMethod.IncrementStackSize(2);

    //                ATLTRACE(_T("::JITCompilationStarted(...) => Instrumenting..."));
    //                //seqPoints.clear();
    //                //brPoints.clear();

    //                // Instrument method
    //                InstrumentMethod(moduleId, instumentedMethod, seqPoints, brPoints);

    //                //instumentedMethod.DumpIL();

    //                CComPtr<IMethodMalloc> methodMalloc;
    //                COM_FAIL_MSG_RETURN_ERROR(m_profilerInfo2->GetILFunctionBodyAllocator(moduleId, &methodMalloc),
    //                    _T("    ::JITCompilationStarted(...) => GetILFunctionBodyAllocator=> 0x%X"));

    //                IMAGE_COR_ILMETHOD* pNewMethod = (IMAGE_COR_ILMETHOD*)methodMalloc->Alloc(instumentedMethod.GetMethodSize());
    //                instumentedMethod.WriteMethod(pNewMethod);
    //                COM_FAIL_MSG_RETURN_ERROR(m_profilerInfo2->SetILFunctionBody(moduleId, functionToken, (LPCBYTE)pNewMethod),
    //                    _T("    ::JITCompilationStarted(...) => SetILFunctionBody => 0x%X"));

    //                ULONG mapSize = instumentedMethod.GetILMapSize();
    //                COR_IL_MAP * pMap = (COR_IL_MAP *)CoTaskMemAlloc(mapSize * sizeof(COR_IL_MAP));
    //                instumentedMethod.PopulateILMap(mapSize, pMap);
    //                COM_FAIL_MSG_RETURN_ERROR(m_profilerInfo2->SetILInstrumentedCodeMap(functionId, TRUE, mapSize, pMap),
    //                    _T("    ::JITCompilationStarted(...) => SetILInstrumentedCodeMap => 0x%X"));

    //                // only do this for .NET4 and above as there are issues with earlier runtimes (Access Violations)
    //                if (m_runtimeVersion.usMajorVersion >= 4)
    //                    CoTaskMemFree(pMap);

    //                // resize the threshold array 
    //                if (m_threshold != 0)
    //                {
    //                    if (seqPoints.size() > 0)
    //                        Resize(seqPoints.back().UniqueId + 1);
    //                    if (brPoints.size() > 0)
    //                        Resize(brPoints.back().UniqueId + 1);
    //                }
    //            }
    //        }
    //    }
    //}
    //
    //if (m_chainedProfiler != NULL)
    //    return m_chainedProfiler->JITCompilationStarted(functionId, fIsSafeToBlock);
	
	//LOG4CPLUS_DEBUG(logger,"JITCompilationStarted");
	//LogLog::getLogLog()->debug(LOG4CPLUS_TEXT("this is JITCompliationStarted"));

	std::wstring modulePath;
	mdToken functionToken;
	ModuleID moduleId;
	AssemblyID assemblyId;
	LPCBYTE pModuleBaseLoadAddress;
	
	mdModuleRef agentModuleRef;
	mdTypeRef agentShimRef;
	mdMemberRef agentGetTracerRef;
	mdMemberRef agentFinishTracerRef;
#ifdef _WIN64
	COR_SIGNATURE sigAgent[] = {
		IMAGE_CEE_CS_CALLCONV_DEFAULT,      // default calling convention
		0x06,                               // number of arguments == 6
		ELEMENT_TYPE_VOID,                  // return type == void
		/*ELEMENT_TYPE_STRING,
		ELEMENT_TYPE_STRING,
		ELEMENT_TYPE_STRING,
		ELEMENT_TYPE_STRING,
		ELEMENT_TYPE_U4,
		ELEMENT_TYPE_OBJECT*/
		//ELEMENT_TYPE_U8,                    // arg 1: UInt64 moduleIDCur
		//ELEMENT_TYPE_U4,                    // arg 2: UInt32 mdCur
		//ELEMENT_TYPE_I4,                    // arg 3: int nVersionCur
	};
#else //  ! _WIN64 (32-bit code follows)
	COR_SIGNATURE sigAgent[] = {
		IMAGE_CEE_CS_CALLCONV_DEFAULT ,      // default calling convention,static method
		0x06,                               // number of arguments == 3
		ELEMENT_TYPE_STRING,                  // return type == void
		//ELEMENT_TYPE_STRING,
		ELEMENT_TYPE_STRING,
		ELEMENT_TYPE_STRING,
		ELEMENT_TYPE_STRING,
		ELEMENT_TYPE_STRING,
		ELEMENT_TYPE_U4,
		ELEMENT_TYPE_OBJECT
		//ELEMENT_TYPE_U4,                    // arg 1: UInt32 moduleIDCur
		//ELEMENT_TYPE_U4,                    // arg 2: UInt32 mdCur
		//ELEMENT_TYPE_I4,                    // arg 3: int nVersionCur
	};
#endif //_WIN64

#ifdef _WIN64
	COR_SIGNATURE sigAgentFinish[] = {
		IMAGE_CEE_CS_CALLCONV_DEFAULT,      // default calling convention
		0x01,                               // number of arguments == 3
		ELEMENT_TYPE_VOID,                  // return type == void
		/*ELEMENT_TYPE_STRING,
		ELEMENT_TYPE_STRING,
		ELEMENT_TYPE_STRING,
		ELEMENT_TYPE_STRING,
		ELEMENT_TYPE_U4,*/
		ELEMENT_TYPE_OBJECT
		//ELEMENT_TYPE_U8,                    // arg 1: UInt64 moduleIDCur
		//ELEMENT_TYPE_U4,                    // arg 2: UInt32 mdCur
		//ELEMENT_TYPE_I4,                    // arg 3: int nVersionCur
	};
#else //  ! _WIN64 (32-bit code follows)
	COR_SIGNATURE sigAgentFinish[] = {
		IMAGE_CEE_CS_CALLCONV_DEFAULT ,      // default calling convention,static method
		0x01,                               // number of arguments == 3
		ELEMENT_TYPE_VOID,                  // return type == void
		ELEMENT_TYPE_OBJECT
		//ELEMENT_TYPE_U4,                    // arg 1: UInt32 moduleIDCur
		//ELEMENT_TYPE_U4,                    // arg 2: UInt32 mdCur
		//ELEMENT_TYPE_I4,                    // arg 3: int nVersionCur
	};
#endif //_WIN64


	if(GetTokenAndModule(functionId, functionToken, moduleId, modulePath, pModuleBaseLoadAddress,&assemblyId))
	{
		PCCOR_SIGNATURE ppvSigBlob;
		ULONG pcbSigBlob;
		mdTypeDef classId;
		DWORD dwMethodAttr;
		DWORD dwMethodImplFlags;
		std::wstring szMethodName = GetTypeAndMethodName(functionId,ppvSigBlob,pcbSigBlob,dwMethodAttr,dwMethodImplFlags,classId);
		std::wstring szAssemblyName = GetAssemblyName(assemblyId);
		std::wstring key = szAssemblyName + L"|" + szMethodName;
		
		if(!m_tracerMap[key])
			return S_OK;
		ATLTRACE((L"JITCompilationStarted:" + key).c_str());
		fwprintf(stream,(szMethodName + std::wstring(L"\n")).c_str());
		/*if(szMethodName.find(L"DemoMethod")==std::wstring::npos)
			return S_OK;*/
		ATLTRACE(_T("JITCompilationStarted @ :%s"),szMethodName.c_str());
		ASSEMBLYMETADATA assembly;
		ZeroMemory(&assembly, sizeof(assembly));
		assembly.usMajorVersion = 1;
		assembly.usMinorVersion = 0;
		assembly.usBuildNumber = 0; 
		assembly.usRevisionNumber = 0;
		BYTE publicKey[] = { 0xd1,0xb7,0x96,0xb4,0x78,0x57,0xb0,0x98 };

		CComPtr<IMetaDataEmit> metaDataEmit;
		COM_FAIL_MSG_RETURN_ERROR(m_profilerInfo->GetModuleMetaData(moduleId, 
			ofRead | ofWrite, IID_IMetaDataEmit, (IUnknown**)&metaDataEmit), 
			_T("JITCompilationStarted(...) => GetModuleMetaData => 0x%X"));      

		CComPtr<IMetaDataAssemblyEmit> metaDataAssemblyEmit;
		COM_FAIL_MSG_RETURN_ERROR(metaDataEmit->QueryInterface(
			IID_IMetaDataAssemblyEmit, (void**)&metaDataAssemblyEmit), 
			_T("JITCompilationStarted(...) => QueryInterface => 0x%X"));

		COM_FAIL_MSG_RETURN_ERROR(metaDataAssemblyEmit->DefineAssemblyRef(publicKey, 
        sizeof(publicKey), L"BeyondAPM.Simple.Agent", &assembly, NULL, 0, 0, 
		&agentModuleRef), _T("JITCompilationStarted(...) => DefineAssemblyRef => 0x%X"));

		
		COM_FAIL_MSG_RETURN_ERROR(metaDataEmit->DefineTypeRefByName(agentModuleRef,L"BeyondAPM.Simple.Agent.AgentShim",&agentShimRef),
			_T("JITCompilationStarted(...) => DefineTypeRefByName => 0x%X"));

		COM_FAIL_MSG_RETURN_ERROR(metaDataEmit->DefineMemberRef(agentShimRef,L"GetTracer",sigAgent,sizeof(sigAgent),&agentGetTracerRef),
			_T("JITCompilationStarted(...) => DefineMemberRef => 0x%X"));

		ATLTRACE(_T("SIZE of GetTracer Signature is:%d"),sizeof(sigAgent));

		COM_FAIL_MSG_RETURN_ERROR(metaDataEmit->DefineMemberRef(agentShimRef,L"FinishTracer",sigAgentFinish,sizeof(sigAgentFinish),&agentFinishTracerRef),
			_T("JITCompilationStarted(...) => DefineMemberRef => 0x%X"));

		InstructionList instructions;
		ATLTRACE(_T("_DEBUG ClrProfiler.cpp L421."));
		ULONG count = GetParamCountFromMethodSig(ppvSigBlob);
		TracerNode* currentTracer = m_tracerMap[key];
		/*
		mdString mdsAssemblyName;
		std::wstring wsAssemblyName(C2W(std::string(currentTracer->_assmeblyName)));
		COM_FAIL_MSG_RETURN_ERROR(metaDataEmit->DefineUserString(wsAssemblyName.c_str(),wsAssemblyName.length(),&mdsAssemblyName),
			_T("JITCompilationStarted(...) => DefineUserString => 0x%X"));
		//ATLTRACE(_T("Get LPCWSTR Assembly Name:%s"),wsAssemblyName.c_str());

		mdString mdsClassName;
		std::wstring wsClassName(C2W(std::string(currentTracer->_className)));
		COM_FAIL_MSG_RETURN_ERROR(metaDataEmit->DefineUserString(wsClassName.c_str(),wsClassName.length(),&mdsClassName),
			_T("JITCompilationStarted(...) => DefineUserString => 0x%X"));
		//ATLTRACE(_T("Get LPCWSTR Class Name:%s"),wsClassName.c_str());

		mdString mdsMethodName;
		std::wstring wsMethodName(C2W(std::string(currentTracer->_mehtodName)));
		COM_FAIL_MSG_RETURN_ERROR(metaDataEmit->DefineUserString(wsMethodName.c_str(),wsMethodName.length(),&mdsMethodName),
			_T("JITCompilationStarted(...) => DefineUserString => 0x%X"));
		//ATLTRACE(_T("Get LPCWSTR Method Name:%s"),mdsMethodName.c_str());
		
		mdString mdsTracerFactoryName;LPWSTR x = CA2WEX<10>(currentTracer->_traceName);
		std::wstring wsTracerFactoryName(C2W(std::string(currentTracer->_traceName)));
		COM_FAIL_MSG_RETURN_ERROR(metaDataEmit->DefineUserString(wsTracerFactoryName.c_str(),wsTracerFactoryName.length(),&mdsTracerFactoryName),
			_T("JITCompilationStarted(...) => DefineUserString => 0x%X"));
		ATLTRACE(_T("before instrument method"));
		*/
		mdString mdsAssemblyName;
		ATLTRACE(L"_DEBUG ClrProfiler.cpp L450.");
		AtlTrace("current assembly name is %s",currentTracer->_assmeblyName);
		CA2W wsAssemblyName(currentTracer->_assmeblyName);
		AtlTrace(wsAssemblyName);
		ATLTRACE(L"wcslen(wsAssemblyName: %d",wcslen(wsAssemblyName));
		
		/*debug*/
		COM_FAIL_MSG_RETURN_ERROR(metaDataEmit->DefineUserString(wsAssemblyName,wcslen(wsAssemblyName),&mdsAssemblyName),
			_T("JITCompilationStarted(...) => DefineUserString => 0x%X"));
		ATLTRACE(wsAssemblyName);

		mdString mdsClassName;
		CA2W wsClassName(currentTracer->_className);
		COM_FAIL_MSG_RETURN_ERROR(metaDataEmit->DefineUserString(wsClassName,wcslen(wsClassName),&mdsClassName),
			_T("JITCompilationStarted(...) => DefineUserString => 0x%X"));
		//ATLTRACE(wsClassName);

		mdString mdsMethodName;
		CA2W wsMethodName(currentTracer->_methodName);
		COM_FAIL_MSG_RETURN_ERROR(metaDataEmit->DefineUserString(wsMethodName,wcslen(wsMethodName),&mdsMethodName),
			_T("JITCompilationStarted(...) => DefineUserString => 0x%X"));
		//ATLTRACE(wsMethodName);

		mdString mdsTracerFactoryName;
		CA2W wsTracerFactoryName(currentTracer->_traceName);
		COM_FAIL_MSG_RETURN_ERROR(metaDataEmit->DefineUserString(wsTracerFactoryName,wcslen(wsTracerFactoryName),&mdsTracerFactoryName),
			_T("JITCompilationStarted(...) => DefineUserString => 0x%X"));
		ATLTRACE(_T("before instrument method"));

		//创建method对象
		LPCBYTE pMethodHeader = NULL;
		ULONG iMethodSize = 0;
		COM_FAIL_MSG_RETURN_ERROR(m_profilerInfo->GetILFunctionBody(moduleId, functionToken, &pMethodHeader, &iMethodSize),
			_T("    ::InstrumentMethodWith(...) => GetILFunctionBody => 0x%X"));

		IMAGE_COR_ILMETHOD* pMethod = (IMAGE_COR_ILMETHOD*)pMethodHeader;
		Method instumentedMethod(pMethod);
		//memcpy(instumentedMethod.m_pMethod,pMethod,iMethodSize);

		//0.修改LocalVarSigTok,增加局部变量
		mdSignature newLocalVarToken;//=0L;
		WORD nIndexOfNewLocalVar = EmitNewLocalVarToken(moduleId,instumentedMethod.GetLocalVarToken(),newLocalVarToken);
		
		

		instructions.push_back(new Instruction(CEE_NOP));
		instructions.push_back(new Instruction(CEE_LDSTR,mdsTracerFactoryName));
		instructions.push_back(new Instruction(CEE_LDSTR,mdsAssemblyName));
		instructions.push_back(new Instruction(CEE_LDSTR,mdsClassName));
		instructions.push_back(new Instruction(CEE_LDSTR,mdsMethodName));
		instructions.push_back(new Instruction(CEE_LDC_I4,0));
		instructions.push_back(new Instruction(CEE_LDARG_0));
		instructions.push_back(new Instruction(CEE_NOP,0));//for step debug
		instructions.push_back(new Instruction(CEE_CALL,agentGetTracerRef));
		//instructions.push_back(new Instruction(CEE_STLOC_S,nIndexOfNewLocalVar));

		/*InstructionList instructionTail;
		instructionTail.push_back(new Instruction(CEE_LDLOCA_S,nIndexOfNewLocalVar));
		instructionTail.push_back(new Instruction(CEE_CALL,agentFinishTracerRef));*/

		/*instumentedMethod.m_classId = classId;
		instumentedMethod.m_dwMethodAttr = dwMethodAttr;
		instumentedMethod.m_dwMethodImplFlags = dwMethodAttr;
		instumentedMethod.m_pvSigBlob = new COR_SIGNATURE;
		instumentedMethod.m_pOldSigCount = pcbSigBlob;
		memcpy(instumentedMethod.m_pvSigBlob,ppvSigBlob,pcbSigBlob);*///ppvSigBlob is  the old signature
		InstrumentMethodWith(moduleId,functionToken,instumentedMethod,newLocalVarToken,instructions);
		
		
	}
    return S_OK; 
}
///<summary>Instrument Method at the Byte offset of 0</summary>
///<param name="moduleId">The Module that has been profiled.</param>
///<param name="functionToken">The function that has been profiled and rewritted.</param>
///<param name="instructions">The IL instructions that is gonna be inserted into the function.</param>
HRESULT CClrProfiler::InstrumentMethodWith(ModuleID moduleId,mdToken functionToken,Method &instumentedMethod, mdSignature newLocalVarSig,InstructionList &instructions)
{

	/*LPCBYTE pMethodHeader = NULL;
	ULONG iMethodSize = 0;
	COM_FAIL_MSG_RETURN_ERROR(m_profilerInfo->GetILFunctionBody(moduleId, functionToken, &pMethodHeader, &iMethodSize),
		_T("    ::InstrumentMethodWith(...) => GetILFunctionBody => 0x%X"));

	IMAGE_COR_ILMETHOD* pMethod = (IMAGE_COR_ILMETHOD*)pMethodHeader;
	Method instumentedMethod(pMethod);*/
	
	//1.instrument before the beginning of the instrumented method
	instumentedMethod.InsertInstructionsAtOriginalOffset(0, instructions);
	//2.instrument after the 
	instumentedMethod.DumpIL();

	// now to write the method back
	CComPtr<IMethodMalloc> methodMalloc;
	COM_FAIL_MSG_RETURN_ERROR(m_profilerInfo->GetILFunctionBodyAllocator(moduleId, &methodMalloc),
		_T("    ::InstrumentMethodWith(...) => GetILFunctionBodyAllocator=> 0x%X"));

	IMAGE_COR_ILMETHOD* pNewMethod = (IMAGE_COR_ILMETHOD*)methodMalloc->Alloc(instumentedMethod.GetMethodSize());
	ATLTRACE(_T("METHOD ALLOC SIZE IS:%d"),instumentedMethod.GetMethodSize());

	//IMAGE_COR_ILMETHOD_FAT* fatImage =  (IMAGE_COR_ILMETHOD_FAT*)&pNewMethod->Fat;
	COR_ILMETHOD_FAT* fatImage = (COR_ILMETHOD_FAT*)&pNewMethod->Fat;
	if(!fatImage->IsFat())
		ATLTRACE(_T("InstrumentMethodWith ==> New Alloc Method is TINY"));
	else
		ATLTRACE(_T("InstrumentMethodWith ==> New Alloc Method is Fat"));

	ATLTRACE(_T("ClrProfiler::InstrumentMethodWith ==> FatImage LocaVarSigTok is %X"),fatImage->LocalVarSigTok);
	
	instumentedMethod.WriteMethod(pNewMethod);

	

	COM_FAIL_MSG_RETURN_ERROR(m_profilerInfo->SetILFunctionBody(moduleId, functionToken, (LPCBYTE)pNewMethod),
		_T("    ::InstrumentMethodWith(...) => SetILFunctionBody => 0x%X"));

    return S_OK;

}
HRESULT CClrProfiler::InstrumentMethodWith2(ModuleID moduleId,mdToken functionToken,Method &instumentedMethod, InstructionList &instructionsBefroe,InstructionList &instructionsEnd,ULONG newLocalVarStart)
{
	/*LPCBYTE pMethodHeader = NULL;
	ULONG iMethodSize = 0;
	COM_FAIL_MSG_RETURN_ERROR(m_profilerInfo->GetILFunctionBody(moduleId, functionToken, &pMethodHeader, &iMethodSize),
		_T("    ::InstrumentMethodWith2(...) => GetILFunctionBody => 0x%X"));
	IMAGE_COR_ILMETHOD* pMethod = (IMAGE_COR_ILMETHOD*)pMethodHeader;
	Method instumentedMethod(pMethod);*/
	instumentedMethod.InsertInstructionsAtOffset(0,instructionsBefroe);
	for (auto it = instumentedMethod.m_instructions.begin(); it != instumentedMethod.m_instructions.end(); ++it)
    {
		if((*it)->m_operation == CEE_RET)
		{
			instructionsEnd.insert(instructionsEnd.begin(),new Instruction(CEE_NOP,0));
			
			instumentedMethod.InsertInstructionsAtOffset((*it)->m_offset,instructionsEnd);
		}
	}
	instumentedMethod.DumpIL();
	CComPtr<IMethodMalloc> methodMalloc;
	COM_FAIL_MSG_RETURN_ERROR(m_profilerInfo->GetILFunctionBodyAllocator(moduleId, &methodMalloc),
		_T("    ::InstrumentMethodWith2(...) => GetILFunctionBodyAllocator=> 0x%X"));

	IMAGE_COR_ILMETHOD* pNewMethod = (IMAGE_COR_ILMETHOD*)methodMalloc->Alloc(instumentedMethod.GetMethodSize());
	instumentedMethod.WriteMethod(pNewMethod);
	COM_FAIL_MSG_RETURN_ERROR(m_profilerInfo->SetILFunctionBody(moduleId, functionToken, (LPCBYTE)pNewMethod),
		_T("    ::InstrumentMethodWith2(...) => SetILFunctionBody => 0x%X"));
    return S_OK;

}

void CClrProfiler::ReadTracerConfig(const char* configXML,std::unordered_map<std::wstring,TracerNode*> &tracerMap)
{
	        tinyxml2::XMLDocument doc;
			doc.LoadFile( configXML );
			ATLTRACE(_T("doc.ErrorID is :%d",doc.ErrorID()));
			tinyxml2::XMLElement* root = doc.RootElement();
			tinyxml2::XMLElement* instrumentation = root->FirstChildElement( "instrumentation" );
			tinyxml2::XMLElement* tagExactMethodMatcher;
			tinyxml2::XMLElement* tagMatch;
			char* temp;
			char* next_token;
			const char* traceName;
			const char* traceParameter;//TraceFactory Arguments
			const char* assemblyName;
			const char* className;
			const char* methodName;
			const char* parameters;// Methd Parameters

			ATLTRACE(_T("start_read_CoreIntrumentation"));
			for(tinyxml2::XMLElement* child = instrumentation->FirstChildElement( "tracerFactory" ); child ; child = child->NextSiblingElement( "tracerFactory" ))
			{
					traceName = child->Attribute("name");
					traceParameter = child->Attribute("");
					
					for(tagMatch = child->FirstChildElement("match"); tagMatch ; tagMatch= tagMatch->NextSiblingElement( "match" ))
					{
						assemblyName = tagMatch->Attribute("assemblyName");
						className = tagMatch->Attribute("className");
						for(tagExactMethodMatcher = tagMatch->FirstChildElement( "exactMethodMatcher" );tagExactMethodMatcher;tagExactMethodMatcher = tagExactMethodMatcher->NextSiblingElement( "exactMethodMatcher" ))
						{
							//ATLTRACE(_T("loop started"));

							if(!traceName) traceName = "";
							
							if(!traceParameter) traceName="";

							if(!assemblyName) assemblyName="";
							
							if(!className) className="";

							methodName = tagExactMethodMatcher->Attribute("methodName");
							if(!methodName) methodName="";

							parameters = tagExactMethodMatcher->Attribute("parameters");
							if(!parameters)parameters ="";

							TracerNode *factory = new TracerNode(traceName,assemblyName,className,methodName,parameters,0);

							std::string key = "";

							key = std::string(factory->_assmeblyName) + "|" + std::string(factory->_className) + "|" + std::string(factory->_methodName);
							std::wstring dest(key.length(),L' ');
							std::copy(key.begin(),key.end(),dest.begin());
							tracerMap[dest]=factory;
						}
					}
			}
			ATLTRACE(_T("end_read_CoreIntrumentation"));
}

WORD CClrProfiler::EmitNewLocalVarToken(ModuleID moduleId,mdSignature tkOldLocalVarToken, mdSignature &tkNewLocalVarToken)
{
	ATLTRACE(_T("Old Local Var Signature Token: %x\n"), tkOldLocalVarToken);

	CComPtr<IMetaDataImport> metaDataImport;
	COM_FAIL_MSG_RETURN_ERROR(m_profilerInfo->GetModuleMetaData(moduleId, 
		ofRead , IID_IMetaDataImport, (IUnknown**)&metaDataImport), 
		_T("EmitNewLocalVarToken(...) => GetModuleMetaData(IID_IMetaDataImport) => 0x%X"));
	CComPtr<IMetaDataEmit> metaDataEmit;
	COM_FAIL_MSG_RETURN_ERROR(m_profilerInfo->GetModuleMetaData(moduleId, 
		ofRead | ofWrite , IID_IMetaDataEmit, (IUnknown**)&metaDataEmit), 
		_T("EmitNewLocalVarToken(...) => GetModuleMetaData(IID_IMetaDataImport) => 0x%X"));

    ULONG nOldLocalVarCount = 0;
    // Net part of signature, enclude CallingConvention & LocalVarCount
    PCCOR_SIGNATURE pvOldLocalVarSigNetPart = NULL;
    ULONG nOldLocalVarSigNetPartSize = 0;

    if(NULL != tkOldLocalVarToken && mdSignatureNil != tkOldLocalVarToken)
    {
        PCCOR_SIGNATURE pvOldLocalVarSig = NULL;
        ULONG nOldLocalVarSigSize = 0;
        COM_FAIL_MSG_RETURN_ERROR(metaDataImport->GetSigFromToken(tkOldLocalVarToken, &pvOldLocalVarSig, &nOldLocalVarSigSize),
			_T("EmitNewLocalVarToken(...) => GetSigFromToken => 0x%X"));
		ATLTRACE(_T("old localvarsig is %16lX"),pvOldLocalVarSig);
		fprintf(stream,"%x",pvOldLocalVarSig);
        _ASSERT(NULL != pvOldLocalVarSig);

        pvOldLocalVarSigNetPart = pvOldLocalVarSig;
        
        ULONG nCallingConvention = ::CorSigUncompressCallingConv(pvOldLocalVarSigNetPart);
        DBG_UNREFERENCED_LOCAL_VARIABLE(nCallingConvention);

        nOldLocalVarCount = ::CorSigUncompressData(pvOldLocalVarSigNetPart);
        nOldLocalVarSigNetPartSize = nOldLocalVarSigSize - (ULONG)(pvOldLocalVarSigNetPart - pvOldLocalVarSig);
    }
    ATLTRACE(_T("Old Local Var Count: %d"), nOldLocalVarCount);
	//暂时不用
    //mdTypeRef tkExceptionTypeRef = this->EmitTypeRefToken(NULL, _T("System.Exception"));

    PCOR_SIGNATURE vLocalVarSignature = new COR_SIGNATURE[(2 + 1) * sizeof(DWORD) + nOldLocalVarSigNetPartSize];
    PCOR_SIGNATURE signatureNewLocalVar = vLocalVarSignature;
    *signatureNewLocalVar++ = IMAGE_CEE_CS_CALLCONV_LOCAL_SIG;
    signatureNewLocalVar += CorSigCompressData(2 + nOldLocalVarCount, signatureNewLocalVar);

    if(0 < nOldLocalVarCount)
    {
        _ASSERT(NULL != pvOldLocalVarSigNetPart);
        _ASSERT(0 < nOldLocalVarSigNetPartSize);
        ::memmove(signatureNewLocalVar, pvOldLocalVarSigNetPart, nOldLocalVarSigNetPartSize);
        signatureNewLocalVar += nOldLocalVarSigNetPartSize;
    }
	
	//只增加一个局部变量
    //signatureNewLocalVar += CorSigCompressElementType(ELEMENT_TYPE_CLASS, signatureNewLocalVar); // throwException
    //signatureNewLocalVar += CorSigCompressToken(tkExceptionTypeRef, signatureNewLocalVar);
    signatureNewLocalVar += CorSigCompressElementType(ELEMENT_TYPE_OBJECT, signatureNewLocalVar); // GetTracer返回的ITracer对象。
    ULONG nSize = (ULONG)(signatureNewLocalVar - vLocalVarSignature);
	PCCOR_SIGNATURE sigTrace = vLocalVarSignature;

	for(int i=0;i<nSize;i++)
	{
		ATLTRACE(_T("Print New LocalSigVarToken byte(%d):(%c)",i,*sigTrace++));
	}
    ATLTRACE(_T("New Local Var Signature Size: %x\n"), nSize);

//    mdToken tokenNewLocalVarSig = mdSignatureNil;
	
	HRESULT hr = metaDataEmit->GetTokenFromSig(vLocalVarSignature, nSize, &tkNewLocalVarToken);
	ATLTRACE(_T("New Local Var Signature Token: %x\n"), tkNewLocalVarToken);

	delete [] vLocalVarSignature;

	if(!SUCCEEDED(hr))
	{
		ATLTRACE("EmitNewLocalVarToken(...) => GetTokenFromSig => 0x%X");
		return -1L;
	}
    /*COM_FAIL_MSG_RETURN_ERROR(metaDataEmit->GetTokenFromSig(vLocalVarSignature, nSize, &tkNewLocalVarToken),
		_T("EmitNewLocalVarToken(...) => GetTokenFromSig => 0x%X"));*/
   
    
    ATLTRACE(_T("New Local Var Signature Token: %x\n"), tkNewLocalVarToken);
    
    _ASSERT(nOldLocalVarCount < (1 << (8 * sizeof(WORD))));
    return (WORD)nOldLocalVarCount;  // also the index of new inserted local-var
}


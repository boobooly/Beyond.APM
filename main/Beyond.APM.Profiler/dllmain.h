// dllmain.h : 模块类的声明。

class CBeyondAPMProfilerModule : public ATL::CAtlDllModuleT< CBeyondAPMProfilerModule >
{
public :
	DECLARE_LIBID(LIBID_BeyondAPMProfilerLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_BEYONDAPMPROFILER, "{C2D01FD4-74BF-4B5D-985B-109DCDA2AE9B}")
};

extern class CBeyondAPMProfilerModule _AtlModule;

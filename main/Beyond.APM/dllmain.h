// dllmain.h : 模块类的声明。

class CBeyondAPMModule : public ATL::CAtlDllModuleT< CBeyondAPMModule >
{
public :
	DECLARE_LIBID(LIBID_BeyondAPMLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_BEYONDAPM, "{DF1FFA41-5446-4775-B9A1-3F9A0BAFB505}")
};

extern class CBeyondAPMModule _AtlModule;

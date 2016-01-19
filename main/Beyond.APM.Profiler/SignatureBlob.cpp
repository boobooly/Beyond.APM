#include "stdafx.h"
#include "ClrProfiler.h"

ULONG CClrProfiler::GetParamCountFromMethodSig(PCCOR_SIGNATURE &pSignature) const
{
    //ASSERT(HitTest(pSignature));
    //-----------------------------------------------------------------------------------
    //  MethodDefSig ::= [HASTHIS [EXPLICTTHIS]] (DEFAULT | VARARG | GENERIC GenParamCount)
    //      ParamCount RetTypeSig (ParamSig*)
    //  MethodRefSig ::= [HASTHIS [EXPLICITTHIS]] VARARG ParamCount RetTypeSig ParamSig*
    //      [SENTINEL Param+]

    // HASTHIS, EXPLICTTHIS, DEFAULT, VARARG, GENERIC are composited in CorCallingConvention
    ULONG nCallingConvention = ::CorSigUncompressCallingConv(pSignature);


    if(IMAGE_CEE_CS_CALLCONV_GENERIC & nCallingConvention) // GENERIC
    {
        // GenParamCount (that following GENERIC)
        ULONG nGenParamCount = ::CorSigUncompressData(pSignature);
        DBG_UNREFERENCED_LOCAL_VARIABLE(nGenParamCount);

    }

    // ParamCount
    ULONG nParamCount = ::CorSigUncompressData(pSignature);

    return nParamCount;
}

CorElementType CClrProfiler::BypassOptCustomMod(PCCOR_SIGNATURE& pSignature) const
{
    //ASSERT(HitTest(pSignature));
    //-----------------------------------------------------------------------------------
    //  CustMod* ::= (CMOD_REQD | CMOD_OPT)*

    CorElementType nElementType;
    do
    {
        nElementType = ::CorSigUncompressElementType(pSignature);
    }
    while((ELEMENT_TYPE_CMOD_REQD == nElementType) || (ELEMENT_TYPE_CMOD_OPT == nElementType));

    return nElementType;
}
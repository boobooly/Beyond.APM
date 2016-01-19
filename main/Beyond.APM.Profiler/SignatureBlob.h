#pragma once
#include "stdafx.h"
struct CSignatureBlob //: public CMemoryRef
{
public:
    CSignatureBlob(void) ;//: CMemoryRef() {};

public:
	ULONG GetParamCountFromMethodSig(PCCOR_SIGNATURE &pSignature) const;

protected:
    CorElementType BypassOptCustomMod(PCCOR_SIGNATURE& pSignature) const;
    
};
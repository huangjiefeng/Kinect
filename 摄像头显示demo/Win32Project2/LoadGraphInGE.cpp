#include "stdafx.h"
HRESULT AddToRotS(IUnknown *pUnkGraph, DWORD *pdwRegister) 
{
	IMoniker * pMoniker;
	IRunningObjectTable *pROT;
	if (FAILED(GetRunningObjectTable(0, &pROT))) {
		return E_FAIL;
	}
	WCHAR wsz[256];
	wsprintfW(wsz, L"FilterGraph %08x pid %08x", (DWORD_PTR)pUnkGraph, GetCurrentProcessId());
	HRESULT hr = CreateItemMoniker(L"!", wsz, &pMoniker);
	if (SUCCEEDED(hr)) {
		hr = pROT->Register(ROTFLAGS_REGISTRATIONKEEPSALIVE, pUnkGraph,
			pMoniker, pdwRegister);
		pMoniker->Release();
	}
	pROT->Release();
	return hr;
}

void RemoveFromRot(DWORD pdwRegister)
{
	IRunningObjectTable *pROT;
	if (SUCCEEDED(GetRunningObjectTable(0, &pROT))) {
		pROT->Revoke(pdwRegister);
		pROT->Release();
	}
}
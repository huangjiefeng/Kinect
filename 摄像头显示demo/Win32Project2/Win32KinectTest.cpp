#include <Windows.h>
#include <streams.h>
#include "SmartPtr.h"
#include "SampleCGB.h"
// ����Ҫ����ǰ������
LRESULT CALLBACK WindowProc(
	_In_  HWND hwnd,
	_In_  UINT uMsg,
	_In_  WPARAM wParam,
	_In_  LPARAM lParam
	);

// ������ڵ�
int CALLBACK WinMain(
	_In_  HINSTANCE hInstance,
	_In_  HINSTANCE hPrevInstance,
	_In_  LPSTR lpCmdLine,
	_In_  int nCmdShow
	)
{
	// ����
	WCHAR* cls_Name = L"My Class";
	// ��ƴ�����
	WNDCLASS wc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hCursor = LoadCursor(hInstance, IDC_ARROW);;
	wc.hIcon = LoadIcon(hInstance, IDI_APPLICATION);;
	wc.lpszMenuName = NULL;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpfnWndProc = WindowProc;
	wc.lpszClassName = cls_Name;
	wc.hInstance = hInstance;
	// ע�ᴰ����
	RegisterClass(&wc);

	// ��������
	HWND hwnd = CreateWindow(
		cls_Name,			//������Ҫ�͸ղ�ע���һ��
		L"�ҵ�Ӧ�ó���",	//���ڱ�������
		WS_OVERLAPPEDWINDOW, //���������ʽ
		38,				//��������ڸ�����X����
		20,				//��������ڸ�����Y����
		1920,				//���ڵĿ��
		1080,				//���ڵĸ߶�
		NULL,				//û�и����ڣ�ΪNULL
		NULL,				//û�в˵���ΪNULL
		hInstance,			//��ǰӦ�ó����ʵ�����
		NULL);				//û�и������ݣ�ΪNULL
	if(hwnd == NULL) //��鴰���Ƿ񴴽��ɹ�
		return 0;

	// ��ʾ����
	ShowWindow(hwnd, SW_SHOW);


	IBindCtx *lpBC=0;
	IMoniker *pMoniker = 0;
	IBaseFilter *pVCap;
	IGraphBuilder *pFg=NULL;

	HRESULT hr = CreateBindCtx(0, &lpBC);
	if(SUCCEEDED(hr))
	{
		ULONG dwEaten;
		hr = MkParseDisplayName(lpBC, L"@device:sw:{860BB310-5D01-11D0-BD3B-00A0C911CE86}\\{E48ECF1A-A5E7-4EB0-8BF7-E15185D66FA4}", &dwEaten, &pMoniker);	
		lpBC->Release();
	}

	IPropertyBag *pBag;
	hr = pMoniker->BindToStorage(NULL, NULL, IID_IPropertyBag, (void **)&pBag);
	if(SUCCEEDED(hr))
	{
		VARIANT var;
		var.vt = VT_BSTR;

		hr = pBag->Read(L"FriendlyName", &var, NULL);		//��ȡFriendlyName��var.bstrVal,������gcap.wachFriendlyName
		if(hr == NOERROR)
		{
			//hr = StringCchCopyW(gcap.wachFriendlyName, sizeof(gcap.wachFriendlyName) / sizeof(gcap.wachFriendlyName[0]), var.bstrVal);
			SysFreeString(var.bstrVal);
		}
		pBag->Release();
	}
	hr = pMoniker->BindToObject(NULL, NULL, IID_IBaseFilter, (void**)&pVCap);
	
	ISampleCaptureGraphBuilder *pBuilder = new ISampleCaptureGraphBuilder( );
	if( NULL == pBuilder )
	{
		return FALSE;
	}
	hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC, IID_IGraphBuilder, (LPVOID *)&pFg);
	hr = pBuilder->SetFiltergraph(pFg);	//gcap.pFg������MakeGraph()��󶨵�Graph Builder
	hr = pFg->AddFilter(pVCap, L"kinect");


	//// ���´���
	//UpdateWindow(hwnd);

	//// ��Ϣѭ��
	//MSG msg;
	//while(GetMessage(&msg, NULL, 0, 0))
	//{
	//	TranslateMessage(&msg);
	//	DispatchMessage(&msg);
	//}
	return 0;
}
// ��WinMain��ʵ��
LRESULT CALLBACK WindowProc(
	_In_  HWND hwnd,
	_In_  UINT uMsg,
	_In_  WPARAM wParam,
	_In_  LPARAM lParam
	)
{
	switch(uMsg)
	{
	case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}


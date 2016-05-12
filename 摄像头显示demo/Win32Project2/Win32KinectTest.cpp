#include <Windows.h>
#include <streams.h>
#include "SmartPtr.h"
#include "SampleCGB.h"
// 必须要进行前导声明
LRESULT CALLBACK WindowProc(
	_In_  HWND hwnd,
	_In_  UINT uMsg,
	_In_  WPARAM wParam,
	_In_  LPARAM lParam
	);

// 程序入口点
int CALLBACK WinMain(
	_In_  HINSTANCE hInstance,
	_In_  HINSTANCE hPrevInstance,
	_In_  LPSTR lpCmdLine,
	_In_  int nCmdShow
	)
{
	// 类名
	WCHAR* cls_Name = L"My Class";
	// 设计窗口类
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
	// 注册窗口类
	RegisterClass(&wc);

	// 创建窗口
	HWND hwnd = CreateWindow(
		cls_Name,			//类名，要和刚才注册的一致
		L"我的应用程序",	//窗口标题文字
		WS_OVERLAPPEDWINDOW, //窗口外观样式
		38,				//窗口相对于父级的X坐标
		20,				//窗口相对于父级的Y坐标
		1920,				//窗口的宽度
		1080,				//窗口的高度
		NULL,				//没有父窗口，为NULL
		NULL,				//没有菜单，为NULL
		hInstance,			//当前应用程序的实例句柄
		NULL);				//没有附加数据，为NULL
	if(hwnd == NULL) //检查窗口是否创建成功
		return 0;

	// 显示窗口
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

		hr = pBag->Read(L"FriendlyName", &var, NULL);		//读取FriendlyName到var.bstrVal,并放入gcap.wachFriendlyName
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
	hr = pBuilder->SetFiltergraph(pFg);	//gcap.pFg是上面MakeGraph()里绑定的Graph Builder
	hr = pFg->AddFilter(pVCap, L"kinect");


	//// 更新窗口
	//UpdateWindow(hwnd);

	//// 消息循环
	//MSG msg;
	//while(GetMessage(&msg, NULL, 0, 0))
	//{
	//	TranslateMessage(&msg);
	//	DispatchMessage(&msg);
	//}
	return 0;
}
// 在WinMain后实现
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


#include "stdafx.h"
#include <dbt.h>
#include <mmreg.h>
#include <msacm.h>
#include <fcntl.h>
#include <io.h>
#include <stdio.h>
#include <commdlg.h>
#include <atlbase.h>
#include <atlcom.h>
#include <atlctl.h>
#include "SampleCGB.h"
#include "GraphUtil.h"
//#include <strsafe.h>
//#include "LoadGraphInGE.cpp"
using namespace msdx;

const CLSID clsid_lead_h264_encoder ={0xE2B7DF52,0x38C5,0x11D5,{0x91,0xF6,0x00,0x10,0x4B,0xDB,0x8F,0xF9}};

#define ID_APP      1000
//------------------------------------------------------------------------------
// Global data
//------------------------------------------------------------------------------
HINSTANCE ghInstApp=0;
HACCEL ghAccel=0;
HFONT  ghfontApp=0;
TEXTMETRIC gtm={0};
TCHAR gszAppName[]=TEXT("AMCAP");
HWND ghwndApp=0, ghwndStatus=0;
DWORD g_dwGraphRegister=0;

extern HRESULT AddToRotS(IUnknown *pUnkGraph, DWORD *pdwRegister) ;
LONG WINAPI  AppWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
		break;

	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}
BOOL AppInit(HINSTANCE hInst, HINSTANCE hPrev, int sw)
{
	WNDCLASS    cls;
	HDC         hdc;

	const DWORD  dwExStyle = 0;

	CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

	/* Save instance handle for DialogBoxs */
	ghInstApp = hInst;

	ghAccel = LoadAccelerators(hInst, MAKEINTATOM(ID_APP));

	if(!hPrev)
	{
		/*
		*  Register a class for the main application window
		*/
		cls.hCursor        = LoadCursor(NULL,IDC_ARROW);
		cls.hIcon          = NULL;
		cls.lpszMenuName   = MAKEINTATOM(ID_APP);
		cls.lpszClassName  = MAKEINTATOM(ID_APP);
		cls.hbrBackground  = (HBRUSH)(COLOR_WINDOW + 1);
		cls.hInstance      = hInst;
		cls.style          = CS_BYTEALIGNCLIENT | CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS;
		cls.lpfnWndProc    = (WNDPROC) AppWndProc;
		cls.cbWndExtra     = 0;
		cls.cbClsExtra     = 0;

		if(!RegisterClass(&cls))
			return FALSE;
	}

	// Is this necessary?
	ghfontApp = (HFONT)GetStockObject(ANSI_VAR_FONT);
	hdc = GetDC(NULL);
	SelectObject(hdc, ghfontApp);
	GetTextMetrics(hdc, &gtm);
	ReleaseDC(NULL, hdc);

	ghwndApp=CreateWindowEx(dwExStyle,
							MAKEINTATOM(ID_APP),    // Class name
							gszAppName,             // Caption
							// Style bits
							WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
							CW_USEDEFAULT, 0,       // Position
							320,300,                // Size
							(HWND)NULL,             // Parent window (no parent)
							(HMENU)NULL,            // use class menu
							hInst,                  // handle to window instance
							(LPSTR)NULL             // no params to pass on
							);


	ShowWindow(ghwndApp,sw);

	IBindCtx *lpBC=0;
	IMoniker *pMoniker = 0;
	IBaseFilter *pFilter=NULL;
	IGraphBuilder *pGraph=NULL;
	HRESULT hr;
	GraphUtil::GetFilterByDisplayName(L"@device:pnp:\\\\?\\usb#vid_046d&pid_0826&mi_02#6&b8ca63f&0&0002#{65e8773d-8f56-11d0-a3b9-00a0c9223196}\\{bbefb6c7-2fc4-4139-bb8b-a58bba724083}",&pFilter);

	ISampleCaptureGraphBuilder *pBuilder = new ISampleCaptureGraphBuilder( );
	if( NULL == pBuilder )
	{
		return FALSE;
	}
	hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC, IID_IGraphBuilder, (LPVOID *)&pGraph);
	hr = pBuilder->SetFiltergraph(pGraph);	//gcap.pFg是上面MakeGraph()里绑定的Graph Builder
	hr = pGraph->AddFilter(pFilter, L"source");

	CComPtr<IBaseFilter> pEncoder;
	GraphUtil::GetFilterByCLSID(clsid_lead_h264_encoder,&pEncoder);
	hr = pGraph->AddFilter(pEncoder,L"lead encoder");

	ConnectFilters(pGraph,pFilter,pEncoder);

	//Render
	IBaseFilter *pRender;
	hr =GraphUtil::GetFilterByDisplayName(L"@device:sw:{083863F1-70DE-11D0-BD40-00A0C911CE86}\\{70E102B0-5556-11CE-97C0-00AA0055595A}",&pRender);
	pGraph->AddFilter(pRender,L"Render");

	ConnectFilters(pGraph,pEncoder,pRender); 



	////InfPinTee
	//CComPtr<IBaseFilter> inf_pin_tee_filter_;
	//hr = GraphUtil::AddFilterByCLSID(pGraph, L"Inf", 
	//	GraphUtil::clsid_inf_pin_tee, &inf_pin_tee_filter_);

	//hr =GraphUtil::ConnectFilters(pGraph,pFilter,inf_pin_tee_filter_);

	////rgb2yuv
	//IBaseFilter *pConverter;
	//hr = GraphUtil::AddFilterByCLSID(pGraph, L"rgb2yuv", 
	//	GraphUtil::clsid_ffdshow_video_decoder, &pConverter);

	//hr = ConnectFilters(pGraph,inf_pin_tee_filter_,pConverter);
	//
	//pBuilder->RenderStream(NULL,&MEDIATYPE_Video,pConverter,NULL,NULL);
	
	////EVR
	//CComPtr<IBaseFilter> evr_renderer_filter_;
	//hr = GraphUtil::AddFilterByCLSID(pGraph, L"EVR", 
	//	CLSID_EnhancedVideoRenderer, &evr_renderer_filter_);

	//hr = pBuilder->RenderStream(&PIN_CATEGORY_PREVIEW, &MEDIATYPE_Video, pFilter, NULL,inf_pin_tee_filter_);







	

	//IBaseFilter *pEncoder;
	//hr = GraphUtil::AddFilterByCLSID(pGraph, L"Intel H.264 Enc", 
	//	GraphUtil::clsid_intel_h264_enc, &pEncoder);


	//hr = ConnectFilters(pGraph,pConverter,pEncoder);

/*	hr = GraphUtil::ConnectFilters(pGraph,pConverter,pRender)*/;
	////pGraph->Render(outpin);

	//IBaseFilter *pRtpSender;
	//hr = GraphUtil::AddFilterByCLSID(pGraph, L"RTP Sender", 
	//	GraphUtil::clsid_scut_rtp_sender, &pRtpSender);

	//hr = GraphUtil::ConnectFilters(pGraph,pEncoder,pRtpSender);
	


	//IAMStreamConfig *pVSC;      // for video cap

	//hr = pBuilder->FindInterface(&PIN_CATEGORY_CAPTURE,                                                                                                                                                                                                                                                                                                                                                                         
	//		&MEDIATYPE_Video, pVCap,
	//		IID_IAMStreamConfig, (void **)&pVSC);		//pVC - IAMStreamConfig*
	

	DWORD dwRegister;

	//msdx::GraphUtil::AddToRot(L"Kinect",pGraph,&dwRegister);
	hr = AddToRotS(pGraph, &dwRegister);


	//IVideoWindow *pVW;
	//hr = pFg->QueryInterface(IID_IVideoWindow, (void **)&pVW);	//Video Window 接口

	//IMediaControl *pMC = NULL;
	//hr = pGraph->QueryInterface(IID_IMediaControl, (void **)&pMC);
	//if(SUCCEEDED(hr))
	//{
	//	hr = pMC->Run();
	//	if(FAILED(hr))
	//	{
	//		// stop parts that ran
	//		pMC->Stop();
	//	}
	//	pMC->Release();
	//}


	return TRUE;
}
int PASCAL WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR szCmdLine, int sw)
{
	MSG msg;

	/* Call initialization procedure */
	if(!AppInit(hInst,hPrev,sw))
		return FALSE;

	/*
	* Polling messages from event queue
	*/
	for(;;)
	{
		while(PeekMessage(&msg, NULL, 0, 0,PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
				break;  // Leave the PeekMessage while() loop

			if(TranslateAccelerator(ghwndApp, ghAccel, &msg))
				continue;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if(msg.message == WM_QUIT)
			break;  // Leave the for() loop

		WaitMessage();
	}

	// Reached on WM_QUIT message
	CoUninitialize();
	return ((int) msg.wParam);
}

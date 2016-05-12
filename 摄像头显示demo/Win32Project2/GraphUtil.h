/************************************************************************/
/* �˴�����graph��صĹ��߷���                                          */
/************************************************************************/

#ifndef GRAPH_UTIL_H
#define GRAPH_UTIL_H

#define SAFE_DELETE_PTR(ptr) \
	if (ptr) { delete (ptr); (ptr) = nullptr; }



namespace msdx
{

	//graph������
	class GraphUtil
	{
	private:
		GraphUtil(void);
		~GraphUtil(void);

	public:
		static HRESULT GetFilterByCLSID(const CLSID &clsid, IBaseFilter **ppF);
		//���������ֲ���filter�ķ�����Ϊһ��--HJF
		static HRESULT getFilterByVariousName(const CLSID & clsidDeviceClass, const int type,LPCWSTR wszName, IBaseFilter **ppF);
		//Set wszFrendlyName to be NULL to get the Default Device
		static HRESULT GetFilterByFriendlyName(const CLSID &clsidDeviceClass, LPCWSTR wszFrendlyName, IBaseFilter **ppF);
		static HRESULT GetFilterByDisplayName(LPCWSTR wszDisplayName, IBaseFilter **ppF);
		//Device path is just a part of display name
		static HRESULT GetFilterByDevicePath(const CLSID &clsidDeviceClass, LPCWSTR wszDevicePath, IBaseFilter **ppF);
		static HRESULT AddFilterByCLSID(IGraphBuilder *graph, LPCWSTR wszName, const CLSID &clsid, IBaseFilter **ppF);
		static HRESULT GetUnconnectedPin(IBaseFilter *pFilter, PIN_DIRECTION PinDir, IPin **ppPin);
		static HRESULT ConnectFilters(IGraphBuilder *pGraph, IBaseFilter *pSource, IBaseFilter *pDest);
		static HRESULT ConnectFilters(IGraphBuilder *pGraph, IPin *pPinOut, IBaseFilter *pDest);


		//Debug���ߣ���ӵ�graph editor����
		static HRESULT AddToRot(LPCWSTR wszDescription, IUnknown *pUnkGraph, DWORD *pdwRegister);
		static void RemoveFromRot(DWORD pdwRegister);


	public:

		//GUID
		//video
		static const CLSID clsid_intel_h264_enc;
		static const CLSID clsid_intel_h264_dec;
		static const CLSID clsid_scut_rgb2yuv;
		static const CLSID clsid_scut_h264_enc;
		static const CLSID clsid_scut_h264_dec;
		//audio
		static const CLSID clsid_scut_aec_source;
		static const CLSID clsid_scut_speex_enc;
		static const CLSID clsid_scut_speex_dec;
		static const CLSID clsid_monogram_aac_enc;
		static const CLSID clsid_monogram_aac_dec;
		//new rtp
		static const CLSID clsid_scut_rtp_sender;
		static const CLSID clsid_scut_rtp_receiver;
		//old rtp
		static const CLSID clsid_scut_rtp_out_v;
		static const CLSID clsid_scut_rtp_out_a;
		static const CLSID clsid_scut_rtp_in_v;
		static const CLSID clsid_scut_rtp_in_a;
		//record
		static const CLSID clsid_gdcl_mp4_muxer;
		//file play
		static const CLSID clsid_gdcl_mp4_demuxer;
		static const CLSID clsid_scut_play_ctrl;
		//other
		static const CLSID clsid_null_renderer;
		static const CLSID clsid_inf_pin_tee;
		static const CLSID clsid_scut_multi_smart_tee;
		static const CLSID clsid_file_source;
		
		//ffdshow
		static const CLSID clsid_ffdshow_video_decoder;
	};

}


#endif

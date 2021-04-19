#pragma once

//enum CAPTURE_TECH_METHOD
//{
//	DESKTOP_DUPLICATION_API = 1,
//	GDI_BITBLT = 2,
//};
//
//enum CAPTURE_WINDOW_METHOD
//{
//	NON_CAPTURE = 0,
//	MONITOR_CAPTURE = 1,
//	WINDOW_CAPTURE = 2
//};
//
//typedef struct _CaptureMethod
//{
//	CAPTURE_TECH_METHOD		techStatus;
//	CAPTURE_WINDOW_METHOD	windowStatus;
//
//	union {
//		struct {
//			UINT monitorNumber;
//		} monitors;
//
//		struct {
//			HANDLE windowHandle;
//		} windows;
//	};
//
//} MonitorCapture;


//struct D3D11Monitors 
//{
//	CComPtrCustom<ID3D11Device> lDevice;
//	CComPtrCustom<ID3D11DeviceContext> lImmediateContext;
//	CComPtrCustom<IDXGIOutputDuplication> lDeskDupl;
//	CComPtrCustom<ID3D11Texture2D> lAcquiredDesktopImage;
//	CComPtrCustom<ID3D11Texture2D> lGDIImage;
//	CComPtrCustom<ID3D11Texture2D> lDestImage;
//
//	DXGI_OUTPUT_DESC lOutputDesc;
//	DXGI_OUTDUPL_DESC lOutputDuplDesc;
//
//	UINT  ddMonitors;
//
//};


	//bool CreateDesktopDuplicationHandle();
	//bool DetectMultipleMonitors();


	//MonitorCapture	monitorCapture;
	//D3D11Monitors	d3dMonitors;

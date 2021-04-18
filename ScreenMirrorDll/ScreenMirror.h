// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the NATIVELIBTEST_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// SCREENMIRROR_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef SCREENMIRROR_EXPORTS
#define SCREENMIRROR_API __declspec(dllexport)
#else
#define SCREENMIRROR_API  __declspec(dllimport)
#endif

#include <d3d11.h>
#include "ComPtr.h"
#include "DXGI1_2.h"
#include <vector>

enum CAPTURE_TECH_METHOD
{
	DESKTOP_DUPLICATION_API = 1,
	GDI_BITBLT = 2,
};

enum CAPTURE_WINDOW_METHOD
{
	NON_CAPTURE = 0,
	MONITOR_CAPTURE = 1,
	WINDOW_CAPTURE = 2
};

typedef struct _CaptureMethod
{
	CAPTURE_TECH_METHOD		techStatus;
	CAPTURE_WINDOW_METHOD	windowStatus;

	union {
		struct {
			UINT monitorNumber;
		} monitors;

		struct {
			HANDLE windowHandle;
		} windows;
	};

} MonitorCapture;

typedef void (*ChangedWindowCallback)(int width, int height);
typedef std::vector<ChangedWindowCallback> ChangedWindowFuncList;

struct GDIMonitors
{
	std::vector<int>       iMonitors;
	std::vector<HMONITOR>  hMonitors;
	std::vector<HDC>       hdcMonitors;
	std::vector<RECT>      rcMonitors;

	static BOOL CALLBACK MonitorEnum(HMONITOR hMon, HDC hdc, LPRECT lprcMonitor, LPARAM pData)
	{
		GDIMonitors* pThis = reinterpret_cast<GDIMonitors*>(pData);

		pThis->hMonitors.push_back(hMon);
		pThis->hdcMonitors.push_back(hdc);
		pThis->rcMonitors.push_back(*lprcMonitor);
		pThis->iMonitors.push_back(pThis->hdcMonitors.size());
		return TRUE;
	}

	GDIMonitors()
	{
		EnumDisplayMonitors(0, 0, MonitorEnum, (LPARAM)this);
	}

	int width(int monitorId) 
	{
		if (monitorId >= rcMonitors.size()) {

			return 0;
		}

		return rcMonitors[monitorId].right - rcMonitors[monitorId].left;
	}

	int height(int monitorId)
	{
		if (monitorId >= rcMonitors.size()) {

			return 0;
		}

		return rcMonitors[monitorId].bottom - rcMonitors[monitorId].top;
	}
};

struct D3D11Monitors 
{
	CComPtrCustom<ID3D11Device> lDevice;
	CComPtrCustom<ID3D11DeviceContext> lImmediateContext;
	CComPtrCustom<IDXGIOutputDuplication> lDeskDupl;
	CComPtrCustom<ID3D11Texture2D> lAcquiredDesktopImage;
	CComPtrCustom<ID3D11Texture2D> lGDIImage;
	CComPtrCustom<ID3D11Texture2D> lDestImage;

	DXGI_OUTPUT_DESC lOutputDesc;
	DXGI_OUTDUPL_DESC lOutputDuplDesc;

	UINT  ddMonitors;

};

class SCREENMIRROR_API ScreenMirrorWrapper
{
public:
	ScreenMirrorWrapper();
	~ScreenMirrorWrapper();

public:

	bool Initialize();
	bool RegisterCallback(ChangedWindowCallback callBack);

	bool StartCapture();
	bool CloseCapture();
	bool GetScreenSize(int &width, int &height);
	bool GetScreenFrame(void* buffer, UINT bufferSize);
	int  GetRefCount();
	bool SelectWindow();

private:
	int PrintLog(const char* fmt...);

	bool Finalize();

	bool CreateDesktopDuplicationHandle();
	bool DetectMultipleMonitors();

	int GetNextScreenFrame();
	friend void CaptureThread(ScreenMirrorWrapper* wrapper);

private:
	UINT refCount;

	UINT width;
	UINT height;

	BOOL bThreadStarted;
	
	PVOID screenBuffer;
	UINT  screenBufferSize;

	ChangedWindowFuncList changedWindowFuncs;

	MonitorCapture	monitorCapture;
	GDIMonitors		gdiMonitors;
	D3D11Monitors	d3dMonitors;
};
#pragma once
#include <vector>
#include <Windows.h>
#include "MonitorBase.h"

class GDIMonitors : public MonitorBase
{
public:

	GDIMonitors();
	~GDIMonitors();
	
public:
	virtual bool isInitialized();
	virtual bool isSupported();

	virtual int GetMonitors();

	virtual int GetWidth();
	virtual int GetHeight();
	virtual BOOL GetScreenSize(int& width, int& height);
	virtual BOOL GetScreenData(void* buffer, unsigned int bufferSize);
	
	virtual BOOL SetCurrentMonitor(int monitorId);
	BOOL SetTargetWindow(HWND targetWnd);

private:

	BOOL UpdateBitmapForCapture();
	BOOL CreateCaptureStructure(HWND targetWnd);

	friend static BOOL CALLBACK MonitorEnum(HMONITOR hMon, HDC hdc, LPRECT lprcMonitor, LPARAM pData);

private:
	std::vector<HMONITOR>  hMonitors;
	std::vector<HDC>       hdcMonitors;
	std::vector<RECT>      rcMonitors;

	char* buffer;
	unsigned int bufferSize;

	HWND	targetWindow;
	HBITMAP hBitmap;
	HDC		hMemDC;
	HDC		hDC;

	unsigned int width;
	unsigned int height;

	BOOL	isMonitorCapture;
};

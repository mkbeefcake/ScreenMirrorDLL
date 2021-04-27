#include "stdafx.h"
#include "GDIMonitor.h"
#include "ScreenMirror.h"
#include <mutex>

static BOOL CALLBACK MonitorEnum(HMONITOR hMon, HDC hdc, LPRECT lprcMonitor, LPARAM pData)
{
	GDIMonitors* pThis = reinterpret_cast<GDIMonitors*>(pData);

	pThis->hMonitors.push_back(hMon);	
	pThis->rcMonitors.push_back(*lprcMonitor);

	MONITORINFOEX info;
	ZeroMemory(&info, sizeof(info));
	info.cbSize = sizeof(info);

	::GetMonitorInfo(hMon, &info);

	HDC hDC = CreateDC(TEXT("DISPLAY"), info.szDevice, NULL, NULL);
	pThis->hdcMonitors.push_back(hDC);

	ScreenMirrorWrapper::PrintLog("GDIMonitors::MonitorEnum() - HDC = 0x%x, Index = %d \n", 
		hDC, pThis->hdcMonitors.size());

	return TRUE;
}

GDIMonitors::GDIMonitors()
{
	ScreenMirrorWrapper::PrintLog("GDIMonitors::GDIMonitors() is called\n");

	HDC desktopDC = ::GetDC(NULL);
	EnumDisplayMonitors(desktopDC, 0, MonitorEnum, (LPARAM)this);
	::ReleaseDC(NULL, desktopDC);

	ScreenMirrorWrapper::PrintLog("GDIMonitors::GDIMonitors() - Finished enumerate monitors\n");

	buffer = NULL;
	bufferSize = 0;

	targetWindow = NULL;
	hBitmap = NULL;
	hMemDC = NULL;

	isMonitorCapture = TRUE;

	width = 0;
	height = 0;

	SetCurrentMonitor(0);
}

GDIMonitors::~GDIMonitors()
{
	for (int i = 0; i < hdcMonitors.size(); i++) {
		::DeleteDC(hdcMonitors[i]); hdcMonitors[i] = NULL;
	}

	hMonitors.clear();
	hdcMonitors.clear();
	rcMonitors.clear();

	if (hDC) {
		::ReleaseDC(targetWindow, hDC); hDC = NULL;
	}

	if (hBitmap) {
		::DeleteObject(hBitmap); hBitmap = NULL;
		bufferSize = 0; buffer = NULL;
	}

	if (hMemDC) {
		::DeleteDC(hMemDC); hMemDC = NULL;
	}

}

bool GDIMonitors::isInitialized()
{
	return true;
}

bool GDIMonitors::isSupported()
{
	return true;
}

int GDIMonitors::GetMonitors()
{
	return (int)rcMonitors.size();
}

int GDIMonitors::GetWidth()
{
	return width;
}

int GDIMonitors::GetHeight()
{
	return height;
}

BOOL GDIMonitors::GetScreenSize(int& width, int& height)
{
	width = GetWidth();
	height = GetHeight();

	return TRUE;
}

BOOL GDIMonitors::GetScreenData(void* dstBuffer, unsigned int dstBufferSize)
{
	unsigned int minSize = min(dstBufferSize, bufferSize);

	if (dstBuffer == NULL)
		return FALSE;

	if (minSize <= 0)
		return FALSE;

	if (UpdateBitmapForCapture())
		return FALSE;

	::UpdateWindow(targetWindow);
	::BitBlt(hMemDC, 0, 0, width, height, hDC, 0, 0, SRCCOPY);
	::GdiFlush();

	memcpy(dstBuffer, buffer, minSize);

	return TRUE;
}

BOOL GDIMonitors::SetCurrentMonitor(int monitorId)
{
	if (monitorId < 0 || monitorId >= rcMonitors.size())
		return false;

	ScreenMirrorWrapper::PrintLog("GDIMonitors::SetCurrentMonitor() - monitor Id = %d\n", monitorId);

	MonitorBase::SetCurrentMonitor(monitorId);

	isMonitorCapture = TRUE;
	CreateCaptureStructure(NULL);

	return TRUE;
}

BOOL GDIMonitors::SetTargetWindow(HWND targetWnd)
{
	if (targetWnd == NULL) {
		return FALSE;
	}

	isMonitorCapture = FALSE;
	CreateCaptureStructure(targetWnd);

	return TRUE;
}

BOOL GDIMonitors::CreateCaptureStructure(HWND targetWnd)
{
	if (targetWnd == NULL && isMonitorCapture == FALSE) {
		return FALSE;
	}

	if (hDC) {

		if (targetWindow) {
			::ReleaseDC(targetWindow, hDC); 
		}

		hDC = NULL;
	}

	if (hMemDC) {
		::DeleteDC(hMemDC); hMemDC = NULL;
	}

	if (hBitmap) {
		::DeleteObject(hBitmap); hBitmap = NULL;		
		bufferSize = 0;  buffer = NULL;
	}
	
	targetWindow = targetWnd;

	//
	// Create compatible memory DC
	//

	if (isMonitorCapture == FALSE) {
		hDC = ::GetDC(targetWindow);
	}
	else {
		hDC = hdcMonitors[curMonitorID];
	}

	hMemDC = ::CreateCompatibleDC(hDC);

	//
	// Set width, height
	//

	RECT rect;
	if (isMonitorCapture == FALSE) {
		::GetWindowRect(targetWindow, &rect);
	}
	else {
		rect = rcMonitors[curMonitorID];
	}

	width = rect.right - rect.left;
	height = rect.bottom - rect.top;

	//
	// Create Bitmap buffer
	//

	//bufferSize = width * height * 4;
	//buffer = (char*) malloc(bufferSize);
	//memset(buffer, 0, bufferSize);

	BITMAPINFO bmpInfo;
	bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpInfo.bmiHeader.biWidth = width;
	bmpInfo.bmiHeader.biHeight = height;
	bmpInfo.bmiHeader.biPlanes = 1;
	bmpInfo.bmiHeader.biBitCount = 32;
	bmpInfo.bmiHeader.biCompression = BI_RGB;

	hBitmap = ::CreateDIBSection(NULL, &bmpInfo, DIB_RGB_COLORS, (void**)&buffer, NULL, 0);
	bufferSize = width * height * 4;
	::SelectObject(hMemDC, hBitmap);

	ScreenMirrorWrapper::PrintLog("GDIMonitors::CreateCaptureStructure() is completed\n");

	return TRUE;
}

BOOL GDIMonitors::UpdateBitmapForCapture()
{
	RECT rect;
	UINT curWidth = 0, curHeight = 0;

	if (isMonitorCapture == FALSE) {
		::GetWindowRect(targetWindow, &rect);
	}
	else {
		//
		return FALSE;
	}

	curWidth = rect.right - rect.left;
	curHeight = rect.bottom - rect.top;

	if (curWidth == width && curHeight == height)
		return FALSE;

	printf("Resized : [%d, %d] => [%d, %d]\n", width, height, curWidth, curHeight);

	//
	// Create bitmap information
	//
	if (hBitmap) {
		::DeleteObject(hBitmap); hBitmap = NULL;
		bufferSize = 0;  buffer = NULL;
	}

	width = curWidth;
	height = curHeight;

	BITMAPINFO bmpInfo;
	bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpInfo.bmiHeader.biWidth = width;
	bmpInfo.bmiHeader.biHeight = height;
	bmpInfo.bmiHeader.biPlanes = 1;
	bmpInfo.bmiHeader.biBitCount = 32;
	bmpInfo.bmiHeader.biCompression = BI_RGB;

	hBitmap = ::CreateDIBSection(NULL, &bmpInfo, DIB_RGB_COLORS, (void**)&buffer, NULL, 0);
	bufferSize = width * height * 4;
	::SelectObject(hMemDC, hBitmap);

	return TRUE;
}

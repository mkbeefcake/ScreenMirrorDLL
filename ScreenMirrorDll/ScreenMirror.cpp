// ScreenMirror.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "ScreenMirror.h"
#include <algorithm>
#include <assert.h>
#include <mutex>
#include <cstdio>
#include "FindWindow.h"
#include "GDIMonitor.h"

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")

std::mutex globalMutex;
std::thread globalThread;

void CaptureThread(ScreenMirrorWrapper* wrapper);

ScreenMirrorWrapper::ScreenMirrorWrapper()
{
	PrintLog("ScreenMirrorWrapper::ScreenMirrorWrapper() is called.\n");

	refCount = 0;

	screenBuffer = NULL;
	screenBufferSize = 0;
	bThreadStarted = FALSE;

	gdiMonitors = new GDIMonitors();
	//changedWindowFuncs.clear();
}

ScreenMirrorWrapper::~ScreenMirrorWrapper()
{
	Finalize();
	PrintLog("ScreenMirrorWrapper::~ScreenMirrorWrapper() is called.\n");
}

int ScreenMirrorWrapper::PrintLog(const char* fmt...)
{
#define PRINT_LOG
#ifdef  PRINT_LOG

	FILE* fp;
	fp = fopen("scnmir-log.txt", "a");

	va_list argptr;
	va_start(argptr, fmt);
	vfprintf(fp, fmt, argptr);
	va_end(argptr);

	fclose(fp);

#endif	
	return 0;
}

bool ScreenMirrorWrapper::Initialize()
{
	bool bRet = true;

	PrintLog("ScreenMirrorWrapper::Initialize() - Initialized resources. \n");

	//
	// Create screen buffer
	//

	int width = gdiMonitors->GetWidth();
	int height = gdiMonitors->GetHeight();

	if (width > 0 && height > 0)
	{
		if (screenBuffer) 
		{
			free(screenBuffer); screenBuffer = NULL;
		}

		screenBuffer = malloc(width * height * 4);
		screenBufferSize = width * height * 4;
		memset(screenBuffer, 0, screenBufferSize);
	}


	return true;
}

bool ScreenMirrorWrapper::Finalize()
{
	if (bThreadStarted) 
	{
		refCount = 0;
		globalThread.join();
		bThreadStarted = FALSE;
	}

	globalMutex.lock();
	{
		if (screenBuffer) {
			free(screenBuffer);
			screenBuffer = NULL; screenBufferSize = 0;
		}
	}
	globalMutex.unlock();

	bThreadStarted = FALSE;
	//changedWindowFuncs.clear();

	return true;
}

//bool ScreenMirrorWrapper::RegisterCallback(ChangedWindowCallback callBack)
//{
//	if (callBack == NULL)
//		return false;
//
//	changedWindowFuncs.push_back(callBack);
//	return true;
//}

bool ScreenMirrorWrapper::StartCapture()
{
	if (refCount == 0) 
	{
		refCount++;

		//
		// Create capture thread
		//

		globalThread = std::thread(&CaptureThread, this);
		bThreadStarted = TRUE;
	}
	else
	{
		refCount++;
	}

	PrintLog("ScreenMirrorWrapper::StartCapture() - Ref = %d\n", refCount);
	return true;
}

void CaptureThread(ScreenMirrorWrapper* wrapper)
{
	ScreenMirrorWrapper::PrintLog("CaptureThread::() Started...\n");

	wrapper->bThreadStarted = TRUE;

	do
	{
		::Sleep(100);
		if (wrapper->GetNextScreenFrame() < -1)
			break;

	} while (wrapper->GetRefCount() > 0);

	wrapper->PrintLog("CaptureThread::() Stopped...\n");
}

bool ScreenMirrorWrapper::CloseCapture()
{
	refCount--;
	if (refCount < 0) 
	{
		refCount = 0;
	}

	PrintLog("ScreenMirrorWrapper::CloseCapture() - Ref = %d\n", refCount);
	return true;
}


bool ScreenMirrorWrapper::GetScreenSize(int& width, int& height)
{
	width = gdiMonitors->GetWidth();
	height = gdiMonitors->GetHeight();

	// PrintLog("ScreenMirrorWrapper::GetScreenSize() - width = %d, height = %d\n", width, height);
	return true;
}

bool ScreenMirrorWrapper::GetScreenFrame(void* buffer, UINT bufferSize)
{
	if (buffer == NULL || bufferSize <= 0)
		return false;

	UINT copySize = std::min<UINT>(bufferSize, screenBufferSize);

	globalMutex.lock();
	{
		if (screenBuffer)
		{
			memcpy(buffer, screenBuffer, copySize);
		}
	}
	globalMutex.unlock();

	return true;
}

int ScreenMirrorWrapper::GetRefCount()
{
	return refCount;
}

bool ScreenMirrorWrapper::SelectWindowDialog()
{
	INT selectedMonitor = -1;
	HWND selectedWindow = NULL;

	FindWindowDialog findWindow;

	BOOL bRet = findWindow.StartSearchWindowDialog(gdiMonitors->GetMonitors(), selectedMonitor, selectedWindow);	// multiple monitors count
	if (bRet == TRUE)
	{
		globalMutex.lock();
		{
			if (selectedWindow) 
			{
				gdiMonitors->SetTargetWindow(selectedWindow);
			}
			else if (selectedMonitor != -1) 
			{
				gdiMonitors->SetCurrentMonitor(selectedMonitor);
			}

			if (screenBuffer) 
			{
				free(screenBuffer);
				screenBuffer = NULL; screenBufferSize = 0;
			}
		}
		globalMutex.unlock();

		//
		// invoke callback functions
		//

		//for (int i = 0; i < changedWindowFuncs.size(); i++) 
		//{
		//	changedWindowFuncs[i](gdiMonitors->GetWidth(), gdiMonitors->GetHeight());
		//}
	}

	return bRet;
}


int ScreenMirrorWrapper::GetNextScreenFrame()
{
	int size = -1;
	globalMutex.lock();
	{
		if (screenBuffer == NULL || screenBufferSize == 0) 
		{
			int width = gdiMonitors->GetWidth();
			int height = gdiMonitors->GetHeight();

			screenBufferSize = width * height * 4;
			screenBuffer = malloc(screenBufferSize);
			memset(screenBuffer, 0, screenBufferSize);
		}

		gdiMonitors->GetScreenData(screenBuffer, screenBufferSize);
	}
	globalMutex.unlock();
	return size;
}


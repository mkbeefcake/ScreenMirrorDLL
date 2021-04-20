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

//typedef void (*ChangedWindowCallback)(int width, int height);
//typedef std::vector<ChangedWindowCallback> ChangedWindowFuncList;

class GDIMonitors;

class SCREENMIRROR_API ScreenMirrorWrapper
{
public:
	ScreenMirrorWrapper();
	~ScreenMirrorWrapper();

public:

	bool Initialize();
	//bool RegisterCallback(ChangedWindowCallback callBack);

	bool StartCapture();
	bool CloseCapture();
	int  GetRefCount();
	bool SelectWindowDialog();

	bool GetScreenSize(int& width, int& height);
	bool GetScreenFrame(void* buffer, UINT bufferSize);
	
	static int PrintLog(const char* fmt...);

private:

	bool Finalize();
	int GetNextScreenFrame();

	friend void CaptureThread(ScreenMirrorWrapper* wrapper);

private:
	UINT refCount;
	BOOL bThreadStarted;
	
	PVOID screenBuffer;
	UINT  screenBufferSize;

	//ChangedWindowFuncList changedWindowFuncs;
	GDIMonitors*		gdiMonitors;

};
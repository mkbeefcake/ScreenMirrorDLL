#include <windows.h>
#include <shlobj.h>
#include <shellapi.h>
#include "DXGI1_2.h"
#include <d3d11.h>
#include <iostream>       // std::cout
#include <thread>         // std::thread
#include <mutex>          // std::mutex
#include <algorithm>
#include <string>
#include <assert.h>
#include <string.h>
#include <strsafe.h>
#include <tchar.h>
#include <psapi.h>

#include "../ScreenMirrorDll/ComPtr.h"
#include "../ScreenMirrorDll/ScreenMirror.h"

// #pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")


// Driver types supported
D3D_DRIVER_TYPE gDriverTypes[] =
{
	D3D_DRIVER_TYPE_HARDWARE
};
UINT gNumDriverTypes = ARRAYSIZE(gDriverTypes);

// Feature levels supported
D3D_FEATURE_LEVEL gFeatureLevels[] =
{
	D3D_FEATURE_LEVEL_11_0,
	D3D_FEATURE_LEVEL_10_1,
	D3D_FEATURE_LEVEL_10_0,
	D3D_FEATURE_LEVEL_9_1
};

UINT gNumFeatureLevels = ARRAYSIZE(gFeatureLevels);

void saveBMP(void* buffer, UINT bufferSize, UINT threadId, UINT index, int width, int height)
{
	BITMAPINFO	lBmpInfo;

	// BMP 32 bpp

	ZeroMemory(&lBmpInfo, sizeof(BITMAPINFO));

	lBmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);

	lBmpInfo.bmiHeader.biBitCount = 32;

	lBmpInfo.bmiHeader.biCompression = BI_RGB;

	lBmpInfo.bmiHeader.biWidth = width;

	lBmpInfo.bmiHeader.biHeight = height;

	lBmpInfo.bmiHeader.biPlanes = 1;

	lBmpInfo.bmiHeader.biSizeImage = width * height * 4;


	std::unique_ptr<BYTE> pBuf(new BYTE[lBmpInfo.bmiHeader.biSizeImage]);
	memcpy(pBuf.get(), buffer, lBmpInfo.bmiHeader.biSizeImage);


	WCHAR lMyDocPath[MAX_PATH];

	HRESULT hr = SHGetFolderPath(nullptr, CSIDL_PERSONAL, nullptr, SHGFP_TYPE_CURRENT, lMyDocPath);

	if (FAILED(hr))
		return;

	std::wstring lFilePath = std::wstring(lMyDocPath) + L"\\ScreenShot-" 
		+ std::to_wstring(threadId) + L"-" + std::to_wstring(index) + L".bmp";

	FILE* lfile = nullptr;

	auto lerr = _wfopen_s(&lfile, lFilePath.c_str(), L"wb");

	if (lerr != 0)
		return;

	if (lfile != nullptr)
	{

		BITMAPFILEHEADER	bmpFileHeader;

		bmpFileHeader.bfReserved1 = 0;
		bmpFileHeader.bfReserved2 = 0;
		bmpFileHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + lBmpInfo.bmiHeader.biSizeImage;
		bmpFileHeader.bfType = 'MB';
		bmpFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

		fwrite(&bmpFileHeader, sizeof(BITMAPFILEHEADER), 1, lfile);
		fwrite(&lBmpInfo.bmiHeader, sizeof(BITMAPINFOHEADER), 1, lfile);
		fwrite(pBuf.get(), lBmpInfo.bmiHeader.biSizeImage, 1, lfile);

		fclose(lfile);

		// ShellExecute(0, 0, lFilePath.c_str(), 0, 0, SW_SHOW);
	}

	pBuf.release();
}

void foo(ScreenMirrorWrapper* wrapper, int threadId)
{
	void* buffer = malloc(1920 * 1080 * 4);
	UINT bufferSize = 1920 * 1080 * 4;

	wrapper->StartCapture();

	for (int i = 0; i < 203; i++) {
		::Sleep(40);
		wrapper->GetScreenFrame(buffer, bufferSize);
		
		if (i % 100 == 0) 
		{
			int width = 0, height = 0;
			wrapper->GetScreenSize(width, height);

			printf("Grabbed %d frame \n", i);
			saveBMP(buffer, bufferSize, threadId, i, width, height);
		}
	}

	wrapper->CloseCapture();
}

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	WCHAR class_name[512];
	WCHAR title[512];

	if (!hwnd)
		return TRUE;		// Not a window
	if (!::IsWindowVisible(hwnd))
		return TRUE;		// Not visible
	if (!SendMessage(hwnd, WM_GETTEXT, sizeof(title), (LPARAM)title))
		return TRUE;		// No window title

	GetClassName(hwnd, class_name, sizeof(class_name));
	wprintf(L"%s : %s \n", title, class_name);

	return TRUE;
}

void PrintProcessNameAndID(DWORD processID)
{
	TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");

	// Get a handle to the process.

	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
		PROCESS_VM_READ,
		FALSE, processID);

	// Get the process name.

	if (NULL != hProcess)
	{
		HMODULE hMod;
		DWORD cbNeeded;

		if (EnumProcessModules(hProcess, &hMod, sizeof(hMod),
			&cbNeeded))
		{
			GetModuleBaseName(hProcess, hMod, szProcessName,
				sizeof(szProcessName) / sizeof(TCHAR));
		}

		// Print the process name and identifier.
		_tprintf(TEXT("%s  (PID: %u)\n"), szProcessName, processID);
	}


	// Release the handle to the process.
	CloseHandle(hProcess);
}


int PrintProcesses()
{
	DWORD aProcesses[1024], cbNeeded, cProcesses;
	unsigned int i;

	if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
	{
		return 1;
	}


	// Calculate how many process identifiers were returned.

	cProcesses = cbNeeded / sizeof(DWORD);

	// Print the name and process identifier for each process.

	for (i = 0; i < cProcesses; i++)
	{
		if (aProcesses[i] != 0)
		{
			PrintProcessNameAndID(aProcesses[i]);
		}
	}

	return 0;
}

int main()
{
	HRESULT hr;

	//PrintProcesses();
	//return 0;

	//EnumWindows(EnumWindowsProc, NULL);
	//return 0;

	ScreenMirrorWrapper* wrapper = new ScreenMirrorWrapper();
	wrapper->Initialize();
	wrapper->SelectWindowDialog();

	std::thread thread1(foo, wrapper, 1);
	//std::thread thread2(foo, wrapper, 2);
	//std::thread thread3(foo, wrapper, 3);
	//std::thread thread4(foo, wrapper, 4);

	////::Sleep(500);

	thread1.join();
	//thread2.join();
	//thread3.join();
	//thread4.join();

	printf("Done\n");

	delete wrapper;
	return 0;
}
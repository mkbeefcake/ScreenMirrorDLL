#pragma once

#include <windows.h>

class FindWindowDialog
{

public:
	FindWindowDialog();
	~FindWindowDialog();

public:
	BOOL StartSearchWindowDialog(UINT monitors, INT& selectedMonitor, HWND& selectedWindow);

private:
	static BOOL CALLBACK SearchWindowDialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);

private:

	// nothing 
};


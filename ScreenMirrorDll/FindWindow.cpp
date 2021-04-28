#include "stdafx.h"
#include "FindWindow.h"
#include "resource.h"
#include <string>
#include <map>

ULONG		globalMonitors = 0;
INT			globalSelectedMonitor = -1;
HWND		globalFoundWindow = NULL;

std::map<int, HWND>	globalWindowList;
std::map<int, std::wstring>	globalWindowTitleList;

extern HINSTANCE globalInstance;

FindWindowDialog::FindWindowDialog()
{
	globalWindowList.clear();
	globalWindowTitleList.clear();

	EnumWindows(EnumWindowsProc, NULL);
}

FindWindowDialog::~FindWindowDialog()
{

}

BOOL CALLBACK FindWindowDialog::EnumWindowsProc(HWND hWnd, LPARAM lParam)
{
	//WCHAR class_name[512];
	WCHAR title[512];

	int len = GetWindowTextLength(hWnd);
	HWND owner = GetWindow(hWnd, GW_OWNER);
	LONG exstyle = GetWindowLong(hWnd, GWL_EXSTYLE);
	
	if (len == 0 || !IsWindow(hWnd) || IsIconic(hWnd) || !IsWindowVisible(hWnd) ||
		(owner && !(exstyle & WS_EX_APPWINDOW))) {
		return TRUE;
	}

	// Skip the Program Manager window and the Start button.
	const size_t kClassLength = 256;
	WCHAR class_name[kClassLength];
	GetClassName(hWnd, class_name, kClassLength);

	// Skip Program Manager window and the Start button. This is the same logic
	// that's used in Win32WindowPicker in libjingle. Consider filtering other
	// windows as well (e.g. toolbars).
	if (wcscmp(class_name, L"Progman") == 0 || wcscmp(class_name, L"Button") == 0)
		return TRUE;

	if (!SendMessage(hWnd, WM_GETTEXT, sizeof(title), (LPARAM)title))
		return TRUE;		// No window title

	// GetClassName(hwnd, class_name, sizeof(class_name));
	// wprintf(L"%s : %s \n", title, class_name);

	std::wstring windowTitle = L"Window : ";
	windowTitle.append(title);

	globalWindowList[globalWindowList.size()] = hWnd;
	globalWindowTitleList[globalWindowTitleList.size()] = windowTitle;

	return TRUE;
}

BOOL FindWindowDialog::StartSearchWindowDialog(UINT monitors, INT& selectedMonitor, HWND& selectedWindow)
{
	long lRet = 0;

	selectedMonitor = -1;
	selectedWindow = NULL;

	globalFoundWindow = NULL;
	globalMonitors = monitors;

	lRet = (long)DialogBox
	(
		(HINSTANCE)globalInstance, // handle to application instance 
		(LPCTSTR)MAKEINTRESOURCE(IDD_DIALOG_SEARCH_WINDOW), // identifies dialog box template 
		(HWND)GetDesktopWindow(), // handle to owner window 
		(DLGPROC)SearchWindowDialogProc // pointer to dialog box procedure 
	);

	if (lRet != IDOK)
		return false;

	//
	// update necessary variable
	//
	selectedMonitor = globalSelectedMonitor;
	selectedWindow = globalFoundWindow;

	return true;
}

BOOL CALLBACK FindWindowDialog::SearchWindowDialogProc(
	HWND hwndDlg, // handle to dialog box 
	UINT uMsg, // message 
	WPARAM wParam, // first message parameter 
	LPARAM lParam // second message parameter 
)
{
	BOOL bRet = FALSE;  // Default return value.

	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		//
		// add monitors to combo box
		//
		HWND hComboBox = ::GetDlgItem(hwndDlg, IDC_MONITORS);
		::SendMessage(hComboBox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)L"No Monitor");

		for (ULONG i = 0; i < globalMonitors; i++) 
		{
			std::wstring monitor = L"Monitor ";
			monitor.append(std::to_wstring(i));
			::SendMessage(hComboBox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)monitor.c_str());
		}

		for (ULONG i = 0; i < globalWindowTitleList.size(); i++)
		{
			::SendMessage(hComboBox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)globalWindowTitleList[i].c_str());
		}

		::SendMessage(hComboBox, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
		bRet = TRUE;
		break;
	}


	case WM_COMMAND:
	{
		WORD wNotifyCode = HIWORD(wParam); // notification code 
		WORD wID = LOWORD(wParam);         // item, control, or accelerator identifier 
		HWND hwndCtl = (HWND)lParam;      // handle of control 

		if ((wID == IDOK) || (wID == IDCANCEL))
		{
			if (wID == IDOK)
			{
				HWND hComboBox = ::GetDlgItem(hwndDlg, IDC_MONITORS);
				INT selectedIndex = (INT)::SendMessage(hComboBox, CB_GETCURSEL, (WPARAM)0, (LPARAM)0);

				if (selectedIndex == CB_ERR) {
					globalSelectedMonitor = -1;
				}
				else {
					if (selectedIndex <= globalMonitors) {
						
						//
						// Monitor selected, No monitor, Monitor index
						// 

						globalSelectedMonitor = selectedIndex - 1;
					}
					else {

						// 
						// Window selected
						//

						globalFoundWindow = globalWindowList[selectedIndex - 1 - globalMonitors];
					}

				}
			}

			bRet = TRUE;
			EndDialog(hwndDlg, wID);
		}

		break;
	}

	default:
	{
		bRet = FALSE;
		break;
	}
	}

	return bRet;
}

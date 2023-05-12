///////////////////////////////////////////////////////////////////////////////
// Based on the Article of http://www.songho.ca/opengl/gl_mvc.html
// 
// ControllerMain.h, ControllerMain.h
// Derived baseWinController class for main window
//
//////////////////////////////////////////////////////////////////////////////
// winMainController.cpp
// 
// Window controller for mnain window
// 
// Modfier: M. Nakano (mnsk.kn@outlook.jp)
// Modified: Jan 2023
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <windows.h>
#include <shellapi.h>  // AddNotificationIcon, etc.
#include <sstream>

#include "baseWinController.h"

#include <source/main/resource.h>
#include <mylibrary/tools.h>

namespace Win
{
	BOOL CALLBACK ChildPlusDialog(HWND, LPARAM);
	BOOL CALLBACK WindowClose(HWND, LPARAM);

	INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

	class MainController :public baseWinController
	{
	public:
		
		struct ChildWindow
		{
		public:
			ChildWindow() {};
			ChildWindow(int numchilds) {
				//NumChilds = numchild;
				for (int i = 0; i < numchilds; i++) {
					List.push_back(i);
				};
			};

			std::vector<int> List;
		};

		struct Lparam {
			RECT rec;
			ChildWindow wins;
		};

		MainController();
		~MainController();

		LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

		LRESULT  AddNotificationIcon(HWND);
		LRESULT  DeleteNotificationIcon();
		LRESULT close();
		LRESULT create();                               // for WM_CRERATE
		LRESULT destroy();
		LRESULT size(WPARAM wParam, LPARAM lParam);      // for WM_SIZE: width, height, type(SIZE_MAXIMIZED...)

		void setDialogHandle(HWND handle) { handleDialog = handle; }
		HWND getDialogHandle() { return handleDialog; }
		ChildWindow childWins;;

	protected:
		UINT   WMAPP_notificationcallback{ 0 };
		UINT   WMAPP_hideflyout{ 0 };
	private:
		HWND handleDialog{ 0 };                                // handle of form dialog
	};


	UINT_PTR const HIDEFLYOUT_TIMER_ID = 1;

	/// <summary>
	/// handle events(messages) on all child windows that belong to the parent window.
	/// For example, close all child windows when the parent got WM_CLOSE message.
	/// lParam can be used to specify a event or message.
	/// </summary>
	MainController::MainController() :handleDialog(0) {}  

	MainController::~MainController() {}

	LRESULT MainController::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{

		switch (uMsg)
		{
		case WM_CREATE:
		{
			//Create ChildWindows to get list of child windows 
			childWins = ChildWindow{ getNumChildWins() }; // timin of main window creation

			return DefWindowProcW(handle, uMsg, wParam, lParam);

		}
		case WM_SIZE:// main window changed size 
		{

			return size(wParam, lParam);  // width, height, type

		}
		case WM_MOUSEHOVER:

			return DefWindowProcW(handle, uMsg, wParam, lParam);

		case WMAPP_NOTIFYCALLBACK:
		{
			return DefWindowProcW(handle, uMsg, wParam, lParam);

		}
		case WM_ENABLE:
		{
			bool flag;
			if (wParam) flag = true;
			else flag = false;
			return enable(flag);  // TRUE or FALSE

		}
		case WM_PAINT:
		{
			//paint();
			return DefWindowProcW(handle, uMsg, wParam, lParam);
		}

		case WM_COMMAND:
		{
			int wmId = LOWORD(wParam);
			// Parse the menu selections:
			switch (wmId)
			{
			case ID_SHOWABOUT_MODE:
			case IDM_ABOUT:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), handle, About);
				return 0;
			case ID_APPQUIT_MODE:
			case IDM_EXIT:
				return close();
			case ID_TOGGLE_DIALOG:
				(IsWindowVisible(handleDialog)) ? ShowWindow(handleDialog, SW_MINIMIZE) : ShowWindow(handleDialog, SW_RESTORE);
				return 0;
			}

		}
		case WM_CLOSE:
		{
			return close();

		}
		case WM_DESTROY:
		{

			return destroy();

		}
		case WM_SYSCOMMAND:
		{

			return DefWindowProcW(handle, uMsg, wParam, lParam);
		}

		//case  WM_MOUSEHOVER:
		case  WM_LBUTTONDOWN:
		{
			return DefWindowProcW(handle, uMsg, wParam, lParam);
		}

		default:
			return DefWindowProcW(handle, uMsg, wParam, lParam);

		}

		return 0;
	};


	LRESULT  MainController::AddNotificationIcon(HWND hWnd)
	{
		//NOTIFYICONDATAA structure (shellapi.h)  https://docs.microsoft.com/en-us/windows/win32/api/shellapi/ns-shellapi-notifyicondataa#remarks

		NOTIFYICONDATA nid = { sizeof(nid) };//https://docs.microsoft.com/en-us/windows/win32/api/shellapi/ns-shellapi-notifyicondataa
		nid.hWnd = hWnd;
		// add the icon, setting the icon, tooltip, and callback message.
		// the icon will be identified with the GUID
		nid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;// | NIF_SHOWTIP | NIF_GUID;
		// nid.guidItem = __uuidof(PrinterIcon);
		 //nid.uID = IDI_NOTIFICATIONICON;  // ??
		nid.uID = IDI_main;  // ??
		nid.uCallbackMessage = WMAPP_notificationcallback; //https://docs.microsoft.com/en-us/windows/win32/api/shellapi/ns-shellapi-notifyicondataa#nif_showtip-0x00000080
		//LoadIconMetric(hInst, MAKEINTRESOURCE(IDI_NOTIFICATIONICON), LIM_SMALL, &nid.hIcon);
		LoadIconMetric(hInst, MAKEINTRESOURCE(IDI_main), LIM_SMALL, &nid.hIcon);
		LoadString(hInst, IDS_APP_TITLE, nid.szTip, ARRAYSIZE(nid.szTip));
		Shell_NotifyIcon(NIM_ADD, &nid);

		wcscpy_s(nid.szTip, L"custom program name" + NULL);// The text displayed when the mouse is placed on the tray icon
		// NOTIFYICON_VERSION_4 is prefered
		nid.uVersion = NOTIFYICON_VERSION_4;
		Shell_NotifyIcon(NIM_ADD, &nid);// Add Icon in tray area
		return Shell_NotifyIcon(NIM_SETVERSION, &nid);
		// return Shell_NotifyIcon(NIM_ADD, &nid);// Add Icon in tray area
	}


	LRESULT  MainController::DeleteNotificationIcon()
	{
		NOTIFYICONDATA nid = { sizeof(nid) };
		//nid.uFlags = NIF_GUID;
		nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
		//nid.guidItem = __uuidof(PrinterIcon);
		nid.uID = IDI_main;  // ??
		wcscpy_s(nid.szTip, L"custom program name");// The text displayed when the mouse is placed on the tray icon
		return Shell_NotifyIcon(NIM_DELETE, &nid);
	}

	//void MainController::ToTray(HWND hWnd)
	//Minimize to tray and restore of windows in C + + Win32:: https://developpaper.com/minimize-to-tray-and-restore-of-windows-in-c-win32/
	//{
	//    NOTIFYICONDATA nid;
	//    nid.cbSize = (DWORD)sizeof(nid);
	//    nid.hWnd = hWnd;
	//    nid.uID = IDR_MENU1;
	//    nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	//    nid.uCallbackMessage = WM_TO_TRAY;// Custom message handling tray icon events
	//    nid.hIcon = LoadIcon(GetModuleHandle(0), MAKEINTRESOURCE(IDI_SMALL));
	//    wcscpy_s(nid.szTip, L"custom program name");// The text displayed when the mouse is placed on the tray icon
	//    Shell_NotifyIcon(NIM_ADD, &nid);// Add Icon in tray area
	//
	//}

	//void MainController::DeleteTray(HWND hWnd)
	//{
	//    NOTIFYICONDATA nid;
	//    nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
	//    nid.hWnd = hWnd;
	//    nid.uID = IDR_MAINFRAME;
	//    nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	//    nid.uCallbackMessage = WM_ TO_ TRAY;// Custom message name handles tray icon events
	//    nid.hIcon = LoadIcon(GetModuleHandle(0), MAKEINTRESOURCE(IDI_SMALL));
	//    wcscpy_ s(nid.szTip, L"custom program name")// The text displayed when the mouse is placed on the tray icon
	//     Shell_ NotifyIcon(NIM_ DELETE, &nid);// Delete icon in tray
	//}

	LRESULT MainController::close()
	{
		WIN_LOG("Terminating application...");

		// close all child windows first
		EnumChildWindows(handle, WindowClose, (LPARAM)WM_CLOSE);

		DestroyWindow(handle);    // close itself
		return 0;
	}



	LRESULT MainController::destroy()
	{
		DeleteNotificationIcon();

		PostQuitMessage(0);       // exit the message loop

		WIN_LOG("Main window is destroyed");
		return 0;
	}



	LRESULT MainController::create()
	{

		//WIN_LOG("Main Window WM_CREATE PASSED " );

		return 0;
	}

	///////////////////////////////////////////////////////////////////////////////
   // handle WM_SIZE
   // the width and height are for the client area
   ///////////////////////////////////////////////////////////////////////////////
	LRESULT MainController::size(WPARAM wParam, LPARAM lParam)
	{
		if (handleDialog == NULL) return 0;
		
	   // reference to https://learn.microsoft.com/en-us/windows/win32/winmsg/using-windows
		//EnumChildWindows(handle, ChildPlusDialog, (LPARAM)&lparam);
		EnumChildWindows(handle, ChildPlusDialog, (LPARAM)&childWins);

		return FALSE;
	}


	INT_PTR CALLBACK About(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		UNREFERENCED_PARAMETER(lParam);
		switch (uMsg)
		{
		case WM_INITDIALOG:
			WIN_LOG("About Box Displayed");
			return (INT_PTR)TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
			{
				EndDialog(hwndDlg, LOWORD(wParam));
				WIN_LOG("About Box Closed");
				return (INT_PTR)TRUE;
			}
			break;
		}
		return (INT_PTR)FALSE;
	}



	BOOL CALLBACK ChildPlusDialog(HWND hwndChild, LPARAM lparam)
	{

		MainController::ChildWindow* childWins = (MainController::ChildWindow*)lparam;

		BOOL mached{ FALSE };
		int idChild = GetWindowLong(hwndChild, GWL_ID);
		for (auto i : childWins->List)
		{
			if (idChild == i) {
				mached = TRUE;
				break;
			}
		}

		// debug
		if (!mached) { // Exclude unitended callback:: only when dialog is child
			WIN_LOG("Child Window(" << idChild << " ) is Detected: DIALOG BOX WINDOW");
			return TRUE;
		}

		
		RECT rcParent{};
		HWND hwndParent = GetParent(hwndChild);
		GetClientRect(hwndParent, &rcParent);

		// Size and position the child window.  
		int xPos = 0;
		int yPos = 0;
		float width = (float)(rcParent.right - rcParent.left) / childWins->List.size();
		float height = (float)(rcParent.bottom - rcParent.top);
		xPos = width * idChild;
		yPos = 0;
		SetWindowPos(hwndChild, NULL, xPos, yPos, width, height, SWP_NOZORDER);
		// Make sure the child window is visible. 
		ShowWindow(hwndChild, SW_SHOW);


		return TRUE;
	}


	BOOL CALLBACK WindowClose(HWND hwndChild, LPARAM lParam)
	{

		if (lParam == WM_CLOSE)
		{
			int idChild = GetWindowLong(hwndChild, GWL_ID);
			WIN_LOG("Closing Child Window(" << (int)idChild << ")");
			SendMessage(hwndChild, WM_CLOSE, 0, 0);      // close child windows

		}

		return true;
	}

}
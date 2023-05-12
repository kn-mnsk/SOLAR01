///////////////////////////////////////////////////////////////////////////////
// Based on the Article of http://www.songho.ca/opengl/gl_mvc.html
// 
// ControllerForm.h, ControllerForm.cpp
// Derived Controller class for dialog window
// 
//////////////////////////////////////////////////////////////////////////////
// winDialogController.h
// 
// Window controller for dialog window
// 
// Modfier: M. Nakano (mnsk.kn@outlook.jp)
// Modified: Jan 2023
///////////////////////////////////////////////////////////////////////////////

#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#include <windows.h>
#include <windowsx.h>
#include <commdlg.h>

#include <source/winControls/baseWinController.h>
#include <source/sub/winDialogForm.h>

#include <mylibrary/tools.h>

namespace Win
{

	class DialogController : public baseWinController
	{
	public:

		DialogController();

		~DialogController();

		LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

		LRESULT command(UINT msg, WPARAM wParam, LPARAM lParam);
		LRESULT syscommand(UINT msg, WPARAM wParam, LPARAM lParam);
		LRESULT create();
		LRESULT close();
		LRESULT size(WPARAM wParam, LPARAM lParam);
		LRESULT destroy();
		LRESULT hScroll(WPARAM, LPARAM);
		LRESULT OnHscrollTB(HWND, WPARAM, LPARAM);
		LRESULT OnHscrollSB(WPARAM, LPARAM);
		LRESULT vScroll(WPARAM, LPARAM);
		LRESULT OnVscrollTB(HWND, WPARAM, LPARAM);
		LRESULT OnVscrollSB(WPARAM , LPARAM );
		LRESULT OnMouseWheelVscrollSB(int state, int delta, int x, int y);
		LRESULT notify(int id, LPARAM lParam);
				
		//void setGLController(shared_ptr<GlController> ctrl);
		void setGLControllers(std::vector<shared_ptr<GlController>>* ctrls);

		void OnMenuShowSimpleDialog();
		//https://devblogs.microsoft.com/oldnewthing/20050218-00/?p=36413
		void CreateFindDialogUIModally(HWND hwnd);
		void OnChar(HWND hwnd, TCHAR ch, int cRepeat);/* {
			switch (ch) {
			case ' ': CreateFindDialogUIModally(hwnd); break;
			}
		};*/
		void OnFindReplace(HWND hwnd, FINDREPLACE* pfr);/* {
			if (pfr->Flags & FR_DIALOGTERM) {
				EnableWindow(hwnd, TRUE);
				DestroyWindow(g_hwndFR);
				g_hwndFR = NULL;
			}
		};*/
		// end

	private:
		shared_ptr <DialogForm> dialogForm{ nullptr };  // pointer to view component
		std::vector<shared_ptr<GlController>>* glControllers{ nullptr };

		//https://devblogs.microsoft.com/oldnewthing/20050218-00/?p=36413
		HWND g_hwndFR;
		TCHAR g_szFind[80];
		FINDREPLACE g_fr = { sizeof(g_fr) };
		UINT g_uMsgFindMsgString;


		int m_xCurrentScroll{ 0 };
		int	m_yCurrentScroll{ 0 };
		// end

	};

	/// <summary>
	/// default contructor
	/// </summary>
	DialogController::DialogController()
	{
		dialogForm = make_shared <DialogForm>();
	}

	DialogController::~DialogController()	{}

	LRESULT DialogController::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		
		switch (uMsg)
		{
	
		case WM_INITDIALOG:
		{
			return create();
		}

		case WM_SIZE:
		{
			return  DefWindowProcW(handle, uMsg, wParam, lParam);
			//return  size(wParam, lParam);
		}
			
		case WM_COMMAND:
		{
			return command(uMsg, wParam, lParam);
		}
			
		case WM_TIMER:
		{
			return timer(LOWORD(wParam), HIWORD(wParam));
		}

		case WM_PAINT:
		{
			//[WM_PAINT message - Example](https://learn.microsoft.com/en-us/windows/win32/gdi/wm-paint#example)
			if ((HWND)lParam)
			{
				HWND hWndTB = (HWND)lParam;
				HDC  hDc = GetDC(hWndTB);
				RECT rect{};
				//LPARAM rect{};
				//SendMessage(hWnd, TBM_GETTHUMBRECT, 0, (LPARAM)&rect);
				SendMessage(hWndTB, TBM_GETTHUMBRECT, 0, (LPARAM)&rect);
				PAINTSTRUCT ps{};
				static HBRUSH hbrush = CreateSolidBrush(RGB(255, 0, 0));
				HDC hdc = BeginPaint(hWndTB, &ps);
				FillRect(hdc, &rect, hbrush);
				EndPaint(hWndTB, &ps);

				return FALSE;
			}
		
			return DefWindowProcW(handle, uMsg, wParam, lParam);
		}
			

		case WM_DESTROY:

			return destroy();

		case WM_CLOSE:
		{
			return close();
		}
		case WM_HSCROLL:
		{
			
			return hScroll(wParam, lParam);

		}
		case WM_VSCROLL:
		{
			
			return vScroll(wParam, lParam);
		}
			
		case WM_NOTIFY:

			return notify((int)wParam, lParam);                                      // controllerID, lParam

		case WM_MOUSEMOVE:

			return mouseMove(wParam, LOWORD(lParam), HIWORD(lParam));

		case WM_MOUSEWHEEL:
			// [WM_MOUSEWHEEL message] //https://learn.microsoft.com/en-us/windows/win32/inputdev/wm-mousewheel

			return OnMouseWheelVscrollSB(GET_KEYSTATE_WPARAM(wParam), GET_WHEEL_DELTA_WPARAM(wParam), GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));  // state, delta, x, y
		
		case WM_SYSCOMMAND:
			return DefWindowProcW(handle, uMsg, wParam, lParam);
		case WM_CTLCOLORBTN:                                                        // hdc, hWnd (MUST return handle to brush)
			return ctlColorBtn((HDC)wParam, (HWND)lParam);

		case WM_CTLCOLORDLG:                                                        // hdc, handle (MUST return handle to brush)
		{
			//HDC hdcStatic = (HDC)wParam;
			//SetTextColor(hdcStatic, RGB(255, 0, 0));
			//SetBkColor(hdcStatic, TRANSPARENT);

			///*if (hbrBkgnd == NULL)
			//{
			//	hbrBkgnd = CreateSolidBrush(RGB(0, 0, 0));
			//}*/
			//return (INT_PTR)CreateSolidBrush(RGB(0, 0, 0));
			return ctlColorDlg((HDC)wParam, (HWND)lParam);
		}

		case WM_CTLCOLOREDIT:                                                       // hdc, hWnd (MUST return handle to brush)
		{
			
			return ctlColorEdit((HDC)wParam, (HWND)lParam);
		}
			

		case WM_CTLCOLORLISTBOX:                                                    // hdc, hWnd (MUST return handle to brush)
		{
			// This the example of coding that paint  combobox background. In this program, done with property panel, "Transparent" set to True; 
			//// [How to change background color of combobox] (https://stackoverflow.com/questions/37728074/how-to-change-background-color-of-combobox)
			//HWND hComboBox = (HWND)lParam;
			//COMBOBOXINFO ci = { sizeof(COMBOBOXINFO) };
			//GetComboBoxInfo(hComboBox, &ci);
			//static HBRUSH hbrush;
			//hbrush = CreateSolidBrush(RGB(255, 255, 255));


			//if (
			//	hComboBox == ci.hwndItem ||
			//	hComboBox == ci.hwndList
			//	)
			//{
			//	HDC hdc = (HDC)wParam;
			//	SetTextColor(hdc, RGB(0, 0, 0));
			//	SetBkMode(hdc, TRANSPARENT);
			//	return (INT_PTR)hbrush;
			//}

			//return ctlColorListBox((HDC)wParam, (HWND)lParam);
			return DefWindowProcW(handle, uMsg, wParam, lParam);
		}
		case WM_PRINTCLIENT:
		{
		//	//[WM_PRINTCLIENT message](https://learn.microsoft.com/en-us/windows/win32/gdi/wm-printclient)
		//	//[WM_PAINT message - Example](https://learn.microsoft.com/en-us/windows/win32/gdi/wm-paint#example)
		// 
		// The following coding are related to TB backgrounfg painting , but not work!!
			//if (lParam == PRF_CLIENT)
		//	if (lParam == (PRF_ERASEBKGND&PRF_CLIENT))
		//	{

		//		//HWND hWnd = (HWND)lParam;
		//		HDC hdc = (HDC)wParam;

		//		RECT rect{};
		//		//LPARAM rect{};
		//		//SendMessage(hWnd, TBM_GETTHUMBRECT, 0, (LPARAM)&rect);
		//		SendMessage(handle, TBM_GETTHUMBRECT, 0, (LPARAM)&rect);

		////		PAINTSTRUCT ps{};
		////		ps.hdc = hdc;
		////		ps.fErase = 0;
		////		ps.rcPaint = rect;
		////	
		////		//hdc = BeginPaint(handle, &ps);
		//		static HBRUSH hbrush = CreateSolidBrush(RGB(255, 0, 0));
		////		// All painting occurs here, between BeginPaint and EndPaint.
		//		FillRect(hdc, &rect, hbrush);
		////		FillRect(hdc, &ps.rcPaint, hbrush);
		////		//EndPaint(handle, &ps);


		////		//SetTextColor(hdc, RGB(0, 0, 255));
		////		//SetBkMode(hdc, TRANSPARENT);
		////		//static HBRUSH hbrush;
		////		//hbrush = CreateSolidBrush(RGB(255, 255, 255));
		////		//return (INT_PTR)hbrush;
		//		return FALSE;

		//	}

			return FALSE;
		}
		
		case WM_CTLCOLORSCROLLBAR:                                                  // hdc, hWnd (MUST return handle to brush)

			return ctlColorScrollBar((HDC)wParam, (HWND)lParam);

		case WM_CTLCOLORSTATIC:                                                     // hdc, hWnd (MUST return handle to brush)
		{
			
			return ctlColorStatic((HDC)wParam, (HWND)lParam);
		}
			
			//case WM_SETCURSOR:
			//    ctrl->setCursor((HWND)wParam, LOWORD(lParam), HIWORD(lParam));          // handle, hit-test, getShaderID() of mouse event
			//    return true;

			//case WM_DPICHANGED:
			//    ctrl->dpiChanged(LOWORD(wParam), HIWORD(wParam), (RECT*)lParam);        // x-dpi, y-dpi, rect
			//    return true;

		//// https://www.keicode.com/windows/win02.php
		//	HANDLE_MSG(handle, WM_CHAR, OnChar);
		//	
	

		default:
			/*if (uMsg == g_uMsgFindMsgString && g_uMsgFindMsgString) {
				OnFindReplace(handle, (FINDREPLACE*)lParam);
			}*/
			return DefWindowProcW(handle, uMsg, wParam, lParam);
		}

		return FALSE;
	}


	///////////////////////////////////////////////////////////////////////////////
	// handle WM_DESTROY
	///////////////////////////////////////////////////////////////////////////////
	LRESULT DialogController::destroy()
	{
		WIN_LOG("Form dialog is destroyed");
		//Win::log("Form dialog is destroyed.");
		//return 0;
		return FALSE;
	}

	/// <summary>
	/// function for WM_CREATE
	/// </summary>
	/// <returns></returns>
	LRESULT DialogController::create()
	{
		// initialize all controls
		dialogForm->initControls(handle, glControllers);

		HWND hWndInsertAfter = NULL; // HWND_TOP;
		//HWND hWndInsertAfter = HWND_TOPMOST;
		UINT uFlags = SWP_SHOWWINDOW;// | SWP_NOZORDER; // SWP_NOSIZE;
		SetWindowPos(handle, hWndInsertAfter, 0, 0, 580, 950, uFlags);
		
		// initializing  for vertical and horinzontal scroll bars
		RECT rc = { 0 };
		GetClientRect(handle, &rc);
		SCROLLINFO si = { 0 };
		si.cbSize = sizeof(SCROLLINFO);
		si.fMask = SIF_ALL;
		si.nMin = 0;
		si.nMax = 1100;
		si.nPage = (rc.bottom - rc.top);
		si.nPos = 0;
		si.nTrackPos = 0;
		SetScrollInfo(handle, SB_VERT, &si, true);

		si.nMax = 600;
		si.nPage = (rc.right - rc.left);
		SetScrollInfo(handle, SB_HORZ, &si, true);

		return TRUE;
	}

	/// <returns></returns>
	LRESULT DialogController::close()
	{
		SendMessage(hWndParent, WM_CLOSE, 0, 0);  // main window closing
		destroy();
		return FALSE;
	}


	LRESULT DialogController::size(WPARAM wParam, LPARAM lParam)
	{
		HWND hWndInsertAfter = NULL; 
		UINT uFlags = SWP_SHOWWINDOW;// SWP_NOZORDER;
		//UINT uFlags = SWP_NOMOVE;
		SetWindowPos(handle, hWndInsertAfter, 0, 0, LOWORD(lParam), HIWORD(lParam), uFlags);
		//ShowWindow(handle, SW_SHOW);

		//return 0;
		return TRUE;
	}


	/// <summary>
	/// function for  WM_SYSCOMMAND
	/// </summary>
	/// <param name="msg"></param>
	/// <param name="wParam"></param>
	/// <param name="lParam"></param>
	/// <returns></returns>
	LRESULT DialogController::syscommand(UINT msg, WPARAM wParam, LPARAM lParam)
	{
		return 0;
	}

	/// <summary>
	///  function for  WM_COMMAND
	/// </summary>
	/// <param name="msg"></param>
	/// <param name="wParam"></param>
	/// <param name="lParam"></param>
	/// <returns></returns>
	LRESULT DialogController::command(UINT msg, WPARAM wParam, LPARAM lParam)
	{
		int id = LOWORD(wParam);
		int command = HIWORD(wParam);
		//int command = lParam;

		switch (id)
		{
		// Main WIndow Menu handle
		// https://learn.microsoft.com/en-us/previous-versions/visualstudio/visual-studio-6.0/aa232544(v=vs.60)?redirectedfrom=MSDN
		case ID_SHOWABOUT_MODE:
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), handle, About);
			return FALSE;
		case ID_APPQUIT_MODE:
		case IDM_EXIT:
			return close();
		case ID_TOGGLE_DIALOG:
			(IsWindowVisible(handle)) ? ShowWindow(handle, SW_MINIMIZE) : ShowWindow(handle, SW_RESTORE);
			return FALSE;
		//
		case IDC_BUTTON_RESET:
			if (command == BN_CLICKED)
			{
				dialogForm->resetControls();
			}
			break;
		case IDC_CHECK_CAM2:
			if (command == BN_CLICKED)
			{
				dialogForm->toggleCam2();
				dialogForm->resetControls();
			}
			break;

		case IDC_CHECK_GRID:
			if (command == BN_CLICKED)
			{
				dialogForm->toggleGrid();
			}
			break;

		case IDC_CHECK_MOTIONDATAUPDATE:
			if (command == BN_CLICKED)
			{
				dialogForm->toggleMotiondataUpdate();
			}
			break;

		case IDC_LABEL_SOLAR_DAYS: // update motion data  and camera info
			if (command == BN_CLICKED)
			{
				dialogForm->updateCtrlTargetStarDynamicInfo();
			}
			break;

		case IDC_COMBO_STAR_TARGET:
			switch (command)
			{
			case CBN_SELCHANGE:
			{
				HWND hwndCombo = (HWND)lParam;

				// Get selected index.
				int cbItem = dialogForm->getComboBox().getCurrentSelection();

				// Do update
				dialogForm->updateTargetSstarId(cbItem);
				dialogForm->resetControls();
				break;
			}

			}
		default:
			return  FALSE;
		}

		return TRUE;

	}



	///////////////////////////////////////////////////////////////////////////////
	// handle horizontal scroll notification
	///////////////////////////////////////////////////////////////////////////////
	LRESULT DialogController::hScroll(WPARAM wParam, LPARAM lParam)
	{
		// check if the message comming from trackbar
		if ((HWND)lParam)
		{
			// coming from trackbar
			return OnHscrollTB((HWND)lParam, wParam, lParam);
		}

		return OnHscrollSB(wParam, lParam);
	}

	LRESULT DialogController::OnHscrollTB(HWND trackbar, WPARAM wParam, LPARAM lParam)
	{
		
		int HiValue = HIWORD(wParam);              // current tick mark value
		int LoValue = LOWORD(wParam);
			switch (LoValue)
			{
			case TB_THUMBTRACK:     // user dragged the slider
			{
				dialogForm->updateTrackbars(trackbar, HiValue);
				break;
			}
			case TB_THUMBPOSITION:  // by WM_LBUTTONUP, or WM_MOUSEWHEEL
			{
				dialogForm->updateTrackbars(trackbar, HiValue);
				break;
			}
			case TB_LINEUP:         // by VK_RIGHT, VK_DOWN
			{
				break;
			}
			case TB_LINEDOWN:       // by VK_LEFT, VK_UP
			{
				break;
			}
			case TB_TOP:            // by VK_HOME
			{
				break;
			}
			case TB_BOTTOM:         // by VK_END
			{
				break;
			}
			case TB_PAGEUP:         // by VK_PRIOR (User click the channel to prior.)
			{
				break;
			}
			case TB_PAGEDOWN:       // by VK_NEXT (User click the channel to next.)
			{
				break;
			}
			case TB_ENDTRACK:       // by WM_KEYUP (User release a key.)
			{
				HiValue = (int)::SendMessage(trackbar, TBM_GETPOS, 0, 0);
				dialogForm->updateTrackbars(trackbar, HiValue);
				break;
			}
			}

		return FALSE;
	}


	LRESULT DialogController::OnHscrollSB(WPARAM wParam, LPARAM lParam)
	{
		//[how to control scrollbar in vc++ win32 api](https://stackoverflow.com/questions/32094254/how-to-control-scrollbar-in-vc-win32-api)

			// [How to Scroll Text](https://learn.microsoft.com/en-us/windows/win32/controls/scroll-text-in-scroll-bars)

		int pos = -1;
		switch (LOWORD(wParam))
		{
		case SB_PAGEUP: // User clicked the scroll bar shaft above the scroll box.
			pos = m_xCurrentScroll - 30;
			break;

		case SB_PAGEDOWN: // User clicked the scroll bar shaft below the scroll box.
			pos = m_xCurrentScroll + 30;
			break;

		case SB_LINEUP: // User clicked the top arrow.
			pos = m_xCurrentScroll - 10;
			break;

		case SB_LINEDOWN: // User clicked the bottom arrow.
			pos = m_xCurrentScroll + 10;
			break;

		case SB_THUMBPOSITION: // User dragged the scroll box.
		case SB_THUMBTRACK: // User dragging the scroll box.
			pos = HIWORD(wParam);
			break;
			//default:
			//	pos = m_yCurrentScroll;
		}

		if (pos == -1) return FALSE;

		// Set the scroll bar.
		SCROLLINFO si = {};
		si.cbSize = sizeof(SCROLLINFO);
		//si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
		si.fMask = SIF_POS;
		si.nPos = pos;
		si.nTrackPos = 0;

		SetScrollInfo(handle, SB_HORZ, &si, TRUE);
		GetScrollInfo(handle, SB_HORZ, &si);

		// Scroll the window.
		pos = si.nPos;
		POINT pt{};
		pt.x = pos - m_xCurrentScroll;
		pt.y = 0;
		auto hdc = GetDC(handle);
		LPtoDP(hdc, &pt, 1);
		ReleaseDC(handle, hdc);
		//ScrollWindowEx(handle, 0, -pt.y, NULL, NULL, NULL, NULL, SW_INVALIDATE);
		ScrollWindow(handle, -pt.x, NULL, NULL, NULL);// , NULL, NULL, SW_INVALIDATE);
		m_xCurrentScroll = pos;
		UpdateWindow(handle);

		return FALSE;
	}

	LRESULT DialogController::vScroll(WPARAM wParam, LPARAM lParam)
	{ 
		// check if the message comming from trackbar
		if ((HWND)lParam)
		{
			// coming from trackbar
			return OnVscrollTB((HWND)lParam, wParam, lParam);
		}

		return OnVscrollSB(wParam, lParam);
	}

	LRESULT DialogController::OnVscrollTB(HWND trackbar, WPARAM wParam, LPARAM lParam)
	{
		int value = HIWORD(wParam);              // current tick mark value
	
			switch (LOWORD(wParam))
			{

			case TB_THUMBTRACK:     // user dragged the slider

				dialogForm->updateTrackbars(trackbar, value);
				break;

			case TB_THUMBPOSITION:  // by WM_LBUTTONUP
				break;

			case TB_LINEUP:         // by VK_RIGHT, VK_DOWN
				break;

			case TB_LINEDOWN:       // by VK_LEFT, VK_UP
				break;

			case TB_TOP:            // by VK_HOME
				break;

			case TB_BOTTOM:         // by VK_END
				break;

			case TB_PAGEUP:         // by VK_PRIOR (User click the channel to prior.)
				break;

			case TB_PAGEDOWN:       // by VK_NEXT (User click the channel to next.)
				break;

			case TB_ENDTRACK:       // by WM_KEYUP (User release a key.)
				value = (int)::SendMessage(trackbar, TBM_GETPOS, 0, 0);
				dialogForm->updateTrackbars(trackbar, value);
				break;
			}
		


		return FALSE;
	}

	LRESULT DialogController::OnVscrollSB(WPARAM wParam, LPARAM lParam)
	{
		//[how to control scrollbar in vc++ win32 api](https://stackoverflow.com/questions/32094254/how-to-control-scrollbar-in-vc-win32-api)

		// [How to Scroll Text](https://learn.microsoft.com/en-us/windows/win32/controls/scroll-text-in-scroll-bars)

		int pos = -1;
		switch (LOWORD(wParam))
		{
		case SB_PAGEUP: // User clicked the scroll bar shaft above the scroll box.
			pos = m_yCurrentScroll - 50;
			break;

		case SB_PAGEDOWN: // User clicked the scroll bar shaft below the scroll box.
			pos = m_yCurrentScroll + 50;
			break;

		case SB_LINEUP: // User clicked the top arrow.
			pos = m_yCurrentScroll - 10;
			break;

		case SB_LINEDOWN: // User clicked the bottom arrow.
			pos = m_yCurrentScroll + 10;
			break;

		case SB_THUMBPOSITION: // User dragged the scroll box.
		case SB_THUMBTRACK: // User dragging the scroll box.
			pos = HIWORD(wParam);
			break;
		//default:
		//	pos = m_yCurrentScroll;
		}

		if (pos == -1) return FALSE;

		// Set the scroll bar.
		SCROLLINFO si = {};
		si.cbSize = sizeof(SCROLLINFO);
		//si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
		si.fMask = SIF_POS;
		si.nPos =pos;
		si.nTrackPos = 0;

		SetScrollInfo(handle, SB_VERT, &si, TRUE);
		GetScrollInfo(handle, SB_VERT, &si);

		// Scroll the window.
		pos = si.nPos;
		POINT pt{};
		pt.x = 0;
		pt.y =pos - m_yCurrentScroll;
		auto hdc = GetDC(handle);
		LPtoDP(hdc, &pt, 1);
		ReleaseDC(handle, hdc);
		//ScrollWindowEx(handle, 0, -pt.y, NULL, NULL, NULL, NULL, SW_INVALIDATE);
		ScrollWindow(handle, 0, -pt.y, NULL, NULL);// , NULL, NULL, SW_INVALIDATE);
		m_yCurrentScroll = pos;
		UpdateWindow(handle);

		return FALSE;
	}

	LRESULT DialogController::OnMouseWheelVscrollSB(int state, int delta, int x, int y)
	{
		int ZOOM_SCALE = 1;
		int mydelta = (delta / WHEEL_DELTA) * ZOOM_SCALE;
		DWORD myWparam{};
		myWparam = SETHIDWORD(myWparam, mydelta);

		if (delta < 0)// the wheel was rotated backward, toward the user.
		{
			//mywparam = LODWORD(mywparam, SB_THUMBPOSITION);
			myWparam = SETLODWORD(myWparam, SB_PAGEDOWN);
		}
		else
		{ //the wheel was rotated forward, away from the user;
			myWparam = SETLODWORD(myWparam, SB_PAGEUP);
		}

		return	SendMessage(handle, WM_VSCROLL, myWparam, 0);
	}


	///////////////////////////////////////////////////////////////////////////////
	// handle WM_NOTIFY
	// The id is not guaranteed to be unique, so use NMHDR.hwndFrom and NMHDR.idFrom.
	///////////////////////////////////////////////////////////////////////////////
	LRESULT DialogController::notify(int id, LPARAM lParam)
	{
		// first cast lParam to NMHDR* to know what the control is
		NMHDR* nmhdr = (NMHDR*)lParam;
		HWND hwndFrom = nmhdr->hwndFrom;
		 UINT idFrom = nmhdr->idFrom;
		UINT code = nmhdr->code;

		switch (code)
		{
			/*
			// SysLink notifications ==========
			case NM_CLICK:
			case NM_RETURN:
				{
				// cast again lParam to NMLINK* {NMHDR, LITEM}
				NMLINK* nmlink = (NMLINK*)lParam;
				LITEM litem = nmlink->item; // SysLink item {maks, iLink, state, stateMask, szID, szUrl}
				view->handleLink(from, litem.iLink);
				}
				break;
			*/

			// UpDownBox notifications =========
		case UDN_DELTAPOS:         // the change of position has begun
		{
			// cast again lParam to NMUPDOWN*
			NMUPDOWN* nmUpDown = (NMUPDOWN*)lParam;
			dialogForm->changeUpDownPosition(hwndFrom, nmUpDown->iPos + nmUpDown->iDelta);
			//glController->paint();

			return FALSE;
		}
		//case NM_CUSTOMDRAW:
		//{// for future coding
		//	//[How to Process Trackbar Notification Messages](https://learn.microsoft.com/en-us/windows/win32/controls/process-trackbar-notification-messages)

		//	// [win32 C++ Custom Color for Trackbar thumb - not much of help, though] (https://stackoverflow.com/questions/25679421/win32-c-custom-color-for-trackbar-thumb)

		//	// [Using Custom Draw - not trackbar, e.g. list view] (https://learn.microsoft.com/en-us/windows/win32/controls/using-custom-draw)
		//	
		//	// cast again lParam to LPNMCUSTOMDRAW
		//	// [NMCUSTOMDRAW structure (commctrl.h)](https://learn.microsoft.com/en-us/windows/win32/api/commctrl/ns-commctrl-nmcustomdraw)
		//	

		//	LPNMCUSTOMDRAW nmTrackBar = (LPNMCUSTOMDRAW)lParam;
		//	UINT idc = nmTrackBar->hdr.idFrom;
		//	/*if (idc == IDC_SLIDER_POSX)
		//	{

		//		return FALSE;
		//	}*/
		//	switch (nmTrackBar->dwDrawStage) 
		//	{
		//	case CDDS_PREPAINT:
		//	{

		//		return CDRF_NOTIFYITEMDRAW;
		//	}
		//	case CDDS_ITEMPREPAINT:
		//	{
		//		/*SelectObject(nmTrackBar->hdc,
		//			GetFontForItem(nmTrackBar->dwItemSpec,nmTrackBar->lItemlParam));
		//		nmTrackBar->clrext = GetColorForItem(nmTrackBar->dwItemSpec,
		//			nmTrackBar->lItemlParam);
		//		nmTrackBar->clrTextBk = GetBkColorForItem(nmTrackBar->dwItemSpec,
		//			nmTrackBar->lItemlParam);*/

		//		/* At this point, you can change the background colors for the item
		//		and any subitems and return CDRF_NEWFONT. If the list-view control
		//		is in report mode, you can simply return CDRF_NOTIFYSUBITEMDRAW
		//		to customize the item's subitems individually */

		//		return CDRF_NEWFONT;
		//	}
		//		/*HDC hdc = nmTrackBar->hdc;
		//		SetTextColor(hdc, RGB(255, 0, 0));
		//		SetBkColor(hdc, TRANSPARENT);
		//
		//		return (INT_PTR)CreateSolidBrush(RGB(0, 0, 0));*/
		//
		//		//return FALSE;
		//	}
		//}

		}

		return FALSE;
	}


	/// <summary>
	/// remember GL controllers
	/// </summary>
	/// <param name="ctrl"></param>
	//void DialogController::setGLController(shared_ptr<GlController> ctrl)
	//{
	//	glController = ctrl;
	//	//glController->paint();
	//}
	void DialogController::setGLControllers(std::vector<shared_ptr<GlController>>* ctrls)
	{
		glControllers = ctrls;
	}

	// test https://devblogs.microsoft.com/oldnewthing/20050218-00/?p=36413
	void DialogController::CreateFindDialogUIModally(HWND hwnd)
	{
		if (!g_hwndFR) {
			g_uMsgFindMsgString = RegisterWindowMessage(FINDMSGSTRING);
			if (g_uMsgFindMsgString) {
				g_fr.hwndOwner = hwnd;
				//g_fr.hInstance = g_hinst;
				g_fr.hInstance = hInst;
				g_fr.lpstrFindWhat = g_szFind;
				g_fr.wFindWhatLen = 80;
				g_hwndFR = FindText(&g_fr);
				/* // To MOrdal
				if (g_hwndFR) { //
					EnableWindow(hwnd, FALSE);
				}*/
			}
		}
	}
		void DialogController::OnChar(HWND hwnd, TCHAR ch, int cRepeat)
		{
			switch (ch) {
			case ' ': CreateFindDialogUIModally(hwnd); break;
			}
		}

		void DialogController::OnFindReplace(HWND hwnd, FINDREPLACE* pfr)
		{
			if (pfr->Flags & FR_DIALOGTERM) {
				//EnableWindow(hwnd, TRUE); // to Mordal
				DestroyWindow(g_hwndFR);
				g_hwndFR = NULL;
			}
		}
		//test end


}

//////////////////////////////////////////////////////////////////////////////
//Reference to https://github.com/microsoft/Windows-classic-samples/tree/main/Samples/Win7Samples/begin/LearnWin32/BaseWindow
// 
// baseWindow.h
// 
// Modfier: M. Nakano (mnsk.kn@outlook.jp)
// Modified: Jan 2023
////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <windows.h>
#include <mylibrary/tools.h>


namespace Win
{
	/// <summary>
	/// 
	/// 
	/// Base class for Window Creation
	/// 
	/// </summary>
	/// <typeparam name="DERIVED_TYPE">Window Class Name</typeparam>
	/// <typeparam name="CONTROLLER_TYPE">Controller Class for Window</typeparam>
	/// <typeparam name="NUMCHILDWINS">Number of Child Windows</typeparam>
	template <class DERIVED_TYPE, class CONTROLLER_TYPE, UINT NUMCHILDWINS>
	class baseWindow
	{
	public:
		static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
		{
			DERIVED_TYPE* pThis = NULL;

			if (uMsg == WM_NCCREATE)
			{
				CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
				pThis = (DERIVED_TYPE*)pCreate->lpCreateParams;
				SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);

				pThis->ctrl->setHandle(hwnd);
			}
			else
			{
				pThis = (DERIVED_TYPE*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

			}
			if (pThis)
			{

				pThis->ctrl->setHinstance(pThis->m_hinst);
				pThis->ctrl->setHandleParent(pThis->m_hwndparent);
				pThis->ctrl->setHmenu(pThis->m_hmenu); // only after window creattion
				pThis->ctrl->setNumChildWins(pThis->m_numchildwins);
				return pThis->ctrl->HandleMessage(uMsg, wParam, lParam);
			}
			else
			{
				return DefWindowProc(hwnd, uMsg, wParam, lParam);
			}
		};

		baseWindow() : m_hwndparent(0), m_hwnd(0) 
		{
			ctrl = make_shared< CONTROLLER_TYPE>();
			m_hinst = GetModuleHandle(NULL);
			m_numchildwins = NUMCHILDWINS;
		};

		BOOL Create(
			std::string   windowName,
			UINT hIcon,
			LPWSTR hCursor,
			UINT hIcomSm,
			UINT menuName,
			DWORD dwStyle,
			DWORD dwExStyle = 0,
			UINT classStyle = CS_HREDRAW | CS_VREDRAW, // as default
			int x = CW_USEDEFAULT,
			int y = CW_USEDEFAULT,
			int nWidth = CW_USEDEFAULT,
			int nHeight = CW_USEDEFAULT,
			//std::string   classname = "",
			HWND hWndParent = 0,
			HMENU hMenu = 0)
		{

			LPCWSTR lpClassName{};
			LPCWSTR lpWindowName{};
			std::wstring wstr{};
			if (windowName == "")
			{
				lpWindowName = ClassName();
				lpClassName = ClassName();
			}
			else
			{
				std::string& str = windowName;
				wstr = std::wstring(str.begin(), str.end());
				lpWindowName = wstr.c_str();
				lpClassName = wstr.c_str();
			}

			//Window Class and Create
			WNDCLASSEXW wcex = {};
			wcex.cbSize = sizeof(WNDCLASSEXW);
			wcex.style = classStyle; 
			wcex.lpfnWndProc = DERIVED_TYPE::WindowProc;
			wcex.cbClsExtra = 0;
			wcex.cbWndExtra = 0;
			wcex.hInstance = m_hinst;// GetModuleHandle(NULL);
			wcex.hIcon = (HICON)::LoadImageW(m_hinst, MAKEINTRESOURCE(hIcon), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE);
			wcex.hCursor = (HCURSOR)::LoadImageW(m_hinst, MAKEINTRESOURCE(hCursor), IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE);
			wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1 + (int)hMenu);
			//wcex.lpszClassName = ClassName();
			wcex.lpszClassName = lpClassName;
			wcex.hIconSm = (HICON)::LoadImageW(m_hinst, MAKEINTRESOURCE(hIcomSm), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE);
			wcex.lpszMenuName = MAKEINTRESOURCE(menuName);

			ATOM a = RegisterClassExW(&wcex);
			ERROR_EXIT("RegisterClassExW(&wcex): " << windowName);

			m_hwndparent = hWndParent;
			m_hmenu = hMenu; // Applicable for child window only

			m_hwnd = CreateWindowEx(
				dwExStyle,
				//ClassName(),
				lpClassName,
				lpWindowName,
				dwStyle,
				x,
				y,
				nWidth,
				nHeight,
				m_hwndparent,
				m_hmenu,
				m_hinst,
				this
			);
			//ERROR_EXIT(" CreateWindowEx");

			return (m_hwnd ? TRUE : FALSE);
		};

		HWND Window() const { return m_hwnd; };
		HWND WindowParent() const { return m_hwndparent; };
		HINSTANCE Hinst() const { return m_hinst; }
		shared_ptr<CONTROLLER_TYPE> ctrl{ nullptr };

		UINT m_numchildwins; // Number of child windos to be created, effective foe main window

	protected:

		virtual LPCWSTR  ClassName() const = 0;

		HWND m_hwnd;
		HWND m_hwndparent;
		HINSTANCE m_hinst;
		HMENU m_hmenu;
		
	};

		/// <summary>
		///  Base Class for Dialog Window
		/// </summary>
		/// <typeparam name="DERIVED_TYPE"></typeparam>
		/// <typeparam name="CONTROLLER_TYPE"></typeparam>
	template <class DERIVED_TYPE, class CONTROLLER_TYPE>
	class baseDialogWindow
	{
	public:
		static LRESULT CALLBACK DialogWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
		{
			DERIVED_TYPE* pThis = NULL;

			//WM_INITDIALOG message is called before displaying the dialog box.
			// lParam contains the value of dwInitParam of CreateDialogBoxParam(),
			// which is the pointer to the Controller.
			if (uMsg == WM_INITDIALOG)
			{
				// When WM_INITDIALOG is called, all controls in the dialog are created.
				// It is good time to initalize the appearance of controls here.
				// NOTE that we don't handle WM_CREATE message for dialogs because
				// the message is sent before controls have been create, so you cannot
				// access them in WM_CREATE message handler.

			   // retrieve the pointrer to the controller associated with the current dialog box.
				pThis = (DERIVED_TYPE*)lParam;
				//store the pointer to the Controller into GWL_USERDATA, so, other messege can be routed to the associated Controller.
				SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);

				pThis->m_hwnd = hwnd;
				pThis->ctrl->setHandle(hwnd);
				pThis->ctrl->setHandleParent(pThis->m_hwndparent);
				pThis->ctrl->setHinstance(pThis->m_hinst);
				//pThis->ctrl->create();
			}
			else
			{
				pThis = (DERIVED_TYPE*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
			}
			if (pThis)
			{
				return pThis->ctrl->HandleMessage(uMsg, wParam, lParam);
			}
			else
			{
				return DefWindowProc(hwnd, uMsg, wParam, lParam);
			}
		};

		baseDialogWindow() : m_hwnd(0)
		{
			ctrl = make_shared< CONTROLLER_TYPE>();
			m_hinst = GetModuleHandle(NULL);
		};

		// Using dialog box https://learn.microsoft.com/en-us/windows/win32/dlgbox/using-dialog-boxes

		// Custom Dialog box https://learn.microsoft.com/en-us/windows/win32/dlgbox/dlgbox-programming-considerations#custom-dialog-boxes
		BOOL Create(
			WORD formid,
			HWND  hWndParent)
		{
			m_hwndparent = hWndParent;

			{
				WNDCLASSEXW wcex2 = {};
				wcex2.cbSize = sizeof(WNDCLASSEXW);
				wcex2.style = CS_HREDRAW | CS_VREDRAW;
				wcex2.lpfnWndProc = DERIVED_TYPE::DialogWindowProc;
				wcex2.cbClsExtra = 0;
				wcex2.cbWndExtra = DLGWINDOWEXTRA; 
				wcex2.hInstance = m_hinst;// GetModuleHandle(NULL);
				wcex2.hIcon = (HICON)::LoadImageW(m_hinst, MAKEINTRESOURCE(IDI_main), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE);
				wcex2.hCursor = (HCURSOR)::LoadImageW(m_hinst, MAKEINTRESOURCE(IDC_ARROW), IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE);

				//wcex2.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
				wcex2.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
				//wcex2.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);

				wcex2.lpszClassName = DialogWindowName(); // also need to be resgistered in class name of dialog form control properties panel
				wcex2.hIconSm = (HICON)::LoadImageW(m_hinst, MAKEINTRESOURCE(IDI_SMALL), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE);
				wcex2.lpszMenuName = MAKEINTRESOURCE(IDC_main);

				ATOM a = RegisterClassExW(&wcex2);
				//ATOM a = RegisterClassExA(&wcex2);
				ERROR_EXIT("RegisterClassExW(&wcex): " << DialogWindowName());

				m_hwnd = CreateDialogParamW
				(
					m_hinst,
					MAKEINTRESOURCE(formid),
					hWndParent,
					DERIVED_TYPE::DialogWindowProc,
					(LPARAM)this
				);
			}


			return (m_hwnd ? TRUE : FALSE);
		};

		HWND Window() const { return m_hwnd; };
		HWND WindowParent() const { return m_hwndparent; };
		HINSTANCE Hinst() const { return m_hinst; };
		shared_ptr<CONTROLLER_TYPE> ctrl{ nullptr };

	protected:
		virtual LPCWSTR  DialogWindowName() const = 0;
		//virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;

		HWND m_hwnd;
		HINSTANCE m_hinst;
		HWND m_hwndparent;

	};
}
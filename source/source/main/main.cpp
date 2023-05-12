// 
// Window OpenGL Sample - main window, child windows, dialog window and multi threads, one opengl context in one thread
//

//////////////////////////////////////////////////////////
// Reference to: ////
// Platform specifics: Windows
// https://www.khronos.org/opengl/wiki/Platform_specifics:_Windows
// 
// setting up and using modern OpenGL 4.5 core context on Windows 
// https://gist.github.com/mmozeiko/ed2ad27f75edf9c26053ce332a1f6647
// 
//setting up modern OpenGL 4.5 context for drawing to multiple windows 
// https://gist.github.com/mmozeiko/6825cb94d393cb4032d250b8e7cc9d14
// 
//Imge loading by Gdiplus
//  https://www.codeproject.com/questions/842393/can-loadimage-windows-function-be-used-to-load-oth
// https://peter.bloomfield.online/using-gdi-plus-in-cpp-win32-programming/
// https://docs.microsoft.com/en-us/windows/win32/api/gdiplusheaders/nl-gdiplusheaders-image
// 
//Opengl multithreading 
// https://www.codeproject.com/articles/15344/a-multithreaded-opengl-enabled-application#:~:text=Multithreading%20The%20skeleton%20application%20takes%20advantage%20of%20a,a%20separate%20thread%20to%20handle%20the%20OpenGL%20specifics.
//

//// refer to https://docs.microsoft.com/en-us/answers/questions/318648/again-how-to-activate-comctl32-version-6.html  to activate ComCtl32.dll in order to let dialog box open in DialogController, otherwise  error 1407 will happen:: do not know WHY
//#pragma comment(linker,"\"/manifestdependency:type='win32' \
// name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
// processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
//// For Common Control DLL Versions Numbers, refer to https://docs.microsoft.com/en-us/windows/win32/controls/common-control-versions#structure-sizes-for-different-common-control-versions

#include "main.h"
//#include<algorithm>
#include "framework.h"

//#include <windowsx.h>
#include <commctrl.h>                   // common controls
#include <sstream>
//#include <afxwin.h>

#include <source/winControls/baseWindow.h>
#include <source/winControls/winMainController.h>
#include <source/winControls/winDialogController.h>
#include <source/winOpenGL/winGlController.h>

#include <mylibrary/SolarSys.hpp>
#include <local_configuration/root_directory.h>
#define MAX_LOADSTRING 100

	// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

namespace Win {

class MainWindow : public Win::baseWindow< MainWindow, Win::MainController, 2>
	{
	public:
		LPCWSTR  ClassName() const { return L"Main Window Class"; }
	};

	class OpenGLWindow : public Win::baseWindow<OpenGLWindow, Win::GlController, 0>
	{
	public:
		LPCWSTR  ClassName() const { return L"OpenGL Window Class"; };
	};

	class DialogWindow : public Win::baseDialogWindow<DialogWindow, Win::DialogController>
	{
	public:
		LPCWSTR  DialogWindowName() const { return L"Dialog Window"; }
	};
}

// Forward declarations of functions included in this code module:
int mainMessageLoop(HACCEL hAccelTable = 0);


//////////////////////////////////////////////////////////////////////////////
// main function of a windows application
///////////////////////////////////////////////////////////////////////////////
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR   lpCmdLine, _In_  int  nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// set log mode: FILE, DIALOG or BOTH
	Win::logMode(Win::LOG_MODE_FILE); // LOG_MODE_FILE is default

	// refer to https://docs.microsoft.com/en-us/windows/win32/controls/window-controls about Windows Control
	// register slider(trackbar) from comctl32.dll brfore creating windows
	// init comctl32.dll before creating windows:: NOTE ::actually  no need to do here since activated with #pragma
	INITCOMMONCONTROLSEX commonCtrls{};


	BOOL returnV;
	// create main window
	Win::MainWindow mainWin{ };
	returnV = mainWin.Create("", IDI_main, IDC_ARROW, IDI_SMALL, IDC_main, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, 0, CS_HREDRAW | CS_VREDRAW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,  0, 0);
	if (!returnV)
	{
		WIN_LOG_ERROR("mainWindow Failed to be Created");
		Tools::ErrorExit(GET_FILE_NAME, GET_FUNCTION, GET_LINE, "mainWindow Failed to be Created");
	}

	int xPos{ 125 }, yPos{ 10 };
	float mainWinWidth{ 1750.0 }, mainWinHeight{ 1000.0 };
	RECT rcParent{ 0.0, 0.0, mainWinWidth, mainWinHeight }; // default sizel value;

	DWORD style = (DWORD)::GetWindowLongPtr(mainWin.Window(), GWL_STYLE);
	DWORD styleEx = (DWORD)::GetWindowLongPtr(mainWin.Window(), GWL_EXSTYLE);
	AdjustWindowRectEx(&rcParent, style, FALSE, styleEx);
	SetWindowPos(mainWin.Window(), 0, xPos, yPos, mainWinWidth, mainWinHeight, SWP_NOZORDER);
	ShowWindow(mainWin.Window(), nCmdShow);
	//UpdateWindow(mainWin.Window());
	WIN_LOG("Main Window Created: " << mainWinWidth << "X" << mainWinHeight << " at (" << xPos << ", " << yPos << ")");

	//HWND dlg = (HWND)DisplayMyMessage(hInstance, mainWin.Window(), "Main Window Created :");
	//HWND dlg = (HWND)CreateDialogTemplate(hInstance, mainWin.Window(), (DLGPROC)dialogWndProc);
	

	// create OpenGL rendering windows
	std::vector<Win::OpenGLWindow> glWins{};
	std::vector<shared_ptr<Win::GlController>> glControllers{};
	for (auto i : mainWin.ctrl->childWins.List)
	{
		glWins.push_back(Win::OpenGLWindow{});
		glControllers.push_back(glWins.back().ctrl);
	}

	std::ostringstream oss;
	int idChild{ 0 };
	for (auto& glwin : glWins)
	{
		oss.str(""); oss << "Child WIndow_" << idChild ;
		returnV = glwin.Create(oss.str(), IDI_main, IDC_ARROW, IDI_SMALL, NULL, WS_CHILD |WS_BORDER , 0, CS_OWNDC | CS_VREDRAW | CS_HREDRAW, 0, 0, 0, 0, mainWin.Window(), (HMENU)idChild);
		if (!returnV)
		{
			WIN_LOG_ERROR("glWindow Failed to be Created: idChild=" << idChild);
			ERROR_EXIT("glWindow Failed to be Created: idChild=" << idChild);
		}
		else
		{
			WIN_LOG("glWindow Created: idChild=" << idChild);
		}
		idChild += 1;
	}

	// create a child dialog box to control glRender, and show
	Win::DialogWindow dialogWin{ };
	//share glControllers with dialog window;
	dialogWin.ctrl->setGLControllers(&glControllers);  // must be here

	//returnV = dialogWin.Create(IDD_DIALOG_HORIZONTAL, mainWin.Window());
	returnV = dialogWin.Create(IDD_DIALOG_VERTICAL, mainWin.Window());
	if (!returnV)
	{
		WIN_LOG_ERROR("dialogWindow Failed to be Created");
	}
	else
	{
		WIN_LOG("Dialog Window Created");
	}

	// store dialog window handle in the follwoing windows
	mainWin.ctrl->setDialogHandle(dialogWin.Window());
	for (auto& glWin : glWins)
	{
		glWin.ctrl->setDialogHandle(dialogWin.Window());
	}


	// Use WM_SIZE of MainController to make first diaplay of glwins and dialogwin
	//RECT rcDialog{ 0.0, 0.0, mainWinWidth * 0.4, mainWinHeight }; // defalut size value
	//SendMessage(mainWin.Window(), WM_SIZE, (WPARAM)dialogWin.Window(), (LPARAM)&rcDialog);
	SendMessage(mainWin.Window(), WM_SIZE, 0, 0);

	// Accelerator Tables https://docs.microsoft.com/en-us/windows/win32/learnwin32/accelerator-tables
	HACCEL hAccelTable = LoadAcceleratorsW(hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR1));
	//HACCEL hAccelTable = 0;

	int exitCode = mainMessageLoop(hAccelTable);

	WIN_LOG("Application Terminated");
	return exitCode;

}

///////////////////////////////////////////////////////////////////////////////
// main message loop
///////////////////////////////////////////////////////////////////////////////
int mainMessageLoop(HACCEL hAccelTable)
{
	//HWND activeHandle;
	MSG msg;

	while (GetMessage(&msg, 0, 0, 0) > 0)  // loop until WM_QUIT(0) received
	{
		// determine the activated window is dialog box
		// skip if messages are for the dialog windows, except for accelerator key
		//activeHandle = GetActiveWindow();
		//if (GetWindowLongPtr(msg.hwnd, GWL_EXSTYLE) & WS_EX_CONTROLPARENT) // WS_EX_CONTROLPARENT is automatically added by CreateDialogBox()
		//{
		//	if ((IsDialogMessage(msg.hwnd, &msg))&&(!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))) // handle accelerator for dialog
		//		continue;   // message handled, back to while-loop
		//}

		//if ((IsDialogMessage(msg.hwnd, &msg)) && (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))) // handle accelerator for dialog
		//	continue;   // message handled, back to while-loop

		/*if (IsDialogMessage(msg.hwnd, &msg))
		{
			TranslateAccelerator(msg.hwnd, hAccelTable, &msg);
			continue;
		}*/
	
		// now, handle window messages
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

	}

	return (int)msg.wParam;                 // return nExitCode of PostQuitMessage()
}

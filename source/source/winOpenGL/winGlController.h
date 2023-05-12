///////////////////////////////////////////////////////////////////////////////
// Based on the Article of http://www.songho.ca/opengl/gl_mvc.html
// 
// ControllerGL.h, ControllerGL.cpp
// 
//////////////////////////////////////////////////////////////////////////////
//  winGlController.cpp
// 
// Window Conroller for openGL rendering window
// 
// Modfier: M. Nakano (mnsk.kn@outlook.jp)
// Modified: Jan 2023
///////////////////////////////////////////////////////////////////////////////

#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#include <windows.h>
#include <cstdlib>
#include <string>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <thread>
#include <chrono>
#include <atomic>

#include <source/winControls/baseWinController.h>
#include <source/winControls/winMouseController.h>
#include <source/winOpenGL/winGlContext.h>

#include <source/main/resource.h>
#include <source/sub/winGlRender.h>
#include <source/sub/winGlCamera.h>
#include <source/sub/wcharUtil.h>

namespace Win
{
	class GlController : public baseWinController
	{
	public:
		GlController();
		~GlController();

		LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

		LRESULT destroy();
		LRESULT close();                                // close the RC and OpenGL window
		LRESULT command(HWND, UINT, WPARAM, LPARAM);  // for WM_COMMAND
		LRESULT syscommand(HWND, UINT, WPARAM, LPARAM);  // for WM_SYSCOMMAND
		LRESULT create();                               // create RC for OpenGL window and start new thread for rendering
		LRESULT createThread();
		LRESULT paint();
		LRESULT size(WPARAM wParam, LPARAM lParam);

		LRESULT contextMenu(HWND, int, int);
		BOOL WINAPI OnContextMenu(HWND, int, int);
		//VOID APIENTRY DisplayContextMenu(HWND, POINT);
		//BOOL WINAPI OnPopupMenu(HWND , int , int );

		void setDialogHandle(HWND hWnd) { dialogHandle = hWnd; glRender->setDialogHandle(hWnd); };
		void setSolarSys(std::shared_ptr<SolarSys> solarsys) { glRender->setSolarSys(solarsys); }
		void setGlContext(shared_ptr<Win::GlContext> glcontext) { glContext = glcontext; };


		shared_ptr<GlRender> glRender{ nullptr };
		shared_ptr<GlContext> glContext{ nullptr };
		shared_ptr<Camera> camera{ nullptr };
		shared_ptr<MouseController> mouse{ nullptr };

	private:
		void runThread();                               // thread for OpenGL rendering

		HWND  dialogHandle{};
		std::thread glThread;                           // opengl rendering thread object
		volatile bool loopFlag;                         // rendering loop flag
		bool mouseHovered;                              // mouse hover flag
		int idChild{0};
	};

	///////////////////////////////////////////////////////////////////////////////
// default contructor
///////////////////////////////////////////////////////////////////////////////
	GlController::GlController() : loopFlag(false), mouseHovered(false)
	{
		
	}

	GlController::~GlController() {
		if (glThread.joinable())
		{
			WIN_LOG("Thread to Close: Id=" << glThread.get_id() << "of Child Window(" << (int)idChild << ")");
			glThread.join();  // Should be done in ~GlController() otherwise cause error here

		}
	}

	///////////////////////////////////////////////////////////////////////////////
	// handle WM_DESTROY
	///////////////////////////////////////////////////////////////////////////////
	LRESULT GlController::destroy()
	{
		DeleteNotificationIcon();
		WIN_LOG("Child Window(" << (int)idChild << ") is destroyed");

		return 0;
	}


	///////////////////////////////////////////////////////////////////////////////
	// handle WM_CLOSE
	///////////////////////////////////////////////////////////////////////////////
	LRESULT GlController::close()
	{
		loopFlag = false;

		DestroyWindow(handle);

		return 0;
	}

	///////////////////////////////////////////////////////////////////////////////
	// handle WM_CREATE
	///////////////////////////////////////////////////////////////////////////////
	LRESULT GlController::create()
	{
		/*Note: The following coding need to be placed here due to timing, not in constructor*/
		idChild = (int)GetMenu(handle);

		camera = make_shared<Camera>(idChild);

		glRender = make_shared<GlRender>(idChild);
		glRender->setCam(camera);

		glContext = make_shared<GlContext>(idChild);

		mouse = make_shared<MouseController>(idChild);
		mouse->setCam(camera);
		/*   */

		{ // OpenGLL initialization
		//  crteate opngl context
		glContext->createContext(handle, 32, 24, 8); 

		// remembre dc and hglrc in glRender, too
		HDC hdc = glContext->getDC();
		HGLRC hglrc = glContext->getRC();
		glRender->setDC(hdc);
		glRender->setRC(hglrc);
		
		 // rendering initialization 
			glContext->makeGlContextCurrent(std::this_thread::get_id()); //test
			glContext->glLoader();
			glRender->init();
			glContext->makeGlContextUnCurrent(std::this_thread::get_id());
		}

		return 0;
	}

	LRESULT GlController::createThread()
	{
		// create a thread for OpenGL rendering
		loopFlag = true;
		glThread = std::thread(&GlController::runThread, this);

		return 0;

	}

	///////////////////////////////////////////////////////////////////////////////
	// rendering thread
	// start rendering loop
	///////////////////////////////////////////////////////////////////////////////
	void GlController::runThread()
	{

		WIN_LOG("Rendering thread  Successfully created: Id=" << glThread.get_id() << " Child Window(" << (int)idChild << ")");

		// set the current RC in this thread again
		glContext->makeGlContextCurrent(glThread.get_id()); //test


		// set to FALSE to disable vsync
	  // refer to https://www.khronos.org/opengl/wiki/Swap_Interval
		BOOL vsync = TRUE;
		//BOOL vsync = FALSE;
		wglSwapIntervalEXT(vsync ? 1 : 0);
		// rendering loop
		WIN_LOG("Rendering-Thread Loop to Start: Id=" << glThread.get_id() << " of Child Window(" << idChild << ")");
		while (loopFlag)
		{

			glRender->draw(idChild);

			glFlush();

			glRender->swapBuffers();

		}

		// close OpenGL Rendering Context (RC)
		glContext->closeContext(handle);

		WIN_LOG("Rendering thread  Loop To be Closed: Thread Id=" << glThread.get_id() << " at Child Window(" << idChild << ")");
	}


	///////////////////////////////////////////////////////////////////////////////
	// handle WM_PAINT
	///////////////////////////////////////////////////////////////////////////////
	LRESULT GlController::paint()
	{
		// do nothing
		return 0;
	}

	///////////////////////////////////////////////////////////////////////////////
	// handle WM_COMMAND
	///////////////////////////////////////////////////////////////////////////////
	LRESULT GlController::command(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{// not used

		return 0;
	}

	LRESULT GlController::syscommand(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		MessageBeep(MB_ICONWARNING);
		// wait for rendering thread is terminated


		return DefWindowProc(hWnd, msg, wParam, lParam);

	}

	//BOOL      GlController::OnPopupMenu(HWND hWnd, int x, int y)
	//{
	//	POINT point{ x, y };
	//
	//	HMENU hMenu = CreatePopupMenu();/*The CreatePopupMenu function creates a popup menu. It returns a handle to the newly created menu. The menu is initially empty.*/
	//
	//	ClientToScreen(hWnd, &point);/*The ClientToScreen function converts the client coordinates of a specified point to screen coordinates. We need these coordinates to display the context menu.*/
	//
	//	AppendMenuW(hMenu, MF_GRAYED, 0, L"\"Change Texture\"");
	//	AppendMenuW(hMenu, MF_STRING, ID_TEXTURE_SUN, L"&Sun");
	//	AppendMenuW(hMenu, MF_STRING, ID_TEXTURE_EARTH, L"&Earth");
	//	AppendMenuW(hMenu, MF_STRING, ID_TEXTURE_MOON, L"&Moon");
	//	AppendMenuW(hMenu, MF_STRING, ID_TEXTURE_MARS, L"&Mars");
	//	AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);
	//	AppendMenuW(hMenu, MF_STRING, NULL, L"&Exit"); // no selection
	//
	//	TrackPopupMenuEx(hMenu, TPM_RIGHTBUTTON, point.x, point.y, hWnd, NULL);/*The function displays a context menu at the specified location and tracks the selection of items on the menu.*/
	//
	//	DestroyMenu(hMenu);/*In the end, the menu object is destroyed with the DestroyMenu function. A menu that was not assigned to a window must be explicitly destroyed.*/
	//
	//	return 0;
	//}

	/*Not used*/
	LRESULT GlController::contextMenu(HWND hWnd, int x, int y)
	{
		return OnContextMenu(hWnd, x, y);
	}

	BOOL WINAPI GlController::OnContextMenu(HWND hWnd, int x, int y)
	{
		RECT rc;                    // client area of window 
		POINT pt = { x, y };        // location of mouse click 

		// Get the bounding rectangle of the client area. 
		GetClientRect(hWnd, &rc);

		// Convert the mouse position to client coordinates. 
		ScreenToClient(hWnd, &pt);

		// If the position is in the client area, display a  
		// shortcut menu. 
		if (PtInRect(&rc, pt))
		{
			//DisplayContextMenu(hWnd, pt);
			return TRUE;
		}

		// Return FALSE if no menu is displayed. 

		return FALSE;
	}

	//VOID APIENTRY GlController::DisplayContextMenu(HWND hWnd, POINT pt)
	//{
	//
	//	HMENU hMenu;            // top-level menu 
	//	HMENU hmenuTrackPopup;  // shortcut menu 
	//
	//	// Load the menu resource. 
	//	HINSTANCE hInst = (HINSTANCE)GetWindowLongW(hWnd, GWLP_HINSTANCE);
	//	//HINSTANCE hInst = GetWindowLongW(hWnd, GWLP_HINSTANCE);
	//	//HINSTANCE hInst = std::static_cast<HINSTANCE>(GetWindowLongW(hWnd, GWLP_HINSTANCE));
	//	if ((hMenu = LoadMenuW(hInst, MAKEINTRESOURCE(IDR_MENU2))) == NULL)
	//		return;
	//
	//	// TrackPopupMenu cannot display the menu bar so get  a handle to the first shortcut menu. 
	//	hmenuTrackPopup = GetSubMenu(hMenu, 0);
	//
	//	// Display the shortcut menu. Track the right mouse  button. 
	//	ClientToScreen(hWnd, &pt);
	//	//TrackPopupMenuEx(hmenuTrackPopup, TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, hWnd, NULL);
	//	BOOL menuItemId = TrackPopupMenuEx(hmenuTrackPopup, TPM_RETURNCMD | TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, hWnd, NULL);
	//
	//	//Win::log("Context Menu Id: %d", menuItemId);
	//	switch (menuItemId)
	//	{
	//	case ID_TEXTURE_SUN:
	//		//glRender->setCam2ToPlanet(0);
	//		glRender->targetStarIdCAM2 = 0;
	//		break;
	//	case ID_TEXTURE_MERCURY:
	//		//glRender->setCam2ToPlanet(1);
	//		glRender->targetStarIdCAM2 = 1;
	//		break;
	//	case ID_TEXTURE_VENUS:
	//		//glRender->setCam2ToPlanet(2);
	//		glRender->targetStarIdCAM2 = 2;
	//		break;
	//	case ID_TEXTURE_EARTH:
	//		//glRender->setCam2ToPlanet(3);
	//		glRender->targetStarIdCAM2 = 3;
	//		break;
	//	case ID_TEXTURE_MOON:
	//		//glRender->setCam2ToPlanet(4);
	//		glRender->targetStarIdCAM2 = 4;
	//		break;
	//	case ID_TEXTURE_MARS:
	//		//glRender->setCam2ToPlanet(5);
	//		glRender->targetStarIdCAM2 = 5;
	//		break;
	//	case ID_TEXTURE_PHOBOS:
	//		//glRender->setCam2ToPlanet(6);
	//		glRender->targetStarIdCAM2 = 6;
	//		break;
	//	case ID_TEXTURE_DEIMOS:
	//		//glRender->setCam2ToPlanet(7);
	//		glRender->targetStarIdCAM2 = 7;
	//		break;
	//	case ID_TEXTURE_CERES:
	//		//glRender->setCam2ToPlanet(8);
	//		glRender->targetStarIdCAM2 = 8;
	//		break;
	//	case ID_TEXTURE_JUPITER:
	//		//glRender->setCam2ToPlanet(9);
	//		glRender->targetStarIdCAM2 = 9;
	//		break;
	//	case ID_TEXTURE_SATURN:
	//		//glRender->setCam2ToPlanet(10);
	//		glRender->targetStarIdCAM2 = 10;
	//		break;
	//	case ID_TEXTURE_URANUS:
	//		//glRender->setCam2ToPlanet(11);
	//		glRender->targetStarIdCAM2 = 11;
	//		break;
	//	case ID_TEXTURE_NEPTUNE:
	//		//glRender->setCam2ToPlanet(12);
	//		glRender->targetStarIdCAM2 = 12;
	//		break;
	//	case ID_TEXTURE_PLUTO:
	//		//glRender->setCam2ToPlanet(13);
	//		glRender->targetStarIdCAM2 = 13;
	//		break;
	//	}
	//
	//	// invoke "Reset Camera" on dialog window
	//	SendMessage(dialogHandle, WM_COMMAND, IDC_BUTTON_RESET, BN_CLICKED);
	//
	//
	//	// Destroy the menu. 
	//	DestroyMenu(hMenu);
	//}
	/*Not used*/



	///////////////////////////////////////////////////////////////////////////////
	// handle WM_SIZE
	///////////////////////////////////////////////////////////////////////////////
	LRESULT GlController::size(WPARAM wParam, LPARAM lParam)
	{
		int width = LOWORD(lParam);
		int height = HIWORD(lParam);
		glRender->resizeWindow(width, height); // width and height

		//paint();    // repaint glWindow
		return 0;
	}


	LRESULT GlController::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{

		// route messages to the associated controller
		switch (uMsg)
		{

		case WM_NCCALCSIZE:
		{
			return DefWindowProcW(handle, uMsg, wParam, lParam);
		}
		case WM_CREATE:
		{
			create();
			createThread();

			return 0;// DefWindowProcW(handle, uMsg, wParam, lParam);
		}
		case WM_SIZE:
		{

			if ((LOWORD(lParam) != 0) && (HIWORD(lParam) != 0))
			{
				return size(wParam, lParam);  // width, height, type
			}

			return DefWindowProcW(handle, uMsg, wParam, lParam);  // width, height, type
		}
		case WMAPP_NOTIFYCALLBACK:
		{
			//switch (LOWORD(lParam))
			//{
			//case NIN_SELECT:
			//	//if (lParam == WM_LBUTTONDBLCLK)

			//	SetForegroundWindow(hWnd);
			//	ShowWindow(hWnd, SW_SHOWNORMAL);
			//	returnValue = DefWindowProcW(hWnd, message, wParam, lParam);
			//	break;
			//default:returnValue = DefWindowProcW(hWnd, message, wParam, lParam);
			//}
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
			return DefWindowProcW(handle, uMsg, wParam, lParam);
		}
		case WM_CLOSE:
		{
			return  close();
		}
		case WM_DESTROY:
		{

			return destroy();
		}
		case WM_MOVE:
		{

			return DefWindowProcW(handle, uMsg, wParam, lParam);
		}

		case WM_SYSCOMMAND:
		{
			//returnValue =  syscommand(m_hwnd, uMsg, wParam, lParam);
			return DefWindowProcW(handle, uMsg, wParam, lParam);
		}
		case WM_CHAR:
		{
			//returnValue =  char(wParam, lParam);   // route keycode
			return DefWindowProcW(handle, uMsg, wParam, lParam);
		}
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
		{
			return keyDown((int)wParam, lParam);                       // keyCode, keyDetail

		}
		case WM_KEYUP:
		case WM_SYSKEYUP:
		{
			return keyUp((int)wParam, lParam);                         // keyCode, keyDetail
		}

		case WM_LBUTTONDOWN:
		{
			return mouse->lButtonDown(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)); // state, x, y

		}
		case WM_LBUTTONUP:
		{
			return mouse->lButtonUp(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)); // state, x, y

		}
		case WM_RBUTTONDOWN:
		{
			
			return mouse->rButtonDown(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)); // state, x, y

		}
		case WM_RBUTTONUP:
		{
			return mouse->rButtonUp(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)); // state, x, y

		}
		case WM_MBUTTONDOWN:
		{
			return mouse->mButtonDown(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)); // state, x, y

		}
		case WM_MBUTTONUP:
		{
			return mouse->mButtonUp(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)); // state, x, y
			//break;
		}
		case WM_MOUSEHOVER:
		{
			return mouse->mouseHover(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)); // state, x, y

		}
		case WM_MOUSELEAVE:
		{
			return mouse->mouseLeave();

		}
		case WM_MOUSEMOVE:
		{
			mouse->setHandle(dialogHandle);
			return mouse->mouseMove(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)); // state, x, y

		}
		case WM_MOUSEWHEEL:
		{
			mouse->setHandle(dialogHandle);
			return mouse->mouseWheel(GET_KEYSTATE_WPARAM(wParam), GET_WHEEL_DELTA_WPARAM(wParam), GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));  // state, delta, x, y

		}
		case WM_HSCROLL:
		{
			return hScroll(wParam, lParam);
		}
		case WM_VSCROLL:
		{
			return vScroll(wParam, lParam);
		}
		case WM_TIMER:
		{
			return timer(LOWORD(wParam), HIWORD(wParam));

		}
		case WM_NOTIFY:
		{
			return notify((int)wParam, lParam);                        // controllerID, lParam

		}
		case WM_CONTEXTMENU:
		{
			
			return DefWindowProcW(handle, uMsg, wParam, lParam);
		}
		case WM_SETCURSOR:
		{
			return setCursor((HWND)wParam, LOWORD(lParam), HIWORD(lParam));     // handle, hit-test, getShaderID() of mouse event

		}
		//case WM_DPICHANGED:
		//    return dpiChanged(LOWORD(wParam), HIWORD(wParam), (RECT*)lParam);  // x-dpi, y-dpi, rect

		//case WM_ERASEBKGND:
		//    return eraseBkgnd((HDC)wParam);                            // handle to device context
		//    break;
		default:
			return DefWindowProcW(handle, uMsg, wParam, lParam);

		}

		return 0;
	}
}

//////////////////////////////////////////////////////////////////////////////
// Based on the Article of http://www.songho.ca/opengl/gl_mvc.html
// 
//  Controller.h, Controller.cpp
//
//////////////////////////////////////////////////////////////////////////////
//  baseWinController.h
// Modfier: M. Nakano (mnsk.kn@outlook.jp)
// Modified: Jan 2023
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <windows.h>

UINT const  WMAPP_NOTIFYCALLBACK = WM_APP + 1;
UINT const  WMAPP_HIDEFLYOUT = WM_APP + 2;


namespace Win
{
    class baseWinController
    {
    
    public:
        // ctor/dtor
        baseWinController();
        ~baseWinController ();

        virtual void setHinstance(HINSTANCE hInstance);
        virtual HINSTANCE getHinstance();
        virtual void setHandle(HWND handle);                            // set window handle
        virtual HWND getHandle();                                       // get window handle
        virtual void setHandleParent(HWND handle);                            // set window handle
        virtual HWND getHandleParent();
        virtual  void setHmenu(HMENU hmenu);
        virtual HMENU getHmenu();
        virtual void setNumChildWins(int NUMCHILDWINS);
        virtual int getNumChildWins();

        virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam); 
        virtual LRESULT close();                                    // for WM_CLOSE
        virtual LRESULT command(int, int, LPARAM);       // for WM_COMMAND: id, cmd, msg
        virtual LRESULT command(HWND, UINT, WPARAM, LPARAM);       // for WM_COMMAND: id, cmd, msg
        virtual LRESULT syscommand(HWND, UINT, WPARAM, LPARAM);       // for WM_SYSCOMMAND: id, cmd, msg
        virtual LRESULT AddNotificationIcon(HWND);       // for WM_SYSCOMMAND: id, cmd, msg
        virtual LRESULT  DeleteNotificationIcon();

        virtual LRESULT contextMenu(HWND handle, int x, int y);     // for WM_CONTEXTMENU: handle, x, y
        virtual LRESULT create();                                   // for WM_CREATE
        virtual LRESULT ctlColorBtn(HDC hdc, HWND handle);          // for WM_CTLCOLORBTN
        virtual LRESULT ctlColorDlg(HDC hdc, HWND handle);          // for WM_CTLCOLORDLG
        virtual LRESULT ctlColorEdit(HDC hdc, HWND handle);         // for WM_CTLCOLOREDIT
        virtual LRESULT ctlColorListBox(HDC hdc, HWND handle);      // for WM_CTLCOLORLISTBOX
        virtual LRESULT ctlColorScrollBar(HDC hdc, HWND handle);    // for WM_CTLCOLORSCROLLBAR
        virtual LRESULT ctlColorStatic(HDC hdc, HWND handle);       // for WM_CTLCOLORSTATIC
        virtual LRESULT destroy();                                  // for WM_DESTROY
        virtual LRESULT dpiChanged(int x, int y, RECT* rect);       // for WM_DPICHANGED: xDpi, yDpi, rect
        virtual LRESULT enable(bool flag);                          // for WM_ENABLE: flag(true/false)
        virtual LRESULT eraseBkgnd(HDC hdc);                        // for WM_ERASEBKGND: HDC
        virtual LRESULT hScroll(WPARAM wParam, LPARAM lParam);      // for WM_HSCROLL
        virtual LRESULT keyDown(int key, LPARAM lParam);            // for WM_KEYDOWN: keyCode, detailInfo
        virtual LRESULT keyUp(int key, LPARAM lParam);              // for WM_KEYUP: keyCode, detailInfo
        virtual LRESULT lButtonDown(WPARAM state, int x, int y);    // for WM_LBUTTONDOWN: state, x, y
        virtual LRESULT lButtonUp(WPARAM state, int x, int y);      // for WM_LBUTTONUP: state, x, y
        virtual LRESULT mButtonDown(WPARAM state, int x, int y);    // for WM_MBUTTONDOWN: state, x, y
        virtual LRESULT mButtonUp(WPARAM state, int x, int y);      // for WM_MBUTTONUP: state, x, y
        virtual LRESULT mouseHover(int state, int x, int y);        // for WM_MOUSEHOVER: state, x, y
        virtual LRESULT mouseLeave();                               // for WM_MOUSELEAVE
        virtual LRESULT mouseMove(WPARAM state, int x, int y);      // for WM_MOUSEMOVE: state, x, y
        virtual LRESULT mouseWheel(int state, int d, int x, int y); // for WM_MOUSEWHEEL: state, delta, x, y
        virtual LRESULT notify(int id, LPARAM lParam);              // for WM_NOTIFY: controllerID, NMHDR
        virtual LRESULT paint();                                    // for WM_PAINT
        virtual LRESULT rButtonDown(WPARAM wParam, int x, int y);   // for WM_RBUTTONDOWN: state, x, y
        virtual LRESULT rButtonUp(WPARAM wParam, int x, int y);     // for WM_RBUTTONUP: state, x, y
        virtual LRESULT setCursor(HWND handle, int hit, int msgId); // for WM_SETCURSOR: handle, hit-test, ID of mouse event(move, down, etc)
        virtual LRESULT size(int w, int h, WPARAM wParam);          // for WM_SIZE: width, height, type(SIZE_MAXIMIZED...)
        virtual LRESULT size(WPARAM wParam, LPARAM lParam);          // for WM_SIZE: width, height, type(SIZE_MAXIMIZED...)
        virtual LRESULT timer(WPARAM id, LPARAM lParam);            // for WM_TIMER: ID, ptr to callback ftn
        virtual LRESULT vScroll(WPARAM wParam, LPARAM lParam);      // for WM_VSCROLL


    protected:
        HINSTANCE hInst;                                            // window handle to map window to this
        HWND handle;                                            // window handle to map window to this
        HWND hWndParent;                                            // window handle to map window to this
        HMENU hMenu;        // active for child window only
        int NumChilds;   // number of child windows
    private:
        // baseWinController class must reference to Model and View components.
        // baseWinController receives an event from the user and translates it to
        // Model and View to perform actions based on that input.
        //Model model;
        //View  view;
    };


    ///////////////////////////////////////////////////////////////////////////////
    // default ctor
    ///////////////////////////////////////////////////////////////////////////////
    baseWinController::baseWinController() :hInst(0), handle(0), hWndParent(0), NumChilds(0), hMenu(0)
    {
    }

    ///////////////////////////////////////////////////////////////////////////////
    // dtor
    ///////////////////////////////////////////////////////////////////////////////
    baseWinController::~baseWinController()
    {
        //DestroyWindow(handle);
    }

    ///////////////////////////////////////////////////////////////////////////
    // inline functions
    ///////////////////////////////////////////////////////////////////////////
    inline void baseWinController::setHinstance(HINSTANCE hInstance) { hInst = hInstance; }
    inline HINSTANCE baseWinController::getHinstance() { return hInst; }
    inline void baseWinController::setHandle(HWND hwnd) { handle = hwnd; }
    inline HWND baseWinController::getHandle() { return handle; }
    inline void baseWinController::setHandleParent(HWND hwnd) { hWndParent = hwnd; }
    inline HWND baseWinController::getHandleParent() { return hWndParent; }
    inline void baseWinController::setHmenu(HMENU hmenu) { hMenu = hmenu; }
    inline HMENU baseWinController::getHmenu() { return hMenu; }
    inline void baseWinController::setNumChildWins(int NUMCHILDWINS) { NumChilds = NUMCHILDWINS; };
   inline  int baseWinController::getNumChildWins() { return NumChilds; }

   inline LRESULT baseWinController::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) { return 0; }

    inline LRESULT baseWinController::close() { ::DestroyWindow(handle); return 0; }
    inline LRESULT baseWinController::command(int id, int cmd, LPARAM msg) { return 0; }
    inline LRESULT baseWinController::command(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) { return 0; }
    inline LRESULT baseWinController::syscommand(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) { return 0; }
    inline LRESULT baseWinController::AddNotificationIcon(HWND hWnd) { return 0; };       // for WMAPP_NOTIFICATIONCALLBACK ::custum WM
   inline LRESULT  baseWinController::DeleteNotificationIcon() { return 0; };

    inline LRESULT baseWinController::contextMenu(HWND handle, int x, int y) { return 0; }
    inline LRESULT baseWinController::create() { return 0; }
    inline LRESULT baseWinController::ctlColorBtn(HDC hdc, HWND handle) { return 0; }
    inline LRESULT baseWinController::ctlColorDlg(HDC hdc, HWND handle) { return 0; }
    inline LRESULT baseWinController::ctlColorEdit(HDC hdc, HWND handle) {return 0; }
    inline LRESULT baseWinController::ctlColorListBox(HDC hdc, HWND handle) { return 0; }
    inline LRESULT baseWinController::ctlColorScrollBar(HDC hdc, HWND handle) { return 0; }
    inline LRESULT baseWinController::ctlColorStatic(HDC hdc, HWND handle) { return 0; }
    inline LRESULT baseWinController::destroy() { return 0; }
    inline LRESULT baseWinController::dpiChanged(int x, int y, RECT* rect) { return 0; }
    inline LRESULT baseWinController::enable(bool flag) { return 0; }
    inline LRESULT baseWinController::eraseBkgnd(HDC hdc) { return 0; }
    inline LRESULT baseWinController::hScroll(WPARAM wParam, LPARAM lParam) { return 0; }
    inline LRESULT baseWinController::keyDown(int key, LPARAM lParam) { return 0; }
    inline LRESULT baseWinController::keyUp(int key, LPARAM lParam) { return 0; }
    inline LRESULT baseWinController::lButtonDown(WPARAM wParam, int x, int y) { return 0; }
    inline LRESULT baseWinController::lButtonUp(WPARAM wParam, int x, int y) { return 0; }
    inline LRESULT baseWinController::mButtonDown(WPARAM wParam, int x, int y) { return 0; }
    inline LRESULT baseWinController::mButtonUp(WPARAM wParam, int x, int y) { return 0; }
    inline LRESULT baseWinController::mouseHover(int state, int x, int y) { return 0; }
    inline LRESULT baseWinController::mouseLeave() { return 0; }
    inline LRESULT baseWinController::mouseMove(WPARAM keyState, int x, int y) { return 0; }
    inline LRESULT baseWinController::mouseWheel(int state, int delta, int x, int y) { return 0; }
    inline LRESULT baseWinController::notify(int id, LPARAM lParam) { return 0; }
    inline LRESULT baseWinController::paint() { return 0; }
    inline LRESULT baseWinController::rButtonDown(WPARAM wParam, int x, int y) { return 0; }
    inline LRESULT baseWinController::rButtonUp(WPARAM wParam, int x, int y) { return 0; }
    inline LRESULT baseWinController::setCursor(HWND handle, int hitTest, int msgId) { return 0; }
    inline LRESULT baseWinController::size(int w, int h, WPARAM type) { return 0; }
    inline LRESULT baseWinController::size(WPARAM wParam, LPARAM lParam) { return 0; }
    inline LRESULT baseWinController::timer(WPARAM id, LPARAM lParam) { return 0; }
    inline LRESULT baseWinController::vScroll(WPARAM wParam, LPARAM lParam) { return 0; }

}
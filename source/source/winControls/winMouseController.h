///////////////////////////////////////////////////////////////////////////////
// 
// WinGlMouse.h
// Mouse OPeration Classs for OpenGL
// Miscellaneous mouse operations for OpenGL objects
//
// Dependencies:  GLM
// 
//////////////////////////////////////////////////////////////////////////////
// winGlMouse.h
// Author: M. Nakano (mnsk.kn@outlook.jp)
// Created: Feb 2023
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <string>

#include <source/main/resource.h>
#include <source/sub/winGlCamera.h>// this header include glm library


namespace Win
{

	struct ButtonStatus
	{
		WPARAM state;
		int x;
		int y;
	} lastLbutton, lastMbutton, lastRbutton;

	class MouseController : public Win::baseWinController
	{
		
	public:

		MouseController(int idchild);
		~MouseController();

		LRESULT  lButtonDown(WPARAM state, int x, int y);
		LRESULT lButtonUp(WPARAM state, int x, int y);
		LRESULT mButtonDown(WPARAM state, int x, int y);
		LRESULT mButtonUp(WPARAM state, int x, int y);
		LRESULT rButtonDown(WPARAM state, int x, int y);
		LRESULT rButtonUp(WPARAM state, int x, int y);
		LRESULT mouseMove(WPARAM state, int x, int y);
		LRESULT mouseHover(int state, int x, int y);    // for WM_MOUSEHOVER:state, x, y
		LRESULT mouseLeave();                           // for WM_MOUSELEAVE
		LRESULT mouseWheel(int state, int delta, int x, int y); // for WM_MOUSEWHEEL:state, delta, x, y

		void setCam(shared_ptr<Camera> cam) { camera = cam; };
		shared_ptr<Camera> getCam() { return camera; };
		void setIdChild(int id) { idChild = id; };

	private:

		void setLastLbutton(WPARAM state, int x, int y) { lastLbutton.state = state; lastLbutton.x = x;   lastLbutton.y = y; };
		void setLastMbutton(WPARAM state, int x, int y) { lastMbutton.state = state; lastMbutton.x = x;   lastMbutton.y = y; };
		void setLastRbutton(WPARAM state, int x, int y) { lastRbutton.state = state; lastRbutton.x = x;   lastRbutton.y = y; };

		void rotate(WPARAM state, int x, int y);


		shared_ptr<Camera> camera{ nullptr };
		WPARAM mouseState{};
		glm::vec2 mousePosVec2{};
		int idChild;

	};

	///////////////////////////////////////////////////////////////////////////////
	// default contructor and dstor
	///////////////////////////////////////////////////////////////////////////////
	MouseController::MouseController(int idchild): idChild(idchild)
	{
	}

	MouseController::~MouseController()
	{
	}

	///////////////////////////////////////////////////////////////////////////////
		// handle Left mouse down & up
		///////////////////////////////////////////////////////////////////////////////
	LRESULT MouseController::lButtonDown(WPARAM state, int x, int y)
	{
		switch (state)
		{
		case MK_LBUTTON:	//rotate camera
		{
			camera->setLastEulerRoll(x);

			break;
		}
		case (MK_SHIFT + MK_LBUTTON)://mouseMove camera
		{
			/*To be coded*/
			break;
		}
		}

		return 0;
	}
	LRESULT MouseController::lButtonUp(WPARAM state, int x, int y)
	{
		return 0;
	}

	///////////////////////////////////////////////////////////////////////////////
	// handle Middle mouse down & up
	///////////////////////////////////////////////////////////////////////////////
	LRESULT MouseController::mButtonDown(WPARAM state, int x, int y)
	{
		switch (state)
		{
		case MK_MBUTTON://rotate camera
		{
			camera->setLastEulerPitch(y);
			break;
		}
		case (MK_SHIFT + MK_MBUTTON)://move camera
		{
			/*To be coded*/
			break;
		}
		}
		return 0;
	}
	LRESULT MouseController::mButtonUp(WPARAM state, int x, int y)
	{
		return 0;
	}

	///////////////////////////////////////////////////////////////////////////////
		// handle reft mouse down & up
		///////////////////////////////////////////////////////////////////////////////
	LRESULT MouseController::rButtonDown(WPARAM state, int x, int y)
	{
		switch (state)
		{
		case MK_RBUTTON: // rotate camera
		{
			camera->setLastEulerYaw(x);
			break;
		}
		case (MK_SHIFT + MK_MBUTTON): // move camera
		{
			//move camera
			/*To be coded*/
			break;
		}
		}

		return 0;
	}
	LRESULT MouseController::rButtonUp(WPARAM state, int x, int y)
	{
		//update mouse position
		//setLastrButton(state, x, y);  TO BE

	return 0;
}

///////////////////////////////////////////////////////////////////////////////
// handle WM_MOUSEMOVE
///////////////////////////////////////////////////////////////////////////////
LRESULT MouseController::mouseMove(WPARAM state, int x, int y)
{
	// for tracking mouse hover/leave tracking
	//if (!mouseHovered)
	//{
	//	// mouse hover/leave tracking
	//	TRACKMOUSEEVENT tme;
	//	tme.cbSize = sizeof(tme);
	//	tme.dwFlags = TME_HOVER | TME_LEAVE;
	//	tme.hwndTrack = handle;
	//	tme.dwHoverTime = HOVER_DEFAULT;
	//	::TrackMouseEvent(&tme);
	//	mouseHovered = true;
	//}

	switch (state)
	{
	case MK_LBUTTON: // rotate camera
	{
		camera->rotateEulerRoll(x);
		SendMessage(handle, WM_COMMAND, IDD_DIALOG_HORIZONTAL, idChild); // update view data on dialog form 
		break;
	}
	case MK_MBUTTON:
	{
		camera->rotateEulerPitch(y);
		SendMessage(handle, WM_COMMAND, IDD_DIALOG_HORIZONTAL, idChild); // update view data on dialog form 
		break;
	}
	case MK_RBUTTON:
	{
		camera->rotateEulerYaw(x);
		SendMessage(handle, WM_COMMAND, IDD_DIALOG_HORIZONTAL, idChild); // update view data on dialog form 
		break;
	}
	}

	return 0;
}

///////////////////////////////////////////////////////////////////////////////
	// handle WM_MOUSEHOVER
	///////////////////////////////////////////////////////////////////////////////
LRESULT MouseController::mouseHover(int state, int x, int y)
{
	//mouseHovered = true;
	return 0;
}



///////////////////////////////////////////////////////////////////////////////
// handle WM_MOUSELEAVE
///////////////////////////////////////////////////////////////////////////////
LRESULT MouseController::mouseLeave()
{
	//mouseHovered = false;
	return 0;
}


///////////////////////////////////////////////////////////////////////////////
// handle WM_MOUSEWHEEL
///////////////////////////////////////////////////////////////////////////////
LRESULT MouseController::mouseWheel(int state, int delta, int x, int y)
{
	
	camera->zoomCameraDelta(float(delta), x, y);
	SendMessage(handle, WM_COMMAND, IDD_DIALOG_HORIZONTAL, idChild); // update view data on dialog form 
	
	return 0;
}

}

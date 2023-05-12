///////////////////////////////////////////////////////////////////////////////
// Based on the Article of http://www.songho.ca/opengl/gl_mvc.html
// 
// ViewForm.h
//
//////////////////////////////////////////////////////////////////////////////
// winDialogForm.h
// Modfier: M. Nakano (mnsk.kn@outlook.jp)
// Modified: Jan 2023
///////////////////////////////////////////////////////////////////////////////

#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <iomanip>
//#include <wingdi.h>

#include <source/winControls/baseWinForm.h>
#include <source/winOpenGL/winGlController.h>
//#include <source/winLogs/Log.h>
#include "wcharUtil.h"

#include <source/main/resource.h>

constexpr auto WinChild1 = 1; // dialogform handle window1 only in this application

namespace Win
{

	const int UPDOWN_LOW = 10;
	const int UPDOWN_HIGH = 100;
	const int UPDOWN_AngleToLookAt_LOW = 0;
	const int UPDOWN_AngleToLookAt_HIGH = 180;
	const float POSITION_SCALE = 1.0f;
	const float TARGET_SCALE = 0.1f;

	class DialogForm : public baseWinForm
	{
	public:
		DialogForm();
		~DialogForm();

		void initControls(HWND handle, std::vector<shared_ptr<GlController>>* glcontrollers);         // init all controls
		void resetControls();

		void updateTrackbars(HWND handle, int trackbarvalue);
		void updateCtrlTargetStarDynamicInfo();
		//void handleLink(HWND handle, int index);
		void changeUpDownPosition(HWND handle, int pos);
		void toggleCam2();
		void toggleGrid();
		void toggleMotiondataUpdate();

		void updateTargetSstarId(int cbItem);
		ComboBox getComboBox() { return comboBox; };

		//void setIdChild(int id) { idChild = id; };
		/*void updateViewData(int idchild) {
			idChild = idchild;
			updateCtrlViewMatrix();
			updateCtrlRotationMatrix();
			updateCtrlViewEuler();
			updateCtrlViewQuaternion();
			updateCtrlViewPos();
			updateCtrlViewTarget();
		};
		*/
		//shared_ptr<GlController> glController{ nullptr };
		std::vector<shared_ptr<GlController>>* glControllers{ nullptr };

	protected:

	private:


		void updateCtrlAngle();
		void updateCtrlPosition();
		void updateCtrlTarget();
		void updateCtrlMatrix();
		void updateCtrlQuaternion();
		void updateCtrlTargetStarStaticInfo();

		int idChild;

		std::wstringstream labelValue;
		float valueF{};
		int sliderValue{};

		HWND parentHandle;
		float positionScale;
		float targetScale;

		const float eulerBasis{ 180.0f };
		const float eulerRange{ 360.0f };
		const glm::vec3 eulerBasisVec3{ 180.0f, 180.0f, 180.0f };
		const glm::vec3 eulerRangeVec3{ 360.0f, 360.0f, 360.0f };

		const float positionBasis{ 200.0f };
		const float positionRange{ 400.0f };

		const float targetBasis{ 200.0f };
		const float targetRange{ 400.0f };

		// controls
		struct CtrlPair
		{
			Trackbar* slider;
			TextBox* label;
		};

		std::vector<std::pair<int, int>> IdcAngle
		{
			{IDC_SLIDER_PITCH, IDC_LABEL_PITCH },
			{IDC_SLIDER_YAW, IDC_LABEL_YAW },
			{IDC_SLIDER_ROLL, IDC_LABEL_ROLL }
		};
		std::vector<CtrlPair> ctrlAngle{};

		std::vector<std::pair<int, int>> IdcPosition
		{
			{IDC_SLIDER_POSX, IDC_LABEL_POSX },
			{IDC_SLIDER_POSY, IDC_LABEL_POSY},
			{IDC_SLIDER_POSZ, IDC_LABEL_POSZ }
		};
		std::vector<CtrlPair> ctrlPosition{};

		std::vector<std::pair<int, int>> IdcTarget
		{
			{IDC_SLIDER_TARX, IDC_LABEL_TARX },
			{IDC_SLIDER_TARY, IDC_LABEL_TARY},
			{IDC_SLIDER_TARZ, IDC_LABEL_TARZ }
		};
		std::vector<CtrlPair> ctrlTarget{};

		std::vector< int> IdcMatrix
		{
			IDC_LABEL_M00,
			IDC_LABEL_M01,
			IDC_LABEL_M02,
			IDC_LABEL_M03,
			IDC_LABEL_M04,
			IDC_LABEL_M05,
			IDC_LABEL_M06,
			IDC_LABEL_M07,
			IDC_LABEL_M08,
			IDC_LABEL_M09,
			IDC_LABEL_M10,
			IDC_LABEL_M11,
			IDC_LABEL_M12,
			IDC_LABEL_M13,
			IDC_LABEL_M14,
			IDC_LABEL_M15
		};
		std::vector<TextBox* > ctrlMatrix{};

		std::vector< int> IdcQuaternion
		{
			IDC_LABEL_QS,
			IDC_LABEL_QX,
			IDC_LABEL_QY,
			IDC_LABEL_QZ,
		};
		std::vector<TextBox* > ctrlQuaternion{};

		std::vector< int> IdcTargetStarStaticInfo
		{
			IDC_LABEL_STAR_NAME,
			IDC_LABEL_STAR_MASS,
			IDC_LABEL_STAR_RADIUS,
			IDC_LABEL_STAR_OBLIQUITY,
			IDC_LABEL_STAR_INCLINATION,
			IDC_LABEL_STAR_ROTATION,
			IDC_LABEL_STAR_REVOLUTION,
			IDC_LABEL_STAR_DISTANCE_KM,
			IDC_LABEL_STAR_DISTANCE_AU
		};
		std::vector<TextBox* > ctrlTargetStarStaticInfo{};

		std::vector< std::vector<int>> IdcTargetStarDynamicInfo
		{
			{IDC_LABEL_STAR_POSITIONX, IDC_LABEL_STAR_POSITIONY, IDC_LABEL_STAR_POSITIONZ},
			{IDC_LABEL_STAR_VELOCITYX, IDC_LABEL_STAR_VELOCITYY, IDC_LABEL_STAR_VELOCITYZ},
			{IDC_LABEL_STAR_ACCELERATIONX, IDC_LABEL_STAR_ACCELERATIONY, IDC_LABEL_STAR_ACCELERATIONZ},
			{IDC_LABEL_CAMERA_POSITIONX, IDC_LABEL_CAMERA_POSITIONY, IDC_LABEL_CAMERA_POSITIONZ},
			{IDC_LABEL_CAMERA_TARGETX, IDC_LABEL_CAMERA_TARGETY, IDC_LABEL_CAMERA_TARGETZ},
			{IDC_LABEL_CAMERA_EULERX, IDC_LABEL_CAMERA_EULERY, IDC_LABEL_CAMERA_EULERZ},
		};
		std::vector<std::vector<TextBox*>> ctrlTargetStarDynamicInfo{};

		TextBox* ctrlSolarElapsedDays{};

		//ListBox* listStar;

		ComboBox  comboBox;
		Button buttonReset;
		Button buttonAbout;

		CheckBox  checkGrid;
		CheckBox  checkCam2;
		CheckBox  checkMotiondataUpdate;
		EditBox   editFov;
		UpDownBox spinFov;
		EditBox   editAngleToLookAt;
		UpDownBox spinAngleToLookAt;

		//SysLink  linkTest;
	};


	///////////////////////////////////////////////////////////////////////////////
	// default ctor
	///////////////////////////////////////////////////////////////////////////////
	DialogForm::DialogForm() : baseWinForm(), parentHandle(0), positionScale(POSITION_SCALE), idChild(WinChild1), targetScale{ TARGET_SCALE }
	{
	}

	///////////////////////////////////////////////////////////////////////////////
	// default dtor
	///////////////////////////////////////////////////////////////////////////////
	DialogForm::~DialogForm()
	{
	}

	///////////////////////////////////////////////////////////////////////////////
	// initialize all controls
	///////////////////////////////////////////////////////////////////////////////
	void DialogForm::initControls(HWND handle, std::vector<shared_ptr<GlController>>* glcontrollers)
	{
		// remember the handle to parent window
		parentHandle = handle;

		//glController = glcontroller;
		glControllers = glcontrollers;

		// Angle
		for (unsigned int i = 0; i < IdcAngle.size(); i++)
		{
			ctrlAngle.push_back({ new Trackbar{ parentHandle, IdcAngle[i].first }, new TextBox{parentHandle, IdcAngle[i].second} });
			//ctrlAngle.back().slider->setRange(0, eulerRange);
			ctrlAngle.back().slider->setRange(0, eulerRangeVec3[i]);
		}
		updateCtrlAngle();

		// Position
		//for (unsigned int i = 0; i < IdcPosition.size(); i++)
		//{
		//	ctrlPosition.push_back({ new Trackbar{ parentHandle, IdcPosition[i].first }, new TextBox{parentHandle, IdcPosition[i].second} });
		//	//ctrlPosition.back().slider->setRange(0, positionRange);
		//}
		for (auto &p: IdcPosition)
		{
			ctrlPosition.push_back({ new Trackbar{ parentHandle, p.first }, new TextBox{parentHandle, p.second} });
			//ctrlPosition.back().slider->setRange(0, positionRange);
		}
		for (auto& p : ctrlPosition)
		{
			p.slider->setRange(0, positionRange);
			// [Trackbar control Sttyles](https://learn.microsoft.com/en-us/windows/win32/controls/trackbar-control-styles)
			//p.slider->addStyles(TBS_TRANSPARENTBKGND); // make TB background transparent, in this program, it is done with TB property panel, "Transparent Background" set to True;
		}

		updateCtrlPosition();

		// Target
		for (unsigned int i = 0; i < IdcTarget.size(); i++)
		{
			ctrlTarget.push_back({ new Trackbar{ parentHandle, IdcTarget[i].first }, new TextBox{parentHandle, IdcTarget[i].second} });
			ctrlTarget.back().slider->setRange(0, targetRange);
			////ctrlTarget.back().slider->setTic(5);

			////[Trackbar Control Styles](https://learn.microsoft.com/en-us/windows/win32/controls/trackbar-control-styles) : not work
			//HWND myHandle= ctrlTarget.back().slider->getHandle();
			//HDC  myHdc = GetDC(myHandle);
			//SendMessage(myHandle, TBS_AUTOTICKS, 0, 0);
			////SendMessage(handle, TBS_TRANSPARENTBKGND, (WPARAM)myWparam, PRF_ERASEBKGND & PRF_CLIENT);

		}
		updateCtrlTarget();

		// Matrix
		for (unsigned int i = 0; i < IdcMatrix.size(); i++)
		{
			ctrlMatrix.push_back(new TextBox{ parentHandle, IdcMatrix[i] });
		}
		updateCtrlMatrix();

		// Quaternion
		for (unsigned int i = 0; i < IdcQuaternion.size(); i++)
		{
			ctrlQuaternion.push_back(new TextBox{ parentHandle, IdcQuaternion[i] });
		}
		updateCtrlQuaternion();

		// Target Star Static Info
		for (unsigned int i = 0; i < IdcTargetStarStaticInfo.size(); i++)
		{
			ctrlTargetStarStaticInfo.push_back(new TextBox{ parentHandle, IdcTargetStarStaticInfo[i] });
		}
		updateCtrlTargetStarStaticInfo();

		// Target Star Dynamic Info
		for each (auto idc in IdcTargetStarDynamicInfo)
		{
			ctrlTargetStarDynamicInfo.push_back({
				new TextBox{ parentHandle, idc[0]},
				new TextBox{ parentHandle, idc[1]},
				new TextBox{ parentHandle, idc[2]}
				});
		}
		ctrlSolarElapsedDays = new TextBox{ parentHandle, IDC_LABEL_SOLAR_DAYS };

		updateCtrlTargetStarDynamicInfo(); // do'nt here

		buttonReset.set(parentHandle, IDC_BUTTON_RESET);
		//buttonAbout.set(parentHandle, IDC_BUTTON_ABOUT);
		buttonAbout.setImage(::LoadIcon(NULL, IDI_INFORMATION));

		checkGrid.set(parentHandle, IDC_CHECK_GRID);
		checkGrid.check();

		checkMotiondataUpdate.set(parentHandle, IDC_CHECK_MOTIONDATAUPDATE);
		checkMotiondataUpdate.uncheck(); // manually start momition data calc


		// target star; COMBO BOX initialization
		comboBox.set(parentHandle,  IDC_COMBO_STAR_TARGET);
		for each (auto star in glControllers->at(idChild)->glRender->solar->starsData)
		{
			labelValue.str(L"");
			labelValue << star.second.starName.c_str();
			comboBox.addString(labelValue.str().c_str());
		}
		// set cursor to the top as initial setting
		int initialDisplayOfStar = 3;// note: set StarId; Earth here
		comboBox.setCurrentSelection(initialDisplayOfStar);  // for debug ; phobos
		updateTargetSstarId(initialDisplayOfStar);// for debug ; phobos to display at first 

		checkCam2.set(parentHandle, IDC_CHECK_CAM2);
		checkCam2.check();

		labelValue.str(L"");
		labelValue << std::fixed << std::setprecision(0);

		valueF = glControllers->at(idChild)->camera->initFov;
		labelValue << valueF;
		editFov.set(parentHandle, IDC_EDIT_FOV);
		editFov.setText(labelValue.str().c_str());
		spinFov.set(parentHandle, IDC_SPIN_FOV);
		spinFov.setRange(UPDOWN_LOW, UPDOWN_HIGH);
		spinFov.setPos(valueF);
		spinFov.setBuddy(editFov.getHandle());

		labelValue.str(L"");
		valueF = glControllers->at(idChild)->camera->initAngleToLookAt;
		labelValue << valueF;
		editAngleToLookAt.set(parentHandle, IDC_EDIT_ANGLE_TO_LOOK_AT);
		editAngleToLookAt.setText(labelValue.str().c_str());
		spinAngleToLookAt.set(parentHandle, IDC_SPIN_ANGLE_TO_LOOK_AT);
		spinAngleToLookAt.setRange(UPDOWN_AngleToLookAt_LOW, UPDOWN_AngleToLookAt_HIGH);
		spinAngleToLookAt.setPos(valueF);
		spinAngleToLookAt.setBuddy(editAngleToLookAt.getHandle());
		spinAngleToLookAt.enable();
		editAngleToLookAt.enable();


		//@@TEST
		//linkTest.set(handle, IDC_LINK1);
		/*
		wchar_t buff[1024];
		memset(buff, 0, sizeof(wchar_t)*1024);
		linkTest.getText(buff, 1024);
		log(L"SysLink: %ls", buff);
		linkTest.setUrl(0, L"http://www.google.ca");
		linkTest.getUrl(0, buff);
		log("SysLink URL: %ls", buff);
		*/
	}


	///////////////////////////////////////////////////////////////////////////////
	// update trackbars (Slider)
	///////////////////////////////////////////////////////////////////////////////
	void DialogForm::updateTrackbars(HWND handle, int trackbarvalue)
	{

		unsigned int ctrlId = GetDlgCtrlID(handle); // get control object id on dialog box
		unsigned  i{ 0 };

		// Angle (in degree)
		for (std::vector<std::pair<int, int>>::iterator it = IdcAngle.begin(); it != IdcAngle.end(); ++it)
		{
			if (it->first == ctrlId)
			{
				//valueF = (float)(trackbarvalue - eulerBasis);
				valueF = (float)(trackbarvalue - eulerBasisVec3[i]);
				labelValue << std::fixed << std::setprecision(0);
				labelValue.str(L"");
				labelValue << valueF;
				ctrlAngle[i].slider->setPos(trackbarvalue);
				ctrlAngle[i].label->setText(labelValue.str().c_str());

				// i:: 0:x, 1:y, 2:z
				glm::vec3 deltaEulers = glControllers->at(idChild)->camera->deltaEulers; // in degree
				deltaEulers[i] = valueF;
				glControllers->at(idChild)->camera->deltaEulers = deltaEulers;

				//updateCtrlPosition();
				//updateCtrlMatrix();
				//updateCtrlQuaternion();

				// unset floating format
				labelValue << std::resetiosflags(std::ios_base::fixed | std::ios_base::floatfield);
				return;
			};
			i += 1;
		}

		// Position
		i = 0;
		for (std::vector<std::pair<int, int>>::iterator it = IdcPosition.begin(); it != IdcPosition.end(); ++it)
		{
			if (it->first == ctrlId)
			{
				valueF = (trackbarvalue - positionBasis) * positionScale;

				labelValue << std::fixed << std::setprecision(1);
				labelValue.str(L"");

				labelValue << valueF;
				ctrlPosition[i].slider->setPos(trackbarvalue);
				ctrlPosition[i].label->setText(labelValue.str().c_str());

				// i:: 0:x, 1:y, 2:z
				//glm::vec3 position = modelgl->cam2->getPosition();
				glm::vec3 deltaPosition = glControllers->at(idChild)->camera->deltaPosition;
				//position[i] = valueF;
				//deltaPosition[i] += valueF;
				deltaPosition[i] = valueF;
				//modelgl->cam2->setPosition(position);
				glControllers->at(idChild)->camera->deltaPosition = deltaPosition;

				//modelgl->setCam2ToPlanet();  // update cam2

				//updateCtrlAngle();
				//updateCtrlMatrix();
				//updateCtrlQuaternion();

				// unset floating format
				labelValue << std::resetiosflags(std::ios_base::fixed | std::ios_base::floatfield);
				return;
			};
			i += 1;
		}

		// Target
		i = 0;
		for (std::vector<std::pair<int, int>>::iterator it = IdcTarget.begin(); it != IdcTarget.end(); ++it)
		{
			if (it->first == ctrlId)
			{
				valueF = (trackbarvalue - targetBasis) * targetScale;
				labelValue << std::fixed << std::setprecision(1);
				labelValue.str(L"");
				labelValue << valueF;
				ctrlTarget[i].slider->setPos(trackbarvalue);
				ctrlTarget[i].label->setText(labelValue.str().c_str());

				// update target      i:: 0:x, 1:y, 2:z
				glm::vec3 deltaTarget = glControllers->at(idChild)->camera->deltaTarget; // first get current one
				deltaTarget[i] = valueF;
				glControllers->at(idChild)->camera->deltaTarget = deltaTarget;

				// unset floating format
				labelValue << std::resetiosflags(std::ios_base::fixed | std::ios_base::floatfield);
				return;
			};
			i += 1;
		}

	}


	///////////////////////////////////////////////////////////////////////////////
	// reset controls
	///////////////////////////////////////////////////////////////////////////////
	void DialogForm::resetControls()
	{
		updateCtrlAngle();
		updateCtrlPosition();
		updateCtrlTarget();
		updateCtrlMatrix();
		updateCtrlQuaternion();

		updateCtrlTargetStarStaticInfo();

		labelValue << std::fixed << std::setprecision(0);
		labelValue.str(L"");

		valueF = glControllers->at(idChild)->camera->initFov;
		labelValue << valueF;
		spinFov.setPos(valueF);
		editFov.setText(labelValue.str().c_str());

		labelValue.str(L"");
		valueF = glControllers->at(idChild)->camera->initAngleToLookAt;
		labelValue << valueF;
		spinAngleToLookAt.setPos(valueF);
		editAngleToLookAt.setText(labelValue.str().c_str());

		for (auto& ctrl : *glControllers) 
		{
			ctrl->camera->reset();
		}

	}



	///////////////////////////////////////////////////////////////////////////////
	// update controls for angle
	///////////////////////////////////////////////////////////////////////////////
	void DialogForm::updateCtrlAngle()
	{

		labelValue << std::fixed << std::setprecision(0);

		int i = 0;
		for (auto & a: ctrlAngle)
		{
			a.slider->setPos(eulerBasisVec3[i]);
			labelValue.str(L"");	 labelValue << 0.0;
			a.label->setText(labelValue.str().c_str());
			i++;
		}

		// unset floating format
		labelValue << std::resetiosflags(std::ios_base::fixed | std::ios_base::floatfield);
	}


	///////////////////////////////////////////////////////////////////////////////
	// update controls for position with current cam2 position
	///////////////////////////////////////////////////////////////////////////////
	void DialogForm::updateCtrlPosition()
	{
		labelValue << std::fixed << std::setprecision(1);

		for (auto & p : ctrlPosition)
		{
			sliderValue = positionBasis;
			p.slider->setPos(sliderValue);
			labelValue.str(L""); 	labelValue << 0.0;
			p.label->setText(labelValue.str().c_str());
		}

		/*for (unsigned int i = 0; i < IdcPosition.size(); i++)
		{
			sliderValue = positionBasis;
			ctrlPosition[i].slider->setPos(sliderValue);
			labelValue.str(L""); 	labelValue << 0.0;
			ctrlPosition[i].label->setText(labelValue.str().c_str());
		}*/

		// unset floating format
		labelValue << std::resetiosflags(std::ios_base::fixed | std::ios_base::floatfield);
	}


	///////////////////////////////////////////////////////////////////////////////
	// update controls for target
	///////////////////////////////////////////////////////////////////////////////
	void DialogForm::updateCtrlTarget()
	{
		labelValue << std::fixed << std::setprecision(1);

		for (auto &t : ctrlTarget)
		{
			sliderValue = targetBasis;
			t.slider->setPos(sliderValue);
			labelValue.str(L""); labelValue << 0.0;
			t.label->setText(labelValue.str().c_str());
		}

		/*for (unsigned int i = 0; i < IdcTarget.size(); i++)
		{
			sliderValue = targetBasis;
			ctrlTarget[i].slider->setPos(sliderValue);
			labelValue.str(L""); labelValue << 0.0;
			ctrlTarget[i].label->setText(labelValue.str().c_str());
		}*/

		// unset floating format
		labelValue << std::resetiosflags(std::ios_base::fixed | std::ios_base::floatfield);
	}

	///////////////////////////////////////////////////////////////////////////////
	// update matrix elements
	///////////////////////////////////////////////////////////////////////////////
	void DialogForm::updateCtrlMatrix()
	{
		labelValue << std::fixed << std::setprecision(3);

		glm::mat4 m = glControllers->at(idChild)->camera->getViewMatrix(); // coloumn major [column][row]
		unsigned int c{ 0 }, r{ 0 };

		for (unsigned int i = 0; i < IdcMatrix.size(); i++)
		{
			r = i % 4;
			labelValue.str(L"");
			labelValue << m[c][r];
			ctrlMatrix[i]->setText(labelValue.str().c_str());
			if (r == 3)
				c += 1;
		}

		// unset floating format
		labelValue << std::resetiosflags(std::ios_base::fixed | std::ios_base::floatfield);
	}

	///////////////////////////////////////////////////////////////////////////////
	// update qiaternion
	///////////////////////////////////////////////////////////////////////////////
	void DialogForm::updateCtrlQuaternion()
	{
		labelValue << std::fixed << std::setprecision(3);

		glm::quat q = glControllers->at(idChild)->camera->getViewQuaternion();

		for (unsigned int i = 0; i < IdcQuaternion.size(); i++)
		{
			labelValue.str(L"");
			labelValue << q[i];
			ctrlQuaternion[i]->setText(labelValue.str().c_str());
		}

		// unset floating format
		labelValue << std::resetiosflags(std::ios_base::fixed | std::ios_base::floatfield);
	}


	///////////////////////////////////////////////////////////////////////////////
	// update Target Star Info
	///////////////////////////////////////////////////////////////////////////////
	void DialogForm::updateCtrlTargetStarStaticInfo()
	{

		int starId = glControllers->at(idChild)->glRender->targetStarIdCAM2;

		// Name
		labelValue.str(L"");
		labelValue << glControllers->at(idChild)->glRender->solar->starsData[starId].starName.c_str();
		ctrlTargetStarStaticInfo[0]->setText(labelValue.str().c_str());
		WIN_LOG(glControllers->at(idChild)->glRender->solar->starsData[starId].starName.c_str());

		//labelValue << std::fixed << std::setprecision(5);
		labelValue << std::scientific << std::setprecision(4);
		// Mass
		labelValue.str(L"");
		labelValue << glControllers->at(idChild)->glRender->solar->starsData[starId].mass;
		ctrlTargetStarStaticInfo[1]->setText(labelValue.str().c_str());

		// Radius
		labelValue.str(L"");
		labelValue << glControllers->at(idChild)->glRender->solar->starsData[starId].radius;
		ctrlTargetStarStaticInfo[2]->setText(labelValue.str().c_str());

		// Obliquity
		labelValue << std::fixed << std::setprecision(3);
		labelValue.str(L"");
		labelValue << glControllers->at(idChild)->glRender->solar->starsData[starId].obliquity;
		ctrlTargetStarStaticInfo[3]->setText(labelValue.str().c_str());

		// Inclinantion
		labelValue << std::fixed << std::setprecision(3);
		labelValue.str(L"");
		labelValue << glControllers->at(idChild)->glRender->solar->starsData[starId].inclination;
		ctrlTargetStarStaticInfo[4]->setText(labelValue.str().c_str());

		// Rotation
		labelValue << std::fixed << std::setprecision(3);
		labelValue.str(L"");
		labelValue << glControllers->at(idChild)->glRender->solar->starsData[starId].rotationPeriod;
		ctrlTargetStarStaticInfo[5]->setText(labelValue.str().c_str());

		//Revolution
		labelValue << std::fixed << std::setprecision(3);
		labelValue.str(L"");
		labelValue << glControllers->at(idChild)->glRender->solar->starsData[starId].orbitalPeriod;
		ctrlTargetStarStaticInfo[6]->setText(labelValue.str().c_str());

		//Distance km
		labelValue << std::scientific << std::setprecision(4);
		labelValue.str(L"");
		labelValue << glControllers->at(idChild)->glRender->solar->starsData[starId].distanceToParent;
		ctrlTargetStarStaticInfo[7]->setText(labelValue.str().c_str());

		//Distance au
		labelValue << std::fixed << std::setprecision(4);
		labelValue.str(L"");
		labelValue << glControllers->at(idChild)->glRender->solar->starsData[starId].distanceToParentAu;
		ctrlTargetStarStaticInfo[8]->setText(labelValue.str().c_str());


		// unset floating format
		labelValue << std::resetiosflags(std::ios_base::fixed | std::ios_base::floatfield);
	}

	void DialogForm::updateCtrlTargetStarDynamicInfo()
	{

		if (!checkMotiondataUpdate.isChecked())  // no need to update dynamic info
			return;

		int starId = glControllers->at(idChild)->glRender->targetStarIdCAM2;

		labelValue << std::fixed << std::setprecision(3);
		std::vector<TextBox*> Vec{};

		// Star Position
		Vec = ctrlTargetStarDynamicInfo[0];
		for (int i = 0; i < Vec.size(); i++)
		{
			labelValue.str(L"");
			labelValue << glControllers->at(idChild)->glRender->MidMemberOfMotionData[starId].position[i];
			Vec[i]->setText(labelValue.str().c_str());
		}

		// Star Velocity
		Vec = ctrlTargetStarDynamicInfo[1];
		for (int i = 0; i < Vec.size(); i++)
		{
			labelValue.str(L"");
			labelValue << glControllers->at(idChild)->glRender->MidMemberOfMotionData[starId].velocity[i];
			Vec[i]->setText(labelValue.str().c_str());
		}

		// Star Acceleration
		Vec = ctrlTargetStarDynamicInfo[2];
		for (int i = 0; i < Vec.size(); i++)
		{
			labelValue.str(L"");
			labelValue << glControllers->at(idChild)->glRender->MidMemberOfMotionData[starId].acceleration[i];
			Vec[i]->setText(labelValue.str().c_str());
		}

		// Camera Position
		Vec = ctrlTargetStarDynamicInfo[3];
		for (int i = 0; i < Vec.size(); i++)
		{
			labelValue.str(L"");
			labelValue << glControllers->at(idChild)->camera->getViewPos()[i];
			Vec[i]->setText(labelValue.str().c_str());
		}

		// Camera Target
		Vec = ctrlTargetStarDynamicInfo[4];
		for (int i = 0; i < Vec.size(); i++)
		{
			labelValue.str(L"");
			labelValue << glControllers->at(idChild)->camera->getViewTarget()[i];
			Vec[i]->setText(labelValue.str().c_str());
		}

		// Camera Euler
		Vec = ctrlTargetStarDynamicInfo[5];
		for (int i = 0; i < Vec.size(); i++)
		{
			labelValue.str(L"");
			labelValue << glControllers->at(idChild)->camera->getViewEuler()[i];
			Vec[i]->setText(labelValue.str().c_str());
		}

		// Solar DaylabelValue.str(L"");
		labelValue.str(L"");
		labelValue << glControllers->at(idChild)->glRender->MidMemberOfMotionData[starId].systimer;
		ctrlSolarElapsedDays->setText(labelValue.str().c_str());

		updateCtrlMatrix();

		updateCtrlQuaternion();


		// unset floating format
		labelValue << std::resetiosflags(std::ios_base::fixed | std::ios_base::floatfield);

	}

	/*
	///////////////////////////////////////////////////////////////////////////////
	// handle SysLink event
	///////////////////////////////////////////////////////////////////////////////
	void ViewForm::handleLink(HWND handle, int index)
	{
		if(handle == linkTest.getHandle())
		{
			// open url
			wchar_t url[1024];
			memset(url, 0, sizeof(wchar_t)*1024);
			linkTest.getUrl(index, url);
			::ShellExecute(0, L"open", url, 0, 0, SW_SHOW);
			log(L"URL: %ls", url);
		}
	}
	*/



	///////////////////////////////////////////////////////////////////////////////
	// change updown position
	///////////////////////////////////////////////////////////////////////////////
	void DialogForm::changeUpDownPosition(HWND handle, int position)
	{

		labelValue << std::fixed << std::setprecision(0);

		//Camera FOV)
		if (handle == spinFov.getHandle())
		{
			if (position >= UPDOWN_LOW && position <= UPDOWN_HIGH)
			{
				labelValue.str(L"");
				labelValue << position;
				editFov.setText(labelValue.str().c_str());
				glControllers->at(idChild)->camera->setFov((float)position);
			}
		}

		//AngleToLookAt
		if (handle == spinAngleToLookAt.getHandle())
		{
			if (position >= UPDOWN_AngleToLookAt_LOW && position <= UPDOWN_AngleToLookAt_HIGH)
			{
				labelValue.str(L"");
				labelValue << position;
				editAngleToLookAt.setText(labelValue.str().c_str());
				glControllers->at(idChild)->camera->angleToLookAt = ((float)position);
			}
		}

		// unset floating format
		labelValue << std::resetiosflags(std::ios_base::fixed | std::ios_base::floatfield);
	}


	///////////////////////////////////////////////////////////////////////////////
	// toggle on/off FOV control
	///////////////////////////////////////////////////////////////////////////////
	void DialogForm::toggleCam2()
	{
		if (checkCam2.isChecked())
		{
			glControllers->at(idChild)->glRender->enableCam2();
		}
		else
		{
			glControllers->at(idChild)->glRender->disableCam2();
		}
	}

	void DialogForm::toggleGrid()
	{
		for (auto& ctrl : *glControllers)
			checkGrid.isChecked() ? ctrl->glRender->enableGrid() : ctrl->glRender->disableGrid();

		//if (checkGrid.isChecked())
		//{
		//	/*modelgl1->enableGrid();
		//		modelgl2->enableGrid();*/
		//	for (auto& ctrl : *glControllers)
		//		ctrl->glRender->enableGrid();
		//}
		//else
		//{
		//	/*modelgl1->disableGrid();
		//	modelgl2->disableGrid();*/
		//	for (auto& ctrl : *glControllers)
		//		/*modelgl1->enableGrid();
		//		modelgl2->enableGrid();*/
		//		ctrl->glRender->disableGrid();
		//}
	}

	void DialogForm::updateTargetSstarId(int cbItem)
	{
		glControllers->at(idChild)->glRender->targetStarIdCAM2 = cbItem;
	}

	void DialogForm::toggleMotiondataUpdate()
	{
		for (auto& ctrl : *glControllers)
			checkMotiondataUpdate.isChecked() ? ctrl->glRender->enableMotiondataUpdate() : ctrl->glRender->disableMotiondataUpdate();

		/*if (checkMotiondataUpdate.isChecked())
		{
			modelgl1->enableMotiondataUpdate();
			modelgl2->enableMotiondataUpdate();
		}
		else
		{
			modelgl1->disableMotiondataUpdate();
			modelgl2->disableMotiondataUpdate();
		}*/
	}
}

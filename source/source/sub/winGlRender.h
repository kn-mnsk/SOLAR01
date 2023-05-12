///////////////////////////////////////////////////////////////////////////////
// Based on the Article of http://www.songho.ca/opengl/gl_mvc.html
// 
// ModelGL.h 
// Model component of OpenGL
//
//////////////////////////////////////////////////////////////////////////////
// winGlRender.h
// Modfier: M. Nakano (mnsk.kn@outlook.jp)
// Modified: Jan 2023
///////////////////////////////////////////////////////////////////////////////

#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>    // include windows.h to avoid thousands of compile errors even though this class is not depending on Windows

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glad/glad.h>
#include <source/main/resource.h>
#include <source/sub/winGlCamera.h>// this header include glm library

#include <mylibrary/shader.h>
#include <mylibrary/model.h>
#include <mylibrary/tools.h>
#include <mylibrary/LoaderGLTF.h>
#include <mylibrary/SolarSys.hpp>
#include <local_configuration/root_directory.h>


#define Assert(cond) do { if (!(cond)) __debugbreak(); } while (0)

class GlRender
{
public:

	int targetStarIdCAM2{ 0 };
	std::string starsdataFile{};
	std::shared_ptr<SolarSys> solar;

	int windowWidth{10};  // any non-zero value to avoid initial crash
	int windowHeight{10}; // any non-zero value to avoid initial crash

	enum class ProjectionType {
		Pers,
		PersFOV,
		InfinitePers,
		Ortho,
		PixelOrtho,

		Num_ProjectionType,
	};

	std::vector<SolarSys::MotionDataFormat> LastMemberOfMotionData{};
	std::vector<SolarSys::MotionDataFormat> MidMemberOfMotionData{};
	std::vector<SolarSys::MotionDataFormat> FrontMemberOfMotionData{};

	GlRender(int idchild);

	~GlRender();

	// member functions
	void init();
	void swapBuffers();
	void resizeWindow(int width, int height);
	void quit();                            // clean up OpenGL objects

	glm::mat4  projection(int w, int h, float fov, ProjectionType projectiontype = ProjectionType::Pers);
	glm::mat4 calcModelMat4(SolarSys::MotionDataFormat& star);

	void draw(int);
	void drawStars();
	void setCam2OnEcripticOrbit(float angle);
	void setCam2OnEquatorOfPlanet(float angle);
	void setSolarSys(std::shared_ptr<SolarSys> solarsys) { solar = solarsys; };

	void setCam(shared_ptr<Camera> cam) { camera = cam; };
	void setDialogHandle(HWND handle) { dialogHandle = handle; };
	void setIdChild(int id) { idChild = id; };

	// for Cam2 - ecriptic or quator(cellestrial)
	void enableCam2() { cam2EcripticEnabled = true; }
	void disableCam2() { cam2EcripticEnabled = false; }

	// for grid
	void drawGridXZ(Shader::Shader* shader, glm::mat4& model, glm::mat4& view, glm::mat4& projection);
	void setGridSize(float radius);
	void enableGrid() { gridEnabled = true; }
	void disableGrid() { gridEnabled = false; }

	// for motion data
	void enableMotiondataUpdate() { motiondataUpdateEnabled = true; }
	void disableMotiondataUpdate() { motiondataUpdateEnabled = false; }

	void setDC(HDC hdc) { Hdc = hdc; };
	HDC getDC() { return Hdc; };
	void setRC(HGLRC hglrc) { Hglrc = hglrc; };
	HGLRC  getRC() { return Hglrc; };

	// constants
	const float GRID_SIZE = 10.0f;
	const float GRID_STEP = 1.0f;
	const float NEAR_PLANE = 1.0f;
	const float FAR_PLANE = 3000.0f;

	Tools::LineVertices gridLines{};
	Tools::LineVertices axesLines{};
	Tools::LineVertices  normalLines{};

	const float depthZNear = 0.0f;
	const float depthZFar = 1.0f;

	// camera
	shared_ptr<Camera> camera{ nullptr };

protected:

private:
	// members
	int  idChild;
	float nearPlane;
	float farPlane;
	bool gridEnabled;

	float gridSize;         // half length of grid
	float gridStep;         // step for next grid line

	bool windowResized;

	bool    cam2EcripticEnabled;
	bool    motiondataUpdateEnabled;


	glm::mat4 ModelSun{ 1.0f };

	HDC Hdc{};
	HGLRC  Hglrc{};
	LoaderGLTF::glTFLoader* starModel{ nullptr };
	HWND dialogHandle{};

	GLuint gridVao{}, gridVbo{}, gridEbo{};
	GLuint axesVao{}, axesVbo{}, axesEbo{};

};

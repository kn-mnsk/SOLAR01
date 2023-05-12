////////////////////////////////////////////////////////////////////////////////
// Based on the Article of http://www.songho.ca/opengl/gl_mvc.html
// 
// ModelGL.cpp 
// Model component of OpenGL
//
//////////////////////////////////////////////////////////////////////////////
// winGlRender.cpp
// Modfier: M. Nakano (mnsk.kn@outlook.jp)
// Modified: Jan 2023
///////////////////////////////////////////////////////////////////////////////

#include "winGlRender.h"

GlRender::GlRender(int idchild) : idChild(idchild), windowResized(false), 
	nearPlane(NEAR_PLANE), farPlane(FAR_PLANE), gridEnabled(false), gridSize(GRID_SIZE), gridStep(GRID_STEP), cam2EcripticEnabled(true), motiondataUpdateEnabled{ false } 
{
}

///////////////////////////////////////////////////////////////////////////////
// destructor
///////////////////////////////////////////////////////////////////////////////
GlRender::~GlRender()
{
	// deallocate framebuffer
	//delete [] frameBuffer;
	//frameBuffer = 0;
	//Gdiplus::GdiplusShutdown(gdiplusToken);
}

//Initialize OpenGL API
///////////////////////////////////////////////////////////////////////////////
// OpemGL initialization Process
///////////////////////////////////////////////////////////////////////////////
void GlRender::init()
{
	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_ALWAYS);
	glDepthFunc(GL_LEQUAL);
	//glDepthFunc(GL_GEQUAL);
	glDepthMask(GL_TRUE);
	glDepthRange(depthZNear, depthZFar);
	glEnable(GL_DEPTH_CLAMP);

	// Axes lines
	Tools::genMeshAxes(axesLines, GRID_SIZE);
	glGenVertexArrays(1, &axesVao);	glGenBuffers(1, &axesVbo); glGenBuffers(1, &axesEbo);
	Tools::bindBufferLines(axesLines, axesVao, axesVbo, axesEbo);

	// Grid lines
	Tools::genMeshGridXZ(gridLines, GRID_SIZE, GRID_STEP, glm::vec4{ 0.1f, 0.1f, 0.1f, 0.5f });
	glGenVertexArrays(1, &gridVao);	glGenBuffers(1, &gridVbo); glGenBuffers(1, &gridEbo);
	Tools::bindBufferLines(gridLines, gridVao, gridVbo, gridEbo);
	GL_ERROR_CHECK;

	// solar object , including  solar planets
	solar = std::make_shared<SolarSys>(idChild);
	solar->initialSetup(); // debug bool

	WIN_LOG("GlRender Initialization Process completed: Child Window=" << idChild);

}

///////////////////////////////////////////////////////////////////////////////
// clean up OpenGL objects
///////////////////////////////////////////////////////////////////////////////
void GlRender::quit()
{
}

///////////////////////////////////////////////////////////////////////////////
// swap OpenGL frame buffers
///////////////////////////////////////////////////////////////////////////////
void GlRender::swapBuffers()
{
	SwapBuffers(getDC());
}


///////////////////////////////////////////////////////////////////////////////
// toggle to resize window
///////////////////////////////////////////////////////////////////////////////
void GlRender::resizeWindow(int w, int h)
{
	// assign the width/height of viewport
	windowWidth = w;
	windowHeight = h;
	windowResized = true;
}


glm::mat4 GlRender::calcModelMat4(SolarSys::MotionDataFormat& star)
{

	// OPenGL coordinates in pixel; to Orbit
	glm::mat4 Model = glm::mat4{ 1.0f };
	Model = glm::translate(Model, star.scaledPosition);

	//obliquity - axis tilt
	Model = glm::rotate(Model, -glm::radians(solar->starsData.at(star.starId).obliquity), glm::vec3(0.0f, 0.0f, 1.0f));

	//rotation
	float i;// integer part

	float rotationAngle = solar->PI2 * glm::modf(star.systimer / solar->starsData.at(star.starId).rotationPeriod, i);
	Model = glm::rotate(Model, rotationAngle, glm::vec3(0.0f, 1.0f, 0.0f));

	// scaling
	//float size = solar->constant.at(star.starId).size * 1.0f;
	float size = solar->starsData.at(star.starId).size * 1.0f;
	Model = glm::scale(Model, glm::vec3(size));

	return Model;
}


void GlRender::draw(int idChildWindow)
{

	glViewport(0, 0, windowWidth, windowHeight);
	glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
	//glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
	//glClearColor(0.5f, 0.5f, 0.5f, 0.0f);  // debug
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	if ((windowWidth == 0) || (windowHeight == 0))
	{
		// could happen due to timing of WM_SIZE
		WIN_LOG_ERROR("g_displayWidth=" << windowWidth << ", g_displayHeight=" << windowHeight);
	}

	LastMemberOfMotionData = solar->motionDataContainer.back();
	MidMemberOfMotionData = solar->motionDataContainer[SolarSys::midIndex]; // used for rendering
	FrontMemberOfMotionData = solar->motionDataContainer.front();

	drawStars();

	if (gridEnabled)
	{
		// drawGridXZ(); nrrd to modify
	}

	if (motiondataUpdateEnabled)
	{
		solar->updateMotionData();  
	}
	
}

void GlRender::drawStars()
{

	glm::mat4 Projection{ 1.0f };
	glm::mat4 View{ 1.0f };
	glm::mat4 Model{ 1.0f };

	for (SolarSys::MotionDataFormat& star : MidMemberOfMotionData)
	{

		switch (idChild)
		{
		case 1: // window to show a target star: this therad normaly created in the second: may be called CAM_2 camera
		{
			// projection matrix
			Projection = projection(windowWidth, windowHeight, camera->getFov());

			Model = calcModelMat4(star);

			// view matrix: change eyeposition and angles to better show a target star
			if (cam2EcripticEnabled)
			{
				setCam2OnEcripticOrbit(camera->angleToLookAt);
			}
			else
			{
				setCam2OnEquatorOfPlanet(camera->angleToLookAt); // need to constantly update cam2
			}
			View = camera->getViewMatrix();

			break;
		}
		default: // window to show the whole solar stars
		{
			// projection matrix
			Projection = projection(windowWidth, windowHeight, camera->getFov());
			// view matrix
			View = camera->getViewMatrix();

			Model = calcModelMat4(star);
		}
		}

		// do here last scaling  due to gltf file; mesh size differencies
		switch (star.starId)
		{
		case 0: // Sun
		{
			Model = glm::scale(Model, glm::vec3{ 1.0f });
			ModelSun = Model;
			break;
		}
		case 6: // Phobos
		{
			Model = glm::scale(Model, glm::vec3{ 1.0f / 15.0f });
			break;
		}
		case 7: // Deimos
		{
			Model = glm::scale(Model, glm::vec3{ 1.0f / 10.0f });
			break;
		}
		default:
			Model = glm::scale(Model, glm::vec3{ 1.0f / 1000.0f });
		}
		//// do here last scaling  due to gltf file; mesh size differencies
		//if (star.starId == 0)  // Sun
		//{
		//	Model = glm::scale(Model, glm::vec3{ 1.0f });
		//	ModelSun = Model;
		//}
		//else // others
		//{
		//	Model = glm::scale(Model, glm::vec3{ 1.0f / 1000.0f });
		//}

		Shader::Shader* shaderPtr = solar->starsData.at(star.starId).shaderPtr;

		if (solar->starsData[star.starId].starName == "NULL")
		{
			WIN_LOG_ERROR("idChild="<< idChild << "star=" << star.starId  << "shadeId=" << solar->starsData[star.starId].shaderId);
		}

		shaderPtr->useShader();
		shaderPtr->setMat4Shader("model", Model);
		shaderPtr->setMat4Shader("modelsun", ModelSun);
		shaderPtr->setMat4Shader("view", View);
		shaderPtr->setMat4Shader("projection", Projection);
		shaderPtr->setIntShader("starId", star.starId);
		shaderPtr->setFloatShader("F0value", solar->starsData[star.starId].F0);

		solar->starsData.at(star.starId).starModel->drawModel(shaderPtr);

	}

	// at last, update data on dialog window
	SendMessage(dialogHandle, WM_COMMAND, IDC_LABEL_SOLAR_DAYS, BN_CLICKED);
}


void GlRender::setCam2OnEcripticOrbit(float angle)
{
	// find parent star of the target planet
	int parentStarId = solar->starsData[targetStarIdCAM2].satteliteOfId;

	glm::vec3 lastPosition = LastMemberOfMotionData[targetStarIdCAM2].scaledPosition;
	glm::vec3 midPosition = MidMemberOfMotionData[targetStarIdCAM2].scaledPosition;
	glm::vec3 frontPosition = FrontMemberOfMotionData[targetStarIdCAM2].scaledPosition;

	// up
	glm::vec3 Up = glm::normalize(glm::cross((frontPosition - FrontMemberOfMotionData[parentStarId].scaledPosition), (lastPosition - LastMemberOfMotionData[parentStarId].scaledPosition)));

	//eye direction
	glm::vec3 unitEyeDirection = frontPosition - lastPosition;
	unitEyeDirection = glm::rotate(glm::mat4(1.0f), glm::radians(angle), Up) * glm::vec4(unitEyeDirection, 0.0f);
	unitEyeDirection = glm::normalize(unitEyeDirection);

	//eye
	glm::vec3 cam2Position = midPosition
		+ (+unitEyeDirection) * solar->starsData[targetStarIdCAM2].size * 2.0f  // distance adjustment due to star size
		+ camera->deltaPosition; // adjust camera position via slider of dialog window

	// target
	glm::vec3 cam2Target = midPosition + 
		camera->deltaTarget; // adjust camera target via slider of dialog window

	if (targetStarIdCAM2 == 0) // Sun
	{
		camera->lookAt(camera->initViewPos, midPosition);
	}
	else
	{
		camera->lookAt(cam2Position, cam2Target, Up); // not work; Up pertubating
	}

	// euler
	glm::vec3 cam2Eulers = camera->deltaEulers;
	camera->setRotation(cam2Eulers);

}

void GlRender::setCam2OnEquatorOfPlanet(float angle)
{
	// find parent star of the target planet
	int parentStarId = solar->starsData[targetStarIdCAM2].satteliteOfId;

	SolarSys::MotionDataFormat targetPlanet = MidMemberOfMotionData[targetStarIdCAM2];

	glm::mat4 Model = calcModelMat4(targetPlanet);

	glm::vec3 ypoint = Model * glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	glm::vec3 ydirection = glm::normalize(ypoint - targetPlanet.scaledPosition);
	glm::vec3 zpoint = Model * glm::vec4(.0f, 0.0f, 1.0f, 1.0f);
	glm::vec3 zdirection = glm::normalize(zpoint - targetPlanet.scaledPosition);

	glm::vec3 xpoint = glm::vec4(targetPlanet.scaledPosition, 0.0f) + glm::rotate(glm::mat4(1.0f), glm::radians(angle), ydirection) * glm::vec4(zdirection, 0.0f);
	glm::vec3 xdirection = glm::normalize(xpoint - targetPlanet.scaledPosition);

	// eye position ; // adjust camera position via slider of dialog window
	glm::vec3 cam2Position = xpoint + xdirection * solar->starsData[targetStarIdCAM2].size * 2.0f + 
		camera->deltaPosition;

	// target position
	glm::vec3 unitEyeDirection = targetPlanet.scaledPosition - xpoint;
	unitEyeDirection = glm::normalize(unitEyeDirection);
	glm::vec3 cam2Target = targetPlanet.scaledPosition + 
		camera->deltaTarget; // adjust camera target via slider of dialog window

	// Up vector 
	glm::vec3 Up = glm::normalize(ydirection);

	camera->lookAt(cam2Position, cam2Target, Up); // 

	// euler angle
	glm::vec3 cam2Eulers = camera->deltaEulers;
	camera->setRotation(cam2Eulers);

}


glm::mat4  GlRender::projection(int w, int h, float fov, ProjectionType projectiontype) {

	ProjectionType ptype = projectiontype;
	glm::mat4 projectionMat4{ 1.0f };

	switch (ptype) {
	case ProjectionType::Pers:
		projectionMat4 = glm::perspective(glm::radians(fov), (float)w / (float)h, nearPlane, farPlane);
		break;
	case ProjectionType::PersFOV:
		projectionMat4 = glm::perspectiveFov(glm::radians(fov), (float)w, (float)h, nearPlane, farPlane);
		break;
	case ProjectionType::InfinitePers:
		projectionMat4 = glm::infinitePerspective(glm::radians(fov), (float)w / (float)h, nearPlane);
		break;
	case ProjectionType::Ortho:
		projectionMat4 = glm::ortho(0.0f, float(w), 0.0f, float(h));
		break;
	case ProjectionType::PixelOrtho://??
		projectionMat4 = glm::orthoZO(0.0f, float(w), 0.0f, float(h), nearPlane, farPlane);
		break;
	}

	return projectionMat4;

};


///////////////////////////////////////////////////////////////////////////////
// draw a grid on XZ-plane
///////////////////////////////////////////////////////////////////////////////
void GlRender::drawGridXZ(Shader::Shader* shader, glm::mat4& model, glm::mat4& view, glm::mat4& projection)
{
	shader->useShader();
	shader->setMat4Shader("model", model);
	shader->setMat4Shader("view", view);
	shader->setMat4Shader("projection", projection);
	Tools::drawElements(gridVao, gridVbo, gridEbo, GL_LINES, (GLuint)gridLines.indices.size(), 0);

	Tools::drawElements(axesVao, axesVbo, axesEbo, GL_LINES, (GLuint)axesLines.indices.size(), 0);

	glUseProgram(0);
}


///////////////////////////////////////////////////////////////////////////////
// compute grid size and step
///////////////////////////////////////////////////////////////////////////////
void GlRender::setGridSize(float size)
{
	gridSize = size;
	gridStep = GRID_STEP;
}

#pragma once

#define WIN32_LEAN_AND_MEAN             // exclude rarely-used stuff from Windows headers
#include <windows.h>    // include windows.h to avoid thousands of compile errors even though this class is not depending on Windows
#include <fstream>
#include <iostream>
//#include <functional>
#include <map>
// <WinUser.h>
//#include <vector>
#include<stdio.h>

#include <glad/glad.h>
#include <glm/ext/matrix_transform.hpp>

#include <boost/algorithm/string.hpp>
//#include <boost/range/combine.hpp>

#include <mylibrary/shader.h>
#include <mylibrary/tools.h>
#include <mylibrary/LoaderGLTF.h>


class SolarSys
{
	// The Theory of Gravitation
	// https://www.feynmanlectures.caltech.edu/I_07.html

public:
	const std::string Debug_Output_Folder = pathToResources("SolarSys/motiondataArchive/debug/");
	const bool StarsData_Debug{ true };
	const bool MotionData_Debug{ true };
	bool MotionDataHeader_Write{ false };

	const int Chunk = 140;;

	const float PI = 3.1415926f;
	const float PI2 = 2.0f * PI;
	bool isConstantsCreated{ false };

	float Starttime = 0.0f;;
	const float Timestep = 0.01f;;    //unit:days
	static const int midIndex = 50;
	const int motionDataContainerSize = midIndex * 2 + 1;  // odd number
	const int N = 2;       // unit : a round of earth orbit
	//const int Repeats = int(365 * N / Timestep);
	const std::string starsdataFile = pathToResources("SolarSys/StarsData/solarsys_starsdata.csv");


	int tailLineLength{ 0 };
	const float Gm = 6.674300e-11f; // Gravity constant  6.67 x 10(-11) Nm(2)/kg(2)
	const float Am = 1.495979e+11f; // Astornomical unit in meter
	const float Am2 = pow(Am, 2.0f);
	const float Pau = 60.0f; //one au in pixel, pixel / au
	const float Px2 = pow(Pau, 2.0f);
	const float Gpx0 = (Gm / Am2) * Px2; //Gm now converted to display unit
	//coeffcient to calculate velociy in pixel / day
	const float Ds = 60.0f * 60.0f * 24.0f; //seconds / day
	const float Pday = (1.0f / Am) * Pau * Ds; // pixel / day: velocity(m / s) * Pday :
	const float Gpx = Gpx0 * (Pau * pow(Ds, 2.0f) / Am); //Newton : converted to kg •Å@pixel / day(2)

	int numRecordsReadIn{ 0 };

	struct MotionDataFormatCSV {
		int repeat{};
		float systimer{};
		int starId{};
		std::string starName{};
		float scaledPositionX{};
		float scaledPositionY{};
		float scaledPositionZ{};
		float positionX{};
		float positionY{};
		float positionZ{};
		float velocityX{};
		float velocityY{};
		float velocityZ{};
		float accelerationX{};
		float accelerationY{};
		float accelerationZ{};
	};

	struct MotionDataFormat {
		int repeat{ 0 };
		float systimer{ 0.0f };
		int starId{ 0 };
		glm::vec3 scaledPosition{ 0.0f }; // in pixel
		glm::vec3 position{ 0.0f }; // in pixel
		glm::vec3 velocity{ 0.0f }; // in pixel
		glm::vec3 acceleration{ 0.0f }; // in pixel

		glm::dvec3 positionM{ 0.0 }; // in m, double
		glm::dvec3 velocityM{ 0.0 }; // in m/s, double
		glm::dvec3 accelerationM{ 0.0 }; // in m/s*2, double

		glm::vec3 momentum{ 0.0f };
		glm::vec3 orbitalAngularMomentum{ 0.0f };
		glm::vec3 spinAngularMomentum{ 0.0f };
	};

	struct starData {
		int starId{ 0 };
		std::string starName{ "NULL" };
		bool modelToUse{ false };
		std::string vert{};
		std::string frag{};
		std::string tesc{};
		std::string tese{};
		std::string geom{};
		Shader::Shader* shaderPtr{ nullptr };
		GLuint shaderId{ 0 };
		std::string modelFilePath{};
		LoaderGLTF::glTFLoader* starModel{ nullptr };
		float F0{ 0.0f };
		float size{ 0.0f };
		int satteliteOfId{ 0 };
		glm::vec3 pixelScaler{ 0.0f };
		float mass{ 0.0f }; // Kg
		float radius{ 0.0f }; // Km
		float obliquity{ 0.0f }; // in degree, against its orbit plane
		float inclination{ 0.0f }; // in degree, to the ecliptic - earth orbital plane
		float rotationPeriod{ 0.0f }; // day
		float rotationVelocity{ 0.0f }; //km/s
		float orbitalPeriod{ 0.0f }; // day
		float orbitalSpeed{ 0.0f }; // km/s
		float distanceToParent{ 0.0f }; // km
		float distanceToParentAu{ 0.0f };
		glm::vec3 initPosition{ 0.0f }; // km , i.e. distance from parent
		glm::vec3 initVelocity{ 0.0f }; // m / s
		glm::vec3 initAcceleration{ 0.0f }; // m / s*2
	};

	//enum class subDataType {
	//	model,
	//	view,
	//	projection
	//};

	enum class DifferentialAlgorism {
		feynman,
		rungekutta
	};

	std::map<int, starData> starsData{}; // key: starId

	MotionDataFormat motionData{};
	std::vector<MotionDataFormat> motionDataContainerRow;
	std::vector<std::vector<MotionDataFormat>> motionDataContainer;

	SolarSys::SolarSys(int windowid);
	~SolarSys();

	// members
	std::vector<std::vector<std::string> > readFileAll(std::string filename, bool headerpass = true, std::string delm = ",");
	bool iterateMotionFileByChunk(std::string filename, int chunk, std::function<void(std::vector<MotionDataFormatCSV>&)> callback, bool headerpass = true, std::string delm = ",");

	void genStarsData(std::string filename);

	// for Read In
	void SolarSys::initialSetup(std::string motionfile, std::string starsdatafile);
	// for AlgorismInside
	void initialSetup();

	void drawStarsByFileChunk(std::function<void(std::vector<MotionDataFormatCSV>&)> callback);
	void genMotionData(int containersize);  // Note:: constainer size be odd number: when drawing, use vale at mid index, e,.g, 100 here
	void updateMotionData();
	void debugWriteMotionData(bool debug);
	void debugWriteStarsData(std::vector<std::vector<std::string> > data, bool debug);

private:

	void scalePixelPosition(std::vector<MotionDataFormat>& motion);
	void acceleration(std::vector<MotionDataFormat>& motion);
	void rungekutta(std::vector<MotionDataFormat>& motion);
	void rungekutta2(std::vector<MotionDataFormat>& motion); // test case

	std::string motionFile{};
	int windowId{ 0 }; // for debug
};

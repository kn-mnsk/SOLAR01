
#include <mylibrary/SolarSys.hpp>

SolarSys::SolarSys(int windowid)
{
	windowId = windowid; // for debug

};

SolarSys::~SolarSys() {};


// Function to fetch data from a CSV File
std::vector<std::vector<std::string> > SolarSys::readFileAll(std::string filename, bool headerpass, std::string delm)
{
	bool headerPass{ headerpass };
	std::string delimeter{ delm };
	std::ifstream infile(filename);
	if (!infile) {
		infile.close();
		ERROR_EXIT(filename);
	}

	std::vector<std::vector<std::string> > datalist{};

	std::string line = "";
	// Iterate through each line and split the content using delimeter
	datalist.clear();
	int count{ 0 };
	numRecordsReadIn = count;
	while (getline(infile, line))
	{
		if (headerPass) { // read trrough the header
			headerPass = false;
			continue;
		}

		std::vector<std::string> vec;
		boost::algorithm::split(vec, line, boost::is_any_of(delimeter));
		datalist.push_back(vec);
		count += 1;

	}
	// Close the File
	infile.close();
	return datalist;
}

bool SolarSys::iterateMotionFileByChunk(std::string filename, int chunk, std::function<void(std::vector<MotionDataFormatCSV>&)> callback, bool headerpass, std::string delm)
{
	bool headerPass{ headerpass };
	std::string delimeter{ delm };
	// open the File 
	std::ifstream infile(filename);

	if (!infile) {
		infile.close();

		ERROR_EXIT(filename);
	}

	std::vector<MotionDataFormatCSV> datalist{};

	MotionDataFormatCSV element{};
	std::string line{};
	std::vector<std::string> vecline{};
	// Iterate through each line and split the content using delimeter
	int countLine{ 0 };
	numRecordsReadIn = countLine;
	int repetition{ 0 };
	while (std::getline(infile, line)) {
		// Call teh given call back
		if (headerPass) { // read trrough the header
			headerPass = false;
			continue;
		}

		if (countLine > chunk) {
			callback(datalist);
			repetition += 1;
			datalist.clear();
			countLine = 1;
		}


		boost::algorithm::split(vecline, line, boost::is_any_of(delimeter));
		element = {
			std::stoi(vecline[0]),
			std::stof(vecline[1]),
			std::stoi(vecline[2]),
			vecline[3],
			std::stof(vecline[4]),
			std::stof(vecline[5]),
			std::stof(vecline[6]),
			std::stof(vecline[7]),
			std::stof(vecline[8]),
			std::stof(vecline[9]),
			std::stof(vecline[10]),
			std::stof(vecline[11]),
			std::stof(vecline[12]),
			std::stof(vecline[13]),
			std::stof(vecline[14]),
			std::stof(vecline[15]),
		};
		vecline.clear();
		datalist.push_back(element);

		countLine += 1;
		numRecordsReadIn += 1;

	}
	// Close the file

	callback(datalist);
	repetition += 1;
	std::cout << "Callback Counts: " << repetition << " Ended" << std::endl; // debug
	infile.close();
	return true;
}

// for Read_In
void SolarSys::initialSetup(std::string motionfile, std::string starsdatafile)
{
	motionFile = motionfile;

	genStarsData(starsdataFile);

}

// for AlgorismInside
void SolarSys::initialSetup() 
{
	genStarsData(starsdataFile);

	genMotionData(motionDataContainerSize);
	WIN_LOG("SolarSys::initialSetup() finished");
}


void SolarSys::genStarsData(std::string filename)
{
	// Read star data file
	std::vector<std::vector<std::string> > data = readFileAll(filename);
	WIN_LOG("Window = " << windowId << ", genStarsData::file = " <<filename.c_str());

	debugWriteStarsData(data, StarsData_Debug);  // debug

	starsData.clear();
	for (auto& d : data)
	{
		starData element;

		element.starId = std::stoi(d[0]);
		element.starName = d[1];
		element.modelToUse = std::stoi(d[2]);
		element.vert = pathToShader(d[3]);
		element.frag = pathToShader(d[4]);
		element.tesc = d[5] == "NULL" ? d[5] : pathToShader(d[5]);
		element.tese = d[6] == "NULL" ? d[6] : pathToShader(d[6]);
		element.geom = d[7] == "NULL" ? d[7] : pathToShader(d[7]);
		element.shaderPtr = new Shader::Shader{ element.vert.c_str(), element.frag.c_str(),
						element.tesc == "NULL" ? nullptr : element.tesc.c_str(),
						element.tese == "NULL" ? nullptr : element.tese.c_str(),
						element.geom == "NULL" ? nullptr : element.geom.c_str() };
		element.shaderId = element.shaderPtr->getShaderID();
		element.modelFilePath = (d[8] == "NULL") ? "NULL" : pathToResources(d[8]);
		element.starModel = (d[8] == "NULL") ? nullptr : new LoaderGLTF::glTFLoader{ pathToResources(d[8]), element.shaderPtr };
		element.F0 = std::stof(d[9]);
		element.size = std::stof(d[10]);
		element.satteliteOfId = std::stoi(d[11]);
		element.pixelScaler = glm::vec3{ std::stof(d[12]), std::stof(d[13]), std::stof(d[14]) };
		element.mass = std::stof(d[15]);
		element.radius = std::stof(d[16]);
		element.obliquity = std::stof(d[17]);
		element.inclination = std::stof(d[18]);
		element.rotationPeriod = std::stof(d[19]);
		element.rotationVelocity = std::stof(d[20]);
		element.orbitalPeriod = std::stof(d[21]);
		element.orbitalSpeed = std::stof(d[22]);
		element.distanceToParent = std::stof(d[23]);
		element.distanceToParentAu = std::stof(d[24]);
		element.initPosition = glm::vec3{ std::stof(d[25]), std::stof(d[26]), std::stof(d[27]) };
		element.initVelocity = glm::vec3{ std::stof(d[28]), std::stof(d[29]), std::stof(d[30]) };
		element.initAcceleration = glm::vec3{ 0.0f };

		//starsData.emplace(element.starId, element); // not work
		starsData[element.starId] = element;

		//debug
		if (starsData[element.starId].starName == "NULL")
		{
			ERROR_EXIT("starsData Generation::windowId =" << windowId << ", starId =" << element.starId << ", starName = " << starsData[element.starId].starName.c_str() << ",  shaderId = " << starsData[element.starId].shaderId);
		}

	}

	// debug
	for each (auto & d in starsData)
	{
		if (d.second.starName == "NULL")
		{
			Win::log("ERROR:: starsData Generation::windowId = % d, starId = % d, starName = % s, shaderId = % d, \n %s", windowId, d.first, d.second.starName.c_str(), d.second.shaderId, GET_FILE_LINE);
			exit(-1);
		}
		Win::log("starsData Generation:: windowId = %d, starId = %d, starName = %s, shaderId = %d", windowId, d.first, d.second.starName.c_str(), d.second.shaderId);
	}

}

void SolarSys::genMotionData(int containersize)
{
	// Note:: constainer size be odd number: when drawing, use vale at mid index
	//tailLineLength = int(containersize / timeStep);  // containersize in days

	motionData = {};
	motionDataContainerRow.clear();
	for (auto& c : starsData) {
		motionData.repeat = 0;// due to  initial value
		motionData.systimer = 0.0f;// due to initial value
		motionData.starId = c.first; // key, i.e. starId
		motionData.position = (c.second.initPosition * 1000.0f / Am) * Pau; // in pixel
		motionData.velocity = c.second.initVelocity * Pday; // in pixel

		motionData.positionM = glm::dvec3(c.second.initPosition) * 1000.0; // Km to m
		motionData.velocityM = glm::dvec3(c.second.initVelocity); // in m/s
		motionData.accelerationM = glm::dvec3(c.second.initAcceleration); // in m/s*2

		motionData.momentum = glm::vec3{ 0.0f };  // data to be set
		motionData.orbitalAngularMomentum = glm::vec3{ 0.0f };  // data to be set
		motionData.spinAngularMomentum = glm::vec3{ 0.0f };  // data to be set

		motionDataContainerRow.push_back(motionData); // vector array[100][14], to be coded as stack later
	}

	scalePixelPosition(motionDataContainerRow);
	debugWriteMotionData(MotionData_Debug); // print the initial value

	motionDataContainer.clear();
	for (int i = 1; i <= containersize; i++) 
	{
		for (auto& m : motionDataContainerRow)
		{
			m.systimer +=Timestep;
			m.repeat = i; 
		}
		rungekutta(motionDataContainerRow);
		scalePixelPosition(motionDataContainerRow);
		motionDataContainer.push_back(motionDataContainerRow);

		debugWriteMotionData(MotionData_Debug);  // print the first generated data
	}

}

void SolarSys::updateMotionData()
{

	debugWriteMotionData(MotionData_Debug);  // print the repeating motion data

	motionDataContainerRow.clear();
	motionDataContainerRow = motionDataContainer.back();
	// update tick timer and repeat
	for (auto& m : motionDataContainerRow)
	{
		m.systimer += Timestep;
		m.repeat += 1;
	}
	rungekutta(motionDataContainerRow);
	scalePixelPosition(motionDataContainerRow);
	motionDataContainer.push_back(motionDataContainerRow);
	motionDataContainer.erase(motionDataContainer.begin());

}

void SolarSys::drawStarsByFileChunk(std::function<void(std::vector<MotionDataFormatCSV>&)> callback)
{
	bool check = iterateMotionFileByChunk(motionFile, Chunk, callback);
};


void SolarSys::debugWriteStarsData(std::vector<std::vector<std::string> > data, bool debug)
{
	if (!debug) return;

	int size;
	const char* buffer;
	std::string delimiter = ",";
	std::string headline;
	std::string line;
	std::string fileName = Debug_Output_Folder + "StarsData_Win" + std::to_string(windowId) + "debug.csv";
	std::ofstream outfile;

	outfile.open(fileName, std::ios::out);

	headline =
		"starId" + delimiter +
		"starName" + delimiter +
		"modelToUse" + delimiter +
		"vert" + delimiter +
		"frag" + delimiter +
		"tesc" + delimiter +
		"tese" + delimiter +
		"geom" + delimiter +
		"modelFilePath" + delimiter +
		"size" + delimiter +
		"parentId" + delimiter +
		"scaledPositionX" + delimiter +
		"scaledPositionY" + delimiter +
		"scaledPositionZ" + delimiter +
		"mass" + delimiter +
		"radius" + delimiter +
		"obliquity" + delimiter +
		"inclination" + delimiter +
		"rotationPeriod" + delimiter +
		"rotationVelocity" + delimiter +
		"oribitalPeriod" + delimiter +
		"oribitalSpeed" + delimiter +
		"distanceToParent" + delimiter +
		"distanceToParentAU" + delimiter +
		"positionX" + delimiter +
		"positionY" + delimiter +
		"positionZ" + delimiter +
		"velocityX" + delimiter +
		"velocityy" + delimiter +
		"velocityZ" + delimiter + "\n";
	size = headline.size();
	buffer = new char[size];
	buffer = headline.c_str();
	outfile.write(buffer, size);

	for (auto& d : data)
	{
		line = {};
		line.append(d[0] + delimiter);
		line.append(d[1] + delimiter);
		line.append(d[2] + delimiter);
		line.append(d[3] + delimiter);
		line.append(d[4] + delimiter);
		line.append(d[5] + delimiter);
		line.append(d[6] + delimiter);
		line.append(d[7] + delimiter);
		line.append(d[8] + delimiter);
		line.append(d[9] + delimiter);
		line.append(d[10] + delimiter);
		line.append(d[11] + delimiter);
		line.append(d[12] + delimiter);
		line.append(d[13] + delimiter);
		line.append(d[14] + delimiter);
		line.append(d[15] + delimiter);
		line.append(d[16] + delimiter);
		line.append(d[17] + delimiter);
		line.append(d[18] + delimiter);
		line.append(d[19] + delimiter);
		line.append(d[20] + delimiter);
		line.append(d[21] + delimiter);
		line.append(d[22] + delimiter);
		line.append(d[23] + delimiter);
		line.append(d[24] + delimiter);
		line.append(d[25] + delimiter);
		line.append(d[26] + delimiter);
		line.append(d[27] + delimiter);
		line.append(d[28] + delimiter);
		line.append(d[29] + "\n");

		size = line.size();
		buffer = new char[size];
		buffer = line.c_str();

		outfile.write(buffer, size);
	}
	outfile.close();

}


void SolarSys::debugWriteMotionData(bool debug)
{
	if (!debug) return;

	int size;
	const char* buffer;
	std::string delimiter = ",";
	std::string headline;
	std::string line;

	std::string fileName = Debug_Output_Folder + "MotionData_Win" + std::to_string(windowId) + "debug.csv";

	std::ofstream outfile;
	outfile.open(fileName, std::ios::app);
	double k = (double)(Pau / Am);  // conversion factor: m to px

	if (!MotionDataHeader_Write)
	{
		outfile.open(fileName, std::ios::app);
		if (outfile.is_open())
		{  // delete existin file, amd then reopen;
			outfile.close();
			int res = remove(fileName.c_str());
			outfile.open(fileName, std::ios::app);
		}
		headline =
			"repeat" + delimiter +
			"systimer" + delimiter +
			"starId" + delimiter +
			"starName" + delimiter +
			"scaledPosXpx" + delimiter +
			"scaledPosYpx" + delimiter +
			"scaledPosZpx" + delimiter +
			"posXpx" + delimiter +
			"posXkm" + delimiter +
			"posYpx" + delimiter +
			"posYkm" + delimiter +
			"posZpx" + delimiter +
			"posZkm" + delimiter +
			"velXpx" + delimiter +
			"velXm" + delimiter +
			"velYpx" + delimiter +
			"velYm" + delimiter +
			"velZpx" + delimiter +
			"velZm" + delimiter +
			"accelXpx" + delimiter +
			"accelXm" + delimiter +
			"accelYpx" + delimiter +
			"accelYm" + delimiter +
			"accelZpx" + delimiter +
			"accelZm" + delimiter +
			"momentumX" + delimiter +
			"momentumY" + delimiter +
			"momentumZ" + delimiter +
			"orbitalAngularMomentumX" + delimiter +
			"orbitalAngularMomentumY" + delimiter +
			"orbitalAngularMomentumZ" + delimiter +
			"spinAngularMomentumX" + delimiter +
			"spinAngularMomentumY" + delimiter +
			"spinAngularMomentumZ" + "\n";
		size = headline.size();
		buffer = new char[size];
		buffer = headline.c_str();
		outfile.write(buffer, size);

		MotionDataHeader_Write = true;
	}

	for (auto& motion : motionDataContainerRow)
	{
		line = {};
		line.append(std::to_string(motion.repeat) + delimiter);
		line.append(std::to_string(motion.systimer) + delimiter);
		line.append(std::to_string(motion.starId) + delimiter);
		line.append(starsData.at(motion.starId).starName + delimiter);
		line.append(std::to_string(motion.scaledPosition.x) + delimiter);
		line.append(std::to_string(motion.scaledPosition.y) + delimiter);
		line.append(std::to_string(motion.scaledPosition.z) + delimiter);
		line.append(std::to_string(motion.position.x) + delimiter);
		line.append(std::to_string(motion.positionM.x * k) + delimiter);
		line.append(std::to_string(motion.position.y) + delimiter);
		line.append(std::to_string(motion.positionM.y * k) + delimiter);
		line.append(std::to_string(motion.position.z) + delimiter);
		line.append(std::to_string(motion.positionM.z * k) + delimiter);
		line.append(std::to_string(motion.velocity.x) + delimiter);
		//line.append(std::to_string(motion.velocityM.x * k) + delimiter);
		line.append(std::to_string(motion.velocityM.x) + delimiter);
		line.append(std::to_string(motion.velocity.y) + delimiter);
		//line.append(std::to_string(motion.velocityM.y * k) + delimiter);
		line.append(std::to_string(motion.velocityM.y) + delimiter);
		line.append(std::to_string(motion.velocity.z) + delimiter);
		//line.append(std::to_string(motion.velocityM.z * k) + delimiter);
		line.append(std::to_string(motion.velocityM.z) + delimiter);
		line.append(std::to_string(motion.acceleration.x) + delimiter);
		//line.append(std::to_string(motion.accelerationM.x * k) + delimiter);
		line.append(std::to_string(motion.accelerationM.x) + delimiter);
		line.append(std::to_string(motion.acceleration.y) + delimiter);
		//line.append(std::to_string(motion.accelerationM.y * k) + delimiter);
		line.append(std::to_string(motion.accelerationM.y) + delimiter);
		line.append(std::to_string(motion.acceleration.z) + delimiter);
		//line.append(std::to_string(motion.accelerationM.z * k) + delimiter);
		line.append(std::to_string(motion.accelerationM.z) + delimiter);
		line.append(std::to_string(motion.momentum.x) + delimiter);
		line.append(std::to_string(motion.momentum.y) + delimiter);
		line.append(std::to_string(motion.momentum.z) + delimiter);
		line.append(std::to_string(motion.orbitalAngularMomentum.x) + delimiter);
		line.append(std::to_string(motion.orbitalAngularMomentum.y) + delimiter);
		line.append(std::to_string(motion.orbitalAngularMomentum.z) + delimiter);
		line.append(std::to_string(motion.spinAngularMomentum.x) + delimiter);
		line.append(std::to_string(motion.spinAngularMomentum.y) + delimiter);
		line.append(std::to_string(motion.spinAngularMomentum.z) + "\n");

		size = line.size();
		buffer = new char[size];
		buffer = line.c_str();

		outfile.write(buffer, size);
	}



}


void SolarSys::scalePixelPosition(std::vector<MotionDataFormat>& motion)
{

	for (auto& s : motion) {
		int starId = s.starId;// 
		int parentId = starsData.at(starId).satteliteOfId; // need to subtrac 1 for vector index; Constant is map with key as starId;
		s.scaledPosition = (s.position - motion[parentId].position) * starsData.at(starId).pixelScaler + motion[parentId].position;
	}

}


void SolarSys::acceleration(std::vector<MotionDataFormat>& motion)
{
	// Note
	/*
	c : planets.constants  # no use
		d : motion data container
		pos : stars position vectors as argument
		pos = np.array(list(zip(d._xx, d._yy, d_zz)))
		see Feynman Physics
		calculate acceleration at time 0 and after
		a(t) = -Gpx * m * r / |r | (3)      a and r are vectors
		r : vector(in pixel)
		m : counter planet
		Gpx : gravity constant in pixel
		"""

	*/

	//# calcualte position vectors for each star to others, with itself centered
	//# deltaPos.ndim is 3 - dimention
	glm::vec3 A;// { 0.0f, 0.0f, 0.0f };
	glm::vec3 distanceR{ 0.0f, 0.0f, 0.0f };
	float distance{ 0.0f };
	float mass{ 0.0f };

	glm::dvec3 Ad;// { 0.0, 0.0, 0.0 };
	glm::dvec3 distanceRM;// { 0.0, 0.0, 0.0 };
	double distanceM{ 0.0 };
	double massD{ 0.0 };

	//int starId{ 0 };
	for (auto& me : motion) {
		A = glm::vec3{ 0.0f, 0.0f, 0.0f };
		Ad = glm::dvec3{ 0.0, 0.0, 0.0 };
		//distanceR = -s.position;
		for (auto& other : motion) {
			if (me.starId != other.starId) {
				// in pixel
				distanceR = other.position - me.position;
				distance = glm::length(distanceR);
				mass = starsData.at(other.starId).mass;
				A += Gpx * mass * distanceR / pow(distance, 3.0f);

				// in meter
				distanceRM = other.positionM - me.positionM;
				distanceM = glm::length(distanceRM); // in meter
				massD = (double)starsData.at(other.starId).mass;// *(double)starsData.at(s.starId).mass;
				Ad += (double)Gm * massD * distanceRM / pow(distanceM, 3.0);
			}
		}

		me.acceleration = A;
		me.accelerationM = Ad;
	}

	// Note

	/*
	R = np.array([posv3 - posv3[i - 1, :] for i in self.starId])

		#pair - wise distance
		distR = squareform(pdist(posv3))
		distR = distR.reshape((distR.shape + (1, )))  # conform to the same dimention as deltaPos for later calc.
		# counter mass
		mass = self.constant.mass.to_numpy() # although including self, mass* R makes itself none
		mass = mass.reshape((mass.shape + (1, )))  # conform to 2 dimensions

		with np.errstate(divide = 'ignore', invalid = 'ignore') :
		accel = (np.nan_to_num(self.Gpx * mass * R / np.power(distR, 3))).sum(axis = 1)
		#accel = accel.sum(axis = 1)   # we get acceleration by star

		return accel   # numpy ndarray
	*/

}

// Motion Algorism
void SolarSys::rungekutta(std::vector<MotionDataFormat>& motion)
{
	// The Fourth Order-Runge Kutta Method.
	// https://lpsa.swarthmore.edu/NumInt/NumIntFourth.html
	// 
	// https://math.libretexts.org/Courses/Monroe_Community_College/MTH_225_Differential_Equations/3%3A_Numerical_Methods/3.3%3A_The_Runge-Kutta_Method
	// 
	// https://cattech-lab.com/science-tools/category/lecture/lecture2/
	// 
	//value of integral f at a given var
	/*

	dy(t) / dt = y'(t) = f(y(t), t),    with y(t0) = y0
	h : time step

	k1 = f(y * (t0), t0) = y'*(t0)              p*(t0) is an approximation to p(t0)
	k2 = f(y * (t0)+k1 * h / 2, t0 + h / 2) = y'*(t0) + k1 * h/2
	k3 = f(y * (t0)+k2 * h / 2, t0 + h / 2) = y'*(t0) + k2 * h/2
	k4 = f(y * (t0)+k3 * h, t0 + h) = y'*(t0) + k3 * h

	y * (t0 + h) = y * (t0)+(k1 * 1 / 6 + k2 * 1 / 3 + k3 * 1 / 3 + k4 * 1 / 6) * h

	Example given as follows :
dv / dt = a(t), dp / dt = v(t), a = GM * mass / power(r, 2)
	initial values : t = t0, p(t0) = p0, v(t0) = v0, a(t0) = a0
	h : time step

	step1 : at t0
	a0 = a(p0)
	v0 = v(t0)
	p0 = p(t0)

	step2 : at t0 + h / 2
	v1 = v0 + a0 * h / 2
	p1 = p0 + v1 * h / 2
	a1 = a(p1)

	step3 : at t0 + h / 2
	v2 = v0 + a1 * h / 2
	p2 = p0 + v2 * h / 2
	a2 = a(p2)

	step4 : at t0 + h
	v3 = v0 + a2 * h
	p3 = p0 + v3 * h
	a3 = a(p3)

	finally, at t0 + h, we calculate :
p = p0 + (v0 / 6 + v1 / 3 + v2 / 3 + v3 / 6) * h
	v = v0 + (a0 / 6 + a1 / 3 + a2 / 3 + a3 / 6) * h
	a = a(p)

	we get(p, v, a)

	Repeating the above steps, we get the trajectory of a motion
	*/

	float h = Timestep; // day
	double dh = (double)Timestep * 24.0 * 60.0 * 60.0;// day to second
	// at t0(Now)
	std::vector<MotionDataFormat>K1 = motion;
	acceleration(K1);

	// at t0 + h/2
	std::vector<MotionDataFormat>K2 = K1;
	for (int i = 0; i < motion.size(); i++)
	{
		K2[i].position = K1[i].position + K1[i].velocity * h / 2.0f;
		K2[i].velocity = K1[i].velocity + K1[i].acceleration * h / 2.0f;

		K2[i].velocityM = K1[i].velocityM + K1[i].accelerationM * dh / 2.0;
		K2[i].positionM = K1[i].positionM + K1[i].velocityM * dh / 2.0;

	}
	acceleration(K2);

	// at t0 + h/2
	std::vector<MotionDataFormat>K3 = K2;
	for (int i = 0; i < motion.size(); i++)
	{
		K3[i].position = K1[i].position + K2[i].velocity * h / 2.0f;
		K3[i].velocity = K1[i].velocity + K2[i].acceleration * h / 2.0f;

		K3[i].positionM = K1[i].positionM + K2[i].velocityM * dh / 2.0;
		K3[i].velocityM = K1[i].velocityM + K2[i].accelerationM * dh / 2.0;
	}
	acceleration(K3);

	// at t0 + h
	std::vector<MotionDataFormat>K4 = K3;
	for (int i = 0; i < motion.size(); i++)
	{
		K4[i].position = K1[i].position + K3[i].velocity * h;
		K4[i].velocity = K1[i].velocity + K3[i].acceleration * h;

		K4[i].positionM = K1[i].positionM + K3[i].velocityM * dh;
		K4[i].velocityM = K1[i].velocityM + K3[i].accelerationM * dh;
	}
	acceleration(K4);

	// finaly calculate at t0 + h
	for (int i = 0; i < motion.size(); i++)
	{
		motion[i].position = K1[i].position + (K1[i].velocity / 6.0f + K2[i].velocity / 3.0f + K3[i].velocity / 3.0f + K4[i].velocity / 6.0f) * h;
		motion[i].velocity = K1[i].velocity + (K1[i].acceleration / 6.0f + K2[i].acceleration / 3.0f + K3[i].acceleration / 3.0f + K4[i].acceleration / 6.0f) * h;

		motion[i].positionM = K1[i].positionM + (K1[i].velocityM / 6.0 + K2[i].velocityM / 3.0 + K3[i].velocityM / 3.0 + K4[i].velocityM / 6.0) * dh;
		motion[i].velocityM = K1[i].velocityM + (K1[i].accelerationM / 6.0 + K2[i].accelerationM / 3.0 + K3[i].accelerationM / 3.0 + K4[i].accelerationM / 6.0) * dh;
	}
	acceleration(motion);

	//Note

	////debug
	//for (int i = 0; i < motion.size(); i++) {
	//	if (K1[i].position == motion[i].position) {
	//		std::cout << i << " Position  ->old motion==new motion: True" << std::endl;
	//	}
	//}
	//for (int i = 0; i < motion.size(); i++) {
	//	if (K1[i].velocity == motion[i].velocity) {
	//		std::cout << i << " Velocity  ->old motion==new motion: True" << std::endl;
	//	}
	//}
	//for (int i = 0; i < motion.size(); i++) {
	//	if (K1[i].acceleration == motion[i].acceleration) {
	//		std::cout << i << " Acceleration  ->old motion==new motion: True" << std::endl;
	//	}
	//}
	//// debug end
	/*
		// p0, vo are arguments
		a0 = self.acceleration(p0)
		p1 = p0 + v0 * h / 2
		v1 = v0 + a0 * h / 2

		a1 = self.acceleration(p1)

		v2 = v0 + a1 * h / 2
		p2 = p0 + v1 * h / 2
		a2 = self.acceleration(p2)

		v3 = v0 + a2 * h
		p3 = p0 + v2 * h
		a3 = self.acceleration(p3)

		v = v0 + (a0 / 6 + a1 / 3 + a2 / 3 + a3 / 6) * h
		p = p0 + (v0 / 6 + v1 / 3 + v2 / 3 + v3 / 6) * h
		a = self.acceleration(p)

		return (p, v, a)  //at next time step
	*/

}

void SolarSys::rungekutta2(std::vector<MotionDataFormat>& motion) // test case
{
	// The Fourth Order-Runge Kutta Method.
	// https://lpsa.swarthmore.edu/NumInt/NumIntFourth.html
	// 
	//value of integral f at a given var
	/*

	dy(t) / dt = y'(t) = f(y(t), t),    with y(t0) = y0
	h : time step

	k1 = f(y * (t0), t0) = y'*(t0)              p*(t0) is an approximation to p(t0)
	k2 = f(y * (t0)+k1 * h / 2, t0 + h / 2) = y'*(t0) + k1 * h/2
	k3 = f(y * (t0)+k2 * h / 2, t0 + h / 2) = y'*(t0) + k2 * h/2
	k4 = f(y * (t0)+k3 * h, t0 + h) = y'*(t0) + k3 * h

	y * (t0 + h) = y * (t0)+(k1 * 1 / 6 + k2 * 1 / 3 + k3 * 1 / 3 + k4 * 1 / 6) * h

	Example given as follows :
dv / dt = a(t), dp / dt = v(t), a = GM * mass / power(r, 2)
	initial values : t = t0, p(t0) = p0, v(t0) = v0, a(t0) = a0
	h : time step

	step1 : at t0
	a0 = a(p0)
	v0 = v(t0)
	p0 = p(t0)

	step2 : at t0 + h / 2
	v1 = v0 + a0 * h / 2
	p1 = p0 + v1 * h / 2
	a1 = a(p1)

	step3 : at t0 + h / 2
	v2 = v0 + a1 * h / 2
	p2 = p0 + v2 * h / 2
	a2 = a(p2)

	step4 : at t0 + h
	v3 = v0 + a2 * h
	p3 = p0 + v3 * h
	a3 = a(p3)

	finally, at t0 + h, we calculate :
p = p0 + (v0 / 6 + v1 / 3 + v2 / 3 + v3 / 6) * h
	v = v0 + (a0 / 6 + a1 / 3 + a2 / 3 + a3 / 6) * h
	a = a(p)

	we get(p, v, a)

	Repeating the above steps, we get the trajectory of a motion
	*/

	float h = Timestep; // day
	double dh = (double)(Timestep * Ds);// day to second
	// at t0(Now)
	std::vector<MotionDataFormat>K1 = motion;
	acceleration(K1);

	// at t0 + h/2
	std::vector<MotionDataFormat>K2 = K1;
	for (int i = 0; i < motion.size(); i++)
	{
		K2[i].position = K1[i].position + K1[i].velocity * h / 2.0f;
		K2[i].velocity = K1[i].velocity + K1[i].acceleration * h / 2.0f;

		K2[i].positionM = K1[i].positionM + K1[i].velocityM * dh / 2.0;
		K2[i].velocityM = K1[i].velocityM + K1[i].accelerationM * dh / 2.0;
	}
	acceleration(K2);

	// at t0 + h/2
	std::vector<MotionDataFormat>K3 = K2;
	for (int i = 0; i < motion.size(); i++)
	{
		K3[i].position = K1[i].position + K2[i].velocity * h / 2.0f;
		K3[i].velocity = K1[i].velocity + K2[i].acceleration * h / 2.0f;

		K3[i].positionM = K1[i].positionM + K2[i].velocityM * dh / 2.0;
		K3[i].velocityM = K1[i].velocityM + K2[i].accelerationM * dh / 2.0;
	}
	acceleration(K3);

	// at t0 + h
	std::vector<MotionDataFormat>K4 = K3;
	for (int i = 0; i < motion.size(); i++)
	{
		K4[i].position = K1[i].position + K3[i].velocity * h;
		K4[i].velocity = K1[i].velocity + K3[i].acceleration * h;

		K4[i].positionM = K1[i].positionM + K3[i].velocityM * dh;
		K4[i].velocityM = K1[i].velocityM + K3[i].accelerationM * dh;
	}
	acceleration(K4);

	// finaly calculate at t0 + h
	for (int i = 0; i < motion.size(); i++)
	{
		motion[i].position = K1[i].position + (K1[i].velocity / 6.0f + K2[i].velocity / 3.0f + K3[i].velocity / 3.0f + K4[i].velocity / 6.0f) * h;
		motion[i].velocity = K1[i].velocity + (K1[i].acceleration / 6.0f + K2[i].acceleration / 3.0f + K3[i].acceleration / 3.0f + K4[i].acceleration / 6.0f) * h;

		motion[i].positionM = K1[i].positionM + (K1[i].velocityM + 2.0 * K2[i].velocityM + 2.0 * K3[i].velocityM + K4[i].velocityM) * dh / 6.0;
		motion[i].velocityM = K1[i].velocityM + (K1[i].accelerationM + 2.0 * K2[i].accelerationM + 2.0 * K3[i].accelerationM + K4[i].accelerationM) * dh / 6.0;
	}
	acceleration(motion);

	//Note

	////debug
	//for (int i = 0; i < motion.size(); i++) {
	//	if (K1[i].position == motion[i].position) {
	//		std::cout << i << " Position  ->old motion==new motion: True" << std::endl;
	//	}
	//}
	//for (int i = 0; i < motion.size(); i++) {
	//	if (K1[i].velocity == motion[i].velocity) {
	//		std::cout << i << " Velocity  ->old motion==new motion: True" << std::endl;
	//	}
	//}
	//for (int i = 0; i < motion.size(); i++) {
	//	if (K1[i].acceleration == motion[i].acceleration) {
	//		std::cout << i << " Acceleration  ->old motion==new motion: True" << std::endl;
	//	}
	//}
	//// debug end
	/*
		// p0, vo are arguments
		a0 = self.acceleration(p0)
		p1 = p0 + v0 * h / 2
		v1 = v0 + a0 * h / 2

		a1 = self.acceleration(p1)

		v2 = v0 + a1 * h / 2
		p2 = p0 + v1 * h / 2
		a2 = self.acceleration(p2)

		v3 = v0 + a2 * h
		p3 = p0 + v2 * h
		a3 = self.acceleration(p3)

		v = v0 + (a0 / 6 + a1 / 3 + a2 / 3 + a3 / 6) * h
		p = p0 + (v0 / 6 + v1 / 3 + v2 / 3 + v3 / 6) * h
		a = self.acceleration(p)

		return (p, v, a)  //at next time step
	*/

}

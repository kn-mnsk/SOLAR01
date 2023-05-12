#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#include <Windows.h>
#include <WinBase.h>
#include <direct.h>
#include <fstream>
#include <sstream>
#include <string.h>
#include <vector>
#include <strsafe.h>

#include <objidl.h>
using namespace std;
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment(lib, "GdiPlus.lib")

#include <glad/glad.h>
#include <glm/ext/matrix_transform.hpp>
#include <stb_image/stb_helper.h>

#include <glimg/glimg.h>
#include <glimg/ImageCreatorExceptions.h>

#include <local_configuration/root_directory.h>


//using namespace Win;
#include <source/winLogs/Log.h>



#define ARRAY_COUNT( array ) (sizeof( array ) / (sizeof( array[0] ) * (sizeof( array ) != sizeof(void*) || sizeof( array[0] ) <= sizeof(void*))))
#define GET_FILE std::string( __FILE__ )
#define GET_LINE std::to_string( __LINE__ )
#define GET_FUNCTION std::string(__FUNCTION__)
#define GET_FILE_LINE  (GET_FILE  + ":" + GET_LINE).c_str()
#define GET_FILE_NAME ((std::string( __FILE__ )).substr((std::string( __FILE__ )).find_last_of("/\\") +1))


#define GL_FramebufferStatus_ERROR_CHECK { \
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);\
	if (status != GL_FRAMEBUFFER_COMPLETE) \
	{\
		std::ostringstream oss; oss<< Tools::glEnumToString(status); \
		Win::log( "GL_FramebufferStatus_ERROR_CHECK: \t%s; \tat line %s, %s of %s\n", (oss.str()).c_str(), GET_LINE.c_str(), GET_FUNCTION.c_str(), GET_FILE_NAME.c_str() ); \
	}\
}

#define GL_ERROR_CHECK {\
	GLenum glErr = glGetError();\
	if (glErr != GL_NO_ERROR) \
	{\
		std::ostringstream oss; oss<< glErr << "=" << Tools::glEnumToString(glErr); \
		Win::log( "GL_ERROR: \t%s; \tat line %s, %s of %s\n", (oss.str()).c_str(), GET_LINE.c_str(), GET_FUNCTION.c_str(), GET_FILE_NAME.c_str() );\
	} \
}	

#define GL_ERROR_CHECK2(arg) {\
	GLenum glErr = glGetError();\
	if (glErr != GL_NO_ERROR) \
	{\
		std::ostringstream oss; oss<< glErr << "=" << Tools::glEnumToString(glErr) <<  ": " << arg; \
		Win::log( "GL_ERROR: \t%s; \tat line %s, %s of %s\n", (oss.str()).c_str(), GET_LINE.c_str(), GET_FUNCTION.c_str(), GET_FILE_NAME.c_str() );\
	} \
}	

#define WIN_LOG( arg ) {\
	std::ostringstream oss; oss<< arg; \
	Win::log( "Log: \t%s; \tat line %s, %s of %s\n", (oss.str()).c_str(), GET_LINE.c_str(), GET_FUNCTION.c_str(), GET_FILE_NAME.c_str() );\
}
#define WIN_LOG_ERROR( arg ) {\
	std::ostringstream oss{}; oss << arg; \
	Win::log( "ERROR: \t%s; \tat line %s, %s of %s\n", (oss.str()).c_str(), GET_LINE.c_str(), GET_FUNCTION.c_str(), GET_FILE_NAME.c_str());\
}

#define ERROR_EXIT(arg) {\
	std::ostringstream oss; oss<<arg; \
	Tools::ErrorExit(GET_FILE_NAME, GET_FUNCTION, GET_LINE, oss.str());\
}				

// [LOWORD and HIWORD operations](https://stackoverflow.com/questions/52435029/loword-and-hiword-operations)
#define SETHIDWORD(dw, hw) LOWORD(dw) | (hw << 16)
#define SETLODWORD(dw, lw) (HIWORD(dw) << 16) | lw


// refer to Khongston https://github.com/khongton/Dynamic-Cubemaps

namespace Tools
{

	template <typename T>
	std::string NumberToString(T x) // refer to Khongston https://github.com/khongton/Dynamic-Cubemaps
	{
		std::ostringstream ss;
		ss << x;
		return ss.str();
	}

	const float PI = 3.14159265f;

	// screen settings
	static int SCR_WIDTH = 1280;
	static int SCR_HEIGHT = 960;

	static float lastX = (float)SCR_WIDTH / 2.0f;
	static float lastY = (float)SCR_HEIGHT / 2.0f;
	static bool firstMouse = true;
	// camera
	//glm::vec3 camPos(0.0f, 5.0f, 10.0f);
	//MyCamera::Camera camera = MyCamera::Camera{ camPos };

	struct Vertex2 {
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texcoord;
	};


	struct meshVertices {
		std::vector<Vertex2> vertices;
		std::vector<GLuint> indices;
	};

	struct LineVertex {
		glm::vec3 pos;
		glm::vec4 color;
		//glm::vec3 endpos;
		//glm::vec4 endcolor;
	};

	struct LineVertices {
		std::vector<LineVertex> segments; // to element
		std::vector<GLuint> indices; // two element
	};

	// 3 elements vector to glm::vec3
	template <class T>
	struct VEC3
	{
		operator glm::vec3() const;
		std::vector<float> vec{};
	};
	template <class T>
	VEC3<T>::operator glm::vec3() const
	{
		glm::vec3 v{};
		v.x = vec[0];
		v.y = vec[1];
		v.z = vec[2];
		return v;
	}


	struct ImageDataInfo
	{
		unsigned char* buffer;   // GDI image data
		int  width;
		int  height;
		int  bitCount;
		int  byteCount;
	};

	static unsigned char* bmBuffer{};

	static BITMAPINFO bitmapinfo{};
	static HBITMAP   hBitmap{};
	static Gdiplus::Bitmap* bitmap{};

	// Function declalation
	inline void Console_Fontsize(int a, int b);
	//inline void showOpenGLInfo(GLenum name);
	inline const GLubyte* showOpenGLInfo(GLenum name);
	std::vector<std::string> Get_Program_Infos();
	std::string GetCurrentDir();
	static HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);


	//load  texture from  BMP, ICON, GIF, JPEG, Exif, PNG, TIFF, WMF, and EMF file
	inline void swapRedBlue(unsigned char* data, int dataSize, int channelCount)
	{
		if (!data) return;
		if (channelCount < 3) return;            // must be 3 or 4
		if (dataSize % channelCount) return;     // must be divisible by the number of channels

		unsigned char tmp;
		int i;

		// swap the position of red and blue components
		for (i = 0; i < dataSize; i += channelCount)
		{
			tmp = data[i];
			data[i] = data[i + 2];
			data[i + 2] = tmp;
		}
	}

	inline void flipImage(unsigned char* data, int width, int height, int channelCount)
	{
		if (!data) return;

		int lineSize = width * channelCount;
		unsigned char* tmp = new unsigned char[lineSize];
		int half = height / 2;

		int line1 = 0;                          // first line
		int line2 = (height - 1) * lineSize;    // last line

		// scan only half of height
		for (int i = 0; i < half; ++i)
		{
			// copy line by line
			memcpy(tmp, &data[line1], lineSize);
			memcpy(&data[line1], &data[line2], lineSize);
			memcpy(&data[line2], tmp, lineSize);

			// move to next line
			line1 += lineSize;
			line2 -= lineSize;
		}

		// deallocate temp arrays
		delete[] tmp;
	}

	// to be depricated
	inline GLuint loadTexture(const char* path, bool flip_vertically);
	inline GLuint loadTexture_DDS(char const* path);
	inline GLuint loadTexture_HDR(char const* path, bool flip_vertically);
	inline GLuint loadTexture_NonHDR(char const* path, bool flip_vertically);
	// to be depricated


	inline const char* glEnumToString(GLenum err) // refer to Khongston https://github.com/khongton/Dynamic-Cubemaps
	{
		switch (err) {
		case GL_NO_ERROR:
			return "No error";
		case GL_INVALID_ENUM:
			return "Invalid enum";
		case GL_INVALID_VALUE:
			return "Invalid value";
		case GL_INVALID_OPERATION:
			return "Invalid operation";
		case GL_STACK_OVERFLOW:
			return "Stack overflow";
		case GL_STACK_UNDERFLOW:
			return "Stack underflow";
		case GL_OUT_OF_MEMORY:
			return "Out of memory";
		case GL_FRAMEBUFFER_UNDEFINED:
			return "GL_FRAMEBUFFER_UNDEFINED";
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
			return "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
			return "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";
		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
			return "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER";
		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
			return "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER";
		case GL_FRAMEBUFFER_UNSUPPORTED:
			return "GL_FRAMEBUFFER_UNSUPPORTED";
		case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
			return "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE";
		case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
			return "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS";
		case GL_NEAREST:
			return "GL_NEAREST";
		case GL_LINEAR:
			return "GL_LINEAR";
		case GL_NEAREST_MIPMAP_NEAREST:
			return "GL_NEAREST_MIPMAP_NEAREST";
		case GL_NEAREST_MIPMAP_LINEAR:
			return "GL_NEAREST_MIPMAP_LINEAR";
		case GL_LINEAR_MIPMAP_NEAREST:
			return "GL_LINEAR_MIPMAP_NEAREST";
		case GL_LINEAR_MIPMAP_LINEAR:
			return "GL_LINEAR_MIPMAP_LINEAR";
		case GL_CLAMP_TO_EDGE:
			return "GL_CLAMP_TO_EDGE";
		case GL_CLAMP_TO_BORDER:
			return "GL_CLAMP_TO_BORDER";
		case GL_MIRRORED_REPEAT:
			return "GL_MIRRORED_REPEAT";
		case GL_REPEAT:
			return "GL_REPEAT";
		case GL_MIRROR_CLAMP_TO_EDGE:
			return "GL_MIRROR_CLAMP_TO_EDGE";
		case GL_UNSIGNED_INT:
			return "GL_UNSIGNED_INT";
		case GL_UNSIGNED_BYTE:
			return "GL_UNSIGNED_BYTE";
		case GL_UNSIGNED_SHORT:
			return "GL_UNSIGNED_SHORT";
		case GL_INT:
			return "GL_INT";
		case GL_BYTE:
			return "GL_BYTE";
		case GL_SHORT:
			return "GL_SHORT";
		case GL_FLOAT:
			return "GL_FLOAT";
		case GL_ARRAY_BUFFER:
			return "GL_ARRAY_BUFFER";
		case GL_TRIANGLES:
			return "GL_TRIANGLES";
		case GL_TRIANGLE_STRIP:
			return "GL_TRIANGLE_STRIP";
		case GL_TRIANGLE_FAN:
			return "GL_TRIANGLE_FAN";
		default:
			return "Can't To_String";
		}
	}


	inline GLuint loadTextureDDS(char const*);
	inline GLuint loadTextureSTBI(char const*, bool);

	inline GLuint loadTextureGDI(const WCHAR*, bool);
	inline ImageDataInfo  getImageDataInfoGDI(const WCHAR*, bool);
	inline GLuint createTexture(unsigned char*, int&, int&, int&, int&);
	inline GLuint loadArrayTexture_NonHDR(std::vector<std::string> path, bool flip_vertically);
	inline GLuint loadArrayTexture_NonHDR();
	inline void genMeshSphere(GLuint x_segs, GLuint y_segs, meshVertices& trianglestrip, meshVertices& patches, float sphereRadius, bool debug);
	inline void genMeshTorus(GLfloat R, GLuint R_segs, GLfloat r, GLuint r_segs, meshVertices& trianglestrip, meshVertices& patches, bool debug);
	inline void genMeshParaboloid(GLfloat semimajor, GLfloat semiminor, GLuint elliptic_segs, GLfloat height, GLuint height_segs, meshVertices& trianglestrip, meshVertices& patches, bool debug);
	inline void genMeshCube(meshVertices& patches, LineVertices& normalline, float bendnormal, bool debug);
	inline void genMeshQuad(meshVertices& patches);
	inline void genMeshQuadXZ(meshVertices& patches);
	inline void genMeshQuadXY(meshVertices& patcches);
	inline void genMeshQuadXY2(meshVertices& patches, float quadside, int segs, bool debug);
	inline void genMeshFov(meshVertices& patches, float fov, float fovheight, float ratio);
	inline void genMeshGridXZ(LineVertices& gridlines, const float size, const float step, glm::vec4 gridlinecolor);
	inline void genMeshPoint(LineVertices& point, glm::vec3 pointpos, glm::vec4 pointcolor);
	inline void genMeshLine(LineVertices& line);
	inline void genMeshLine(LineVertices& line, glm::vec3 startpos, glm::vec3 endpos, glm::vec4 linecolor);
	inline void genMeshAxes(LineVertices& axes, float axislength);
	inline void genMeshAxes(LineVertices& axes);
	inline void drawElements(GLuint vao, GLuint vbo, GLuint ebo, GLenum mode, GLuint indicessize, const void* indices);
	inline void bindBufferTriangleStrip(meshVertices& trianglestrip, GLuint& vao, GLuint& vbo, GLuint& ebo);
	inline void bindBufferPatches(meshVertices& patches, GLuint& vao, GLuint& vbo, GLuint& ebo);
	inline void bindBufferLines(LineVertices& lines, GLuint& vao, GLuint& vbo, GLuint& ebo);
	inline void renderCubeMap(GLuint vao, GLuint vbo);
	inline void renderQuad();
	inline glm::mat3 genTBNMat3(glm::vec3 pos1, glm::vec3 pos2, glm::vec3 pos3, glm::vec2 uv1, glm::vec2 uv2, glm::vec2 uv3, glm::vec3 nm);
	inline void writeCubemap_png(std::string, GLuint, int, int, std::string);
	inline void writeCubemap_hdr(GLuint cubemapID, int width, int height, std::string filename);
	inline void writeTexture2D_bmp(GLuint textureID, int width, int height, std::string filename);
	//inline static void ErrorExit(std::string fileName, std::string  functionName, std::string lineNo, int value);
	inline static void ErrorExit(std::string fileName, std::string  functionName, std::string lineNo, std::string remarks = "");

	inline std::string lpcwstrTo_String(LPCWSTR lpcwstr)
	{
		std::wstring wstr(lpcwstr);
		return std::string(wstr.begin(), wstr.end());
	};
	inline void stringTo_wString(std::string& str, std::wstring& wst)
	{
		wst = std::wstring(str.begin(), str.end());
	};

	inline void Console_Fontsize(int a, int b) {
		PCONSOLE_FONT_INFOEX lpConsoleCurrentFontEx = new CONSOLE_FONT_INFOEX();
		lpConsoleCurrentFontEx->cbSize = sizeof(CONSOLE_FONT_INFOEX);
		GetCurrentConsoleFontEx(out, 0, lpConsoleCurrentFontEx);
		lpConsoleCurrentFontEx->dwFontSize.X = a;
		lpConsoleCurrentFontEx->dwFontSize.Y = b;
		SetCurrentConsoleFontEx(out, 0, lpConsoleCurrentFontEx);
	}

	inline const GLubyte* showOpenGLInfo(GLenum name)
	{
		const GLubyte* gl_string = glGetString(name);

		return gl_string;

	}


	inline std::vector<std::string> Get_Program_Infos()
	{
		std::vector<std::string> program_infos;

		const char* pValue = _pgmptr;
		std::string sValue = std::string(pValue);
		size_t size = sValue.length();

		size_t currentP = -1;
		size_t lastP;
		do {
			currentP += 1;
			lastP = currentP - 1;
			currentP = sValue.find("\\", currentP);

		} while (currentP < size);

		std::string name = sValue.substr(lastP + 1, size - 1);
		std::string execDir = sValue.substr(0, lastP - 1);
		size = name.length();
		name = name.erase(size - 4);
		program_infos.push_back(name);
		program_infos.push_back(execDir);

		char buff[FILENAME_MAX]; //create string buffer to hold path
		char* wk_ch = _getcwd(buff, FILENAME_MAX);
		std::string cwd(buff);
		program_infos.push_back(cwd);

		//WIN_LOG("Program Name: " << program_infos.at(0) << ", Exec Directory: " << program_infos.at(1) << ", Current Working Dir: " << program_infos.at(2));

		return program_infos;
	}


	inline std::string GetCurrentDir() {
		char buff[FILENAME_MAX]; //create string buffer to hold path
		char* wk_ch = _getcwd(buff, FILENAME_MAX);
		std::string cwd(buff);

		//WIN_LOG("Current Working Dir: " << cwd);
		//Win::log("Current Working Directory: %s", cwd);

		return cwd;
	}


	inline void debugMat4(std::string name, glm::mat4 matrix) {

		//WIN_LOG(name);
		//Win::log("%s :", name);
		for (int row = 0; row < 4; row++) {
			for (int column = 0; column < 4; column++) {
				//WIN_LOG("(colum=%d, row=%d) = %f, " << column, row, matrix[column][row]);
				//Win::log("(colum=%d, row=%d) = %f, ", column, row, matrix[column][row]);
			}
		}

	}

	inline void genMeshSphere(GLuint x_segs, GLuint y_segs, meshVertices& trianglestrip, meshVertices& patches, float sphereRadius = 1.0f, bool debug = false) {

		// Generate sphere vertices for GL_TRIANGLE_STRIP
		trianglestrip.vertices.clear();
		trianglestrip.indices.clear();

		std::vector<Vertex2> vertices{};
		std::vector<GLuint> indices{};
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texcoord;

		//std::vector<GLuint> indices;

		for (unsigned int y = 0; y <= y_segs; ++y)
		{
			for (unsigned int x = 0; x <= x_segs; ++x)
			{
				float xSegment = (float)x / (float)x_segs;
				float ySegment = (float)y / (float)y_segs;
				float theta = ySegment * PI; float phi = xSegment * 2.0f * PI;
				float xPos = sphereRadius * sinf(theta) * sinf(phi);
				float yPos = sphereRadius * cosf(theta);
				float zPos = sphereRadius * sinf(theta) * cosf(phi);

				position = glm::vec3(xPos, yPos, zPos);
				normal = glm::normalize(glm::vec3(xPos, yPos, zPos));
				texcoord = glm::vec2(xSegment, ySegment);

				vertices.push_back(Vertex2{ position, normal, texcoord });
			}
		}

		bool oddMeridian;
		//int k;
		for (int x = 0; x <= x_segs; ++x) {
			oddMeridian = x % 2;
			int k = (x + 1) % (x_segs + 1);
			if (!oddMeridian) {
				for (int y = 0; y <= y_segs; ++y) {
					indices.push_back(x + y * (y_segs + 1));
					indices.push_back(k + y * (y_segs + 1));
				}
			}
			else {
				for (int y = y_segs; y >= 0; --y) {
					indices.push_back(x + y * (y_segs + 1));
					indices.push_back(k + y * (y_segs + 1));
				}
			}
		}

		trianglestrip.vertices.clear();
		trianglestrip.indices.clear();
		trianglestrip = meshVertices{ vertices, indices };

		if (debug) { // DEBUG::output vertices and indices of trianglestrips
			std::ofstream trianglesIndices_file;
			trianglesIndices_file.open(pathToResources("debug/tools/spherePrianglesIdices.txt"));
			for (unsigned int i = 0; i < trianglestrip.indices.size(); i++) {
				trianglesIndices_file << trianglestrip.indices[i] << "\n";
			}
			trianglesIndices_file.close();

			std::ofstream trianglesPos_file;
			trianglesPos_file.open(pathToResources("debug/tools/spherePrianglesPosition.txt"));
			for (unsigned int i = 0; i < trianglestrip.vertices.size(); i++) {
				trianglesPos_file << trianglestrip.vertices[i].position.x << "," << trianglestrip.vertices[i].position.y << "," << trianglestrip.vertices[i].position.z << "\n";
			}
			trianglesPos_file.close();
		}


		// Tesselation - GL_PATCHES
		vertices.clear();
		indices.clear();
		int k, k_;
		for (int i = 0; i < trianglestrip.indices.size() - 2; i++) {
			bool oddIndex = i % 2;
			for (int j = 0; j < 3; j++) {
				k = (int)trianglestrip.indices[i + j]; // cw
				k_ = (int)trianglestrip.indices[(i + 2) - j]; // reverse ccw
				// let winding order be same
				if (!oddIndex) { // cw
					position = trianglestrip.vertices[k].position;
					normal = trianglestrip.vertices[k].normal;
					texcoord = trianglestrip.vertices[k].texcoord;
					vertices.push_back(Vertex2{ position, normal, texcoord });
				}
				else {// cw from ccw: reverse winding order in case of even index
					position = trianglestrip.vertices[k_].position;
					normal = trianglestrip.vertices[k_].normal;
					texcoord = trianglestrip.vertices[k_].texcoord;
					vertices.push_back(Vertex2{ position, normal, texcoord });
				}

				indices.push_back(3 * i + j);
			}
		}

		patches.vertices.clear();
		patches.indices.clear();
		patches = meshVertices{ vertices, indices };

		if (debug) { // DEBUG::output vertices and indices of patches
			std::ofstream patchesIndices_file;
			patchesIndices_file.open(pathToResources("debug/tools/spherePatchesIdices.txt"));
			for (unsigned int i = 0; i < patches.indices.size(); i++) {
				patchesIndices_file << patches.indices[i] << "\n";
			}
			patchesIndices_file.close();

			std::ofstream patchesPos_file;
			patchesPos_file.open(pathToResources("debug/tools/spherePatchesPosition.txt"));
			for (unsigned int i = 0; i < patches.vertices.size(); i++) {
				patchesPos_file << patches.vertices[i].position.x << "," << patches.vertices[i].position.y << "," << patches.vertices[i].position.z << "\n";
			}
			patchesPos_file.close();
		}
	}

	inline void genMeshTorus(GLfloat R, GLuint R_segs, GLfloat r, GLuint r_segs, meshVertices& trianglestrip, meshVertices& patches, bool debug = false) {
		// Generate vertices for GL_TRIANGLE_STRIP

		trianglestrip.vertices.clear();
		trianglestrip.indices.clear();

		std::vector<Vertex2> vertices{};
		std::vector<GLuint> indices{};
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texcoord;

		for (unsigned int j = 0; j <= R_segs; ++j)
		{
			for (unsigned int k = 0; k <= r_segs; ++k)
			{
				float RSegment = (float)j / (float)R_segs;
				float rSegment = (float)k / (float)r_segs;
				float theta = rSegment * 2.0f * PI;
				float phi = RSegment * 2.0f * PI;
				float xPos = (R + r * cosf(theta)) * sinf(phi);
				float yPos = r * sinf(theta);
				float zPos = (R + r * cos(theta)) * cosf(phi);

				position = glm::vec3(xPos, yPos, zPos);
				normal = position - R * glm::vec3(sinf(phi), 0.0f, cosf(phi));
				normal = glm::normalize(normal);
				texcoord = glm::vec2(RSegment, rSegment);

				vertices.push_back(Vertex2{ position, normal, texcoord });
			}
		}

		//bool oddMeridian;
		//int k;
		for (unsigned int k = 0; k <= r_segs; ++k) {
			//oddMeridian = j % 2;
			int m = (k + 1) % (r_segs + 1);
			//if (!oddMeridian) {
			for (unsigned int j = 0; j <= R_segs; ++j) {
				indices.push_back(k + j * (r_segs + 1));
				indices.push_back(m + j * (r_segs + 1));
			}
			/*}
			else {
				for (int k = r_segs; k >= 0; --k) {
					indices.push_back(j + j * (r_segs + 1));
					indices.push_back(m + k * (r_segs + 1));
				}
			}*/
		}

		trianglestrip.vertices.clear();
		trianglestrip.indices.clear();
		trianglestrip = meshVertices{ vertices, indices };

		if (debug) { // output vertices and indices of trianglestrips
			std::ofstream trianglesindices_file;
			trianglesindices_file.open(pathToResources("debug/tools/torusPrianglesidices.txt"));
			for (int i = 0; i < trianglestrip.indices.size(); i++) {
				trianglesindices_file << trianglestrip.indices[i] << "\n";
			}
			trianglesindices_file.close();

			std::ofstream trianglespos_file;
			trianglespos_file.open(pathToResources("debug/tools/torusPrianglesposition.txt"));
			for (int i = 0; i < trianglestrip.vertices.size(); i++) {
				trianglespos_file << trianglestrip.vertices[i].position.x << "," << trianglestrip.vertices[i].position.y << "," << trianglestrip.vertices[i].position.z << "\n";
			}
			trianglespos_file.close();
		}


		// Tesselation - GL_PATCHES
		vertices.clear();
		indices.clear();
		GLuint k, k_;
		for (unsigned int i = 0; i < trianglestrip.indices.size() - 2; i++) {
			bool oddIndex = i % 2;
			for (unsigned int j = 0; j < 3; j++) {
				k = trianglestrip.indices[i + j]; // cw
				k_ = trianglestrip.indices[i + 2 - j]; // reverse ccw
				// let winding order be same
				if (!oddIndex) { // cw
					position = trianglestrip.vertices[k].position;
					normal = trianglestrip.vertices[k].normal;
					texcoord = trianglestrip.vertices[k].texcoord;
					vertices.push_back(Vertex2{ position, normal, texcoord });
				}
				else {// cw from ccw: reverse winding order in case of even index
					position = trianglestrip.vertices[k_].position;
					normal = trianglestrip.vertices[k_].normal;
					texcoord = trianglestrip.vertices[k_].texcoord;
					vertices.push_back(Vertex2{ position, normal, texcoord });
				}

				indices.push_back(3 * i + j);
			}
		}

		patches.vertices.clear();
		patches.indices.clear();
		patches = meshVertices{ vertices, indices };

		if (debug) { // output vertices and indices of patches
			std::ofstream patchesIndices_file;
			patchesIndices_file.open("patchesIdices.txt");
			for (int i = 0; i < patches.indices.size(); i++) {
				patchesIndices_file << patches.indices[i] << "\n";
			}
			patchesIndices_file.close();

			std::ofstream patchesPos_file;
			patchesPos_file.open("patchesPosition.txt");
			for (int i = 0; i < patches.vertices.size(); i++) {
				patchesPos_file << patches.vertices[i].position.x << "," << patches.vertices[i].position.y << "," << patches.vertices[i].position.z << "\n";
			}
			patchesPos_file.close();
		}
	}


	inline void genMeshParaboloid(GLfloat semimajor, GLfloat semiminor, GLuint elliptic_segs, GLfloat height, GLuint height_segs, meshVertices& trianglestrip, meshVertices& patches, bool debug = false) {

		trianglestrip.vertices.clear();
		trianglestrip.indices.clear();

		patches.vertices.clear();
		patches.indices.clear();

		// Generate vertices for GL_TRIANGLE_STRIP

		std::vector<Vertex2> vertices{};
		std::vector<GLuint> indices{};
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texcoord;

		for (unsigned int j = 0; j <= height_segs; ++j)
		{
			for (unsigned int k = 0; k <= elliptic_segs; ++k)
			{

				float heightSeg = (float)j / (float)height_segs;
				float ellipticSeg = (float)k / (float)elliptic_segs;
				float u = heightSeg * height; // from 0.0 to height
				float v = ellipticSeg * 2.0f * PI; // from 0.0 to 2PI
				float xPos = semimajor * u * cosf(v);
				float yPos = glm::pow(u, 2.0f);
				float zPos = semiminor * u * sinf(v);

				position = glm::vec3(xPos, yPos, zPos);
				// nomal
				float c1 = glm::pow((2.0f * u * cosf(v) * semiminor), 2.0f) + glm::pow((2.0f * u * sinf(v) * semimajor), 2.0f) + glm::pow(semimajor * semiminor, 2.0f);
				float c2 = glm::pow(semimajor * semiminor, 2.0f) + 2.0f * glm::pow(u, 2.0f) * cosf(2.0f * v) * (-glm::pow(semimajor, 2.0f) + glm::pow(semiminor, 2.0f)) + 2.0f * glm::pow(u, 2.0f) * (glm::pow(semimajor, 2.0f) + glm::pow(semiminor, 2.0f));

				normal.x = 2.0f * u * cosf(v) * semiminor / glm::sqrt(c1);
				normal.y = -semimajor * semiminor / glm::sqrt(c2);
				normal.z = 2.0f * u * sinf(v) * semimajor / glm::sqrt(c1);
				normal = glm::normalize(normal);

				texcoord = glm::vec2(heightSeg, ellipticSeg);

				vertices.push_back(Vertex2{ position, normal, texcoord });
			}
		}

		bool oddMeridian;
		//int k;
		for (int k = 0; k <= elliptic_segs; ++k) {
			oddMeridian = k % 2;
			int m = (k + 1) % (elliptic_segs + 1);
			if (!oddMeridian) {
				for (int j = 0; j <= height_segs; ++j) {
					indices.push_back(k + j * (elliptic_segs + 1));
					indices.push_back(m + j * (elliptic_segs + 1));
				}
			}
			else {
				for (int j = height_segs; j >= 0; --j) {
					indices.push_back(k + j * (elliptic_segs + 1));
					indices.push_back(m + j * (elliptic_segs + 1));
				}
			}
		}

		trianglestrip.vertices.clear();
		trianglestrip.indices.clear();
		trianglestrip = meshVertices{ vertices, indices };

		if (debug) { // output vertices and indices of trianglestrips
			std::ofstream trianglesIndices_file;
			trianglesIndices_file.open(pathToResources("debug/tools/paraboloidTrianglesIdices.txt"));
			for (unsigned int i = 0; i < trianglestrip.indices.size(); i++) {
				trianglesIndices_file << trianglestrip.indices[i] << "\n";
			}
			trianglesIndices_file.close();

			std::ofstream trianglesPos_file;
			trianglesPos_file.open(pathToResources("debug/tools/paraboloidPrianglesPosition.txt"));
			for (unsigned int i = 0; i < trianglestrip.vertices.size(); i++) {
				trianglesPos_file << trianglestrip.vertices[i].position.x << "," << trianglestrip.vertices[i].position.y << "," << trianglestrip.vertices[i].position.z << "\n";
			}
			trianglesPos_file.close();
		}


		// Tesselation - GL_PATCHES
		vertices.clear();
		indices.clear();
		GLuint k, k_;
		for (unsigned int i = 0; i < trianglestrip.indices.size() - 2; i++) {
			bool oddIndex = i % 2;
			for (unsigned int j = 0; j < 3; j++) {
				k = trianglestrip.indices[i + j]; // cw
				k_ = trianglestrip.indices[i + 2 - j]; // reverse ccw
				// let winding order be same
				if (!oddIndex) { // cw
					position = trianglestrip.vertices[k].position;
					normal = trianglestrip.vertices[k].normal;
					texcoord = trianglestrip.vertices[k].texcoord;
					vertices.push_back(Vertex2{ position, normal, texcoord });
				}
				else {// cw from ccw: reverse winding order in case of even index
					position = trianglestrip.vertices[k_].position;
					normal = trianglestrip.vertices[k_].normal;
					texcoord = trianglestrip.vertices[k_].texcoord;
					vertices.push_back(Vertex2{ position, normal, texcoord });
				}

				indices.push_back(3 * i + j);
			}
		}

		patches.vertices.clear();
		patches.indices.clear();
		patches = meshVertices{ vertices, indices };

		if (debug) { // output vertices and indices of patches
			std::ofstream patchesIndices_file;
			patchesIndices_file.open(pathToResources("debug/tools/paraboloidPatchesIdices.txt"));
			for (int i = 0; i < patches.indices.size(); i++) {
				patchesIndices_file << patches.indices[i] << "\n";
			}
			patchesIndices_file.close();

			std::ofstream patchesPos_file;
			patchesPos_file.open(pathToResources("debug/tools/paraboloidPatchesPosition.txt"));
			for (int i = 0; i < patches.vertices.size(); i++) {
				patchesPos_file << patches.vertices[i].position.x << "," << patches.vertices[i].position.y << "," << patches.vertices[i].position.z << "\n";
			}
			patchesPos_file.close();
		}


	}

	// Generate cube 1x1 3D cube, as patches, no trianglestrip.
	inline void genMeshCube(meshVertices& patches, LineVertices& normalline, float bendnormal = 0.0f, bool debug = false) { // bendnormal in degree

		patches.vertices.clear();
		patches.indices.clear();

		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texcoord;

		//trianglestrip.vertices.clear();
		//trianglestrip.indices.clear();
		//position = glm::vec3(0.0f, 0.0f, 0.0f);
		//normal = glm::vec3(0.0f, 0.0f, 0.0f);
		//texcoord = glm::vec2(0.0f, 0.0f);
		//trianglestrip.vertices.push_back(Vertex2{ position, normal, texcoord });
		//trianglestrip.indices.push_back(0);


		std::vector<Vertex2> vertices{};

		std::vector<Vertex2> VNT{ // back face only of cube
			// bottom-left
			Vertex2{glm::vec3(0.5f, -0.5f, -0.5f),  // vertex
						glm::vec3(0.0f,  0.0f, -1.0f), // normal
						glm::vec2(0.0f, 0.0f)}, // texcord
						// top-left
					   Vertex2{glm::vec3(0.5f,  0.5f, -0.5f),
								   glm::vec3(0.0f,  0.0f, -1.0f),
								   glm::vec2(0.0f, 1.0f)},
								   // top-right
								   Vertex2{glm::vec3(-0.5f,  0.5f, -0.5f),
											   glm::vec3(0.0f,  0.0f, -1.0f),
											   glm::vec2(1.0f, 1.0f)},
											   // bottom-right
											   Vertex2{glm::vec3(-0.5f, -0.5f, -0.5f),
														   glm::vec3(0.0f,  0.0f, -1.0f),
														   glm::vec2(1.0f, 0.0f)},
														   //////////////////////////////////////////////////
														   // // top-left
														   //Vertex2{glm::vec3(1.0f,  1.0f, -1.0f),
														   //			glm::vec3(0.0f,  0.0f, -1.0f),
														   //			glm::vec2(0.0f, 1.0f)},
														   //// bottom-left
														   //Vertex2{glm::vec3(1.0f, -1.0f, -1.0f),  // vertex
														   //			glm::vec3(0.0f,  0.0f, -1.0f), // normal
														   //			glm::vec2(0.0f, 0.0f)}, // texcord
														   //// bottom-right
														   //Vertex2{glm::vec3(-1.0f, -1.0f, -1.0f),
														   //			glm::vec3(0.0f,  0.0f, -1.0f),
														   //			glm::vec2(1.0f, 0.0f)},
														   //// top-right
														   //Vertex2{glm::vec3(-1.0f,  1.0f, -1.0f),
														   //			glm::vec3(0.0f,  0.0f, -1.0f),
														   //			glm::vec2(1.0f, 1.0f)},
		};

		// orient normal -> manupulation of tessellation
		for (int i = 0; i < VNT.size(); i++) {
			glm::mat4 rotAxisX{ 1.0f };
			rotAxisX = glm::rotate(rotAxisX, glm::radians(-45.0f - 90.0f * i), glm::vec3(0.0f, 0.0f, 1.0f));
			glm::vec3 newX = rotAxisX * glm::vec4(glm::vec3(1.0f, 0.0f, 0.0f), 1.0f);
			glm::mat4 rot{ 1.0f };
			//bendnormal = glm::pow(-1.0f, i) * bendnormal;
			rot = glm::rotate(rot, glm::radians(bendnormal), newX);
			normal = glm::vec3(rot * glm::vec4(VNT[i].normal, 0.0f));
			VNT[i].normal = normal;
		}


		//Four faces; back face, right face, front face and left face, toward +Z axis
		for (int i = 0; i <= 3; i++) {
			glm::mat4 rotY{ 1.0f };
			rotY = glm::rotate(rotY, glm::radians(90.0f * i), glm::vec3(0.0f, 1.0f, 0.0f));
			for (int j = 0; j < VNT.size(); j++) {
				position = glm::vec3(rotY * glm::vec4(VNT[j].position, 1.0f));
				normal = glm::vec3(rotY * glm::vec4(VNT[j].normal, 0.0f));
				texcoord = VNT[j].texcoord;
				vertices.push_back(Vertex2{ position, normal, texcoord });
			}
		}

		//Top face and Bootm face
		for (int i = 1; i <= 3; i += 2) {
			glm::mat4 rotX{ 1.0f };
			rotX = glm::rotate(rotX, glm::radians(90.0f * i), glm::vec3(1.0f, 0.0f, 0.0f));
			for (int j = 0; j < VNT.size(); j++) {
				position = glm::vec3(rotX * glm::vec4(VNT[j].position, 1.0f));
				normal = glm::vec3(rotX * glm::vec4(VNT[j].normal, 0.0f));
				texcoord = VNT[j].texcoord;
				vertices.push_back(Vertex2{ position, normal, texcoord });
			}
		}

		std::vector<GLuint> indices{};

		for (int i = 0; i < vertices.size(); i += 4) {
			for (int j = 0; j <= 1; j++) {
				indices.push_back(i);
				indices.push_back(i + 1 + j);
				indices.push_back(i + 2 + j);
				//indices.push_back(i + 2 + j);
				//indices.push_back(i + 1 + j);
			}
		}

		patches.vertices.clear();
		patches.indices.clear();
		patches = meshVertices{ vertices, indices };

		if (debug) { // output vertices and indices of patches
			std::ofstream patchesIndices_file;
			patchesIndices_file.open(pathToResources("debug/tools/cubePatchesIdices.txt"));
			for (int i = 0; i < patches.indices.size(); i++) {
				patchesIndices_file << patches.indices[i] << "\n";
			}
			patchesIndices_file.close();

			std::ofstream patchesPos_file;
			patchesPos_file.open(pathToResources("debug/tools/cubePatchesPosition.txt"));
			for (int i = 0; i < patches.vertices.size(); i++) {
				patchesPos_file << patches.vertices[i].position.x << "," << patches.vertices[i].position.y << "," << patches.vertices[i].position.z << "\n";
			}
			patchesPos_file.close();
		}

		// normal line
		normalline.segments.clear();
		normalline.indices.clear();
		glm::vec4 whitecolor{ 0.0f, 0.0f, 0.0f, 1.0f };
		for (int i = 0; i < vertices.size(); i++) {
			normalline.segments.push_back(LineVertex{ vertices[i].position, whitecolor });
			normalline.segments.push_back(LineVertex{ (vertices[i].position + vertices[i].normal), whitecolor });
			normalline.indices.push_back(2 * i);
			normalline.indices.push_back(2 * i + 1);

		}

	}

	inline void genMeshQuadXZ(meshVertices& patches) {

		//generate patches
		patches.vertices.clear();
		patches.indices.clear();

		// XZ Plane
		patches.vertices = {
			// positions, normal, texture Coords
			Vertex2{glm::vec3(-1.0f,  0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f),			glm::vec2(0.0f, 1.0f)},
			Vertex2{glm::vec3(1.0f,  0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f),			glm::vec2(1.0f, 1.0f)},
			Vertex2{glm::vec3(1.0f,  0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f),			glm::vec2(1.0f, 0.0f)},
			Vertex2{glm::vec3(-1.0f,  0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f),			glm::vec2(0.0f, 0.0f)},
		};
		// setup plane VAO
		patches.indices = {
			0, 1, 3,
			1, 2, 3,
		};

	}

	inline void genMeshQuadXY(meshVertices& patches) {

		//generate patches
		patches.vertices.clear();
		patches.indices.clear();

		// XY Plane
		patches.vertices = {
			// positions, normal, texture Coords
			Vertex2{glm::vec3(-1.0f,  1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
			Vertex2{glm::vec3(-1.0f,  -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
			Vertex2{glm::vec3(1.0f,  1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f),	 glm::vec2(1.0f, 1.0f)},
			Vertex2{glm::vec3(1.0f,  -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f)},
		};
		// setup plane VAO
		patches.indices = {
			0, 2, 3,
			3, 1, 0,
		};

	}

	inline void genMeshQuadXY2(meshVertices& patches, float quadside = 2.0f, int segs = 2, bool debug = false) {

		patches.vertices.clear();
		patches.indices.clear();

		// set first element
		float x0{ -1.0f };
		float y0{ 1.0f };

		//float quadSide = 2.0f; // each side 2.0f long

		float interval = quadside / (float)segs;
		float x{ 0.0f }, y{ 0.0f }, z{ 0.0f };
		glm::vec3 position{ 0.0f };
		glm::vec3 normal{ 0.0f, 0.0f, 1.0f }; // always
		glm::vec2 texcoord{ 0.0f };


		for (int m = 0; m < segs + 1; m++) {
			y = y0 - m * interval;
			texcoord.y = 1.0f - m * (1.0f / (float)segs);
			for (int n = 0; n < segs + 1; n++) {
				x = x0 + n * interval;
				position = glm::vec3(x, y, z);
				texcoord.x = 0.0f + n * (1.0f / (float)segs);

				patches.vertices.push_back(Vertex2{ position, normal, texcoord });
			}

		}

		int numVertices = (segs + 1) * (segs + 1);
		for (int m = 0; m < segs; m++) {
			for (int n = 0; n < segs; n++) {
				patches.indices.push_back(n);
				patches.indices.push_back(n + 1);
				patches.indices.push_back(n + (m + 1) * (segs + 1));

				patches.indices.push_back(n + 1);
				patches.indices.push_back((n + 1) + (m + 1) * (segs + 1));
				patches.indices.push_back(n + (m + 1) * (segs + 1));
			}
		}

		if (debug) { // output vertices and indices of patches
			std::ofstream patchesIndices_file;
			patchesIndices_file.open(pathToResources("debug/tools/quadPatchesIdices.txt"));
			for (int i = 0; i < patches.indices.size(); i++) {
				patchesIndices_file << patches.indices[i] << "\n";
			}
			patchesIndices_file.close();

			std::ofstream patchesPos_file;
			patchesPos_file.open(pathToResources("debug/tools/quadPatchesPosition.txt"));
			for (int i = 0; i < patches.vertices.size(); i++) {
				patchesPos_file << patches.vertices[i].position.x << "," << patches.vertices[i].position.y << "," << patches.vertices[i].position.z << "\n";
			}
			patchesPos_file.close();
		}

	}

	inline void genMeshFov(meshVertices& patches, float fov, float fovheight, float ratio)
	{
		patches.vertices.clear();
		patches.indices.clear();

		std::vector<glm::vec3> fovVertices{};
		std::vector<glm::vec4> fovColors{}; // to be used in the future

		// compute 5 vertices of the fov
		float halfFov = glm::radians(fov * 0.5f);// deg to radian
		float top_x = tanf(halfFov * ratio) * fovheight;
		float top_y = tanf(halfFov) * fovheight;

		// origin 0
		fovVertices.push_back(glm::vec3{ 0.0f, 0.0f, 0.0f });
		fovColors.push_back(glm::vec4{ 0.5f, 0.5f, 0.5f, 0.0f });
		// top-left 1
		fovVertices.push_back(glm::vec3{ top_x , top_y, fovheight });
		fovColors.push_back(glm::vec4{ 0.5f, 0.5f, 0.5f, 0.8f });
		// bottom-left 2
		fovVertices.push_back(glm::vec3{ top_x, -top_y, fovheight });
		fovColors.push_back(glm::vec4{ 0.5f, 0.5f, 0.5f, 0.8f });
		// bottom-right 3
		fovVertices.push_back(glm::vec3{ -top_x , -top_y, fovheight });
		fovColors.push_back(glm::vec4{ 0.5f, 0.5f, 0.5f, 0.8f });
		// top-right 4
		fovVertices.push_back(glm::vec3{ -top_x, top_y, fovheight });
		fovColors.push_back(glm::vec4{ 0.5f, 0.5f, 0.5f, 0.8f });

		std::vector<unsigned int> faceIdx =
		{
			0, 2, 1,
			1, 4, 0,
			0, 4, 3,
			3, 2, 0,
			1, 2, 3,
			3, 4, 1
		};

		// compute patches
		glm::vec3 v1{ 0.0f };	glm::vec3 v2{ 0.0f }; 	glm::vec3 v3{ 0.0f };
		glm::vec3 n{ 0.0f }; glm::vec3 n1{ 0.0f }; 	glm::vec3 n2{ 0.0f }; 	glm::vec3 n3{ 0.0f };
		glm::vec2 tex{ 0.5f };
		for (GLuint i = 0; i < faceIdx.size(); i += 3)
		{
			v1 = fovVertices[faceIdx[i]];
			v2 = fovVertices[faceIdx[i + 1]];
			v3 = fovVertices[faceIdx[i + 2]];
			n1 = glm::normalize(glm::cross((v2 - v1), (v3 - v1)));
			n2 = glm::normalize(glm::cross((v1 - v2), (v3 - v2)));
			n3 = glm::normalize(glm::cross((v2 - v3), (v1 - v3)));
			n = (n1 + n2 + n3) / 3.0f;
			patches.vertices.push_back(Vertex2{ v1, n, tex });
			patches.vertices.push_back(Vertex2{ v2, n, tex });
			patches.vertices.push_back(Vertex2{ v3, n, tex });
			/*patches.indices.push_back(faceIdx[i]);
			patches.indices.push_back(faceIdx[i +1]);
			patches.indices.push_back(faceIdx[i + 2]);*/
			patches.indices.push_back(i);
			patches.indices.push_back(i + 1);
			patches.indices.push_back(i + 2);
		}

	}


	inline void genMeshGridXZ(LineVertices& gridlines, const float size, const float step, glm::vec4 gridlinecolor) {

		//generate patches
		gridlines.segments.clear();
		gridlines.indices.clear();

		// XZ Plane
		GLuint indicesCnt{ 0 };
		for (float i = step; i <= size; i += step) {
			// lines parallel to X axis
			gridlines.segments.push_back(LineVertex{ glm::vec3(-size, 0.0f, i), gridlinecolor });
			gridlines.segments.push_back(LineVertex{ glm::vec3(size, 0.0f, i), gridlinecolor });
			gridlines.segments.push_back(LineVertex{ glm::vec3(-size, 0.0f, -i), gridlinecolor });
			gridlines.segments.push_back(LineVertex{ glm::vec3(size, 0.0f, -i), gridlinecolor });
			gridlines.indices.push_back(indicesCnt + 0);
			gridlines.indices.push_back(indicesCnt + 1);
			gridlines.indices.push_back(indicesCnt + 2);
			gridlines.indices.push_back(indicesCnt + 3);

			// lines parallel to Z axis
			gridlines.segments.push_back(LineVertex{ glm::vec3(i, 0.0f,-size), gridlinecolor });
			gridlines.segments.push_back(LineVertex{ glm::vec3(i, 0.0f, size), gridlinecolor });
			gridlines.segments.push_back(LineVertex{ glm::vec3(-i, 0.0f, -size), gridlinecolor });
			gridlines.segments.push_back(LineVertex{ glm::vec3(-i, 0.0f, size), gridlinecolor });
			gridlines.indices.push_back(indicesCnt + 4);
			gridlines.indices.push_back(indicesCnt + 5);
			gridlines.indices.push_back(indicesCnt + 6);
			gridlines.indices.push_back(indicesCnt + 7);

			indicesCnt += 8;
		}

	}


	inline void genMeshPoint(LineVertices& point, glm::vec3 pointpos, glm::vec4 pointcolor)
	{

		point.segments.clear();
		point.indices.clear();

		point.segments.push_back(LineVertex{ pointpos, pointcolor });
		point.segments.push_back(LineVertex{ pointpos, pointcolor });
		point.indices.push_back(0);
		point.indices.push_back(1);

	}

	inline void genMeshLine(LineVertices& line) {

		// normal line
		line.segments.clear();
		line.indices.clear();

		glm::vec4 whitecolor{ 0.0f, 0.0f, 0.0f, 1.0f }; // initial color
		line.segments.push_back(LineVertex{ glm::vec3(0.0f, 0.0f, 0.0f), whitecolor });
		line.segments.push_back(LineVertex{ glm::vec3(0.0f, 1.0f, 0.0f), whitecolor });
		line.indices = { 0, 1 };

	}

	inline void genMeshLine(LineVertices& line, glm::vec3 startpos, glm::vec3 endpos, glm::vec4 linecolor)
	{

		// normal line
		line.segments.clear();
		line.indices.clear();

		line.segments.push_back(LineVertex{ startpos, linecolor });
		line.segments.push_back(LineVertex{ endpos, linecolor });
		line.indices.push_back(0);
		line.indices.push_back(1);

	}


	inline void genMeshQuad(meshVertices& patches) {

		//generate patches

		patches.vertices = {
			// positions, normal, texture Coords
			Vertex2{glm::vec3(-1.0f,  0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f),			glm::vec2(0.0f, 1.0f)},
			Vertex2{glm::vec3(1.0f,  0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f),			glm::vec2(1.0f, 1.0f)},
			Vertex2{glm::vec3(1.0f,  0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f),			glm::vec2(1.0f, 0.0f)},
			Vertex2{glm::vec3(-1.0f,  0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f),			glm::vec2(0.0f, 0.0f)},
		};
		// setup plane VAO
		patches.indices = {
			0, 1, 3,
			1, 2, 3,
		};

	}





	inline void genMeshAxes(LineVertices& axes, float axislength) {

		// normal line
		axes.segments.clear();
		axes.indices.clear();
		// initial color
		glm::vec4 red{ 1.0f, 0.0f, 0.0f, 1.0f }; // X axis
		glm::vec4 green{ 0.0f, 1.0f, 0.0f, 1.0f }; // Y axis
		glm::vec4 blue{ 0.0f, 0.0f, 1.0f, 1.0f }; // Z axis
		axes.segments.push_back(LineVertex{ glm::vec3(-axislength, 0.0f, 0.0f), red });
		axes.segments.push_back(LineVertex{ glm::vec3(axislength, 0.0f, 0.0f), red });
		axes.segments.push_back(LineVertex{ glm::vec3(0.0f, -axislength, 0.0f), green });
		axes.segments.push_back(LineVertex{ glm::vec3(0.0f, axislength, 0.0f), green });
		axes.segments.push_back(LineVertex{ glm::vec3(0.0f, 0.0f, -axislength), blue });
		axes.segments.push_back(LineVertex{ glm::vec3(0.0f, 0.0f, axislength), blue });
		axes.indices.push_back(0);
		axes.indices.push_back(1);
		axes.indices.push_back(2);
		axes.indices.push_back(3);
		axes.indices.push_back(4);
		axes.indices.push_back(5);

	}


	inline void genMeshAxes(LineVertices& axes) {

		// normal line
		axes.segments.clear();
		axes.indices.clear();
		float length = 0.50f;
		// initial color
		glm::vec4 red{ 1.0f, 0.0f, 0.0f, 1.0f }; // X axis
		glm::vec4 green{ 0.0f, 1.0f, 0.0f, 1.0f }; // Y axis
		glm::vec4 blue{ 0.0f, 0.0f, 1.0f, 1.0f }; // Z axis
		axes.segments.push_back(LineVertex{ glm::vec3(-length, 0.0f, 0.0f), red });
		axes.segments.push_back(LineVertex{ glm::vec3(length, 0.0f, 0.0f), red });
		axes.segments.push_back(LineVertex{ glm::vec3(0.0f, -length, 0.0f), green });
		axes.segments.push_back(LineVertex{ glm::vec3(0.0f, length, 0.0f), green });
		axes.segments.push_back(LineVertex{ glm::vec3(0.0f, 0.0f, -length), blue });
		axes.segments.push_back(LineVertex{ glm::vec3(0.0f, 0.0f, length), blue });
		axes.indices = { 0, 1, 2, 3, 4, 5 };

	}

	inline void bindBufferTriangleStrip(meshVertices& trianglestrip, GLuint& vao, GLuint& vbo, GLuint& ebo) {

		std::vector<Vertex2> vertices{};
		std::vector<GLuint> indices{};

		vertices = trianglestrip.vertices;
		indices = trianglestrip.indices;

		glBindVertexArray(vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex2), &vertices[0], GL_DYNAMIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_DYNAMIC_DRAW);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}


	inline void bindBufferPatches(meshVertices& patches, GLuint& vao, GLuint& vbo, GLuint& ebo) {

		bindBufferTriangleStrip(patches, vao, vbo, ebo);

		/*std::vector<Vertex2> vertices{};
		std::vector<GLuint> indices{};


		vertices = patches.vertices;
		indices = patches.indices;

		glBindVertexArray(vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex2), &vertices[0], GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);*/
	}



	inline void bindBufferLines(LineVertices& lines, GLuint& vao, GLuint& vbo, GLuint& ebo) {

		std::vector<LineVertex> vertices{};
		std::vector<GLuint> indices{};

		vertices = lines.segments;
		indices = lines.indices;

		glBindVertexArray(vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(LineVertex), &vertices[0], GL_DYNAMIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_DYNAMIC_DRAW);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}


	inline void drawElements(GLuint vao, GLuint vbo, GLuint ebo, GLenum mode, GLuint indicessize, const void* indices) {

		GLsizei stride{};

		switch (mode) {
		case GL_TRIANGLES:
		case GL_TRIANGLE_STRIP:
		case GL_PATCHES: {
			stride = sizeof(Vertex2);
			glBindVertexArray(vao);
			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glEnableVertexAttribArray(2);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));

			glDrawElements(mode, indicessize, GL_UNSIGNED_INT, 0);// no need for indices as argument!!
			//checkGlError(GET_FILE_LINE);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);
			glDisableVertexAttribArray(2);
			glBindVertexArray(0);
			break;
		}

		case GL_LINES:
		case GL_POINTS:
		{
			stride = sizeof(LineVertex);
			glBindVertexArray(vao);
			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
			glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));

			glDrawElements(mode, indicessize, GL_UNSIGNED_INT, 0);
			//checkGlError(GET_FILE_LINE);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);
			glBindVertexArray(0);
			break;
		}

		}

	}

	// renderCube() renders a 1x1 3D cube in NDC.
	// -------------------------------------------------

	inline void renderCubeMap(GLuint vao, GLuint vbo)
	{
		// initialize (if necessary)
		if (vao == 0)
		{
			float vertices[] = {
				// back face
				-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
				 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
				 1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
				 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
				-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
				-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
				// front face
				-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
				 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
				 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
				 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
				-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
				-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
				// left face
				-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
				-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
				-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
				-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
				-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
				-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
				// right face
				 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
				 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
				 1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
				 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
				 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
				 1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
				 // bottom face
				 -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
				  1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
				  1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
				  1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
				 -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
				 -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
				 // top face
				 -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
				  1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
				  1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
				  1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
				 -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
				 -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
			};


			glGenVertexArrays(1, &vao);
			glGenBuffers(1, &vbo);
			// fill buffer
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
			// link vertex attributes
			glBindVertexArray(vao);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		}
		// render Cube
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}


	// renderQuad() renders a 1x1 XY quad in NDC
	// -----------------------------------------
	inline void renderQuad()
	{
		GLuint vao, vbo;
		//if (vao == 0)
		//{
		float quadVertices[] =
		{
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		//float quadVertices[] = {
		//	// positions        // texture Coords
		//	-1.0f,  1.0f, 0.0f, 0.0f, 0.0f,
		//	-1.0f, -1.0f, 0.0f, 0.0f, 1.0f,
		//		1.0f,  1.0f, 0.0f, 1.0f, 0.0f,
		//		1.0f, -1.0f, 0.0f, 1.0f, 1.0f,
		//};
		// setup plane VAO
		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		//}
		//glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);
	}

	// utility function for loading a 2D texture from file
	GLuint loadTexture(char const* path, bool flip_vertically = true) {

		GLuint textureID{ 0 };

		// first, check dds file
		std::string pathStr = (std::string)path;
		std::size_t dds_found = pathStr.find(".dds");
		if (dds_found == std::string::npos) {
			dds_found = pathStr.find(".DDS");
		}

		if (dds_found != std::string::npos) {
			textureID = loadTexture_DDS(path);
			return textureID;
		}

		// then, hdr, png, jpeg, files
		bool hdr = stbi_is_hdr(path);


		if (hdr)
		{
			textureID = loadTexture_HDR(path, flip_vertically);
		}
		else // png, jpeg
		{
			textureID = loadTexture_NonHDR(path, flip_vertically);
		}


		return textureID;
	}

	inline GLuint loadTexture_HDR(char const* path, bool flip_vertically) {
		GLuint textureID{ 0 };

		GLsizei width{ 0 };
		GLsizei height{ 0 };
		GLsizei channels{ 0 };
		// tell stb_image.h to flip loaded texture's on the y-axis.
		stbi_set_flip_vertically_on_load(flip_vertically);
		float* data = stbi_loadf(path, &width, &height, &channels, 0);

		if (!data)
		{
			//WIN_LOG_ERROR("HDR Texture failed to load at %s" << path);
			//Win::log("%s HDR Texture failed to load at path:  %s\t log recorded at line %s of %s", GET_FUNCTION.c_str(), path, GET_LINE.c_str(), GET_FILE_NAME.c_str());
			//Win::log("HDR Texture failed to load at path:  %s", path);
			stbi_image_free(data);
			return textureID;
		}

		// data is not empy!
		//GLint internalformat{ GL_RGB16F };
		GLint internalformat{ GL_RGBA32F };
		GLenum format{ GL_RGB };
		//GLenum format{ GL_RGBA };
		GLenum type{ GL_FLOAT };

		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, internalformat, width, height, 0, format, type, data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		stbi_image_free(data);

		return textureID;
	}

	inline GLuint loadTexture_DDS(char const* path) {
		GLuint textureID{ 0 };

		std::auto_ptr<glimg::ImageSet> pImageSet;

		try
		{
			pImageSet.reset(glimg::loaders::dds::LoadFromFile(path));
			glimg::SingleImage imagedata = pImageSet->GetImage(0, 0, 0);

			glGenTextures(1, &textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, (imagedata.GetDimensions()).width, (imagedata.GetDimensions()).height, 0, GL_RED, GL_UNSIGNED_BYTE, imagedata.GetImageData());
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
			glBindTexture(GL_TEXTURE_2D, 0);

		}
		catch (std::exception& e)
		{
			printf("%s\n", e.what());
			//throw;
		}


		return textureID;
	}

	inline GLuint loadTexture_NonHDR(char const* path, bool flip_vertically) {
		GLuint textureID{ 0 };

		GLsizei width{ 0 };
		GLsizei height{ 0 };
		GLsizei channels{ 0 };
		std::vector<unsigned char> texture{};
		// tell stb_image.h to flip loaded texture's on the y-axis.
		stbi_set_flip_vertically_on_load(flip_vertically);
		unsigned char* data = stbi_load(path, &width, &height, &channels, 0);

		if (!data)
		{
			//WIN_LOG_ERROR_PARAM1()
			//WIN_LOG_ERROR("Non-HDR Texture failed to load at path=" << path);
			//Win::log("%s Non-HDR Texture failed to load at path:  %s\t log recorded at line %s of %s", GET_FUNCTION.c_str(), path, GET_LINE.c_str(), GET_FILE_NAME.c_str());
			//Win::log("Non-HDR Texture failed to load at path: %s", path);
			stbi_image_free(data);
			return textureID;
		}

		/*
		int pixelSize = width * height *channels;
		for (int j = 0; j < pixelSize; j++) {
			texture.push_back(data[j]);
		}*/

		//// reverse the picture
		//for (int h = height - 1; h >= 0; h--) {
		//	for (int w = 0; w < width; w++) {
		//		for (int c = 0; c < channels; c++) {
		//			texture.push_back(data[h * width * channels + w * channels + c]);
		//		}
		//	}
		//}

		//stbi_image_free(data);

		// data is not empy!
		GLint internalformat{ GL_RGB8 };
		GLenum format{ GL_RGB };
		GLenum type{ GL_UNSIGNED_BYTE };

		switch (channels)
		{
		case 1:
		{
			internalformat = GL_RED;
			format = GL_RED;
			/*internalformat = GL_RGB8;
			format = GL_RGB;*/
			break;
		}
		case 3:
		{
			//internalformat = GL_SRGB8; // test purpose
			//internalformat = GL_RGB8_SNORM; // test purpose
			//internalformat = GL_RGB;
			internalformat = GL_RGB32F;
			format = GL_RGB;
			break;
		}
		case 4:
		{
			//internalformat = GL_RGBA8;
			//internalformat = GL_RGBA;
			internalformat = GL_RGBA32F;
			format = GL_RGBA;
			break;
		}
		}
		glEnable(GL_FRAMEBUFFER_SRGB); // test purpose for GL_SRGB8

		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, internalformat, width, height, 0, format, type, data);// &texture[0]);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glDisable(GL_FRAMEBUFFER_SRGB); // test purpose for GL_SRGB8

		//texture.clear();
		stbi_image_free(data);

		return textureID;
	}

	inline GLuint loadTextureDDS(char const* path)
	{
		GLuint textureID{ 0 };

		std::auto_ptr<glimg::ImageSet> pImageSet{ nullptr };

		try
		{
			pImageSet.reset(glimg::loaders::dds::LoadFromFile(path));
			glimg::SingleImage imagedata = pImageSet->GetImage(0, 0, 0);

			glGenTextures(1, &textureID);
			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, (imagedata.GetDimensions()).width, (imagedata.GetDimensions()).height, 0, GL_RED, GL_UNSIGNED_BYTE, imagedata.GetImageData());

			/* glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
			 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
			 glBindTexture(GL_TEXTURE_2D, 0);*/
			glGenerateMipmap(GL_TEXTURE_2D);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}
		catch (std::exception& e)
		{
			//WIN_LOG_ERROR("DDS Texture failed to load at path=" << path);
			//Win::log("%s DDS Texture failed to load at path:  %s\t log recorded at line %s of %s", GET_FUNCTION.c_str(), path, GET_LINE.c_str(), GET_FILE_NAME.c_str());
			//Win::log("Failed to load at path: %s, Error %s", path, e.what());
			//printf("%s\n", e.what());
			//throw;
		}


		return textureID;
	}

	// create texture from  BMP, GIF, JPEG, PNG, TGA, HDR, DDS file
	inline GLuint loadTextureSTBI(char const* path, bool flip_vertically = false)
	{
		GLuint textureID{ 0 };

		// first, check dds or DDS file
		std::string pathStr = (std::string)path;
		std::size_t dds_found = pathStr.find(".dds");
		std::size_t DDS_found = pathStr.find(".DDS");
		if ((dds_found != std::string::npos) || (DDS_found != std::string::npos))
		{
			textureID = loadTextureDDS(path);
			return textureID;
		}

		GLsizei width{ 0 };
		GLsizei height{ 0 };
		GLsizei channels{ 0 };
		// tell stb_image.h to flip loaded texture's on the y-axis.
		stbi_set_flip_vertically_on_load(flip_vertically);
		// check hdr or not
		bool hdr = stbi_is_hdr(path);

		typedef  unsigned char* dataType;
		if (hdr) typedef float* dataType;

		dataType data = stbi_load(path, &width, &height, &channels, 0);
		if (!data)
		{
			//Win::log("%s STBI failed to load at path:  %s\t log recorded at line %s of %s", GET_FUNCTION.c_str(), path, GET_LINE.c_str(), GET_FILE_NAME.c_str());
			//WIN_LOG("STBI failed to load at path:  %s" << path );
			//Win::log("loadTextureSTBI(char const* path, bool flip_vertically) -> Failed to load at path: %s", path);
			stbi_image_free(data);
			//ERROR_EXIT(NULL);
		}

		GLint internalformat{ GL_RGB };
		GLenum format{ GL_RGB };
		GLenum type{ GL_UNSIGNED_BYTE };

		switch (channels)
		{
		case 1:
		{
			internalformat = GL_RED;
			format = GL_RED;
			/*internalformat = GL_RGB8;
			format = GL_RGB;*/
			break;
		}
		case 3:
		{
			//internalformat = GL_SRGB8; // test purpose
			//internalformat = GL_RGB8_SNORM; // test purpose
			internalformat = GL_RGB;
			format = GL_RGB;
			break;
		}
		case 4:
		{
			//internalformat = GL_RGBA8;
			internalformat = GL_RGBA;
			format = GL_RGBA;
			break;
		}
		}

		//glEnable(GL_FRAMEBUFFER_SRGB); // test purpose for GL_SRGB

		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, internalformat, width, height, 0, format, type, data);// &texture[0]);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		//glDisable(GL_FRAMEBUFFER_SRGB); // test purpose for GL_SRGB8

		//texture.clear();
		stbi_image_free(data);

		return textureID;

	}

	// load  texture from  BMP, ICON, GIF, JPEG, Exif, PNG, TIFF, WMF, and EMF file
	inline GLuint loadTextureGDI(const WCHAR* fileName, bool  useEmbeddedColorManagement = false)
	{
		//https://docs.microsoft.com/en-us/windows/win32/api/gdiplusheaders/nl-gdiplusheaders-image

		GLuint textureID{};
		int width{}, height{}, bitCount{}, byteCount{};

		// not applicable to ICON
		bitmap = new Bitmap{ fileName, useEmbeddedColorManagement }; // default false
		bitmap->GetHBITMAP((Color)0, &hBitmap);
		ZeroMemory(&bitmapinfo, sizeof(BITMAPINFO));
		//size_t s2 = sizeof(BITMAPINFOHEADER);
		bitmapinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		// populate the bmp info
		GetDIBits(GetDC(NULL), hBitmap, 0, 0, NULL, &bitmapinfo, DIB_RGB_COLORS);
		width = bitmapinfo.bmiHeader.biWidth;
		height = bitmapinfo.bmiHeader.biHeight;         // negative means top-to-bottom orientation
		bitCount = bitmapinfo.bmiHeader.biBitCount;     // bits per pixel
		byteCount = bitmapinfo.bmiHeader.biSizeImage;   // # of bytes of bitmapinfo

		// get bitmap data as BGR   // get raw bits
		unsigned char* buffer = new unsigned char[byteCount];
		int result = GetDIBits(GetDC(NULL), hBitmap, 0, height, buffer, &bitmapinfo, DIB_RGB_COLORS);

		if (result <= 0)
		{
			//WIN_LOG_ERROR("GDI failed")
			//Win::log("%s GDI failed \t log recorded at line %s of %s", GET_FUNCTION.c_str(),  GET_LINE.c_str(), GET_FILE_NAME.c_str());
			//Win::log(L"[ERROR] Failed to load image data.");
			return NULL;
		}
		// convert BGR to RGB
		swapRedBlue(buffer, byteCount, bitCount >> 3);
		// flip vertically
		flipImage(buffer, width, height, bitCount >> 3);

		//Win::log(L"Loaded a bitmap: %d x %d, %d bits", width, height, bitCount);

		GLenum format{}, internalformat{}, type{ GL_UNSIGNED_BYTE };
		switch (bitCount)
		{
		case 8:
			format = GL_LUMINANCE;
			internalformat = GL_LUMINANCE8;
			break;
		case 24:
			format = GL_RGB;
			internalformat = GL_RGB;
			break;
		case 32:
			format = GL_RGBA;
			internalformat = GL_RGBA;
			break;
		}

		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, internalformat, width, height, 0, format, type, buffer);// &texture[0]);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		//WIN_LOG("Bitmap loaded: " << width << "x" << height << "bit count=" << bitCount);
		//Win::log("%s Bitmap loaded  %d x %d, %d bits\t log recorded at line %s of %s", GET_FUNCTION.c_str(), width, height, bitCount, GET_LINE.c_str(), GET_FILE_NAME.c_str());
		//Win::log(L"Created OpenGL texture object.");

		delete[] buffer;

		DeleteObject(hBitmap);
		DeleteObject(bitmap);

		return textureID;
	}


	//get image data info from  BMP, ICON, GIF, JPEG, Exif, PNG, TIFF, WMF, and EMF file
	inline ImageDataInfo  getImageDataInfoGDI(const WCHAR* fileName, bool useEmbeddedColorManagement = false)
	{
		// GDI++
		Gdiplus::GdiplusStartupInput gdiplusStartupInput;
		ULONG_PTR           gdiplusToken;
		Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

		//https://docs.microsoft.com/en-us/windows/win32/api/gdiplusheaders/nl-gdiplusheaders-image

		int width{}, height{}, bitCount{}, byteCount{};
		HDC Hdc;
		/* Note
		BITMAPINFO bitmapinfo, HBITMAP   hBitmap, Gdiplus::Bitmap* are palced out of the function. otherwise bitmapinfo corrupt
		*/

		bitmap = new Gdiplus::Bitmap{ fileName, useEmbeddedColorManagement }; // default false
		bitmap->GetHBITMAP((Gdiplus::Color)0, &hBitmap);
		ZeroMemory(&bitmapinfo, sizeof(bitmapinfo));
		bitmapinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		// populate the bmp info
		Hdc = GetDC(NULL);  // any device content works here
		GetDIBits(Hdc, hBitmap, 0, 0, NULL, &bitmapinfo, DIB_RGB_COLORS);
		width = bitmapinfo.bmiHeader.biWidth;
		height = bitmapinfo.bmiHeader.biHeight;         // negative means top-to-bottom orientation
		bitCount = bitmapinfo.bmiHeader.biBitCount;     // bits per pixel
		byteCount = bitmapinfo.bmiHeader.biSizeImage;   // # of bytes of bitmapinfo

		// get bitmap data as BGR   // get raw bits
		 //unsigned char* buffer = new unsigned char[byteCount];
		bmBuffer = new unsigned char[byteCount];

		int result = GetDIBits(Hdc, hBitmap, 0, height, bmBuffer, &bitmapinfo, DIB_RGB_COLORS);

		if (result <= 0)
		{
			//ERROR_EXIT(NULL);
		}
		// convert BGR to RGB
		swapRedBlue(bmBuffer, byteCount, bitCount >> 3);
		// flip vertically
		flipImage(bmBuffer, width, height, bitCount >> 3);
		//WIN_LOG("Bitmap loaded: " << width << "x" << height << "bit count=" << bitCount);
		//Win::log("%s Bitmap loaded  %d x %d, %d bits\t log recorded at line %s of %s", GET_FUNCTION.c_str(), width, height, bitCount, GET_LINE.c_str(), GET_FILE_NAME.c_str());
		//Win::log(L"Loaded a bitmap: %d x %d, %d bits", width, height, bitCount);

		ImageDataInfo   imageDataInfo{ bmBuffer, width, height, bitCount, byteCount };

		//delete[] buffer; do not delete

		DeleteDC(Hdc);
		DeleteObject(hBitmap);
		DeleteObject(bitmap);
		Gdiplus::GdiplusShutdown(gdiplusToken);

		return imageDataInfo;
	}

	inline GLuint createTexture(unsigned char* buffer, int& width, int& height, int& bitCount, int& byteCount)
	{
		GLuint textureID{};
		GLenum format{}, internalformat{}, type{ GL_UNSIGNED_BYTE };
		switch (bitCount)
		{
		case 8:
			format = GL_LUMINANCE;
			internalformat = GL_LUMINANCE8;
			break;
		case 24:
			format = GL_RGB;
			internalformat = GL_RGB;
			break;
		case 32:
			format = GL_RGBA;
			internalformat = GL_RGBA;
			break;
		}

		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, internalformat, width, height, 0, format, type, buffer);// &texture[0]);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		//WIN_LOG("OpenGL Texture Object Created: " << width << "x" << height << "bit count=" << bitCount);
		//Win::log("%s OpenGL Texture Object Created  %d x %d, %d bits\t log recorded at line %s of %s", GET_FUNCTION.c_str(), width, height, bitCount, GET_LINE.c_str(), GET_FILE_NAME.c_str());
		//Win::log(L"Created OpenGL texture object.");


		return textureID;
	}


	inline GLuint loadArrayTexture_NonHDR(std::vector<std::string> path, bool flip_vertically) {
		GLuint textureID{ 0 };

		GLsizei width{ 0 };
		GLsizei height{ 0 };
		GLsizei channels{ 0 };
		GLsizei layerCount = (GLsizei)path.size();
		GLsizei mipLevelCount{ 1 };

		std::vector<unsigned char> arrayTexture{};

		for (int i = 0; i < layerCount; i++) {
			// tell stb_image.h to flip loaded texture's on the y-axis.
			stbi_set_flip_vertically_on_load(flip_vertically);
			unsigned char* data = stbi_load(path[i].c_str(), &width, &height, &channels, 0);
			if (!data) {
				//WIN_LOG_ERROR(" One of ArrayTexture failed to load at path=" << path[i]);
				//Win::log("%s One of ArrayTexture failed to load at path: %s\t log recorded at line %s of %s", GET_FUNCTION.c_str(), path[i], GET_LINE.c_str(), GET_FILE_NAME.c_str());
				//Win::log("One of ArrayTexture failed to load at path: %s", path[i]);
				stbi_image_free(data);
				continue;
				//}/*
				//int pixelSize = width * height *channels;
				//for (int j = 0; j < pixelSize; j++) {
				//	arrayTexture.push_back(data[j]);
				//}*/
				//// reverse the picture
				//for (int h = height - 1; h >= 0; h--) {
				//	for (int w = 0; w < width; w++) {
				//		for (int c = 0; c < channels; c++) {
				//			arrayTexture.push_back(data[h * width*channels + w*channels + c]);
				//		}
				//	}
			}

			stbi_image_free(data);
		}

		if (arrayTexture.empty())
		{
			//WIN_LOG_ERROR("All of Array Texture failed to load");
			//Win::log("%s All of Array Textur to load\t log recorded at line %s of %s", GET_FUNCTION.c_str(), GET_LINE.c_str(), GET_FILE_NAME.c_str());
			//Win::log("All of ArrayTexture failed to load");
			return textureID;
		}

		// arrayTexture is not empy, but may be a fewer layer count as expected!
		GLint internalformat{ GL_RGB8 };
		GLenum format{ GL_RGB };
		GLenum type{ GL_UNSIGNED_BYTE };

		switch (channels)
		{
		case 1:
		{
			internalformat = GL_RED;
			format = GL_RED;
			break;
		}
		case 3:
		{
			internalformat = GL_RGB8;
			format = GL_RGB;
			break;
		}
		case 4:
		{
			internalformat = GL_RGBA8;
			format = GL_RGBA;
			break;
		}
		}

		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D_ARRAY, textureID);
		glTexStorage3D(GL_TEXTURE_2D_ARRAY, mipLevelCount, internalformat, width, height, layerCount);
		glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, width, height, layerCount, format, type, &arrayTexture[0]);
		glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		arrayTexture.clear();

		return textureID;
	}

	inline GLuint loadArrayTexture_NonHDR() {// ArrayTexture Sample coding
		GLuint textureID = 0;

		GLsizei width = 2;
		GLsizei height = 2;
		GLsizei layerCount = 6;
		GLsizei mipLevelCount = 1;

		// Read you texels here. In the current example, we have 2*2*6 = 24 texels, with each texel being 4 GLubytes.
		//GLubyte texels[96] =
		//unsigned char texels[96] =
		std::vector<unsigned char> texels = {
			// Texels for first image.
			0,   0,   0,   255,
			255, 0,   0,   255,
			0,   255, 0,   255,
			0,   0,   255, 255,
			// Texels for second image.
			255, 255, 255, 255,
			255, 255,   0, 255,
			0,   255, 255, 255,
			255, 0,   255, 255,
			// Texels for thirdd image.
			255, 255, 255, 255,
			255, 255,   0, 255,
			255,   255, 255, 255,
			255, 0,   255, 255,
			// Texels for 4th image.
			255, 255, 255, 255,
			255, 255,   0, 255,
			255,   255, 255, 255,
			255, 0,   255, 255,
			// Texels for 5th image.
			255, 255, 255, 255,
			255, 255,   0, 255,
			255,   255, 255, 255,
			255, 0,   255, 255,
			// Texels for 6th image.
			255, 255, 255, 255,
			255, 255,   0, 255,
			255,   255, 255, 255,
			255, 0,   255, 255,
		};

		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D_ARRAY, textureID);
		// Allocate the storage.
		glTexStorage3D(GL_TEXTURE_2D_ARRAY, mipLevelCount, GL_RGBA8, width, height, layerCount);
		// Upload pixel data.
		// The first 0 refers to the mipmap level (level 0, since there's only 1)
		// The following 2 zeroes refers to the x and y offsets in case you only want to specify a subrectangle.
		// The final 0 refers to the layer index offset (we start from index 0 and have 2 levels).
		// Altogether you can specify a 3D box subset of the overall texture, but only one mip level at a time.
		//glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, width, height, layerCount, GL_RGBA, GL_UNSIGNED_BYTE, texels);
		glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, width, height, layerCount, GL_RGBA, GL_UNSIGNED_BYTE, &texels[0]);

		// Always set reasonable texture parameters
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		return textureID;

	}

	inline glm::vec3 genTangentVec3(glm::vec3 pos1, glm::vec3 pos2, glm::vec3 pos3, glm::vec2 uv1, glm::vec2 uv2, glm::vec2 uv3)
	{
		// calculate tangent/bitangent vectors of both triangles
		glm::vec3 T; // tangent
		//glm::vec3 B; // bitangent;
		// triangle 1
		// ----------
		glm::vec3 edge1 = pos2 - pos1;
		glm::vec3 edge2 = pos3 - pos1;
		glm::vec2 deltaUV1 = uv2 - uv1;
		glm::vec2 deltaUV2 = uv3 - uv1;

		GLfloat f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

		T.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		T.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		T.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
		T = glm::normalize(T);

		//B.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
		//B.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
		//B.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
		//B = glm::normalize(B);

		return T;
	}

	inline void writeCubemap_png(std::string materialname, GLuint cubemapID, int width, int height, std::string filepath) {

		GLint sidelength;
		size_t sideLength;
		GLuint* BufOutPtr;
		size_t size;
		std::size_t found = filepath.find_last_of("/");  // psotion
		std::string directory = filepath.substr(0, found + 1); // length = found + 1
		std::string fname = filepath.substr(found + 1); // position
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapID);
		for (unsigned int j = 0; j < 6; ++j) {
			glGetTexLevelParameteriv(GL_TEXTURE_CUBE_MAP_POSITIVE_X + j, 0, GL_TEXTURE_WIDTH, &sidelength);
			sideLength = static_cast<size_t>(sidelength);
			//size = sideLength * sideLength * sizeof(GL_RGBA) * sizeof(GL_UNSIGNED_BYTE);
			size = sideLength * sideLength * 4 * sizeof(GL_UNSIGNED_BYTE);
			BufOutPtr = (GLuint*)malloc(size);
			//glGetTexImage(GL_TEXTURE_CUBE_MAP_POSITIVE_X + j, 0, GL_RGB, GL_FLOAT, BufOutFloatPtr);
			glGetTexImage(GL_TEXTURE_CUBE_MAP_POSITIVE_X + j, 0, GL_RGBA, GL_UNSIGNED_BYTE, BufOutPtr);
			stbi_flip_vertically_on_write(false);
			stbi_write_png(pathToResources(directory + materialname + "_side" + std::to_string(j) + "_ID" + std::to_string(cubemapID) + "_" + fname + ".png").c_str(), width, height, 4, BufOutPtr, width * 4);


			free(BufOutPtr);
		}
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	}

	inline void writeCubemap_hdr(GLuint cubemapID, int width, int height, std::string filename) {

		GLint sidelength;
		size_t sideLength;
		GLfloat* BufOutFloatPtr;
		size_t size;

		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapID);
		for (unsigned int j = 0; j < 6; ++j) {
			glGetTexLevelParameteriv(GL_TEXTURE_CUBE_MAP_POSITIVE_X + j, 0, GL_TEXTURE_WIDTH, &sidelength);
			sideLength = static_cast<size_t>(sidelength);
			size = sideLength * sideLength * sizeof(GL_RGB) * sizeof(GL_FLOAT);
			BufOutFloatPtr = (GLfloat*)malloc(size);
			//glGetTexImage(GL_TEXTURE_CUBE_MAP_POSITIVE_X + j, 0, GL_RGB, GL_FLOAT, BufOutFloatPtr);
			glGetTexImage(GL_TEXTURE_CUBE_MAP_POSITIVE_X + j, 0, GL_RGB, GL_FLOAT, BufOutFloatPtr);
			stbi_flip_vertically_on_write(false);
			stbi_write_hdr(pathToResources(filename + "_side" + std::to_string(j) + ".hdr").c_str(), width, height, 3, BufOutFloatPtr);
			free(BufOutFloatPtr);
		}
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	}

	inline void writeTexture2D_bmp(GLuint textureID, int width, int height, std::string filename) {

		GLint sidelength;
		size_t sideLength;
		GLuint* BufOutPtr;
		size_t size;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &sidelength);
		sideLength = static_cast<size_t>(sidelength);
		size = sideLength * sideLength * 4 * sizeof(GL_UNSIGNED_BYTE);
		BufOutPtr = (GLuint*)malloc(size);
		glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, BufOutPtr);
		stbi_flip_vertically_on_write(true);
		stbi_write_bmp(pathToResources(filename + ".bmp").c_str(), width, height, 4, BufOutPtr);
		//stbi_write_png(pathToResources(filename +  ".png").c_str(), width, height, 4, BufOutPtr, width * 4);
		free(BufOutPtr);
		glBindTexture(GL_TEXTURE_2D, 0);

	}

	//refer to https://docs.microsoft.com/en-us/windows/win32/debug/retrieving-the-last-error-code
	inline static void ErrorExit(std::string fileName, std::string  functionName, std::string lineNo, std::string remarks)
	{
		DWORD err = GetLastError();
		std::string& funcName = functionName;
		std::wstring wstr0 = std::wstring(funcName.begin(), funcName.end());
		wchar_t* functionnameW = (LPTSTR)wstr0.c_str();

		std::size_t found = fileName.find_last_of("/\\");
		std::string& file = fileName.substr(found + 1);
		//std::string& fName
		std::wstring wstr1 = std::wstring(file.begin(), file.end());
		wchar_t* fileW = (LPTSTR)wstr1.c_str();

		std::string& lNo = lineNo;
		std::wstring wstr2 = std::wstring(lNo.begin(), lNo.end());
		wchar_t* linenoW = (LPTSTR)wstr2.c_str();

		std::string& Remarks = (remarks == "0") ? "" : remarks;
		std::wstring wstr3 = std::wstring(Remarks.begin(), Remarks.end());
		wchar_t* remarksW = (LPTSTR)wstr3.c_str();

		if (err)
		{
			LPVOID lpMsgBuf;
			LPVOID lpDisplayBuf;

			FormatMessage(
				FORMAT_MESSAGE_ALLOCATE_BUFFER |
				FORMAT_MESSAGE_FROM_SYSTEM |
				FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL,
				err,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				(LPTSTR)&lpMsgBuf,
				0, NULL);

			//lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, (lstrlen((LPCWSTR)lpMsgBuf) + lstrlen((LPCWSTR)functionnameW) + lstrlen((LPCWSTR)fileW) + lstrlen((LPCWSTR)linenoW) + lstrlen((LPCWSTR)remarksW) + 60) * sizeof(TCHAR));
			lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, (lstrlen((LPCWSTR)lpMsgBuf) + lstrlen((LPCWSTR)functionnameW) + lstrlen((LPCWSTR)fileW) + lstrlen((LPCWSTR)linenoW) + lstrlen((LPCWSTR)remarksW) + 50) * sizeof(TCHAR));
			StringCchPrintf((LPTSTR)lpDisplayBuf, (size_t)LocalSize(lpDisplayBuf) / sizeof(TCHAR),
				TEXT("%s failed with error %d: %s, at line %s of %s \n%s"), functionnameW, err, lpMsgBuf, linenoW, fileW, remarksW);
			//MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Tools::ErrorExit Invoked"), MB_OK);
			MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Tools::ERROR_EXIT Invoked"), MB_OK);

			LocalFree(lpMsgBuf);
			LocalFree(lpDisplayBuf);
			ExitProcess(err);
		}
		else
		{
			Win::log("LOG: \t%s...%s; \tat line %s, %s of %s\n", remarks.c_str(), "GetLastError() didi not ditect any ERROR", lineNo.c_str(), functionName.c_str(), fileName.c_str());
		}

	}
}

//#endif
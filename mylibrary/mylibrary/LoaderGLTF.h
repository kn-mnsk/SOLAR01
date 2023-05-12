#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include <windows.h>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <vector>

#include <glad/glad.h> 

#define STB_IMAGE_IMPLEMETATION
#include <stb_image/stb_image.h>


#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>
#include <libgltf/libgltf.h>

#include <mylibrary/shader.h>
#include <mylibrary/model.h>
#include <mylibrary/tools.h>

namespace tinygltf
{
	static inline bool is_base64(unsigned char c)
	{
		return (isalnum(c) || (c == '+') || (c == '/'));
	}


	static inline std::string base64_decode(std::string const& encoded_string)
	{
		int in_len = static_cast<int>(encoded_string.size());
		int i = 0;
		int j = 0;
		int in_ = 0;
		unsigned char char_array_4[4], char_array_3[3];
		std::string ret;

		const std::string base64_chars =
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			"abcdefghijklmnopqrstuvwxyz"
			"0123456789+/";

		while (in_len-- && (encoded_string[in_] != '=') &&
			is_base64(encoded_string[in_])) {
			char_array_4[i++] = encoded_string[in_];
			in_++;
			if (i == 4) {
				for (i = 0; i < 4; i++)
					char_array_4[i] =
					static_cast<unsigned char>(base64_chars.find(char_array_4[i]));

				char_array_3[0] =
					(char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
				char_array_3[1] =
					((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
				char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

				for (i = 0; (i < 3); i++) ret += char_array_3[i];
				i = 0;
			}
		}

		if (i) {
			for (j = i; j < 4; j++) char_array_4[j] = 0;

			for (j = 0; j < 4; j++)
				char_array_4[j] =
				static_cast<unsigned char>(base64_chars.find(char_array_4[j]));

			char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
			char_array_3[1] =
				((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
			char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

			for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
		}

		return ret;
	}

	static inline bool IsDataURI(const std::string& in)
	{
		std::string header = "data:application/octet-stream;base64,";
		if (in.find(header) == 0) {
			return true;
		}

		header = "data:image/jpeg;base64,";
		if (in.find(header) == 0) {
			return true;
		}

		header = "data:image/png;base64,";
		if (in.find(header) == 0) {
			return true;
		}

		header = "data:image/bmp;base64,";
		if (in.find(header) == 0) {
			return true;
		}

		header = "data:image/gif;base64,";
		if (in.find(header) == 0) {
			return true;
		}

		header = "data:text/plain;base64,";
		if (in.find(header) == 0) {
			return true;
		}

		header = "data:application/gltf-buffer;base64,";
		if (in.find(header) == 0) {
			return true;
		}

		return false;
	}

	static inline bool DecodeDataURI(std::vector<unsigned char>* out, std::string& mime_type,
		const std::string& in, size_t reqBytes, bool checkSize)
	{
		std::string header = "data:application/octet-stream;base64,";
		std::string data;
		if (in.find(header) == 0) {
			data = base64_decode(in.substr(header.size()));  // cut mime string.
		}

		if (data.empty()) {
			header = "data:image/jpeg;base64,";
			if (in.find(header) == 0) {
				mime_type = "image/jpeg";
				data = base64_decode(in.substr(header.size()));  // cut mime string.
			}
		}

		if (data.empty()) {
			header = "data:image/png;base64,";
			if (in.find(header) == 0) {
				mime_type = "image/png";
				data = base64_decode(in.substr(header.size()));  // cut mime string.
			}
		}

		if (data.empty()) {
			header = "data:image/bmp;base64,";
			if (in.find(header) == 0) {
				mime_type = "image/bmp";
				data = base64_decode(in.substr(header.size()));  // cut mime string.
			}
		}

		if (data.empty()) {
			header = "data:image/gif;base64,";
			if (in.find(header) == 0) {
				mime_type = "image/gif";
				data = base64_decode(in.substr(header.size()));  // cut mime string.
			}
		}

		if (data.empty()) {
			header = "data:text/plain;base64,";
			if (in.find(header) == 0) {
				mime_type = "text/plain";
				data = base64_decode(in.substr(header.size()));
			}
		}

		if (data.empty()) {
			header = "data:application/gltf-buffer;base64,";
			if (in.find(header) == 0) {
				data = base64_decode(in.substr(header.size()));
			}
		}

		// TODO(syoyo): Allow empty buffer? #229
		if (data.empty()) {
			return false;
		}

		if (checkSize) {
			if (data.size() != reqBytes) {
				return false;
			}
			out->resize(reqBytes);
		}
		else {
			size_t size = data.size();
			out->resize(data.size());
		}
		std::copy(data.begin(), data.end(), out->begin());
		return true;
	}
}


namespace LoaderGLTF
{
	//https://libgltf.readthedocs.io/en/latest/

	class glTFLoader :public  libgltf::IglTFLoader // , public libgltf::TAccessorStream
	{

	public:

		glTFLoader(const std::string& path, Shader::Shader* shaderProg)
		{
			Loader(path);

			setupMeshes(shaderProg);

			///* Cube mapping*/
			//
			//genCubemap(path);

			//cube = new Model::Model{ pathCommonResources("objects/cube/cube_blender.obj") };
			//progSkyBoxImageToCubemap = new Shader::Shader{ pathShader("skyboxcubemap.vert").c_str(), pathShader("skyboxcubemap.frag").c_str() };

			///* Cube mapping*/
		}

		~glTFLoader() {}

		GLenum errNum{};

		Shader::Shader* shaderProg{};
		GLuint vao{}; // vertex array object
		std::vector<GLuint> Vaos{}; // vertex array object
		GLuint abo{}; // GL_ARRAY_BUFFER object
		std::vector<GLuint> Abos{}; // GL_ARRAY_BUFFER object
		GLuint eabo{}; // GL_ELEMENT_ARRAY_BUFFER object
		std::vector<GLuint> Eabos{}; // GL_ELEMENT_ARRAY_BUFFER object
		//int32_t nodeIndex{};


		//std::map<std::string, std::shared_ptr<libgltf::SAccessorData>> AttributesData{};
		std::map<std::string, libgltf::SAccessorData> AttributesData{};
		//std::map<std::string, std::shared_ptr<libgltf::SAccessorData>> MaterialsData{};
		std::map<std::string, libgltf::SAccessorData> MaterialsData{};
		//std::map<std::string, std::shared_ptr<libgltf::SAccessorData>> TriangleData{};
		std::map<std::string, libgltf::SAccessorData> TriangleData{};

		std::shared_ptr<libgltf::IglTFLoader> gltf_loader;
		libgltf::SGlTF* loaded_gltf{};

		GLuint irradianceMapId{ (unsigned int)std::string::npos };
		GLuint prefilterMapId{ (unsigned int)std::string::npos };
		GLuint brdLUTId{ (unsigned int)std::string::npos };
		GLuint baseColorTextureId{ (unsigned int)std::string::npos };
		GLuint metallicRoughnessTextureId{ (unsigned int)std::string::npos };
		GLuint normalTextureId{ (unsigned int)std::string::npos };
		GLuint occlusionTextureId{ (unsigned int)std::string::npos };
		GLuint emissiveTextureId{ (unsigned int)std::string::npos };
		GLuint captureCubemapId{ (unsigned int)std::string::npos };

		bool existIrradianceMap{ false };
		bool existPrefilterMap{ false };
		bool existBrdLUT{ false };
		bool  existBaseColorTexture{ false };
		bool existMetallicRoughnessTexture{ false };
		bool existNormalTexture{ false };
		bool existOcclusionTexture{ false };
		bool existEmissiveTexture{ false };
		bool existCaptureCubemap{ false };

		// for cube mapping
		Model::Model* cube;
		Shader::Shader* progSkyBoxImageToCubemap;
		GLuint captureFBO;
		GLuint captureRBO;
		GLuint cubemapId;

		enum class  MappingMethod {
			Cube = 1,
			cylindrically_mapped_sphere = 2,
			Plane = 3,
			Sphere = 4,

			Default = 0
		};

		MappingMethod mappingMethod{};

		struct MapInfo
		{
			GLuint Id{ (unsigned int)std::string::npos };
			bool Exist{ false };
		};

		struct NodeMapInfo
		{
			int32_t rootNodeIndex;
			int32_t materialIndex{};
			MappingMethod mappingMethod{};
			GLsizei indicesCount{};
			MapInfo irradianceMap;
			MapInfo prefilterMap;
			MapInfo brdLUT;
			MapInfo baseColorTexture;
			MapInfo metallicRoughnessTexture;
			MapInfo normalTexture;
			MapInfo occlusionTexture;
			MapInfo emissiveTexture;
			MapInfo captureCubemap;
		};

		std::vector<NodeMapInfo> nodeMapInfo{};
		// A set of parameter values that are used to define the metallic-roughness material model from Physically Based Rendering (PBR) methodology. When undefined, all the default values of `pbrMetallicRoughness` **MUST** apply.		libgltf::SMaterialPBRMetallicRoughness defaultMaterialPBRMetallicRoughness{};
		libgltf::SMaterialPBRMetallicRoughness defaultMaterialPBRMetallicRoughness{};
		libgltf::SMaterialNormalTextureInfo defaultNormalTexture{};
		libgltf::SMaterialOcclusionTextureInfo defaultOcclusionTexture{};
		libgltf::STextureInfo defaultEmissiveTexture{};


		GLuint textureId{ (unsigned int)std::string::npos };


		std::string   image0_data_type{};
		std::string directory{};
		std::string fileName{};
		bool gammaCorrection;
		GLenum primitiveMode{};

		std::vector<glm::vec3> texCoordsV3{};

		//vector<std::string> faces
		//{
		//	"../../../.localResources/resources/skybox/right.jpg",
		//	"../../../.localResources/resources/skybox/left.jpg",
		//	"../../../.localResources/resources/skybox/top.jpg",
		//	"../../../.localResources/resources/skybox/bottom.jpg",
		//	"../../../.localResources/resources/skybox/front.jpg",
		//	"../../../.localResources/resources/skybox/back.jpg",
		//};

		//GLuint loadCubemap(vector<std::string> faces)
		//{
		//	unsigned int textureID;
		//	glGenTextures(1, &textureID);
		//	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

		//	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, 0); // new
		//	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, 4); // new

		//	int width, height, nrComponents;
		//	for (unsigned int i = 0; i < faces.size(); i++)
		//	{
		//		stbi_set_flip_vertically_on_load(false);
		//		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrComponents, 0);
		//		if (data)
		//		{
		//			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		//			stbi_image_free(data);
		//		}
		//		else
		//		{
		//			Win::log("Cubemap texture failed to load at path: %s", faces[i]);
		//			stbi_image_free(data);
		//		}
		//	}
		//	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//	//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		//	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		//	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		//	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		//	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

		//	return textureID;
		//}



		void setupMeshes(Shader::Shader* shaderProg)
		{
			this->shaderProg = shaderProg;
			shaderProg->useShader();
			shaderProg->setIntShader("irradianceMap", 0);
			shaderProg->setIntShader("prefilterMap", 1);
			shaderProg->setIntShader("brdfLUT", 2);
			shaderProg->setIntShader("baseColorTexture", 3);
			//shaderProg->setIntShader("baseColorCubemap", 8);
			shaderProg->setIntShader("metallicRoughnessTexture", 4);
			shaderProg->setIntShader("normalTexture", 5);
			shaderProg->setIntShader("occlusionTexture", 6);
			shaderProg->setIntShader("emissiveTexture", 7);

			errNum = glGetError();
			if (errNum != GL_NO_ERROR)
			{
				WIN_LOG_ERROR("setupmesh texture location units GL_ERROR %d" << errNum);
			}


			defaultMaterialPBRMetallicRoughness.baseColorFactor = { 1.0f, 1.0f, 1.0f, 1.0f };
			defaultMaterialPBRMetallicRoughness.baseColorTexture = nullptr;
			defaultMaterialPBRMetallicRoughness.metallicFactor = 0.5f;
			defaultMaterialPBRMetallicRoughness.roughnessFactor = 0.5f;
			defaultMaterialPBRMetallicRoughness.metallicRoughnessTexture = nullptr;


			defaultNormalTexture.scale = 1.0f;// The scalar parameter applied to each normal vector of the normal texture.
			defaultNormalTexture.index = nullptr;  // The index of the texture.
			//defaultValues.texCoord = 0.0f;  // The set index of texture's TEXCOORD attribute used for texture coordinate mapping.
			defaultNormalTexture.extensions = nullptr; //JSON object with extension-specific objects.
			defaultNormalTexture.extras = nullptr; //Application-specific data.


			defaultOcclusionTexture.strength = 1.0f;  //? // A scalar multiplier controlling the amount of occlusion applied.
			defaultOcclusionTexture.index = nullptr;  // The index of the texture.
			defaultOcclusionTexture.texCoord = 0.0f;  // The set index of texture's TEXCOORD attribute used for texture coordinate mapping.
			defaultOcclusionTexture.extensions = nullptr; //JSON object with extension-specific objects.
			defaultOcclusionTexture.extras = nullptr; //Application-specific data


			defaultEmissiveTexture.index = nullptr;  // The index of the texture.
			defaultEmissiveTexture.texCoord = 0.0f;  // The set index of texture's TEXCOORD attribute used for texture coordinate mapping.
			defaultEmissiveTexture.extensions = nullptr; //JSON object with extension-specific objects.
			defaultEmissiveTexture.extras = nullptr; //Application-specific data.

			/*glGenVertexArrays(1, &this->vao);
			glGenBuffers(1, &this->abo);
			glGenBuffers(1, &this->eabo);*/

			//glEnable(GL_DEPTH_TEST);
			//glDepthFunc(GL_LESS);
			//glDisable(GL_DEPTH_TEST);

			//glEnable(GL_ALPHA_TEST);
			//glEnable(GL_BLEND);
			//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			//glEnable(GL_COLOR_MATERIAL);

			//glEnable(GL_POLYGON_SMOOTH);

			for each (auto scene in loaded_gltf->scenes)
			{
				Vaos.resize(scene->nodes.size());
				Abos.resize(scene->nodes.size());
				Eabos.resize(scene->nodes.size());
				glGenVertexArrays(scene->nodes.size(), &Vaos[0]);
				glGenBuffers(scene->nodes.size(), &Abos[0]);
				glGenBuffers(scene->nodes.size(), &Eabos[0]);
				errNum = glGetError();
				if (errNum != GL_NO_ERROR)
				{
					WIN_LOG_ERROR("glBuffer Generation: &d" << errNum);
					//Win::log("ERROR: glBuffer Generation \n");
				}

				nodeMapInfo.resize(scene->nodes.size());
				//nodeIndex = 0;
				for each (auto rootNodeIndex in scene->nodes)
				{
					//glGenVertexArrays(1, &this->vao);
					//glGenBuffers(1, &this->abo);
					//glGenBuffers(1, &this->eabo);
					//nodeIndex = rootNodeIndex->int32_tValue;
					//traverseNode(loaded_gltf->nodes[nodeIndex]);
					nodeMapInfo[rootNodeIndex->int32_tValue].rootNodeIndex = rootNodeIndex->int32_tValue;
					traverseNode(rootNodeIndex->int32_tValue);
					//glDeleteBuffers(1, &this->vao);
					//glDeleteBuffers(1, &this->abo);
					//glDeleteBuffers(1, &this->vao);
				}

			}


			//textureId = loadCubemap(faces);
			//texCoordsV3 = getTexCoordsV3();



		}


		//void genCaptureCubemap(unsigned char* data)
		//{
		//	glGenTextures(1, &captureCubemapId);
		//	glBindTexture(GL_TEXTURE_CUBE_MAP, captureCubemapId);
		//	errNum = glGetError();
		//	if (errNum != GL_NO_ERROR) {
		//		Win::log("glBindTexture, Error = %d\n", errNum);
		//	}

		//	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, 0); // new
		//	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, 4); // new

		//	//for (unsigned int i = 0; i < 4; i++) 
		//	{
		//		for (unsigned int face = 0; face < 6; face++)
		//		{
		//			//glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, i, GL_RGBA, 1024, 1024, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		//			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, GL_RGBA, 1024, 1024, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		//		}

		//	}

		//	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // enable pre-filter mipmap sampling (combatting visible dots artifact)
		//	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		//	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		//	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		//	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);


		//	errNum = glGetError();
		//	if (errNum != GL_NO_ERROR) {
		//		Win::log("glBindTexture, Error = %d\n", errNum);
		//	}

		//	glBindTexture(GL_TEXTURE_CUBE_MAP, captureCubemapId);
		//	glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, 0, 0, 1024, 1024, GL_RGBA, GL_UNSIGNED_BYTE, data); // right
		//	errNum = glGetError();
		//	if (errNum != GL_NO_ERROR) {
		//		Win::log("glTexSubImage2D, Error = %d\n", errNum);
		//	}
		//	glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 1, 0, 0, 0, 1024, 1024, GL_RGBA, GL_UNSIGNED_BYTE, data); // left
		//	glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 2, 0, 0, 0, 1024, 1024, GL_RGBA, GL_UNSIGNED_BYTE, data); // top
		//	glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 3, 0, 0, 0, 1024, 1024, GL_RGBA, GL_UNSIGNED_BYTE, data); // bottom
		//	glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 4, 0, 0, 0, 1024, 1024, GL_RGBA, GL_UNSIGNED_BYTE, data); // back
		//	glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 5, 0, 0, 0, 1024, 1024, GL_RGBA, GL_UNSIGNED_BYTE, data); // front


		//	errNum = glGetError();
		//	if (errNum != GL_NO_ERROR)
		//	{
		//		Win::log("glTexSubImage2D 2, Error = %d\n", errNum);
		//	}

		//	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // enable pre-filter mipmap sampling (combatting visible dots artifact)
		//	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		//	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		//	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		//	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
		//}

		std::vector<glm::vec3>  getTexCoordsV3()
		{
			std::vector<glm::vec3>  container{};

			for each (auto v2 in texcoord_0_data)
			{
				container.push_back(glm::vec3((float)v2[0], (float)v2[1], 0.0f));
			}

			return container;

		}

		void drawModel(Shader::Shader* shaderProg)
		{
			for each (auto info in nodeMapInfo)
			{

				drawModelByNode(info.rootNodeIndex, shaderProg);
			}
		}


		void drawModelByNode(int32_t nodeIndex, Shader::Shader* shaderProg)
		{

			std::shared_ptr<libgltf::SMaterial>material = loaded_gltf->materials[nodeMapInfo[nodeIndex].materialIndex];

			glBindVertexArray(Vaos[nodeIndex]);
			//GL_ERROR_CHECK;

			shaderProg->useShader();
			//GL_ERROR_CHECK;

			shaderProg->setIntShader("mappingMethod", (int)nodeMapInfo[nodeIndex].mappingMethod);
			//GL_ERROR_CHECK;

			
			// default setting
			if (&(material->emissiveFactor[0]) != NULL)
			{
				shaderProg->setVec3Shader("emissiveFactor", material->emissiveFactor[0], material->emissiveFactor[1], material->emissiveFactor[2]);
				GL_ERROR_CHECK;
			}
			else
			{
				shaderProg->setVec3Shader("emissiveFactor", 0.0f, 0.0f, 0.0f);
			}



			if (material->alphaCutoff != NULL)
			{
				shaderProg->setFloatShader("alphaCutoff", material->alphaCutoff);
			}
			else
			{
				shaderProg->setFloatShader("alphaCutoff", 0.0f);
			}

			if (material->doubleSided != NULL)
			{
				shaderProg->setBoolShader("doubleSided", material->doubleSided);
			}
			else
			{
				shaderProg->setBoolShader("doubleSided", 0.0f);
			}


			shaderProg->setBoolShader("existBaseColorTexture", nodeMapInfo[nodeIndex].baseColorTexture.Exist);
			GL_ERROR_CHECK;

			//if (nodeMapInfo[nodeIndex].mappingMethod == MappingMethod::Cube)
			//{
			//	updateCubemap(nodeMapInfo[nodeIndex].baseColorTexture.Id, progSkyBoxImageToCubemap, cube);
			//	glActiveTexture(GL_TEXTURE8);
			//	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapId);
			//	GL_ERROR_CHECK; // check openGL error
			//	/*glActiveTexture(GL_TEXTURE3);
			//	glBindTexture(GL_TEXTURE_2D, nodeMapInfo[nodeIndex].baseColorTexture.Id);*/
			//}

			if (nodeMapInfo[nodeIndex].baseColorTexture.Exist)
			{
				glActiveTexture(GL_TEXTURE3);
				glBindTexture(GL_TEXTURE_2D, nodeMapInfo[nodeIndex].baseColorTexture.Id);

			}
			else
			{
				shaderProg->setVec4Shader("baseColorFactor", defaultMaterialPBRMetallicRoughness.baseColorFactor[0], defaultMaterialPBRMetallicRoughness.baseColorFactor[1], defaultMaterialPBRMetallicRoughness.baseColorFactor[2], defaultMaterialPBRMetallicRoughness.baseColorFactor[3]);
			}

			shaderProg->setBoolShader("existMetallicRoughnessTexture", nodeMapInfo[nodeIndex].metallicRoughnessTexture.Exist);
			if (nodeMapInfo[nodeIndex].metallicRoughnessTexture.Exist)
			{
				glActiveTexture(GL_TEXTURE4);
				glBindTexture(GL_TEXTURE_2D, nodeMapInfo[nodeIndex].metallicRoughnessTexture.Id);
			}
			else
			{
				if (material->pbrMetallicRoughness->metallicFactor >= 0.0f)
				{
					shaderProg->setFloatShader("metallicFactor", material->pbrMetallicRoughness->metallicFactor);
				}
				else
				{
					shaderProg->setFloatShader("metallicFactor", defaultMaterialPBRMetallicRoughness.metallicFactor);
				}

				if (material->pbrMetallicRoughness->roughnessFactor >= 0.0f)
				{
					shaderProg->setFloatShader("roughnessFactor", material->pbrMetallicRoughness->roughnessFactor);
				}
				else
				{
					shaderProg->setFloatShader("roughnessFactor", defaultMaterialPBRMetallicRoughness.roughnessFactor);
				}
			}


			shaderProg->setBoolShader("existNormalTexture", nodeMapInfo[nodeIndex].normalTexture.Exist);
			if (nodeMapInfo[nodeIndex].normalTexture.Exist)
			{
				glActiveTexture(GL_TEXTURE5);
				glBindTexture(GL_TEXTURE_2D, nodeMapInfo[nodeIndex].normalTexture.Id);
			}
			else
			{
				shaderProg->setFloatShader("scale", defaultNormalTexture.scale);
			}

			shaderProg->setBoolShader("existOcclusionTexture", nodeMapInfo[nodeIndex].occlusionTexture.Exist);
			if (nodeMapInfo[nodeIndex].occlusionTexture.Exist)
			{
				glActiveTexture(GL_TEXTURE6);
				glBindTexture(GL_TEXTURE_2D, nodeMapInfo[nodeIndex].occlusionTexture.Id);
			}
			else
			{
				shaderProg->setFloatShader("strength", defaultOcclusionTexture.strength);
			}

			shaderProg->setBoolShader("existEmissiveTexture", nodeMapInfo[nodeIndex].emissiveTexture.Exist);
			if (nodeMapInfo[nodeIndex].emissiveTexture.Exist)
			{
				glActiveTexture(GL_TEXTURE7);
				glBindTexture(GL_TEXTURE_2D, nodeMapInfo[nodeIndex].emissiveTexture.Id);
			}
			else
			{
				// do nothing: alreday default was set;
			}

			// to be coded like this
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);
			glDisable(GL_BLEND);
			if (material->alphaMode == "BLEND")  // NO WORK
			{

				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR);
				//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				//glBlendFunc(GL_ONE, GL_ONE);
				//glBlendFunc(GL_CONSTANT_COLOR, GL_ONE_MINUS_CONSTANT_COLOR);

			}

			GLenum mode = primitiveMode; 
			GLsizei count = nodeMapInfo[nodeIndex].indicesCount;

			GLenum type = libgltf::GSAccessorComponentTypes[(uint32_t)TriangleData.at("TRIANGLE").componentType].value;
			//GLenum type = GL_UNSIGNED_SHORT;

			glDrawElements(mode, count, type, 0); 
			GL_ERROR_CHECK2("mode=" << mode << "count=" << count << "type=<< ype");
			{// debug
				std::string folderName = directory.substr(directory.find_last_of('/') + 1, string::npos);
				if (folderName == "NA") // e.g. == "Phobos" to Log) 
				{
					WIN_LOG(folderName.c_str() << ": " << "glDrawElements(" << Tools::glEnumToString(mode) << ", " << count << ", " << Tools::glEnumToString(type) << ",   0);  mappingMethod=" << (int)nodeMapInfo[nodeIndex].mappingMethod);
				}
			}
			glBindVertexArray(0);

			//glUseProgram(0);
		}

		std::shared_ptr<libgltf::IglTFLoader> Loader(const std::string& path)
		{
			gltf_loader = Create
			(
				[path](const std::string& _path)
				{
					std::experimental::filesystem::path file_path = std::experimental::filesystem::path(path);


					if (!std::experimental::filesystem::exists(file_path))
					{
						//std::ostringstream ssMsg;
						//ssMsg << "LoaderGLTF; Load loaded_gltf file: path is wrong! : " << path;
						ERROR_EXIT("LoaderGLTF; Load loaded_gltf file: path is wrong! : %s" << path);
					}

					std::shared_ptr<std::istream> stream_ptr = std::make_shared<std::ifstream>(file_path.string(), std::ios::in | std::ios::binary);
					return stream_ptr;
				}
			);

			loaded_gltf = gltf_loader->glTF().get();

			if (loaded_gltf == nullptr)
			{
				//ssMsg.clear();
				//ssMsg << "modelglth.h; Failed to load  gltf file : " << path;
				ERROR_EXIT("modelglth.h; Failed to load  gltf file : %s" << path;);
			}

			// retrieve the directory path of the filepath
			directory = path.substr(0, path.find_last_of('/'));

			fileName = path.substr(path.find_last_of('/') + 1, string::npos);

			return gltf_loader;
		};

		//public:
			/// get the glTF structure
		const std::unique_ptr<libgltf::SGlTF>& glTF() const
		{

			return std::make_unique<libgltf::SGlTF>();
		};

		/// load the indices data
		bool LoadMeshPrimitiveIndicesData(size_t _mesh_index, size_t _primitive_index, std::shared_ptr<libgltf::IAccessorStream> _accessor_stream)
		{

			int32_t accessorIdx = loaded_gltf->meshes[_mesh_index]->primitives[_primitive_index]->indices->int32_tValue;

			std::shared_ptr<libgltf::SAccessor> accessor = loaded_gltf->accessors[accessorIdx];
			std::shared_ptr<libgltf::SBufferView> bufferview = loaded_gltf->bufferViews[accessor->bufferView->int32_tValue];
			std::shared_ptr<libgltf::SBuffer> buffer = loaded_gltf->buffers[bufferview->buffer->int32_tValue];

			char* dataFile{};
			std::string strData{};
			std::ifstream file{};
			bool dataURI{ false };

			std::vector<unsigned char>  out{};

			std::string mime_type = "application/octet-stream";

			dataURI = tinygltf::IsDataURI(buffer->uri);
			if (dataURI)
			{
				tinygltf::DecodeDataURI(&out, mime_type, buffer->uri, buffer->byteLength, false);
				strData = std::string(out.begin(), out.end());
				strData = strData.substr(bufferview->byteOffset, bufferview->byteLength);
			}
			else
			{// external file
				std::string filepath = directory + "/" + buffer->uri;

				file = std::ifstream{ filepath, std::ifstream::binary };
				if (!file)
				{
					ERROR_EXIT("Cannot open file: buffer uri: " << buffer->uri);
				}

				dataFile = new char[buffer->byteLength];
				file.read(dataFile, buffer->byteLength);
				strData.assign(dataFile, buffer->byteLength);
				strData = strData.substr(bufferview->byteOffset, bufferview->byteLength);
				file.clear();
				file.close();
			}


			libgltf::SAccessorData  accessordata{};
			accessordata.componentType = libgltf::Int32ToAccessorComponentType(accessor->componentType);
			accessordata.count = accessor->count;
			accessordata.type = libgltf::TextToAccessorType(accessor->type);
			accessordata.bufferStride = accessor->byteOffset;

			libgltf::SBufferData bufferdata{};
			//std::string s(out.begin(), out.end());
			bufferdata.buffer = (uint8_t*)strData.c_str();
			bufferdata.bufferSize = bufferview->byteLength;
			bufferdata.bufferStride = bufferview->byteStride;
			//bufferdata.bufferStride = bufferview->byteOffset;

			accessordata.bufferData = bufferdata;
			if (accessordata >> (indices_data))
			{
				_accessor_stream->operator<< (accessordata);
			}
			else
			{
				ERROR_EXIT("can not Load Mesh Primitive Triangle Data: Failed");
			}

			TriangleData.emplace("TRIANGLE", accessordata);

			//delete[] dataFile;
			//file.clear();
			strData.clear();

			accessor.reset();
			bufferview.reset();
			buffer.reset();

			return true;

		};

		/// load the attribute data like position, normal, texcoord, etc
		bool LoadMeshPrimitiveAttributeData(size_t _mesh_index, size_t _primitive_index, const std::string& _attribute, std::shared_ptr<libgltf::IAccessorStream> _accessor_stream)
		{
			int32_t accessorIdx = loaded_gltf->meshes[_mesh_index]->primitives[_primitive_index]->attributes[_attribute]->int32_tValue;

			std::shared_ptr<libgltf::SAccessor> accessor = loaded_gltf->accessors[accessorIdx];
			std::shared_ptr<libgltf::SBufferView> bufferview = loaded_gltf->bufferViews[accessor->bufferView->int32_tValue];
			std::shared_ptr<libgltf::SBuffer> buffer = loaded_gltf->buffers[bufferview->buffer->int32_tValue];


			char* dataFile{};
			unsigned char* dataUcFile{};
			std::string strData{};
			bool dataURI{ false };
			std::ifstream file{};

			std::vector<unsigned char>   out{};

			//std::string mime_type = "application/octet-stream";
			std::string mime_type{};//  = "application/octet-stream";
			dataURI = tinygltf::IsDataURI(buffer->uri);
			if (dataURI)
			{
				tinygltf::DecodeDataURI(&out, mime_type, buffer->uri, buffer->byteLength, false);
				strData = std::string(out.begin(), out.end());
				strData = strData.substr(bufferview->byteOffset, bufferview->byteLength);
			}
			else
			{// external file
				std::string filepath = directory + "/" + buffer->uri;

				file = std::ifstream{ filepath, std::ifstream::binary };

				if (!file)
				{
					ERROR_EXIT("Cannot open file: buffer uri: %s" << buffer->uri);
				}

				dataFile = new char[buffer->byteLength];
				file.read(dataFile, buffer->byteLength);
				strData.assign(dataFile, buffer->byteLength);

				strData = strData.substr(bufferview->byteOffset, bufferview->byteLength);
			}


			libgltf::SAccessorData  accessordata{};
			accessordata.componentType = libgltf::Int32ToAccessorComponentType(accessor->componentType);
			accessordata.count = accessor->count;  // Number of data element
			accessordata.type = libgltf::TextToAccessorType(accessor->type);
			accessordata.bufferStride = accessor->byteOffset;

			libgltf::SBufferData bufferdata{};
			//std::string s(out.begin(), out.end());
			bufferdata.buffer = (uint8_t*)strData.c_str();
			bufferdata.bufferSize = bufferview->byteLength;
			bufferdata.bufferStride = bufferview->byteOffset;

			accessordata.bufferData = bufferdata;

			_accessor_stream->operator<< (accessordata);

			AttributesData.emplace(_attribute, accessordata);

			delete[] dataFile;
			file.clear();
			strData.clear();

			accessor.reset();
			bufferview.reset();
			buffer.reset();

			return true;
		};

		/// load the image data and type
		bool LoadImageData(size_t _index, std::vector<uint8_t>& _data, std::string& _type)
		{
			return true;
		}


		bool LoadImageData(size_t _index, std::vector<uint8_t>& _data, std::string& _type, GLuint& _textureId)
		{
			texture = loaded_gltf->textures[_index];
			image = loaded_gltf->images[texture->source->int32_tValue];

			//Moon does not have sampler;
			if (texture->sampler != nullptr)
			{
				sampler = loaded_gltf->samplers[texture->sampler->int32_tValue];
			}
			else
			{
				sampler = nullptr;
			}

			GLuint textureId{ 0 };

			if (!image->uri.empty())
			{
				// load image data from external file: image->uri
				std::string filepath1 = directory + "/" + image->uri;
				const char* filepath2 = filepath1.c_str();
				textureId = Tools::loadTextureSTBI(filepath2);
				_textureId = textureId;

				image.reset();
				texture.reset();
				sampler.reset();

				return true;

			}

			std::shared_ptr<libgltf::SBufferView> bufferview = loaded_gltf->bufferViews[image->bufferView->int32_tValue];
			std::string imageMimeType = image->mimeType;
			std::shared_ptr<libgltf::SBuffer> buffer = loaded_gltf->buffers[bufferview->buffer->int32_tValue];

			char* dataFile{};
			unsigned char* dataUcFile{};
			std::string strData{};
			std::ifstream file{};
			bool dataURI{ false };

			std::vector<unsigned char>  out{};

			//std::string mime_type = "data:image/png;base64";
			//std::string mime_type = "application/octet-stream";
			std::string mime_type = imageMimeType;

			dataURI = tinygltf::IsDataURI(buffer->uri);
			if (dataURI)
			{
				tinygltf::DecodeDataURI(&out, mime_type, buffer->uri, buffer->byteLength, false);
				strData = std::string(out.begin(), out.end());
				strData = strData.substr(bufferview->byteOffset, bufferview->byteLength);
			}
			else
			{// external file
				std::string filepath = directory + "/" + buffer->uri;

				file = std::ifstream{ filepath, std::ifstream::binary };

				if (!file)
				{
					ERROR_EXIT("Cannot open file: buffer uri: %s" << buffer->uri);
				}

				dataFile = new  char[buffer->byteLength];
				file.read(dataFile, buffer->byteLength);
				strData.assign(dataFile, buffer->byteLength);

				strData = strData.substr((bufferview->byteOffset), (bufferview->byteLength));
			}

			
			{// debug
				if (false)
				{
					char pngSignature[] = "\211PNG\r\n\032\n"; // strData.substr(0, 8);
					//char pngSignature[] = { '\211', 'P', 'N', 'G', '\r', '\n', '\032', '\n' }; // strData.substr(0, 8);
					Win::log("Debug PNG SIGNATURE:%s", pngSignature);
					size_t f = strData.find("IEND");
					//size_t f = strData.find("image/png");
					strData = strData.replace(0, 8, pngSignature);
				}
			}

			size_t len = strData.length();
			unsigned char* data = nullptr;
			int width{}, height{}, channels{};
			stbi_set_flip_vertically_on_load(false);
			//stbi_set_flip_vertically_on_load(true);//debug
			data = stbi_load_from_memory((stbi_uc*)strData.c_str(), bufferview->byteLength, &width, &height, &channels, NULL);
			if (data == NULL)
			{
				stbi_image_free(data);
				ERROR_EXIT("stbi_load_from_memory -> Failed to load at path");
			}

			{// debug
				if (true)
				{
					// for debug
					//std:; string fileNameW = image->name + ".png";
					//stbi_write_png(fileNameW.c_str(), width, height, 4, data, width * 4);

					/*
					//data = (unsigned char*)(strData.c_str());
					std:; vector<unsigned char> vData(data, data + 100);
					size_t s = sizeof(data)/ sizeof(*data);
					/*std::vector<unsigned char> vData{};
					for each (auto c in data)
					{

					}*/

					std::string fileNameW = directory + "/" + image->name + "_debug.png";
					stbi_write_png(fileNameW.c_str(), width, height, 4, data, width * 4);
					//data = stbi_load(fileNameW.c_str(), &width, &height, &channels, NULL);


					/*if (image->name == "Image_1")
					{
						/*std::string fileNameW = image->name + ".png";
						stbi_write_png(fileNameW.c_str(), width, height, 4, data, width * 4);
						unsigned char* data1 = stbi_load(fileNameW.c_str(), &width, &height, &channels, NULL);
						genCaptureCubemap(data1);
						//genCaptureCubemap(data);
					}*/
				}
			} // End: Image data debugging



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

			glGenTextures(1, &textureId);
			glBindTexture(GL_TEXTURE_2D, textureId);
			glTexImage2D(GL_TEXTURE_2D, 0, internalformat, width, height, 0, format, type, data);// 

			//glGenerateMipmap(GL_TEXTURE_2D);
			GL_ERROR_CHECK;

			if (sampler->minFilter)
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, sampler->minFilter);
				//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				WIN_LOG("Channels=" << channels << ", TextureId=" << textureId << " of " << directory << "/" << image->name << ", glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, " << Tools::glEnumToString(sampler->minFilter) << ")");
				GL_ERROR_CHECK;
			}

			if (sampler->magFilter)
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, sampler->magFilter);
				WIN_LOG("TextureId=" << textureId << " of " << directory << "/" << image->name << ", glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, " << Tools::glEnumToString(sampler->magFilter) << ")");
				GL_ERROR_CHECK;
			}

			if (sampler->wrapS)
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, sampler->wrapS);
				WIN_LOG("TextureId=" << textureId << " of " << directory << "/" << image->name << ", glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, " << Tools::glEnumToString(sampler->wrapS) << ")");
				GL_ERROR_CHECK;
			}

			if (sampler->wrapT)
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, sampler->wrapT);
				WIN_LOG("TextureId=" << textureId << " of " << directory << "/" << image->name << ", glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, " << Tools::glEnumToString(sampler->wrapT) << ")");
				GL_ERROR_CHECK;
			}

			//glDisable(GL_FRAMEBUFFER_SRGB); // test purpose for GL_SRGB8

			GL_ERROR_CHECK2("glTexParameter setting");

			glGenerateMipmap(GL_TEXTURE_2D);
			GL_ERROR_CHECK;

			//texture.clear();
			stbi_image_free(data);
			strData.clear();
			file.clear();
			delete[] dataFile;

			image.reset();
			buffer.reset();
			bufferview.reset();
			texture.reset();
			sampler.reset();

			_textureId = textureId;

			return true;
		};

		// Process the meshes, cameras, etc., that are attached to this node
		//void traverseNode(std::shared_ptr<libgltf::SNode> node)
		void traverseNode(int32_t nodeIndex)
		{
			//processElements(node); // mesh, camera, etc.
			processElements(nodeIndex); // mesh, camera, etc.

			// Recursively process all children
			for each (auto child in loaded_gltf->nodes[nodeIndex]->children)
			{
				traverseNode(child->int32_tValue);
			}
		}


		// processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
		//void processElements(std::shared_ptr<libgltf::SNode> node)
		void processElements(int32_t nodeIndex)
		{

			if (loaded_gltf->nodes[nodeIndex]->camera != nullptr)
			{
				processCamera(nodeIndex, loaded_gltf->nodes[nodeIndex]->camera->int32_tValue);
			}

			if (loaded_gltf->nodes[nodeIndex]->skin != nullptr)
			{
				processSkin(nodeIndex, loaded_gltf->nodes[nodeIndex]->skin->int32_tValue);
			}

			// Animation to be coded


			// mesh, including wights
			size_t meshIndex = loaded_gltf->nodes[nodeIndex]->mesh->int32_tValue;
			//processMesh(meshIndex, Meshes[meshIndex]->primitives, node->weights);
			processMesh(nodeIndex, meshIndex);

		}


		MappingMethod findMapping(std::string& in)
		{
			std::string mapping = "Cube"; // SphereToCubeMap
			if (in.find(mapping) == 0)
			{
				return MappingMethod::Cube;
			}

			mapping = "cylindrically_mapped_sphere"; // CylindricalToSphereMap
			if (in.find(mapping) == 0)
			{
				return MappingMethod::cylindrically_mapped_sphere;
			}

			mapping = "Plane"; // RingMap
			if (in.find(mapping) == 0)
			{
				return MappingMethod::Plane;
			}
			mapping = "Sphere"; // SphereToCubeMap
			if (in.find(mapping) == 0)
			{
				return MappingMethod::Sphere;
			}


			return MappingMethod::Default; // plain map, i.e. mesh texcoord
		}


		void processMesh(int32_t nodeIndex, int32_t meshIndex)
		{
			std::shared_ptr<libgltf::SMesh>  mesh = loaded_gltf->meshes[meshIndex];

			mappingMethod = findMapping(loaded_gltf->meshes[meshIndex]->name);
			if (mappingMethod == MappingMethod::Default)
			{
				//ssMsg.clear();
				//ssMsg << "void processMesh(size_t meshIdx): Mesh has no Mapping Mrthod";
				WIN_LOG_ERROR("mappinngMethod: Default Happened; May need to correct it");

			}

			nodeMapInfo[nodeIndex].mappingMethod = mappingMethod;


			/* Cube mapping*/
			//if (nodeMapInfo[nodeIndex].mappingMethod == MappingMethod::Cube) // cube
			//{
			//	genCubemap();

			//	cube = new Model::Model{ pathCommonResources("objects/cube/cube_blender.obj") };
			//	progSkyBoxImageToCubemap = new Shader::Shader{ pathShader("skyboxcubemap.vert").c_str(), pathShader("skyboxcubemap.frag").c_str() };
			//}

			/* Cube mapping*/


			for (int32_t primitiveIndex = 0; primitiveIndex < loaded_gltf->meshes[meshIndex]->primitives.size(); primitiveIndex++)
			{
				//glBindVertexArray(vao);
				glBindVertexArray(Vaos[nodeIndex]);

				processPrimitive(nodeIndex, meshIndex, primitiveIndex); // attributes, indices, materials

				glBindVertexArray(0);
				//glDeleteBuffers

				primitiveIndex += 1;
			}


		}

		// need to recode for multiple primitives, the below coding works only for single primitive!!!
		void processPrimitive(int32_t nodeIndex, int32_t meshIndex, int32_t primitiveIndex)
		{
			AttributesData.clear();
			TriangleData.clear();
			MaterialsData.clear();

			std::shared_ptr<libgltf::SMeshPrimitive> primitive = loaded_gltf->meshes[meshIndex]->primitives[primitiveIndex];

			primitiveMode = primitive->mode;  // for debug

			// buffer size  for attributes
			std::shared_ptr<libgltf::SAccessor> accessor{};
			std::shared_ptr<libgltf::SBufferView> bufferview{};
			int32_t byteLengthInAll{ 0 };

			std::map<GLuint, float> bufferDataOffset{};
			for each (auto pair in primitive->attributes)
			{
				accessor = loaded_gltf->accessors[pair.second->int32_tValue];
				bufferview = loaded_gltf->bufferViews[accessor->bufferView->int32_tValue];
				byteLengthInAll += bufferview->byteLength;

				bufferDataOffset.emplace(pair.second->int32_tValue, bufferview->byteOffset);
			}

			// calculate right offset as bufferdata
			float firstByteOffset{};
			bool firstValue{ true };
			for each (auto pair in bufferDataOffset)
			{
				if (firstValue)
				{
					firstByteOffset = pair.second;
					firstValue = false;
				}
				bufferDataOffset[pair.first] = pair.second - firstByteOffset;
			}
			accessor.reset();
			bufferview.reset();

			// glBufferData, glBufferSubdata params
			GLintptr offset{};
			GLsizeiptr sizeptr{};
			const void* data{};
			//uint16_t * data1{};
			// glVertexAttribPointer params
			GLuint index{};
			GLint sizeint{}; // dimension
			GLenum type{};  //data type
			GLsizei stride{};
			const void* pointer{}; // ???


			std::vector<glm::vec3> pos{};
			GLenum target = GL_ARRAY_BUFFER;
			//glBindBuffer(target, abo);
			glBindBuffer(target, Abos[nodeIndex]);
			glBufferData(target, byteLengthInAll, 0, GL_DYNAMIC_DRAW);

			GL_ERROR_CHECK2("glBufferData: Mesh = " << loaded_gltf->nodes[nodeIndex]->name.c_str() << ", BytelengthAll = " << byteLengthInAll);

			// attributes
			for each (auto pair in primitive->attributes)
			{

				if (pair.first == "POSITION")
				{
					position_data.clear();
					position_stream = std::make_shared<libgltf::TAccessorStream<libgltf::TVertexList<3, float>>>(position_data);

					if (LoadMeshPrimitiveAttributeData(meshIndex, primitiveIndex, pair.first, position_stream))
					{
						// glBufferSubdata params
						data = position_data.Data();

						// glVertexAttribPointer params
						index = 0;
						pointer = 0;// ??
					}
					else
					{
						ERROR_EXIT("can not Load Mesh Primitive Attribute Data :" << pair.first << "Failed");
					};

				}
				else if (pair.first == "NORMAL")
				{
					normal_data.clear();
					normal_stream = std::make_shared<libgltf::TAccessorStream<libgltf::TVertexList<3, float>>>(normal_data);

					if (LoadMeshPrimitiveAttributeData(meshIndex, primitiveIndex, pair.first, normal_stream))
					{
						// glBufferSubdata params
						data = normal_data.Data();

						// glVertexAttribPointer params
						index = 1;
						//pointer = 0;// 0;// ??
					}
					else
					{
						ERROR_EXIT("can not Load Mesh Primitive Attribute Data: %s" << pair.first);
					};
				}
				else if (pair.first == "TEXCOORD_0")
				{
					texcoord_0_data.clear();
					texcoord_0_stream = std::make_shared<libgltf::TAccessorStream<libgltf::TVertexList<2, float>>>(texcoord_0_data);

					if (LoadMeshPrimitiveAttributeData(meshIndex, primitiveIndex, pair.first, texcoord_0_stream))
					{
						// glBufferSubdata params
						data = texcoord_0_data.Data();

						// glVertexAttribPointer params
						index = 2;
						//pointer = 0;// 0;// ??

						{// debug texcoord
							if (false) {
								int size;
								const char* buffer;
								std::string delimiter = ",";
								std::string headline;
								std::string line;
								std::string fileName = directory + "/gltf_texcoord_0_debug.csv";
								std::ofstream outfile;
								outfile.open(fileName, std::ios::out);
								if (!outfile.is_open())
								{	// delete existin file, amd then reopen;
									int res = remove(fileName.c_str());
									outfile.open(fileName, std::ios::out);
								}
								// headline
								for (auto& d : texcoord_0_data)
								{
									headline = {};
									for (int i = 0; i < d.Dimension; i++)
									{
										headline.append("texcoord_0_" + std::to_string(i) + delimiter);
									}
									headline.append("\n");
									size = headline.size();
									buffer = new char[size];
									buffer = headline.c_str();
									outfile.write(buffer, size);
									break; // only one line
								}

								// all lines
								for (auto& d : texcoord_0_data)
								{
									line = {};
									for (int i = 0; i < d.Dimension; i++)
									{
										line.append(std::to_string(d[i]) + delimiter);
									}
									line.append("\n");
									size = line.size();
									buffer = new char[size];
									buffer = line.c_str();
									outfile.write(buffer, size);
								}
								outfile.close();
							}
						}


					}
					else
					{
						ERROR_EXIT("can not Load Mesh Primitive Attribute Data: %s" << pair.first);
					};
				}
				else if (pair.first == "TANGENT")
				{
					tangent_data.clear();
					tangent_stream = std::make_shared<libgltf::TAccessorStream<libgltf::TVertexList<3, float>>>(tangent_data);

					if (LoadMeshPrimitiveAttributeData(meshIndex, primitiveIndex, pair.first, tangent_stream))
					{
						// glBufferSubdata params
						data = tangent_data.Data();

						// glVertexAttribPointer params
						index = 3;
						//pointer = 0;// 0 ??
					}
					else
					{
						ERROR_EXIT("can not Load Mesh Primitive Attribute Data: %s" << pair.first);
					};
				}
				else if (pair.first == "BITANGENT")
				{
					bitangent_data.clear();
					bitangent_stream = std::make_shared<libgltf::TAccessorStream<libgltf::TVertexList<3, float>>>(bitangent_data);

					if (LoadMeshPrimitiveAttributeData(meshIndex, primitiveIndex, pair.first, bitangent_stream))
					{
						// glBufferSubdata params
						data = bitangent_data.Data();

						// glVertexAttribPointer params
						index = 4;
						//pointer = 0;// 0;// ??
					}
					else
					{
						ERROR_EXIT("can not Load Mesh Primitive Attribute Data: %s" << pair.first);
					};
				}
				else
				{
					//ssMsg.clear();
					//ssMsg << "Unrecognized attribute: %s: Failed" << pair.first;
					ERROR_EXIT("Unrecognized attribute: %s: Failed" << pair.first);
				}

				// glBufferSubdata params
				//pointer = (void*)AttributesData.at(pair.first).bufferData.bufferStride;
				offset = bufferDataOffset.at(pair.second->int32_tValue);
				pointer = (void*)offset;
				sizeptr = AttributesData.at(pair.first).bufferData.bufferSize;

				// glVertexAttribPointer params
				sizeint = libgltf::DimensionOfAccessorType(AttributesData.at(pair.first).type);
				type = libgltf::GSAccessorComponentTypes[(uint8_t)AttributesData.at(pair.first).componentType].value;
				stride = sizeint * SizeOfAccessorComponentType(AttributesData.at(pair.first).componentType);

				glBufferSubData(target, offset, sizeptr, data);
				GL_ERROR_CHECK2("glBufferSubData:  Mesh=" << loaded_gltf->nodes[nodeIndex]->name.c_str() << ", Vertex=" << pair.first.c_str() << ", Index=" << index << "bytelengthAll = offset + size-> " << byteLengthInAll << " = " << offset << " + " << sizeptr);

				glEnableVertexAttribArray(index);
				GL_ERROR_CHECK2("glEnableVertexAttribArray: Mesh=" << loaded_gltf->nodes[nodeIndex]->name.c_str() << ", Vertex=" << pair.first.c_str() << ", Index=" << index);

				glVertexAttribPointer(index, sizeint, type, GL_FALSE, stride, pointer);
				GL_ERROR_CHECK2("glVertexAttribPointer: Mesh=" << loaded_gltf->nodes[nodeIndex]->name.c_str() << ", Vertex=" << pair.first.c_str() << ", Index=" << index);
				{// debug
					std::string folderName = directory.substr(directory.find_last_of('/') + 1, string::npos);
					if (folderName == "NA") // e.g. == "Phobos" to Log) 
					{
						WIN_LOG("glVertexAttribPointer: " << folderName.c_str() << ", Mesh = " << loaded_gltf->nodes[nodeIndex]->name.c_str() << ", Attribute = " << pair.first.c_str() << ", Index = " << index << ", Target=" << target << ":" << Tools::glEnumToString(target) << ", bytelengthAll = offset + sizeptr-> " << byteLengthInAll << " = " << offset << " + " << sizeptr << ", Component Size=" << sizeint << ", Type=" << type << ":" << Tools::glEnumToString(type) << ", Stride=" << stride << ", Pointer=" << (int)pointer);
					}
				}

			}


			// indices
			indices_data.clear();

			indices_stream = std::make_shared<libgltf::TAccessorStream<libgltf::TVertexList<1, uint16_t>>>(indices_data);
			if (LoadMeshPrimitiveIndicesData(meshIndex, primitiveIndex, indices_stream))
			{
				target = GL_ELEMENT_ARRAY_BUFFER;
				sizeptr = TriangleData.at("TRIANGLE").bufferData.bufferSize;
				data = indices_data.Data();
				nodeMapInfo[nodeIndex].indicesCount = indices_data.size();
				glBindBuffer(target, Eabos[nodeIndex]);
				glBufferData(target, sizeptr, data, GL_DYNAMIC_DRAW);
			}
			else
			{
				ERROR_EXIT("can not Load Mesh Primitive Triangle Data: Failed");
			};

			//delete data;

			// Materials
			if (primitive->material != nullptr)
			{
				nodeMapInfo[nodeIndex].materialIndex = primitive->material->int32_tValue;
				processMaterial(nodeIndex, primitive->material->int32_tValue);

			}

			// Morpf TARGETS
			//debug
			if (primitive->targets.size() > 0)
			{
				for each (auto target in primitive->targets)
				{
					for each (auto map in target)
					{
						std::string name = map.first;
						int32_t value = map.second->int32_tValue;
					}
				}
			}


			primitive.reset();

		}


		void processAttributes(int32_t nodeIndex, int32_t meshIndex, int32_t primitiveIndex)
		{


		}

		void processIndices(int32_t nodeIndex, int32_t meshIndex, int32_t primitiveIndex)
		{
		}

		void processMaterial(int32_t nodeIndex, int32_t materialIndex)
		{
			std::shared_ptr<libgltf::SMaterial>material = loaded_gltf->materials[materialIndex];

			{// albedo,  and metallic&roughness
				// A set of parameter values that are used to define the metallic-roughness material model from Physically Based Rendering (PBR) methodology. When undefined, all the default values of `pbrMetallicRoughness` **MUST** apply.

				if (material->pbrMetallicRoughness == nullptr)
				{
					// no pbrMetallicRoughness
					nodeMapInfo[nodeIndex].baseColorTexture.Exist = false;
					nodeMapInfo[nodeIndex].metallicRoughnessTexture.Exist = false;
				}
				else if ((material->pbrMetallicRoughness->baseColorTexture != nullptr) && (material->pbrMetallicRoughness->metallicRoughnessTexture == nullptr))
				{
					//baseColorTexture exists
					nodeMapInfo[nodeIndex].baseColorTexture.Exist = true;;

					size_t textureIdx = material->pbrMetallicRoughness->baseColorTexture->index->int32_tValue;
					// The set index of texture's TEXCOORD attribute used for texture coordinate mapping.???
					int32_t texCoordIdx = material->pbrMetallicRoughness->baseColorTexture->texCoord;

					std::vector<uint8_t> data{};
					std::string imageMimeType{};
					LoadImageData(textureIdx, data, imageMimeType, nodeMapInfo[nodeIndex].baseColorTexture.Id);

					// metallicRoughnessTexture non-existant
					nodeMapInfo[nodeIndex].metallicRoughnessTexture.Exist = false;
				}
				else if ((material->pbrMetallicRoughness->baseColorTexture == nullptr) && (material->pbrMetallicRoughness->metallicRoughnessTexture != nullptr))
				{
					//baseColorTexture non-existant
					nodeMapInfo[nodeIndex].baseColorTexture.Exist = false;

					// metallicRoughnessTexture exists
					nodeMapInfo[nodeIndex].metallicRoughnessTexture.Exist = true;
					size_t imageIdx = material->pbrMetallicRoughness->metallicRoughnessTexture->index->int32_tValue;
					// The set index of texture's TEXCOORD attribute used for texture coordinate mapping.???
					int32_t texCoordIdx = material->pbrMetallicRoughness->metallicRoughnessTexture->texCoord;

					std::vector<uint8_t> data{};
					std::string imageMimeType{};
					LoadImageData(imageIdx, data, imageMimeType, nodeMapInfo[nodeIndex].metallicRoughnessTexture.Id);
				}
				else if ((material->pbrMetallicRoughness->baseColorTexture != nullptr) && (material->pbrMetallicRoughness->metallicRoughnessTexture != nullptr))
				{
					//baseColorTexture existrs
					nodeMapInfo[nodeIndex].baseColorTexture.Exist = true;
					size_t imageIdx = material->pbrMetallicRoughness->baseColorTexture->index->int32_tValue;
					// The set index of texture's TEXCOORD attribute used for texture coordinate mapping.???
					int32_t texCoordIdx = material->pbrMetallicRoughness->baseColorTexture->texCoord;

					std::vector<uint8_t> data{};
					std::string imageMimeType{};
					LoadImageData(imageIdx, data, imageMimeType, nodeMapInfo[nodeIndex].baseColorTexture.Id);

					// metallicRoughnessTexture exists
					nodeMapInfo[nodeIndex].metallicRoughnessTexture.Exist = true;
					imageIdx = material->pbrMetallicRoughness->metallicRoughnessTexture->index->int32_tValue;
					// The set index of texture's TEXCOORD attribute used for texture coordinate mapping.???
					texCoordIdx = material->pbrMetallicRoughness->metallicRoughnessTexture->texCoord;

					data.clear();
					LoadImageData(imageIdx, data, imageMimeType, nodeMapInfo[nodeIndex].metallicRoughnessTexture.Id);
				}
			}


			{// The tangent space normal texture.
				if (material->normalTexture == nullptr)
				{
					// normalTexture non-existant
					nodeMapInfo[nodeIndex].normalTexture.Exist = false;
				}
				else
				{
					// normalTexture existant
					nodeMapInfo[nodeIndex].normalTexture.Exist = true;
					size_t imageIdx = material->normalTexture->index->int32_tValue;
					// The set index of texture's TEXCOORD attribute used for texture coordinate mapping.???
					int32_t texCoordIdx = material->normalTexture->texCoord;

					std::vector<uint8_t> data{};
					std::string imageMimeType{};
					LoadImageData(imageIdx, data, imageMimeType, nodeMapInfo[nodeIndex].normalTexture.Id);
				}
			}


			{// The occlusion texture.
				if (material->occlusionTexture == nullptr)
				{
					// occlusionTexture non-existant
					nodeMapInfo[nodeIndex].occlusionTexture.Exist = false;
				}
				else
				{
					// occlusionTexture existant
					nodeMapInfo[nodeIndex].occlusionTexture.Exist = true;
					size_t imageIdx = material->occlusionTexture->index->int32_tValue;
					// The set index of texture's TEXCOORD attribute used for texture coordinate mapping.???
					int32_t texCoordIdx = material->occlusionTexture->texCoord;

					std::vector<uint8_t> data{};
					std::string imageMimeType{};
					LoadImageData(imageIdx, data, imageMimeType, nodeMapInfo[nodeIndex].occlusionTexture.Id);
				}

			}


			{// The emissive texture.
				if (material->emissiveTexture == nullptr)
				{
					// emissiveTexture non-exisatnt
					nodeMapInfo[nodeIndex].emissiveTexture.Exist = false;
				}
				else
				{
					// emissiveTexture existant
					nodeMapInfo[nodeIndex].emissiveTexture.Exist = true;
					size_t imageIdx = material->emissiveTexture->index->int32_tValue;
					// The set index of texture's TEXCOORD attribute used for texture coordinate mapping.???
					int32_t texCoordIdx = material->emissiveTexture->texCoord;

					std::vector<uint8_t> data{};
					std::string imageMimeType{};
					LoadImageData(imageIdx, data, imageMimeType, nodeMapInfo[nodeIndex].emissiveTexture.Id);
				}
			}
			material.reset();

		}

		void processCamera(int32_t nodeIndex, int32_t cameraIndex)
		{
			std::shared_ptr<libgltf::SCamera> camera = loaded_gltf->cameras[cameraIndex];
		}

		void processSkin(int32_t nodeIndex, int32_t skinIndex)
		{
			std::shared_ptr<libgltf::SSkin> skin = loaded_gltf->skins[skinIndex];
		}


		//void processAnimation(std::vector<std::shared_ptr<libgltf::SAnimation>> animations)
		void processAnimation(int32_t animationIndex)
		{

		}


	private:

		std::ostringstream ssMsg{};

		libgltf::TVertexList<1, unsigned short> indices_data{};
		libgltf::TVertexList<3, float> position_data{};
		libgltf::TVertexList<2, float> texcoord_0_data{};
		libgltf::TVertexList<3, float> normal_data{};
		libgltf::TVertexList<3, float>  tangent_data{};
		libgltf::TVertexList<3, float>  bitangent_data{};
		std::vector<uint8_t> image0_data{};
		///*std::vector<libgltf::TVertexList<3, float>> Vertices{};
		//std::vector<libgltf::TVertexList<1, size_t>> Indices{};*/

		std::shared_ptr<libgltf::TAccessorStream<libgltf::TVertexList<1, unsigned short> > > indices_stream{};
		std::shared_ptr<libgltf::TAccessorStream<libgltf::TVertexList<3, float> > > position_stream{};
		std::shared_ptr<libgltf::TAccessorStream<libgltf::TVertexList<3, float> > > normal_stream{};
		std::shared_ptr<libgltf::TAccessorStream<libgltf::TVertexList<2, float> > > texcoord_0_stream{};
		std::shared_ptr<libgltf::TAccessorStream<libgltf::TVertexList<3, float> > > tangent_stream{};
		std::shared_ptr<libgltf::TAccessorStream<libgltf::TVertexList<3, float> > > bitangent_stream{};

		// material parameter
		std::shared_ptr<libgltf::SMaterial> material{ nullptr };
		std::shared_ptr<libgltf::STexture> texture{ nullptr };
		std::shared_ptr<libgltf::SSampler> sampler{ nullptr };
		std::shared_ptr<libgltf::SImage> image{ nullptr };

		std::vector<uint8_t>  pBuffer{};

		/* Cube mapping*/
		struct lookAtPara {
			glm::vec3 eyePos; // cubic center for model space 
			glm::vec3 viewDirectionVec;
			glm::vec3 upVec;
		};

		std::vector<lookAtPara> captureViewPara = { // eyePos: default Position
			{glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)}, // right
			{glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)}, // left
			{glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)}, // top
			{glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)}, // bottom
			{glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, 1.0f), glm::vec3(0.0f, -1.0f,  0.0f)}, // front
			{glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f)} // back
		};

		glm::mat4 getCaptureViewMat(int face, glm::vec3 eyepos) {
			glm::vec3 centerPos = eyepos + captureViewPara[face].viewDirectionVec;
			//centerPos = glm::normalize(centerPos);  // test
			return glm::lookAt(eyepos, centerPos, captureViewPara[face].upVec);
		}

		void genCubemap()
		{
			//1 capture FBO
			glGenFramebuffers(1, &captureFBO);
			glGenRenderbuffers(1, &captureRBO);
			glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
			glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 1024, 1024);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			glGenTextures(1, &cubemapId);
			glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapId);
			for (unsigned int face = 0; face < 6; ++face) {
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, GL_RGBA32F, 1024, 1024, 0, GL_RGBA, GL_FLOAT, 0);
				//glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, GL_RGB, g_captureFboWidth, g_captureFboHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
			}
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // enable pre-filter mipmap sampling (combatting visible dots artifact)
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			GL_ERROR_CHECK; // check openGL error

			WIN_LOG("LoaderGLTF: genCubemap finished, file name = %s, cubemapId =%d\n" << fileName.c_str(), cubemapId);
		}

		GLuint updateCubemap(GLuint textureId, Shader::Shader* shaderprog, Model::Model* cube)
		{
			glm::mat4 projection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 1.1f);

			shaderprog->useShader();
			shaderprog->setMat4Shader("projection", projection);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureId);
			//GL_ERROR_CHECK; // check openGL error

			glViewport(0, 0, 1024, 1024); // don't forget to configure the viewport to the capture dimensions.
			//GL_ERROR_CHECK; // check openGL error

			glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
			//GL_ERROR_CHECK; // check openGL error

			for (unsigned int face = 0; face < 6; ++face) {
				shaderprog->setMat4Shader("view", getCaptureViewMat(face, captureViewPara[face].eyePos));
				shaderprog->setVec3Shader("cameraPos", captureViewPara[face].eyePos);
				shaderprog->setIntShader("face", face);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, cubemapId, 0);
				//GL_ERROR_CHECK; // check openGL error

				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				//GL_ERROR_CHECK; // check openGL error

				cube->Draw(shaderprog);
				GL_ERROR_CHECK; // check openGL error
			}
			glUseProgram(0);

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			// then let OpenGL generate mipmaps from first mip face (combatting visible dots artifact)
			glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapId);
			glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

			GL_ERROR_CHECK; // check openGL error

			WIN_LOG("LoaderGLTF: updateCubemap finished, basecolorTextureId = %d, cubemapId=%d" << textureId, cubemapId);

			return cubemapId;
		}
		/* Cube mapping*/

	};
}
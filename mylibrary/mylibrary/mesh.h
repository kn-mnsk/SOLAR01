// refernce to https://learnopengl.com/Model-Loading/Mesh

#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#include <string>
#include <vector>

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.h"

namespace Mesh {
	class Mesh {
	public:
		struct Vertex
		{
			glm::vec3 Position;
			glm::vec3 Normal;
			glm::vec2 TexCoords;
			glm::vec3 Tangent;
			glm::vec3 Bitangent;
		};

		struct Texture
		{
			unsigned int id{};
			std::string type;
			std::string path;
		};
		/*  Mesh Data*/
		/* Render Data*/
		Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures) {
			this->vertices = vertices;
			this->indices = indices;
			this->textures = textures;

			setupMesh();
		}

		void Draw(Shader::Shader* shader) {
			unsigned int diffuseNr = 1;
			unsigned int specularNr = 1;
			unsigned int normalNr = 1;
			unsigned int heightNr = 1;

			for (unsigned int i = 0; i < textures.size(); i++)
			{
				// activate propere texture unit before 
				glActiveTexture(GL_TEXTURE0 + i);
				// retrieve texture number (e.g. the N in texture_diffuseN)
				std::string number;
				std::string name = textures[i].type;
				if (name == "texture_diffuse")
					number = std::to_string(diffuseNr++);
				else if (name == "texture_specular")
					number = std::to_string(specularNr++);
				else if (name == "texture_normal")
					number = std::to_string(normalNr++);
				else if (name == "texture_height")
					number = std::to_string(heightNr++);

				glUniform1f(glGetUniformLocation(shader->getShaderID(), (name + number).c_str()), (GLfloat)i);
				glBindTexture(GL_TEXTURE_2D, textures[i].id);
				//GLSL::checkError(GET_FILE_LINE);

			}

			// draw mesh
			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
			//GLSL::checkError(GET_FILE_LINE);

			glBindVertexArray(0);
			// set everything back to defaults once configured
			glActiveTexture(GL_TEXTURE0);

		}

		std::vector<Vertex> getVertices() { return vertices; };

	private:
		/* render Data*/
		unsigned int VBO, EBO;
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture> textures;
		unsigned int VAO;

		/* Functions */
		void setupMesh()
		{
			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);
			glGenBuffers(1, &EBO);

			glBindVertexArray(VAO);

			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

			// vertex positions
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
			// vertex normals
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
			// vertex textures
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
			// vertex tangent
			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
			// vertex bitangent
			glEnableVertexAttribArray(4);
			glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

			glBindVertexArray(0);
		}
	};
}
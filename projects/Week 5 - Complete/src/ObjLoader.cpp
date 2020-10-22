#include "ObjLoader.h"

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

// Borrowed from https://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
#pragma region String Trimming

// trim from start (in place)
static inline void ltrim(std::string& s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
		return !std::isspace(ch);
		}));
}

// trim from end (in place)
static inline void rtrim(std::string& s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
		return !std::isspace(ch);
		}).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string& s) {
	ltrim(s);
	rtrim(s);
}

#pragma endregion 


VertexArrayObject::sptr ObjLoader::OBJLoader(const std::string& file)
{
	//Vertext portions
	std::vector< glm::vec3 > temp_vertices;
	std::vector< glm::vec2 > temp_vertexTexture;
	std::vector< glm::vec3 > temp_vertexNormals;

	//Face vectors
	std::vector< GLint > vertexIndices;
	std::vector< GLint > texcoordIndices;
	std::vector< GLint > normalIndices;


	MeshBuilder<VertexPosNormTexCol> mesh;
	std::string line = "";


	std::ifstream in_file;
	in_file.open(file, std::ios::binary);

	//File open error
	if (!in_file) {
		throw std::runtime_error("Failed to open file");
	}

	//Reading one line a time
	while (std::getline(in_file, line))
	{
		trim(line);
		if (line.substr(0, 1) == "#")
		{
			
		}
		else if (line.substr(0, 2) == "v ") //Vertext position
		{
			std::istringstream ss = std::istringstream(line.substr(2));
			glm::vec3 vertex;
			ss >> vertex.x >> vertex.y >> vertex.z;
			temp_vertices.push_back(vertex);
			
		}
		else if (line.substr(0, 3) == "vt ") //Vertext texture
		{
			std::istringstream ss = std::istringstream(line.substr(2));
			glm::vec2 vtexture;
			ss >> vtexture.x >> vtexture.y;
			temp_vertexTexture.push_back(vtexture);

		}
		else if (line.substr(0, 3) == "vn ") //Vertext normal
		{
			std::istringstream ss = std::istringstream(line.substr(2));
			glm::vec3 vnormal;
			ss >> vnormal.x >> vnormal.y >> vnormal.z;
			temp_vertexNormals.push_back(vnormal);

		}
		else if (line.substr(0, 2) == "f ") //Faces
		{
			std::string lineSubstr = line.substr(2);
			std::string withoutSlashes = lineSubstr;

			//Remove slashes
			for (int i = 0; i < withoutSlashes.length(); i++)
			{
				if (withoutSlashes[i] == '/')
					withoutSlashes[i] = ' ';
			}

			std::istringstream ss = std::istringstream(withoutSlashes);
			
			unsigned int vertexIndex[3], texcoordIndex[3], normalIndex[3];

			ss >> vertexIndex[0] >> texcoordIndex[0] >> normalIndex[0]
			   >> vertexIndex[1] >> texcoordIndex[1] >> normalIndex[1] 
			   >> vertexIndex[2] >> texcoordIndex[2] >> normalIndex[2];
			
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);

			texcoordIndices.push_back(texcoordIndex[0]);
			texcoordIndices.push_back(texcoordIndex[1]);
			texcoordIndices.push_back(texcoordIndex[2]);

			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
		
	}

	for (unsigned int i = 0; i < vertexIndices.size(); i++) {
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int texcoordIndex = texcoordIndices[i];
		unsigned int normalIndex = normalIndices[i];
		glm::vec3 vertex = temp_vertices[vertexIndex - 1];
		glm::vec2 texcoord = temp_vertexTexture[vertexIndex - 1];
		glm::vec3 normal = temp_vertexNormals[vertexIndex - 1];

		mesh.AddVertex(vertex, normal, texcoord, glm::vec4(1));
		mesh.AddIndex(i);
	}

	//Loaded Object
	return mesh.Bake();
}


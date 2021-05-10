#pragma once
#ifndef _MESH_H_
#define _MESH_H_
#include <vector>
#include <string>
#include <glm/glm.hpp>
struct Vertex {
	// position
	glm::vec3 Position;
	// normal
	glm::vec3 Normal;
	// tangent
	glm::vec3 Tangent;
	// bitangent
	glm::vec3 Bitangent;
	// texCoords
	glm::vec2 TexCoords;
};

struct Texture {
	std::string type;
	std::string path;
	unsigned int id;
};
class Mesh {
private:
	unsigned int VAO, VBO, EBO;
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	void onMeshInit();
	void onMeshRender(unsigned int shaderHandle);
	void onMeshRelease();
};

#endif
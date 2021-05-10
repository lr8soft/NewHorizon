#pragma once
#ifndef _MODEL_H_
#define _MODEL_H_
#include <assimp/scene.h>
#include <vector>
#include <string>
#include "Mesh.h"
class Model
{
public:
	Model(const std::string& path);
	void onModelInit();
	void onModelRender(unsigned int shaderHandle);
	void onModelRelease();

	bool isModelInit();
private:
	std::string modelPath;
	std::string directory;
	std::vector<Mesh> meshes;

	bool gammaCorrection;
	bool isInit = false;


	void loadModel(std::string path);
	void processNode(aiNode *node, const aiScene *scene);
	Mesh processMesh(aiMesh *mesh, const aiScene *scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
		std::string typeName);
};
#endif
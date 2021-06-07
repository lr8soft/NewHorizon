#define STB_IMAGE_IMPLEMENTATION
#include "ImageLoader.h"
#include <GL3/gl3w.h>
#include "LogUtil.hpp"
#include <string>
std::map<std::string, xc_ogl::ImageStruct> xc_ogl::ImageLoader::textureGroup;
xc_ogl::ImageLoader::ImageLoader()
{
	texture_type = GL_TEXTURE_2D;
	have_release = true;

	glGenTextures(1, &tbo);
	glBindTexture(GL_TEXTURE_2D, tbo);
}

xc_ogl::ImageLoader::ImageLoader(GLenum type,GLuint itbo)
{
	texture_type = type;
	have_release = true;
	tbo = itbo;
	glBindTexture(texture_type, tbo);
}


xc_ogl::ImageLoader::~ImageLoader()
{
	if (!have_release)
		Release();
}

void xc_ogl::ImageLoader::Release()
{
	if (!have_release) {
		//stbi_image_free(texture_ptr);
		have_release = true;
	}		
}

void xc_ogl::ImageLoader::loadTextureFromFile(const char * path, bool filpUpsideDown)
{	
	std::map<std::string, ImageStruct>::iterator item = textureGroup.find(path);
	if (item != textureGroup.end()) {
		tbo = item->second.tbo;
		width = item->second.width;
		height = item->second.height;
		channel = item->second.channel;
	}
	else {
		stbi_set_flip_vertically_on_load(filpUpsideDown);
		void* texture_ptr = stbi_load(path, &width, &height, &channel, STBI_rgb_alpha);
		glBindTexture(texture_type, tbo);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if (texture_ptr) {
			char infoFormat[256];
			if (channel == 1) {
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_ptr);//latter parameter is RGBA
				sprintf_s(infoFormat, sizeof(infoFormat), "Load image from %s format:%s", path, "GREY");
				//LogUtil::printInfo( ("Load image from" + path + "format:GREY") );
				//std::cout << "[INFO] Load image from " << path << " format:GREY" << std::endl;
			}
			else if (channel == 2) {
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_ptr);//latter parameter is RGBA
				sprintf_s(infoFormat, sizeof(infoFormat), "Load image from %s format:%s", path, "GREY ALPHA");
				//std::cout << "[INFO] Load image from " << path << " format:GREY ALPHA" << std::endl;
			}
			else if (channel == 3) {//RGB -> jpeg
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_ptr);//latter parameter is RGBA
				sprintf_s(infoFormat, sizeof(infoFormat), "Load image from %s format:%s", path, "RGB");
				//std::cout << "[INFO] Load image from " << path << " format:RGB" << std::endl;
			}
			else if (channel == 4) {//rgba -> png
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_ptr);//both parameter are RGBA
				sprintf_s(infoFormat, sizeof(infoFormat), "Load image from %s format:%s", path, "RGBA");
				//std::cout << "[INFO] Load image from " << path << " format:RGBA" << std::endl;
			}
			LogUtil::printInfo(infoFormat);
			glGenerateMipmap(texture_type);
			have_release = false;
		}
		else {
			std::cout << "[ERROR] Can\'t load image " << path << " !" << std::endl;
		}
		glBindTexture(texture_type, 0);//Bind nothing.
		stbi_image_free(texture_ptr);

		ImageStruct newImage;
		newImage.channel = channel;
		newImage.height = height;
		newImage.width = width;
		newImage.tbo = tbo;
		textureGroup.insert(std::make_pair(path, newImage));
	}
}


int xc_ogl::ImageLoader::getTextureWidth()
{
	return width;
}

int xc_ogl::ImageLoader::getTextureHeight()
{
	return height;
}

void * xc_ogl::ImageLoader::getTexturePointer(const char* tex, int &width, int &height, int &channels)
{
	stbi_set_flip_vertically_on_load(false);
	void* texture_ptr = stbi_load(tex, &width, &height, &channels, STBI_rgb_alpha);
	if (texture_ptr) {
		return texture_ptr;
	}
	else {
		return nullptr;
	}
}

GLuint xc_ogl::ImageLoader::getTextureBufferObjectHandle()
{
	return tbo;
}

unsigned int xc_ogl::ImageLoader::loadCubeMap(std::vector<std::string> faces)
{
	unsigned int tbo;
	glGenTextures(1, &tbo);
	glBindTexture(GL_TEXTURE_CUBE_MAP, tbo);

	int width, height, channels;
	for (size_t i = 0; i < faces.size(); i++)
	{
		unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &channels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "[ERROR]Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return tbo;
}


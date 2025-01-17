#pragma once

#include <glew.h>

#include "CommonValues.h"

class Texture
{
public:
	Texture();
	Texture(const char* fileLoc);

	bool LoadTexture();
	bool LoadTextureA(); //with alpha

	void UseTexture();
	void ClearTexture();

	~Texture();


private:
	GLuint textureID;
	int width, height, bitDepth;

	const char* fileLocation;
};


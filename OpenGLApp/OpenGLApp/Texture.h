#pragma once

#include <glew.h>
#include "stb_image.h"

class Texture
{
public:
	Texture();
	Texture(char* fileLoc);

	void LoadTexture();
	void UseTexture();
	void ClearTexture();

	~Texture();


private:
	GLuint textureID;
	int width, height, bitDepth;

	char* fileLocation;
};


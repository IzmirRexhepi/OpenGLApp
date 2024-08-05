#pragma once

#include <glm.hpp>
#include <glew.h>

class Light
{
public:
	Light();
	Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat ambIntensity);

	void UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation);
	
	~Light();
private:
	glm::vec3 colour;
	GLfloat ambientIntensity;
};


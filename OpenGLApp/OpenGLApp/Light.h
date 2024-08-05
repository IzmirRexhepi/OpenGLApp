#pragma once

#include <glm.hpp>
#include <glew.h>

class Light
{
public:
	Light();
	Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat ambIntensity, GLfloat xDir, GLfloat yDir, GLfloat zDir, GLfloat dItensity);

	void UseLight(GLuint ambientIntensityLocation, GLuint colourLocation,
			GLuint diffuseIntensityLocation, GLuint directionLocation);
	
	~Light();
private:
	glm::vec3 colour;
	GLfloat ambientIntensity;

	glm::vec3 direction;
	GLfloat diffuseIntensity;
};


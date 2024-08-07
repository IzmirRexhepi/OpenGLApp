#pragma once

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>


#include "ShadowMap.h"

class Light
{
public:
	Light();
	Light(GLfloat shadowWidth, GLfloat shadowHeight,
			GLfloat red, GLfloat green, GLfloat blue,
			GLfloat ambIntensity, GLfloat dItensity);

	ShadowMap* GetShadowMap() { return shadowMap; }

	~Light();
protected:
	glm::vec3 colour;
	GLfloat ambientIntensity;
	GLfloat diffuseIntensity;

	glm::mat4 lightProj;

	ShadowMap* shadowMap;
};


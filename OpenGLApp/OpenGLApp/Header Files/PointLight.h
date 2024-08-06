#pragma once
#include "Light.h"
class PointLight :
    public Light
{
public:
    PointLight();

    PointLight(GLfloat red, GLfloat green, GLfloat blue,
        GLfloat ambIntensity, GLfloat dItensity,
        GLfloat xPos, GLfloat yPos, GLfloat zPos,
        GLfloat con, GLfloat lin, GLfloat exp);

    void UseLight(GLuint ambientIntensityLocation, GLuint colourLocation,
        GLuint diffuseIntensityLocation, GLuint positionLocation,
        GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation);

    ~PointLight();

protected:
    glm::vec3 position;

    GLfloat constant, linear, exponent; //(ax^2+bx+c)/L; x= distance, a = exponent, b = linear, c = constant, L = light
};


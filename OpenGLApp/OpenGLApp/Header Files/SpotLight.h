#pragma once
#include "PointLight.h"

class SpotLight :
    public PointLight
{
public:
    SpotLight();

    SpotLight(GLuint shadowWidth, GLuint shadowHeight,
        GLfloat near, GLfloat far, 
        GLfloat red, GLfloat green, GLfloat blue,
        GLfloat ambIntensity, GLfloat dItensity,
        GLfloat xPos, GLfloat yPos, GLfloat zPos,
        GLfloat xDir, GLfloat yDir, GLfloat zDir,
        GLfloat con, GLfloat lin, GLfloat exp,
        GLfloat edg);

    void UseLight(GLuint ambientIntensityLocation, GLuint colourLocation,
        GLuint diffuseIntensityLocation, GLuint positionLocation, GLuint directionLocation,
        GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation,
        GLuint edgeLocation);

    void SetFlash(glm::vec3 pos, glm::vec3 dir);

    void Toggle();

    ~SpotLight();

private:
    glm::vec3 direction;

    GLfloat edge, procEdge;

    bool isOn;
};


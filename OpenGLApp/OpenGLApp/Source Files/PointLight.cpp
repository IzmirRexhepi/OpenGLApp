#include "../Header Files/PointLight.h"


PointLight::PointLight() :Light() {
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    constant = 1.0f; //avoid dividing by 0
    linear = 0.0f;
    exponent = 0.f;
}

PointLight::PointLight(GLfloat red, GLfloat green, GLfloat blue,
                        GLfloat ambIntensity, GLfloat dItensity,
                        GLfloat xPos, GLfloat yPos, GLfloat zPos,
                        GLfloat con, GLfloat lin, GLfloat exp) : Light(1024, 1024, red, green, blue, ambIntensity, dItensity) {

    position = glm::vec3(xPos, yPos, zPos);
    constant = con;
    linear = lin;
    exponent = exp;
}

void PointLight::UseLight(GLuint ambientIntensityLocation, GLuint colourLocation,
                            GLuint diffuseIntensityLocation, GLuint positionLocation,
                            GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation) {

    glUniform3f(colourLocation, colour.x, colour.y, colour.z);
    glUniform1f(ambientIntensityLocation, ambientIntensity);
    glUniform1f(diffuseIntensityLocation, diffuseIntensity);

    glUniform3f(positionLocation, position.x, position.y, position.z);
    glUniform1f(constantLocation, constant);
    glUniform1f(linearLocation, linear);
    glUniform1f(exponentLocation, exponent);


}


PointLight::~PointLight() {

}
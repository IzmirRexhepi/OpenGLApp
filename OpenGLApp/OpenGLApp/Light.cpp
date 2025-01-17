#include "Light.h"

Light::Light() {
	colour = glm::vec3(1.0f, 1.0f, 1.0f);
	ambientIntensity = 1.0f;

	direction = glm::vec3(0.0f, -1.0f, 0.0f);
	diffuseIntensity = 0.0f;
}
Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat ambIntensity, GLfloat xDir, GLfloat yDir, GLfloat zDir, GLfloat dItensity) {
	colour = glm::vec3(glm::clamp(red, 0.0f, 1.0f), glm::clamp(green, 0.0f, 1.0f), glm::clamp(blue, 0.0f, 1.0f));
	ambientIntensity = ambIntensity;

	direction = glm::vec3(xDir, yDir, zDir);
	diffuseIntensity = dItensity;
}

void Light::UseLight(GLuint ambientIntensityLocation, GLuint colourLocation, GLuint diffuseIntensityLocation, GLuint directionLocation) {
	glUniform3f(colourLocation, colour.x, colour.y, colour.z);
	glUniform1f(ambientIntensityLocation, ambientIntensity);

	glUniform3f(directionLocation, direction.x, direction.y, direction.z);
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);
}

Light::~Light() {

}
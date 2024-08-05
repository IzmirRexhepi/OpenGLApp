#include "Light.h"

Light::Light() {
	colour = glm::vec3(1.0f, 1.0f, 1.0f);
	ambientIntensity = 1.0f;
}
Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat ambIntensity) {
	colour = glm::vec3(glm::clamp(red, 0.0f, 1.0f), glm::clamp(green, 0.0f, 1.0f), glm::clamp(blue, 0.0f, 1.0f));
	ambientIntensity = ambIntensity;
}

void Light::UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation) {
	glUniform3f(ambientColourLocation, colour.x, colour.y, colour.z);
	glUniform1f(ambientIntensityLocation, ambientIntensity);
}

Light::~Light() {

}
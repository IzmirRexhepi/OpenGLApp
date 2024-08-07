#include "../Header Files/Light.h"

Light::Light() {
	colour = glm::vec3(1.0f, 1.0f, 1.0f);
	ambientIntensity = 1.0f;
	diffuseIntensity = 0.0f;
}
Light::Light(GLfloat shadowWidth, GLfloat shadowHeight, GLfloat red, GLfloat green, GLfloat blue, GLfloat ambIntensity, GLfloat dItensity) {
	shadowMap = new ShadowMap();
	shadowMap->Init(shadowWidth, shadowHeight);

	colour = glm::vec3(glm::clamp(red, 0.0f, 1.0f), glm::clamp(green, 0.0f, 1.0f), glm::clamp(blue, 0.0f, 1.0f));
	ambientIntensity = ambIntensity;
	diffuseIntensity = dItensity;
}

Light::~Light() {

}
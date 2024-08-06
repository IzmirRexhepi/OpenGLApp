#include "../Header Files/Camera.h"

Camera::Camera() {
	/*position = glm::vec3(0.0f, 0.0f, 0.0f);
	worldUp = glm::vec3(0.0f, 0.0f, 0.0f);
	yaw = 0;
	pitch = 0;
	front = glm::vec3(0.0f, 0.0f, 0.0f);

	moveSpeed = 0;
	rotateSpeed = 0;
	right = glm::vec3(0.0f, 0.0f, 0.0f);
	up = glm::vec3(0.0f, 0.0f, 0.0f);*/
}

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startRotateSpeed) {
	position = startPosition;
	worldUp = startUp;
	yaw = startYaw;
	pitch = startPitch;
	front = glm::vec3(0.0f, 0.0f, -1.0f);

	moveSpeed = startMoveSpeed;
	rotateSpeed = startRotateSpeed;

	update();
}

void Camera::keyControl(bool* keys, GLfloat deltaTime) {

	GLfloat velocity = moveSpeed * deltaTime;

	if (keys[GLFW_KEY_W]) {
		position += front * velocity;
	}

	if (keys[GLFW_KEY_S]) {
		position -= front * velocity;

	}

	if (keys[GLFW_KEY_A]) {
		position -= right * velocity;
	}

	if (keys[GLFW_KEY_D]) {
		position += right * velocity;
	}
}

void Camera::mouseControl(GLfloat xChange, GLfloat yChange) {
	xChange *= rotateSpeed;
	yChange *= rotateSpeed;

	yaw += xChange;
	pitch += yChange;

	if (pitch > 89.0f) {
		pitch = 89.0f;
	}

	if (pitch < -89.0f) {
		pitch = -89.0f;
	}

	update();
}

glm::mat4 Camera::calculateViewMatrix() {
	return glm::lookAt(position, position + front, up);
}

glm::vec3 Camera::getCameraPosition() {
	return position;
}

void Camera::update() {
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch)); //* cos(glm::radians(roll));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch)); //* cos(glm::radians(roll));
	front = glm::normalize(front);

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}

Camera::~Camera() {

}

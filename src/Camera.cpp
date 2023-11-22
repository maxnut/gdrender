#include "Camera.h"

void Camera::init()
{
	glGenBuffers(1, &uboMatrices);

	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4) + sizeof(glm::vec3), nullptr, GL_STATIC_DRAW);

	viewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

	projectionMatrix = glm::ortho(-2.f, 2.f, -2.f, 2.f, -1.f, 1000.f);

	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projectionMatrix));

	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Camera::updateTransform()
{
	glm::vec3 direction;
	direction.x = cos(glm::radians(cameraEulerAngles.x)) * cos(glm::radians(cameraEulerAngles.y));
	direction.y = sin(glm::radians(cameraEulerAngles.y));
	direction.z = sin(glm::radians(cameraEulerAngles.x)) * cos(glm::radians(cameraEulerAngles.y));

	cameraFront = glm::normalize(direction);

	viewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, -1.0f, 1.0f));
	viewMatrix = scaleMatrix * viewMatrix;
}

void Camera::update()
{
	if (renderTexture && renderTexture->resolution != resolution)
		resize(renderTexture->resolution);
}

void Camera::resize(glm::vec2 resolution)
{
	this->resolution = resolution;

	glm::vec2 size = {1.f, resolution.y / resolution.x};
	size *= this->size;

	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);

	projectionMatrix = glm::ortho(-size.x, size.x, -size.y, size.y, 0.1f, 1000.f);

	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projectionMatrix));

	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Camera::startDraw()
{
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrices, 0, 2 * sizeof(glm::mat4) + sizeof(glm::vec3));

	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);

	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projectionMatrix));
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(viewMatrix));
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 2, sizeof(glm::vec3), glm::value_ptr(cameraPos));

	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	if(renderTexture)
		renderTexture->bind();
	else
		glViewport(0, 0, resolution.x, resolution.y);
}

void Camera::endDraw()
{
	renderTexture->unbind();
}

void Camera::clear()
{
	if(renderTexture)
		renderTexture->clear();
}
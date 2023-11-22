#include <imgui.h>

#include "Transformable.h"

void Transformable::updateTransform()
{
	const float angle = -glm::radians(rotationVector.z);
	const float cosine = std::cos(angle);
	const float sine = std::sin(angle);

	glm::mat4 newMatrix = glm::mat4(scaleVector.x * cosine, -scaleVector.x * sine, 0.0f, 0.0f, scaleVector.y * sine,
									scaleVector.y * cosine, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, positionVector.x,
									positionVector.y, 0.0f, 1.0f);

	transformMatrix = newMatrix;

	dirty = false;
}

void Transformable::setDirtyRecursive()
{
	dirty = true;
}

void Transformable::setPosition(glm::vec3 position)
{
	positionVector = position;
	setDirtyRecursive();
}
void Transformable::setPositionX(float x)
{
	positionVector.x = x;
	setDirtyRecursive();
}
void Transformable::setPositionY(float y)
{
	positionVector.y = y;
	setDirtyRecursive();
}
void Transformable::setPositionZ(float z)
{
	positionVector.z = z;
	setDirtyRecursive();
}
void Transformable::move(glm::vec3 translate)
{
	positionVector += translate;
	setDirtyRecursive();
}

void Transformable::setRotation(glm::vec3 rotation)
{
	rotationVector = rotation;
	setDirtyRecursive();
}
void Transformable::setRotationX(float x)
{
	rotationVector.x = x;
	setDirtyRecursive();
}
void Transformable::setRotationY(float y)
{
	rotationVector.y = y;
	setDirtyRecursive();
}
void Transformable::setRotationZ(float z)
{
	rotationVector.z = z;
	setDirtyRecursive();
}
void Transformable::rotate(glm::vec3 rotation)
{
	rotationVector += rotation;
	setDirtyRecursive();
}

void Transformable::setScale(glm::vec3 scale)
{
	scaleVector = scale;
	setDirtyRecursive();
}
void Transformable::setScaleX(float x)
{
	scaleVector.x = x;
	setDirtyRecursive();
}
void Transformable::setScaleY(float y)
{
	scaleVector.y = y;
	setDirtyRecursive();
}
void Transformable::setScaleZ(float z)
{
	scaleVector.z = z;
	setDirtyRecursive();
}
void Transformable::scale(glm::vec3 scale)
{
	scaleVector += scale;
	setDirtyRecursive();
}
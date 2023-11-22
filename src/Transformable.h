#pragma once

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>
#include <iostream>
#include <vector>

class Transformable
{
  protected:
	glm::mat4 transformMatrix = glm::mat4(1.0f);

	bool dirty = true;

	glm::vec3 positionVector = {0, 0, 0};
	glm::vec3 rotationVector = {0, 0, 0};
	glm::vec3 scaleVector = {1.f, 1.f, 1.f};

  public:
	Transformable()
	{
		this->dirty = true;
	}

	void updateTransform();

	void setDirtyRecursive();

	void setPosition(glm::vec3 position);
	void setPositionX(float x);
	void setPositionY(float y);
	void setPositionZ(float z);
	void move(glm::vec3 translate);

	void setRotation(glm::vec3 rotation);
	void setRotationX(float x);
	void setRotationY(float y);
	void setRotationZ(float z);
	void rotate(glm::vec3 rotation);

	void setScale(glm::vec3 scale);
	void setScaleX(float x);
	void setScaleY(float y);
	void setScaleZ(float z);
	void scale(glm::vec3 scale);

	glm::vec3 getPosition()
	{
		return positionVector;
	}
	glm::vec3 getRotation()
	{
		return rotationVector;
	}
	glm::vec3 getScale()
	{
		return scaleVector;
	}

	bool isDirty()
	{
		return dirty;
	}

	glm::mat4 getTransform()
	{
		return this->transformMatrix;
	}
};
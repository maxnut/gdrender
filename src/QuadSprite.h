#pragma once

#include "Material.h"
#include "Transformable.h"
#include "Vertex.h"

#include <array>

class QuadSprite : public Transformable
{
  private:
	GLuint VBO = 0;
	GLuint IBO = 0;
	GLuint VAO = 0;

	std::array<Vertex, 4> vertices;
	std::array<uint32_t, 6> indices;

	glm::vec4 color = {255, 255, 255, 255};

  public:
	std::shared_ptr<Material> material = nullptr;

  public:
	static std::shared_ptr<QuadSprite> create();

	bool init();

	void draw();

	void setColor(glm::vec4 color);
};
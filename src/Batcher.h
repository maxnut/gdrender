#pragma once

#include <iostream>
#include <vector>

#include "Material.h"
#include "Transformable.h"
#include "Vertex.h"
#include "tsl/ordered_map.h"
#include "Sprite.h"

class Batcher : public Transformable
{
  private:
	GLuint VBO = 0;
	GLuint IBO = 0;
	GLuint VAO = 0;

	tsl::ordered_map<int, Sprite*> sprites;

	bool blending = false;

	bool needSend = false;

	size_t minRange = std::numeric_limits<size_t>::max(), maxRange = 0;

  public:
	std::shared_ptr<Material> material;

	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	glm::vec2 texRes = {0, 0};

	bool batcherDirty = false;

  public:
	static std::shared_ptr<Batcher> create(std::string texture, bool blend = false);

	bool init(std::string texture, bool blend);

	void draw();

	void addSprite(Sprite* sp);

	void removeSprite(Sprite* sp);

	void sendVertex(Vertex&vert, size_t index);

	void updateSprite(Sprite* sp);

	void generateVertices();
};
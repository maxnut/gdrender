#pragma once

#include "stb_image.h"
#include <gl/glew.h>
#include <iostream>
#include <unordered_map>
#include <glm/vec2.hpp>

class Texture
{
  private:
	GLuint textureID = 0;

	static std::unordered_map<std::string, std::shared_ptr<Texture>> cachedTextures;

  public:
	  uint32_t textureSlot = 0;

	  glm::vec2 resolution = {0, 0};

  public:
	static std::shared_ptr<Texture> create(const std::string& path);

	bool init(const std::string& path);

	GLuint getTexture()
	{
		return this->textureID;
	}
};
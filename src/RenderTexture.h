#pragma once

#include <iostream>
#include <glm/vec2.hpp>
#include <gl/glew.h>

class RenderTexture
{
  private:
	GLuint framebuffer = 0;
	GLuint renderbuffer = 0;
	GLuint textureID = 0;

  public:
    glm::vec2 resolution = {0, 0};

  public:

    static std::shared_ptr<RenderTexture> create(glm::vec2 resolution);
    bool init(glm::vec2 resolution);

    void resize(glm::vec2 resolution);

    void bind();
    void unbind();

    void clear();

	GLuint getTexture()
	{
		return textureID;
	}
};
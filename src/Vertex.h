#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

class Vertex
{
  public:
    glm::vec2 position = {0, 0};
    glm::vec4 color = {255, 255, 255, 255};
    glm::vec2 texCoord = {0, 0};
};
#pragma once

#include <glm/vec2.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Layer.h"

class Application
{
  private:

  float lastFrame = 0;

  std::shared_ptr<Layer> currentLayer;
  std::shared_ptr<Layer> pendingLayer;

  public:

  GLFWwindow* window;

  float deltaTime = 0, totalTime = 0;

  glm::vec2 resolution;

  static Application* instance;

  public:
	void init(glm::vec2 resolution, std::string name);
  void setStyle();

  void startUpdate();
	void update();

	void draw();

	void exit();

  void pushLayer(std::shared_ptr<Layer> layer);
  void popLayer();
};
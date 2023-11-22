#include "Input.h"
#include "Application.h"

#include <GLFW/glfw3.h>

glm::vec2 Input::lastMousePosition = {0, 0};
glm::vec2 Input::mousePosition = {0, 0};
glm::vec2 Input::mouseDelta = {0, 0};
float Input::scrollDelta;

std::unordered_map<int, bool> Input::mouseButtons;
std::unordered_map<int, bool> Input::keyMap;

bool first = true;

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    Input::scrollDelta = yoffset;
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Input::keyMap[key] = (action == GLFW_PRESS);
}

void mouseCallback(GLFWwindow* window, double x, double y)
{
	Input::mousePosition.x = x;
	Input::mousePosition.y = y;

	if (first)
		Input::lastMousePosition = Input::mousePosition;

	first = false;
}

void buttonCallback(GLFWwindow* window, int button, int action, int mods)
{
	Input::mouseButtons[button] = action;
}

void Input::init()
{
	glfwSetCursorPosCallback(Application::instance->window, mouseCallback);
	glfwSetMouseButtonCallback(Application::instance->window, buttonCallback);
	glfwSetKeyCallback(Application::instance->window, keyCallback);
	glfwSetScrollCallback(Application::instance->window, scrollCallback);
}

void Input::update()
{
	Input::mouseDelta = {Input::mousePosition.x - Input::lastMousePosition.x,
						 Input::lastMousePosition.y - Input::mousePosition.y};

	Input::lastMousePosition = Input::mousePosition;
}

void Input::postUpdate()
{
	scrollDelta = 0;
}
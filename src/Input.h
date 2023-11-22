#pragma once

#include <glm/vec2.hpp>
#include <unordered_map>

namespace Input
{

    extern glm::vec2 lastMousePosition;
    extern glm::vec2 mousePosition;
    extern glm::vec2 mouseDelta;
    extern float scrollDelta;

    extern std::unordered_map<int, bool> mouseButtons;
    extern std::unordered_map<int, bool> keyMap;

    void init();
    void update();
    void postUpdate();

};
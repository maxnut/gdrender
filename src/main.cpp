#include "Application.h"
#include "SelectLevelLayer.h"

int main()
{
    Application app;
    app.init(glm::vec2(1920, 1080), "gdrender");

    auto layer = SelectLevelLayer::create();

    app.pushLayer(layer);

    app.startUpdate();

    return 0;
}
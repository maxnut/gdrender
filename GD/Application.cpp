#include "Application.h"
#include "SelectLevelLayer.h"

#include "imgui-SFML.h"
#include "imgui.h"

#define MULTITHREADING 0

Application* Application::instance;
const float Application::zoomModifier = 2.f;

void Application::start()
{
    instance = this;

    sf::ContextSettings settings;
    settings.antialiasingLevel = 4;

    window = new sf::RenderWindow(sf::VideoMode::getDesktopMode(), "GD", sf::Style::Fullscreen, settings);
    renderTexture.create(1920, 1080);
    window->setFramerateLimit(50000);
    window->setVerticalSyncEnabled(false);
    window->setActive(!MULTITHREADING);

    ImGui::SFML::Init(*window);

    ImGui::GetIO().Fonts->Clear();
    ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\arial.ttf", 18);
    ImGui::SFML::UpdateFontTexture();

    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(3.f);
    style.WindowBorderSize = 0.0f;
    style.WindowPadding = ImVec2(50.0f, 50.0f);

    std::shared_ptr<SelectLevelLayer> layer = SelectLevelLayer::create();
    pushLayer(layer);

#if MULTITHREADING == 1
    sf::Thread thread(&Application::draw, this);
    thread.launch();
#endif

    while (window->isOpen())
    {
        update();

#if MULTITHREADING == 0
        draw();
#endif

    }

    onQuit();
}

void Application::update()
{

    if (pendingLayer)
    {
        if (currentLayer)
            popLayer();

        currentLayer = pendingLayer;
        pendingLayer = nullptr;
    }

    sf::Time dt = deltaClock.restart();
    deltaTime = dt.asSeconds();

    sf::Event event;
    while (window->pollEvent(event))
    {
        ImGui::SFML::ProcessEvent(event);

        if (event.type == sf::Event::Closed)
            window->close();
        else if (event.type == sf::Event::KeyPressed)
            keyPressedMap[event.key.code] = true;
        else if (event.type == sf::Event::KeyReleased)
            keyPressedMap[event.key.code] = false;
    }

    currentLayer->update();

    ImGui::SFML::Update(*window, dt);

}

void Application::draw()
{
#if MULTITHREADING == 1
    while (window->isOpen())
    {
        window->clear(sf::Color::Blue);
        renderTexture.clear();

        renderTexture.display();
        sf::Sprite sprite(renderTexture.getTexture());
        sf::Vector2f scaleFactor = { 1, 1 };
        scaleFactor *= (float)window->getSize().x / (float)renderTexture.getSize().x;
        sprite.setScale(scaleFactor);
        window->draw(sprite);
        window->display();
    }
#else

    window->clear(sf::Color::Blue);
    renderTexture.clear();

    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
    currentLayer->draw();

    renderTexture.display();
    sf::Sprite sprite(renderTexture.getTexture());
    float scaleFactor = 1.f;
    scaleFactor *= (float)window->getSize().x / (float)renderTexture.getSize().x;
    sprite.setScale(scaleFactor, scaleFactor);
    window->draw(sprite);

    ImGui::PopFont();
    ImGui::SFML::Render(*window);

    window->display();
#endif
}

void Application::onQuit()
{
    ImGui::SFML::Shutdown();
}

void Application::pushLayer(std::shared_ptr<Layer> layer)
{
    pendingLayer = layer;
}

void Application::popLayer()
{
    currentLayer->onExit();
    currentLayer = nullptr;
}
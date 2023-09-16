#include "Application.h"
#include "GameLayer.h"

#include "imgui-SFML.h"
#include "imgui.h"

#define MULTITHREADING 0
#define IMGUI 0

Application* Application::instance;

void Application::start()
{
    instance = this;

    window = new sf::RenderWindow(sf::VideoMode(1920, 1080), "GD", sf::Style::Fullscreen);
    renderTexture.create(1920, 1080);
    window->setFramerateLimit(50000);
    window->setVerticalSyncEnabled(false);
    window->setActive(!MULTITHREADING);

#if IMGUI == 1
    ImGui::SFML::Init(*window);

    ImGui::GetIO().Fonts->Clear();
    ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\arial.ttf", 12);
    ImGui::SFML::UpdateFontTexture();
#endif

    std::shared_ptr<GameLayer> gameLayer = GameLayer::create();
    pushLayer(gameLayer);

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
    sf::Time dt = deltaClock.restart();
    deltaTime = dt.asSeconds();

    sf::Event event;
    while (window->pollEvent(event))
    {
#if IMGUI == 1
        ImGui::SFML::ProcessEvent(event);
#endif
        if (event.type == sf::Event::Closed)
            window->close();
        else if (event.type == sf::Event::KeyPressed)
            keyPressedMap[event.key.code] = true;
        else if (event.type == sf::Event::KeyReleased)
            keyPressedMap[event.key.code] = false;
    }

    currentLayer->update();
#if IMGUI == 1
    ImGui::SFML::Update(*window, dt);
#endif
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

#if IMGUI == 1
    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
#endif
    currentLayer->draw();

    renderTexture.display();
    sf::Sprite sprite(renderTexture.getTexture());
    sf::Vector2f scaleFactor = { 1, 1 };
    scaleFactor *= (float)window->getSize().x / (float)renderTexture.getSize().x;
    sprite.setScale(scaleFactor);
    window->draw(sprite);

#if IMGUI == 1
    ImGui::PopFont();
    ImGui::SFML::Render(*window);
#endif

    window->display();
#endif
}

void Application::onQuit()
{
#if IMGUI == 1
    ImGui::SFML::Shutdown();
#endif
}

void Application::pushLayer(std::shared_ptr<Layer> layer)
{
    if (currentLayer)
        popLayer();

    currentLayer = layer;
}

void Application::popLayer()
{
    currentLayer->onExit();
    currentLayer = nullptr;
}
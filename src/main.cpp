#include <SFML/Graphics.hpp>
#include <gui/imgui/imgui.h>
#include <gui/imgui-sfml/imgui-SFML.h>

#include <iostream>
#include <core/vertex.hpp>

#include <models/camera.hpp>
#include <models/scene.hpp>

#include <math/pipeline.hpp>
#include <math/math.hpp>

#include <utils/imgui_sfml_wrapper.hpp>

#include <gui/view/view.hpp>
#include <gui/controller/controller.hpp>

int main()
{

  sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();

  sf::RenderWindow window(sf::VideoMode(static_cast<int>(desktopMode.width * 0.90), static_cast<int>(desktopMode.height * 0.90)), "ImGui + SFML = <3", sf::Style::Default);
  GUI::UI *ui = new GUI::UI(window);

  while (window.isOpen())
  {
    sf::Event event{};
    while (window.pollEvent(event))
    {
      ui->handleEvents(event, window);
    }

    ImGui::SFML::Update(window, ui->deltaClock.restart());

    ui->render();

    window.clear();
    ImGui::SFML::Render(window);
    window.display();
  }

  delete ui;
}
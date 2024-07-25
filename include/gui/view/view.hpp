/**********************************************************************************************
 *   IDIOM: PORTUGUÊS
 *
 *   mrxguiview v1.0 -
 *
 *   CONVENTIONS: (Convenções)
 *
 *   IDIOM: ENGLISH
 *
 *   mrxguiview v1.0 -
 *
 *   CONVENTIONS:
 *
 *   CONFIGURATION:
 *       ...
 *
 *   DEPENDENCIES:
 *
 *   CONTRIBUTORS:
 *      Marcos Augusto Campagnaro: Initial implementation, review and maintenance
 *
 *   TODO:
 *
 *   LICENSE: GPL 3.0
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 **********************************************************************************************/
#pragma once

#include <SFML/Graphics.hpp>
#include <gui/imgui/imgui.h>
#include <gui/imgui-sfml/imgui-SFML.h>

#include <gui/controller/controller.hpp>
#include <gui/view/components/components.hpp>
#include <models/scene.hpp>
#include <shapes/shapes.hpp>

namespace GUI
{
  class UI
  {
  private:
    /**
     * @brief Referência para a janela da aplicação
     */
    sf::RenderWindow &window;
    /**
     * @brief Relógio para controlar o tempo de execução
     */
    sf::Clock deltaClock;
    /**
     * @brief Controlador da aplicação
     */
    Controller *controller;
    /**
     * @brief Ícone de movimento
     */
    sf::Texture move_icon;
    /**
     * @brief Referencia para o componente Visualizador de hierarquia
     */
    components::HierarchyViewer *hierarchyViewer;

  public:
    UI(sf::RenderWindow &_window);
    ~UI();

    // Getters
    sf::RenderWindow &getWindow();

    // Components
    void menu();
    void hierarchy(models::Scene *_scene);
    void viewport(models::Scene *_scene);
    void object_properties();
    void sidebar();
    void insertOptions();

    // Methods
    void render();
    void handleEvents(const sf::Event &event, sf::RenderWindow &window);
  };
} // namespace GUI
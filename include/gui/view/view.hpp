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

// External dependencies
#include <gui/imgui/imgui.h>
#include <gui/imgui-sdl2/imgui_impl_sdl2.h>
#include <gui/imgui-sdl2/imgui_impl_sdlrenderer2.h>
#include <SDL.h>

// Internal dependencies
#include <gui/view/components/components.hpp>
#include <gui/view/components/menu.hpp>
#include <gui/view/components/imfilebrowser.hpp>
#include <gui/controller/controller.hpp>
#include <models/scene.hpp>
#include <shapes/shapes.hpp>

namespace GUI
{
  class UI
  {
  private:
    /**
     * @brief Controlador da aplicação
     */
    Controller *controller;
    /**
     * @brief Referencia para o componente Visualizador de hierarquia
     */
    components::HierarchyViewer *hierarchyViewer;

  public:
    /**
     * @brief Referência para a janela da aplicação
     */
    SDL_Window *window;
    /**
     * @brief Referência para o renderizador da aplicação
     */
    SDL_Renderer *renderer;
    /**
     * @brief Relógio para controlar o tempo de execução
     */
    float delta_time;
    /**
     * @brief Instancia do file browser
     *
     * @note A biblioteca filebrowser necessita de uma instância para funcionar
     * @see https://github.com/AirGuanZ/imgui-filebrowser/blob/master/README.md
     */
    ImGui::FileBrowser fileDialog;

    UI(SDL_Window *window, SDL_Renderer *renderer);
    ~UI();

    // Components
    void menu();
    void hierarchy(models::Scene *_scene);
    void viewport(models::Scene *_scene);
    void object_properties();
    void sidebar();
    void insertOptions();

    // Methods
    void render();
    void handleEvents(const SDL_Event &event, SDL_Window *window, SDL_Renderer *renderer);
  };
} // namespace GUI
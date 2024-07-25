/**********************************************************************************************
 *   IDIOM: PORTUGUÊS
 *
 *   mrxuicomponents v1.0 -
 *
 *   CONVENTIONS: (Convenções)
 *
 *   IDIOM: ENGLISH
 *
 *   mrxuicomponents v1.0 -
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

#include <utils/imgui_sfml_wrapper.hpp>
#include <gui/controller/controller.hpp>
#include <models/scene.hpp>
#include <shapes/shapes.hpp>

namespace GUI
{
  namespace components
  {
    void HelpMarker(const char *desc);

    void menu(GUI::Controller *_controller);
    class HierarchyViewer
    {
    private:
      GUI::Controller *controller;
      int selected_index = -1;

    public:
      HierarchyViewer(GUI::Controller *_controller);
      void render(models::Scene *scene);
    };

    void object_inspector(models::Scene *scene);
    void viewport(models::Scene *scene);
    void performance_monitor();

  }
} // namespace GUI
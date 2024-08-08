/**********************************************************************************************
 *   IDIOM: PORTUGUÊS
 *
 *   mrximgui_sfml_utils v1.0 - Conjunto de funções utilitárias para trabalhar com ImGui e SFML neste projeto
 *
 *   CONVENTIONS: (Convenções)
 *
 *   IDIOM: ENGLISH
 *
 *   mrximgui_sfml_utils v1.0 - Set of utility functions to work with ImGui and SFML in this project
 *
 *   CONVENTIONS:

 *   CONFIGURATION:
 *       ...
 *
 *   DEPENDENCIES:
 *      <gui/imgui/imgui.h>    - Required for:
 *      <gui/imgui-sfml/imgui-SFML.h> - Required for:
 *     <SFML/Graphics.hpp> - Required for:
 *
 *   CONTRIBUTORS:
 *      Marcos Augusto Campagnaro: Initial implementation, review and maintenance
 *
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

#include <gui/imgui/imgui.h>
#include <gui/imgui-sfml/imgui-SFML.h>
#include <SFML/Graphics.hpp>

#include <core/vertex.hpp>
#include <math/pipeline.hpp>
#include <vector>

namespace utils
{
  // Funções para desenhar vértices e linhas
  void DrawVertex(ImDrawList *draw_list, const core::Vertex &vertex);
  void DrawLine(ImDrawList *draw_list, const std::vector<core::Vector3> vertexes, const sf::Color &color);

  // Funções para desenhar pixels
  void setPixel(int x, int y, float z, const sf::Color &color, std::vector<float> &z_buffer, std::vector<sf::Color> &color_buffer, core::Vector2 window_size);
  void DrawBuffer(ImDrawList *draw_list, const std::vector<float> &z_buffer, const std::vector<sf::Color> &color_buffer, core::Vector2 window_size);
  void DrawVertexBuffer(ImDrawList *draw_list, const core::Vertex &vertex, const sf::Color &color, std::vector<float> &z_buffer, std::vector<sf::Color> &color_buffer, core::Vector2 window_size);
  void DrawLineBuffer(ImDrawList *draw_list, const std::vector<core::Vector3> &vertexes, const sf::Color &color, std::vector<float> &z_buffer, std::vector<sf::Color> &color_buffer, core::Vector2 window_size);
}
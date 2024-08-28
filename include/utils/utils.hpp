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
#include <SDL.h>

#include <core/vertex.hpp>
#include <core/halfedge.hpp>
#include <math/pipeline.hpp>
#include <vector>

namespace utils
{

  /**
   * @brief Cor transparente
   *
   * R = 0; G = 0; B = 0; A = 0
   */
  constexpr SDL_Color TRANSPARENT_COLOR = {0, 0, 0, 0};

  /**
   * @brief Converte uma cor SDL para um valor unsigned int (ImU32 do ImGui)
   *
   * @param color Cor SDL
   * @return unsigned int Valor IM_COL32
   */
  constexpr unsigned int GET_COLOR_UI32(const SDL_Color &color)
  {
    return IM_COL32(color.r, color.g, color.b, color.a);
  }

  /**
   * @brief Compara duas cores
   *
   * @param color1 Cor 1
   * @param color2 Cor 2
   * @return true - Se as cores forem iguais
   * @return false - Se as cores forem diferentes
   */
  constexpr bool CompareColors(const SDL_Color &color1, const SDL_Color &color2)
  {
    return (color1.r == color2.r) &&
           (color1.g == color2.g) &&
           (color1.b == color2.b) &&
           (color1.a == color2.a);
  }

  // Funções para desenhar vértices e linhas
  void DrawVertex(ImDrawList *draw_list, const core::Vertex &vertex);
  void DrawLine(ImDrawList *draw_list, const std::vector<core::Vector3> vertexes, const SDL_Color &color);
  std::vector<core::Vector3> BresenhamLine(core::Vector3 start, core::Vector3 end);

  // Funções para desenhar pixels
  void setPixel(float x, float y, float z, const SDL_Color &color, std::vector<std::vector<float>> &z_buffer, std::vector<std::vector<SDL_Color>> &color_buffer, core::Vector2 window_size);
  void DrawBuffer(ImDrawList *draw_list, const std::vector<std::vector<float>> &z_buffer, const std::vector<std::vector<SDL_Color>> &color_buffer, core::Vector2 window_size);
  void DrawVertexBuffer(ImDrawList *draw_list, const core::Vector3 point, const SDL_Color &color, std::vector<std::vector<float>> &z_buffer, std::vector<std::vector<SDL_Color>> &color_buffer, core::Vector2 window_size);
  void DrawLineBuffer(ImDrawList *draw_list, const std::vector<core::Vector3> &vertexes, const SDL_Color &color, std::vector<std::vector<float>> &z_buffer, std::vector<std::vector<SDL_Color>> &color_buffer, core::Vector2 window_size);

  // Funções para desenhar faces
  void DrawFaceBuffer(ImDrawList *draw_list, std::vector<core::Vector3> vertexes, const SDL_Color &color, std::vector<std::vector<float>> &z_buffer, std::vector<std::vector<SDL_Color>> &color_buffer, core::Vector2 min_window_size, core::Vector2 max_window_size);
}
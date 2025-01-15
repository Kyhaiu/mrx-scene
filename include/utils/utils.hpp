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

#include <SDL.h>

#include <gui/imgui/imgui.h>
#include <core/vertex.hpp>
#include <core/halfedge.hpp>
#include <math/pipeline.hpp>
#include <models/colors.hpp>
#include <models/light.hpp>

#include <vector>

namespace utils
{

  // Funções para desenhar pixel-a-pixel
  void setPixel(const float x, const float y, const float z, const models::Color &color, std::vector<std::vector<float>> &z_buffer, std::vector<std::vector<models::Color>> &color_buffer);
  void DrawVertexBuffer(const core::Vector3 point, const models::Color &color, std::vector<std::vector<float>> &z_buffer, std::vector<std::vector<models::Color>> &color_buffer, const int size);
  void DrawLineBuffer(const std::vector<core::Vector3> &vertexes, const models::Color &color, std::vector<std::vector<float>> &z_buffer, std::vector<std::vector<models::Color>> &color_buffer);

  // Funções para rasterização de polígonos
  void DrawFaceBufferFlatShading(const std::vector<core::Vertex *> &vertexes, const core::Vector3 &eye, const core::Vector3 &face_centroid, const core::Vector3 &face_normal, const models::Material &object_material, const models::Light &global_light, const std::vector<models::Omni> &omni_lights, std::vector<std::vector<float>> &z_buffer, std::vector<std::vector<models::Color>> &color_buffer);
  void DrawFaceBufferGouraudShading(const std::vector<core::Vertex *> &vertexes, const core::Vector3 &eye, const models::Material &object_material, const models::Light &global_light, const std::vector<models::Omni> &omni_lights, std::vector<std::vector<float>> &z_buffer, std::vector<std::vector<models::Color>> &color_buffer);
  void DrawFaceBufferPhongShading(const std::vector<core::Vertex *> &vertexes, const core::Vector3 &centroid, const core::Vector3 &eye, const models::Material &object_material, const models::Light &global_light, const std::vector<models::Omni> &omni_lights, std::vector<std::vector<float>> &z_buffer, std::vector<std::vector<models::Color>> &color_buffer);
  void DrawBuffer(ImDrawList *draw_list, const std::vector<std::vector<float>> &z_buffer, const std::vector<std::vector<models::Color>> &color_buffer, core::Vector2 min_window_size);

  // Demais funções de desenho
  void DrawString(const char *text, const core::Vector3 &position, const models::Color &color);
  void DrawBoundingBox(const core::Vector2 &min, const core::Vector2 &max, const models::Color &color, std::vector<std::vector<float>> &z_buffer, std::vector<std::vector<models::Color>> &color_buffer);

}
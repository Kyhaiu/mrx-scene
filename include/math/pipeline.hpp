/**********************************************************************************************
 *   IDIOM: PORTUGUÊS
 *
 *   mrxpipeline v1.0 - Processos e funções para trabalhar com o Pipeline de Visualização 3D
 *
 *   CONVENTIONS: (Convenções)
 *     - As funções são sempre auto-contidas, nenhuma função usa outra função mrxpipeline dentro,
 *       o código necessário é re-implementado diretamente dentro.
 *          * Esta é uma maneira de evitar qualquer dependência e evitar a sobrecarga de chamadas
 *     - Os parâmetros de entrada das funções são sempre recebidos por valor
 *     - As funções sempre usam uma variável "result" para retorno
 *     - As funções sempre têm uma descrição @brief, @param e @return ESCRITAS EM PORTUGUÊS
 *          - A descrição deve estar no arquivo .cpp
 *
 *   IDIOM: ENGLISH
 *
 *   mrxpipeline v1.0 - Processes and functions to work with the 3D Visualization Pipeline
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

#include <math/math.hpp>
#include <core/vector.hpp>
#include <core/vertex.hpp>
#include <models/colors.hpp>
#include <models/light.hpp>
#include <shapes/shapes.hpp>

#include <tuple>
#include <vector>
#include <algorithm>
#include <optional>

namespace math
{
// Constantes para determinar se o sistema de coordenadas é destro ou canhoto (regra da mão direita ou esquerda)
#define LEFT_HANDED 0
#define RIGHT_HANDED 1

  namespace pipeline_adair
  {
    // Funções do Pipeline de Visualização 3D	- Adair Santa Catarina
    core::Matrix sru_to_src(const core::Vector3 &vrp, const core::Vector3 fp, unsigned int handed = RIGHT_HANDED);
    core::Matrix projection(const core::Vector3 &vrp, const core::Vector3 p, const float d);
    core::Matrix src_to_srt(const core::Vector2 min_window, const core::Vector2 min_viewport, const core::Vector2 max_window, const core::Vector2 max_viewport, bool reflected);
  }

  namespace pipeline_smith
  {
    // Funções do Pipeline de visualização 3D - Alvy Ray Smith
    // OBS.: O pipeline é simplificado
    core::Matrix clipping_transformation(const float d, const float far, const core::Vector2 center_window, const core::Vector2 size_window);
    core::Matrix perspective_transformation(const float near, const float far);
    core::Matrix src_to_srt(const core::Vector2 min_viewport, const core::Vector2 max_viewport, const float near, const float far);
  }

  //-------------------------------------------------------------------------------------------------
  // Funções de Clipping (Clipagem de Linha)
  //-------------------------------------------------------------------------------------------------

#define INSIDE 0b000000
#define LEFT 0b000001
#define RIGHT 0b000010
#define BOTTOM 0b000100
#define TOP 0b001000
#define NEAR 0b010000
#define FAR 0b100000

  bool is_inside(core::Vector3 p, core::Vector2 min, core::Vector2 max, unsigned int edge);
  bool is_inside(core::Vector4 p, core::Vector3 min, core::Vector3 max, unsigned int plane);

  core::Vector3 compute_intersection(core::Vector3 p1, core::Vector3 p2, core::Vector2 min, core::Vector2 max, unsigned int edge);
  std::pair<core::Vector3, core::Vector3> compute_intersection(std::pair<core::Vector3, core::Vector3> p1, std::pair<core::Vector3, core::Vector3>, core::Vector2 min, core::Vector2 max, unsigned int edge);
  std::pair<core::Vector4, core::Vector3> compute_intersection(std::pair<core::Vector4, core::Vector3> p1, std::pair<core::Vector4, core::Vector3> p2, core::Vector3 min, core::Vector3 max, unsigned int plane);

  std::vector<core::Vector3> clip2D_polygon(const std::vector<core::Vector3> &polygon, const core::Vector2 &min, const core::Vector2 &max);
  std::vector<std::pair<core::Vector3, core::Vector3>> clip2D_polygon(const std::vector<std::pair<core::Vector3, core::Vector3>> &polygon, const core::Vector2 &min, const core::Vector2 &max);

  std::vector<std::pair<core::Vector4, core::Vector3>> clip3D_polygon(const std::vector<std::pair<core::Vector4, core::Vector3>> &polygon);
  //-------------------------------------------------------------------------------------------------
  // Funções de Preenchimento de Polígonos e Desenho de Linhas
  //-------------------------------------------------------------------------------------------------

  std::vector<core::Vector3> BresenhamLine(core::Vector3 start, core::Vector3 end);
  void fill_polygon_flat_shading(const std::vector<core::Vector3> &vertexes, const models::Light &global_light, const std::vector<models::Omni> &omni_lights, const core::Vector3 &eye, const core::Vector3 &face_centroid, const core::Vector3 &face_normal, const models::Material &object_material, std::vector<std::vector<float>> &z_buffer, std::vector<std::vector<models::Color>> &color_buffer, core::Vector2 max_window_size);
  void fill_polygon_gourand(const std::vector<std::pair<core::Vector3, models::Color>> &vertexes, std::vector<std::vector<float>> &z_buffer, std::vector<std::vector<models::Color>> &color_buffer);
  void fill_polygon_phong(const std::vector<std::pair<core::Vector3, core::Vector3>> &vertexes, const core::Vector3 &centroid, const models::Light &global_light, const std::vector<models::Omni> &omni_lights, const core::Vector3 &eye, const models::Material &object_material, std::vector<std::vector<float>> &z_buffer, std::vector<std::vector<models::Color>> &color_buffer);
  void z_buffer(const float x, const float y, const float z, const models::Color &color, std::vector<std::vector<float>> &z_buffer, std::vector<std::vector<models::Color>> &color_buffer);

} // namespace math
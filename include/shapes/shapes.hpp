/**********************************************************************************************
 *   IDIOM: PORTUGUÊS
 *
 *   mrxshapes v1.0 - Funções básicas para desenhar formas geométricas
 *
 *   CONVENTIONS: (Convenções)
 *     - As funções sempre têm uma descrição @brief, @param e @return no aquivo .cpp
 *     - As funções deste namespace são usadas para criar formas geométricas básicas
 *
 *   IDIOM: ENGLISH
 *
 *   mrxshapes v1.0 -
 *
 *   CONVENTIONS:
 *    - The functions always have a @brief, @param and @return description in the .cpp file
 *    - The functions of this namespace are used to create basic geometric shapes
 *
 *   CONFIGURATION:
 *       ...
 *
 *   DEPENDENCIES:
 *      <models/mesh.hpp> - Required for: Mesh
 *      <core/vertex.hpp> - Required for: Vertex
 *      <vector>          - Required for: std::vector
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

#include <models/mesh.hpp>
#include <core/vertex.hpp>

#include <vector>
#include <map>
#include <math.h>
#include <unordered_map>
#include <cmath>
#include <algorithm>

namespace shapes
{
  models::Mesh *cube();
  models::Mesh *pyramid(float base = 3.0f, float height = 3.0f);
  models::Mesh *sphere(float radius, int rings = 30, int segments = 30);
  models::Mesh *icosphere(float radius, int subdivisions = 3);
  models::Mesh *polygon(int sides, float radius);
  models::Mesh *cone(float radius, float height, int segments = 30);
}